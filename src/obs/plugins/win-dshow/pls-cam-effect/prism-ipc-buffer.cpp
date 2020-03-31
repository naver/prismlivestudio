#include "prism-ipc-buffer.h"
#include "prism-handle-wrapper.h"

#pragma comment(lib, "WinMM.lib")

#define ALIGN(bytes, align) (((bytes) + ((align)-1)) & ~((align)-1))

#define FILEMAP_BUFFER_CHANGED_EVENT "data-changed-event"
#define FILEMAP_EXTENSION_CHANGED_EVENT "ext-changed-event"

#define FILEMAP_BUFFER_VIEW "buffer-name"
#define FILEMAP_BUFFER_MUTEX_TOP "mutex-top"
#define FILEMAP_BUFFER_MUTEX_ITEM "mutex-item"

CircleBufferIPC::CircleBufferIPC(const char *queueName, int extendSize, int itemHdrSz, int itemCount, int itemSampleSize)
	: itemHeaderSize(itemHdrSz),
	  extendInfoSize(extendSize),
	  itemTotalCount(itemCount),
	  itemBufferSize(itemSampleSize),
	  mutexItems(NULL),
	  mapHandle(NULL),
	  mapViewOfFile(NULL),
	  itemArray(NULL),
	  queueInfo(NULL),
	  readedCount(0),
	  preReadTime(0),
	  strQueueName(queueName ? queueName : "")
{
	assert(queueName && itemCount > 0 && itemSampleSize > 0);

	headerSizeAlign = ALIGN(extendSize + sizeof(QueueInfo), 32);
	itemSizeAlign = ALIGN(itemHdrSz + itemSampleSize, 32);

	bufUpdateEvent = CHandleWrapper::GetEvent(GenerateName(FILEMAP_BUFFER_CHANGED_EVENT, queueName, 0).c_str(), false);
	extUpdateEvent = CHandleWrapper::GetEvent(GenerateName(FILEMAP_EXTENSION_CHANGED_EVENT, queueName, 0).c_str(), false);
	ResetEvent(bufUpdateEvent);
	ResetEvent(extUpdateEvent);

	mutexTop = CHandleWrapper::GetMutex(GenerateName(FILEMAP_BUFFER_MUTEX_TOP, queueName, 0).c_str());
	mutexItems = new HANDLE[itemCount];
	for (int i = 0; i < itemCount; ++i) {
		mutexItems[i] = CHandleWrapper::GetMutex(GenerateName(FILEMAP_BUFFER_MUTEX_ITEM, queueName, i).c_str());
	}
}

CircleBufferIPC::~CircleBufferIPC()
{
	if (mapViewOfFile) {
		UnmapViewOfFile(mapViewOfFile);
		mapViewOfFile = NULL;
	}

	if (itemArray) {
		delete[] itemArray;
		itemArray = NULL;
	}

	CHandleWrapper::CloseHandleEx(mapHandle);

	CHandleWrapper::CloseHandleEx(bufUpdateEvent);
	CHandleWrapper::CloseHandleEx(extUpdateEvent);

	CHandleWrapper::CloseHandleEx(mutexTop);
	if (mutexItems) {
		for (int i = 0; i < itemTotalCount; ++i)
			CHandleWrapper::CloseHandleEx(mutexItems[i]);

		delete[] mutexItems;
		mutexItems = NULL;
	}
}

DWORD CircleBufferIPC::InitMapBuffer()
{
	CAutoLockMutex al(mutexTop);

	if (IsBufferValid())
		return 0;

	int mapTotalSize = headerSizeAlign + itemTotalCount * itemSizeAlign;
	std::string mapName = GenerateName(FILEMAP_BUFFER_VIEW, strQueueName.c_str(), 0);

	bool bNewCreate;
	mapHandle = CHandleWrapper::GetMap(mapName.c_str(), mapTotalSize, &bNewCreate);

	if (!mapHandle) {
		assert(false);
		return GetLastError();
	}

	mapViewOfFile = MapViewOfFile(mapHandle, FILE_MAP_ALL_ACCESS, 0, 0, mapTotalSize);
	if (!mapViewOfFile) {
		assert(false);
		return GetLastError();
	}

	if (bNewCreate)
		memset(mapViewOfFile, 0, mapTotalSize);

	FormatExtensionView(mapViewOfFile, bNewCreate);
	FormatMapView();

	return 0;
}

bool CircleBufferIPC::IsBufferValid()
{
	return !!mapViewOfFile;
}

bool CircleBufferIPC::WaitBufferChanged(DWORD dwWaitMilliSecond)
{
	// Because the event will be reset automally and it may be waiting by multiple objects,
	// So we'd better make sure the waiting time should not be too long.
	assert(dwWaitMilliSecond < 30);

	if (CHandleWrapper::IsHandleSigned(bufUpdateEvent, dwWaitMilliSecond)) {
		return true;
	} else {
		return false;
	}
}

bool CircleBufferIPC::WaitExtendChanged(DWORD dwWaitMilliSecond)
{
	// Because the event will be reset automally and it may be waiting by multiple objects,
	// So we'd better make sure the waiting time should not be too long.
	assert(dwWaitMilliSecond < 30);

	if (CHandleWrapper::IsHandleSigned(extUpdateEvent, dwWaitMilliSecond)) {
		return true;
	} else {
		return false;
	}
}

bool CircleBufferIPC::WriteItemData(const void *hdr, int hdrWriteSize, const void *data, int dataWriteSize)
{
	if (!IsBufferValid() || hdrWriteSize != itemHeaderSize || dataWriteSize > itemBufferSize) {
		assert(false);
		return false;
	}

	int writeIndex = -1;
	{
		CAutoLockMutex al(mutexTop);
		if (!CheckHeaderValid(hdr))
			return false;

		writeIndex = int((queueInfo->writenCount++) % itemTotalCount);
		WaitForSingleObject(mutexItems[writeIndex],
				    INFINITE); // before releasing top lock, request item lock
	}

	memmove(itemArray[writeIndex].hdr, hdr, hdrWriteSize);
	memmove(itemArray[writeIndex].data, data, dataWriteSize);
	ReleaseMutex(mutexItems[writeIndex]);

	SetEvent(bufUpdateEvent);
	return true;
}

bool CircleBufferIPC::ReadItemData(void *hdr, int hdrReadSize, void *buf, int dataReadSize)
{
	if (!IsBufferValid() || hdrReadSize != itemHeaderSize || dataReadSize > itemBufferSize) {
		assert(false);
		return false;
	}

	int index = -1;
	ItemInfo item;
	memset(&item, 0, sizeof(&item));
	{
		CAutoLockMutex al(mutexTop);
		if (IsDataReady()) {
			index = int((readedCount++) % itemTotalCount);
			item = itemArray[index];
			if (!CheckHeaderValid(item.hdr))
				return false;

			// before releasing top lock, request item lock
			WaitForSingleObject(mutexItems[index], INFINITE);
		}
	}

	preReadTime = timeGetTime();
	if (index >= 0) {
		memmove(hdr, item.hdr, hdrReadSize);
		memmove(buf, item.data, dataReadSize);
		ReleaseMutex(mutexItems[index]);
		return true;
	} else {
		return false;
	}
}

std::string CircleBufferIPC::GenerateName(const char *hdr, const char *ext, int index)
{
	char temp[200];
	sprintf_s(temp, 200, "%s-%s-%d", hdr, ext, index);
	return std::string(temp);
}

void CircleBufferIPC::FormatMapView()
{
	BYTE *localPos = (BYTE *)mapViewOfFile + extendInfoSize;
	queueInfo = reinterpret_cast<QueueInfo *>(localPos);

	itemArray = new ItemInfo[itemTotalCount];
	assert(itemArray);
	if (itemArray) {
		BYTE *startPos = (BYTE *)mapViewOfFile + headerSizeAlign;
		for (int i = 0; i < itemTotalCount; ++i) {
			BYTE *itemBegin = startPos + i * itemSizeAlign;
			itemArray[i].hdr = itemBegin;
			itemArray[i].data = itemBegin + itemHeaderSize;
		}
	}
}

bool CircleBufferIPC::IsDataReady()
{
	if (queueInfo->writenCount <= 0) // no data in buffer
		return false;

	LONGLONG latestReadCount = (queueInfo->writenCount - 1);
	if (readedCount <= 0 && ShouldDiscardOutdateData()) // first time to read data
	{
		readedCount = latestReadCount; // start reading from latest frame
		return true;
	}

	if (readedCount > latestReadCount)
		return false; // there is no more frame to be readed

	// There are frames staged in buffer
	// If you are late to read buffer, circle queue may be overwritten. Then you should update your read index
	if (queueInfo->writenCount > itemTotalCount && readedCount < (queueInfo->writenCount - itemTotalCount)) {
		printf("Warning : Lost data because late to read buffer. writeCount:%lld readCount:%lld readSpace:%ums\n", queueInfo->writenCount, readedCount, timeGetTime() - preReadTime);

		if (ShouldDiscardOutdateData()) {
			// jump to latest item
			readedCount = latestReadCount;
		} else {
			// jump to oladest valid item
			readedCount = (queueInfo->writenCount - itemTotalCount);
		}
	}

	return true;
}

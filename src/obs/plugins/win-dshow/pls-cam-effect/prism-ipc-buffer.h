#pragma once
#include <Windows.h>
#include <assert.h>
#include <string>

/*
Format of mapped view:
-----------------------------------------------------------------------------------------------------------
| extensioninfo | queueInfo | itemHeader + itemData | itemHeader + itemData | ... |
-----------------------------------------------------------------------------------------------------------
An instance of CircleBufferIPC should be using for either reading data or writing data;
If it is for reading, it cannot invoke WriteItemData(...).
Similarly if it is for writing, it cannot invoke ReadItemData(...).
*/
class CircleBufferIPC {
public:
	virtual ~CircleBufferIPC();

	// return 0 if success, otherwise return GetLastError()
	DWORD InitMapBuffer();
	bool IsBufferValid();

	bool WaitBufferChanged(DWORD dwWaitMilliSecond);
	bool WaitExtendChanged(DWORD dwWaitMilliSecond);

	bool WriteItemData(const void *hdr, int hdrWriteSize, const void *data,
			   int dataWriteSize);
	bool ReadItemData(void *hdr, int hdrReadSize, void *buf,
			  int dataReadSize);

	virtual void SetExtensionInfo(const void *input) { assert(false); }
	virtual void GetExtensionInfo(void *output) { assert(false); }

protected:
	CircleBufferIPC(const char *queueName, int extendSize,
			int itemHeaderSize, int itemCount, int itemSampleSize);

	virtual void FormatExtensionView(void *ptr, bool bNewCreate) {}
	virtual bool CheckHeaderValid(const void *hdr) { return true; }
	virtual bool ShouldDiscardOutdateData() { return true; }

private:
	std::string GenerateName(const char *hdr, const char *ext, int index);
	void FormatMapView();
	bool IsDataReady();

protected:
#pragma pack(1) // Note: keep memory alignment during IPC
	struct QueueInfo {
		LONGLONG writenCount; // the count that have been written into buffer
	};
	struct ItemInfo {
		void *hdr;
		void *data;
	};
#pragma pack()

	std::string strQueueName;
	int itemHeaderSize;
	int extendInfoSize;
	int itemTotalCount;
	int itemBufferSize;

	int headerSizeAlign; // extendInfo + LocalQueueInfo
	int itemSizeAlign;   // itemHeader + itemBuffer

	LONGLONG readedCount; // the index that currently to be readed
	DWORD preReadTime;

	HANDLE mapHandle;
	void *mapViewOfFile;
	QueueInfo *queueInfo;
	ItemInfo *itemArray;

	// Warning:
	// While requesting lock, you must firstly lock mutexTop, then item's.
	// If you request mutexTop after getting item's lock, it may cause dead-lock.
	HANDLE mutexTop;
	HANDLE *mutexItems;

public:
	HANDLE bufUpdateEvent;
	HANDLE extUpdateEvent;
};

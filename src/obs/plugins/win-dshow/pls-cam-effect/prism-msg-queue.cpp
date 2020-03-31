#include "prism-msg-queue.h"

void PLSMsgQueue::PushMessage(EFFECT_MESSAGE_PTR msg, int length,
			      bool remove_old)
{
	cam_effect_message_info info;
	info.msg = msg;
	info.length = length;

	CAutoLockCS autoLock(msg_lock);
	if (remove_old) {
		RemoveMessage(msg->type);
	}
	msg_list.push_back(info);
}

bool PLSMsgQueue::PopMessage(cam_effect_message_info &output)
{
	CAutoLockCS autoLock(msg_lock);
	if (msg_list.empty()) {
		return false;
	} else {
		output = msg_list.at(0);
		msg_list.erase(msg_list.begin());
		return true;
	}
}

void PLSMsgQueue::RemoveMessage(cam_effect_message_type type)
{
	CAutoLockCS autoLock(msg_lock);
	std::vector<cam_effect_message_info>::iterator itr = msg_list.begin();
	while (itr != msg_list.end()) {
		if ((*itr).msg->type == type) {
			itr = msg_list.erase(itr);
			continue;
		}
		++itr;
	}
}

void PLSMsgQueue::ClearMessage()
{
	CAutoLockCS autoLock(msg_lock);
	msg_list.clear();
}

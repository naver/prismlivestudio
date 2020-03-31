#pragma once
#include "prism-effect-ipc-define.h"
#include "prism-handle-wrapper.h"
#include <vector>

struct cam_effect_message_info {
	EFFECT_MESSAGE_PTR msg;
	int length;
};

class PLSMsgQueue {
public:
	PLSMsgQueue() { msg_list.reserve(20); }
	virtual ~PLSMsgQueue() { ClearMessage(); }

	void PushMessage(EFFECT_MESSAGE_PTR msg, int length, bool remove_old);
	bool PopMessage(cam_effect_message_info &output);
	void RemoveMessage(cam_effect_message_type type);
	void ClearMessage();

private:
	CCriticalSection msg_lock;
	std::vector<cam_effect_message_info> msg_list;
};

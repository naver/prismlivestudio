#pragma once
#include <Windows.h>
#include "interaction_image.h"

enum InteractionButtonState {
	ButtonNormal = 0,
	ButtonHover,
	ButtonClick,
	ButtonDisable,
};

class InteractionButton {
public:
	InteractionButton();
	virtual ~InteractionButton();

	// should be invoked in CEF's main thread
	void Create(HWND hp, int id);
	void SetBkColor(DWORD normal, DWORD hover, DWORD click, DWORD disable);
	void SetImage(GDIP_IMAGE_PTR normal_image,
		      GDIP_IMAGE_PTR hover_image = NULL,
		      GDIP_IMAGE_PTR push_image = NULL,
		      GDIP_IMAGE_PTR disable_image = NULL);

protected:
	static LRESULT CALLBACK InteractionButtonProc(HWND hWnd, UINT message,
						      WPARAM wParam,
						      LPARAM lParam);

	void OnPaint();
	void OnLButtonDown();
	void OnLButtonUp();
	void OnMouseMove();
	void OnMouseLeave();
	void OnWindowEnable(BOOL enable);

	void UpdateButtonState(InteractionButtonState state);

public:
	HWND hWnd = NULL;
	int button_id = 0;
	bool left_button_down = false;

	// Warning : must access them in CEF's main thread
	InteractionButtonState button_state = ButtonNormal;
	std::map<InteractionButtonState, DWORD> bk_colors;
	std::map<InteractionButtonState, GDIP_IMAGE_PTR> bk_images;
};

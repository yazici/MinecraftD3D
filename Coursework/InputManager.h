#pragma once

#include <Windows.h>

#include "Utilities.h"
#include "Singleton.h"

class InputManager : public Singleton<InputManager>
{
public:
	InputManager();
	InputManager(const InputManager&);
	~InputManager();

	// Initialising
	void Initialise();

	void Frame();

	// Getters
	bool GetKeyDown(unsigned int key);
	bool GetKey(unsigned int key);

	inline D3DXVECTOR2 GetMousePosInWindow() { return MousePosInWindow_; }
	inline D3DXVECTOR2 GetMousePosOnScreen() { return MousePosOnScreen_; }

private:
	bool Keys_[256];
	D3DXVECTOR2 MousePosOnScreen_;
	D3DXVECTOR2 MousePosInWindow_;
};



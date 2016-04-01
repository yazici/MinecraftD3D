#include "InputManager.h"

InputManager::InputManager()
{
}

InputManager::InputManager(const InputManager& other)
{
}

InputManager::~InputManager()
{
}

void InputManager::Initialise()
{
	// Initialise the keys to 'not currently pressed'
	for (int i = 0; i<256; i++)
	{
		Keys_[i] = false;
	}

	return;
}

bool InputManager::GetKey(unsigned int key)
{
	// Make sure we are currently on the ApplicationManager to register key presses
	if (!WindowActive())
		return false;

	// Check the status of the key and store it
	if (GetAsyncKeyState(key))
	{
		Keys_[key] = true;
	}
	else
	{
		Keys_[key] = false;
	}

	return Keys_[key];
}

bool InputManager::GetKeyDown(unsigned int key)
{
	// Make sure we are currently on the ApplicationManager to register key presses
	if (!WindowActive())
		return false;

	// If the key we want is down
	if (GetAsyncKeyState(key))
	{
		// If the key is already flagged as down
		if (Keys_[key])
		{
			return false;
		}
		else if (!Keys_[key])
		{
			// Key not down so flag that it is
			Keys_[key] = true;
			return true;
		}
	}
	else if (!GetAsyncKeyState(key))
	{
		Keys_[key] = false;
		return false;
	}

	return false;
}
#pragma once
#include "pch.h";

#include "InputStructs.h"

enum Controls {
	Forward,
	Back,
	Left,
	Right,
	Up,
	Down,
	Sprint,
	Exit,
	LeftClick,
	CameraTracking,
	ControlCount //Placeholder to get the number of controls
};

class InputManager
{
private:
	static std::shared_ptr<InputManager> instance;
	std::vector<Input> controls;
	DirectX::XMFLOAT2 mousePosition;
	DirectX::XMFLOAT2 lastMousePosition;
	DirectX::XMFLOAT2 deltaMouse;
	bool lockMouse;
public:
#pragma region Constructor

	InputManager();
	~InputManager();

#pragma endregion

#pragma region Accessors

	static std::shared_ptr<InputManager> GetInstance();
	DirectX::XMFLOAT2 GetMousePosition();
	DirectX::XMFLOAT2 GetDeltaMouse();
	bool GetLockMouse();
	void SetLockMouse(bool value);

#pragma endregion

#pragma region Input Manager

	void Update(float deltaTime, float totalTime, HWND windowHandle);
	//Returns the given control's current state
	int GetKeyState(int control);
	//Returns true if the given control is currently down
	bool GetKey(int control);
	//Returns true on the first frame that the given key was pressed
	bool GetKeyPressed(int control);
	//Returns true on the first frame that the given key is released
	bool GetKeyReleased(int control);

#pragma endregion
};
#include "InputManager.h"

using namespace std;

#pragma region Constructor

std::shared_ptr<InputManager> InputManager::instance = nullptr;

InputManager::InputManager()
{
	//Set default control states
	controls = vector<Input>();

	for (int i = 0; i < Controls::ControlCount; i++) {
		controls.push_back({});
		controls[i].currentState = InputStates::Up;
		controls[i].lastState = InputStates::Up;
	}

	//Setup control inputs
	controls[Controls::Forward].keyCode = 'W';
	controls[Controls::Back].keyCode = 'S';
	controls[Controls::Left].keyCode = 'A';
	controls[Controls::Right].keyCode = 'D';
	controls[Controls::Up].keyCode = 'E';
	controls[Controls::Down].keyCode = 'Q';
	controls[Controls::Exit].keyCode = VK_ESCAPE;
	controls[Controls::Sprint].keyCode = VK_LSHIFT;
	controls[Controls::LeftClick].keyCode = VK_LBUTTON;
	controls[Controls::CameraTracking].keyCode = VK_RBUTTON;
}

InputManager::~InputManager()
{
}

#pragma endregion

#pragma region Accessors

shared_ptr<InputManager> InputManager::GetInstance()
{
	if (instance == nullptr) {
		instance = make_shared<InputManager>();
	}

	return instance;
}

DirectX::XMFLOAT2 InputManager::GetMousePosition()
{
	return mousePosition;
}

DirectX::XMFLOAT2 InputManager::GetDeltaMouse()
{
	return deltaMouse;
}

bool InputManager::GetLockMouse()
{
	return lockMouse;
}

void InputManager::SetLockMouse(bool value)
{
	lockMouse = value;
}

#pragma endregion

#pragma region Input Manager

void InputManager::Update(float deltaTime, float totalTime, HWND windowHandle)
{
	//Update keys
	for (int i = 0; i < Controls::ControlCount; i++) {
		//update current state
		if (GetAsyncKeyState(controls[i].keyCode) & 0x8000) { //-----------------------------------------------------------Key is down
			if (controls[i].lastState == InputStates::Up || controls[i].lastState == InputStates::Released) {//--------------------Key was up
				controls[i].currentState = InputStates::Pressed;
			}
			else {//---------------------------------------------------------------------------------------------------------------Key was down
				controls[i].currentState = InputStates::Down;
			}
		}
		else { //----------------------------------------------------------------------------------------------------------Key is up
			if (controls[i].lastState == InputStates::Up || controls[i].lastState == InputStates::Released) {//--------------------Key was up
				controls[i].currentState = InputStates::Up;
			}
			else {//---------------------------------------------------------------------------------------------------------------Key was down
				controls[i].currentState = InputStates::Released;
			}
		}

		//Move current state to last state
		controls[i].lastState = controls[i].currentState;
	}

	//Update mouse
	POINT mPos = {};
	GetCursorPos(&mPos);
	ScreenToClient(windowHandle, &mPos);

	mousePosition.x = (float)mPos.x;
	mousePosition.y = (float)mPos.y;

	deltaMouse.x = lastMousePosition.x - mousePosition.x;
	deltaMouse.y = lastMousePosition.y - mousePosition.y;

	lastMousePosition = mousePosition;

	if (lockMouse) {
		ShowCursor(false);

		RECT windowSize;
		GetWindowRect(windowHandle, &windowSize);

		POINT windowCenter;
		windowCenter.x = (windowSize.left + windowSize.right) / 2;
		windowCenter.y = (windowSize.top + windowSize.bottom) / 2;

		SetCursorPos(windowCenter.x, windowCenter.y);
		ScreenToClient(windowHandle, &windowCenter);

		deltaMouse.x = windowCenter.x - mousePosition.x;
		deltaMouse.y = windowCenter.y - mousePosition.y;
	}
	else {
		ShowCursor(true);
	}
}

int InputManager::GetKeyState(int control)
{
	return (int)controls[control].currentState;
}

bool InputManager::GetKey(int control)
{
	return (controls[control].currentState == InputStates::Down || controls[control].currentState == InputStates::Pressed);
}

bool InputManager::GetKeyPressed(int control)
{
	return controls[control].currentState == InputStates::Pressed;
}

bool InputManager::GetKeyReleased(int control)
{
	return controls[control].currentState == InputStates::Released;
}

#pragma endregion

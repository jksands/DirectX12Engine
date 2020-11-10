#pragma once
#include "pch.h";

//Had to put input states enum in a namespace because input manager's controls already has enum states for up and down
namespace InputStates {
	enum InputStates {
		Up, //Key is currently up
		Down, //Key is currently down
		Pressed, //Key was pressed this frame
		Released //Key was released this frame
	};
}

struct Input {
	InputStates::InputStates currentState;
	InputStates::InputStates lastState;
	int keyCode;
};
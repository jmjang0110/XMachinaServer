#pragma once


namespace KeyInfo
{
	enum class KEY_STATE : UINT8
	{
		TAP,     // 막 눌림
		PRESSED, // TAP 이후에도 계속 눌려있음
		AWAY,    // 막 뗀 상황
		NONE,    // 안눌린 상태
	};


	enum class KEY : UINT8
	{
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,
		LEFT, RIGHT, UP, DOWN,
		_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
		NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,
		LSHFT, LALT, LCTRL, SPACE, ENTER, BACK, ESC, TAB, DEL,
		LBTN, RBTN, MBTN,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

		KEY_END,
	};

	enum class MoveKey : UINT8 {
		W = 0b0000'0001,
		A = 0b0000'0010,
		S = 0b0000'0100,
		D = 0b0000'1000,

	};



}

class Key
{
};


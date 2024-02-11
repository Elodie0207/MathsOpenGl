//
// Created by ianpo on 19/07/2023.
//

#pragma once

// Imported from VXM3.h

/* The unknown key */
#define REI_KEY_UNKNOWN            -1

/* Printable keys */
#define REI_KEY_SPACE              32
#define REI_KEY_APOSTROPHE         39  /* ' */
#define REI_KEY_COMMA              44  /* , */
#define REI_KEY_MINUS              45  /* - */
#define REI_KEY_PERIOD             46  /* . */
#define REI_KEY_SLASH              47  /* / */
#define REI_KEY_0                  48
#define REI_KEY_1                  49
#define REI_KEY_2                  50
#define REI_KEY_3                  51
#define REI_KEY_4                  52
#define REI_KEY_5                  53
#define REI_KEY_6                  54
#define REI_KEY_7                  55
#define REI_KEY_8                  56
#define REI_KEY_9                  57
#define REI_KEY_SEMICOLON          59  /* ; */
#define REI_KEY_EQUAL              61  /* = */
#define REI_KEY_A                  65
#define REI_KEY_B                  66
#define REI_KEY_C                  67
#define REI_KEY_D                  68
#define REI_KEY_E                  69
#define REI_KEY_F                  70
#define REI_KEY_G                  71
#define REI_KEY_H                  72
#define REI_KEY_I                  73
#define REI_KEY_J                  74
#define REI_KEY_K                  75
#define REI_KEY_L                  76
#define REI_KEY_M                  77
#define REI_KEY_N                  78
#define REI_KEY_O                  79
#define REI_KEY_P                  80
#define REI_KEY_Q                  81
#define REI_KEY_R                  82
#define REI_KEY_S                  83
#define REI_KEY_T                  84
#define REI_KEY_U                  85
#define REI_KEY_V                  86
#define REI_KEY_W                  87
#define REI_KEY_X                  88
#define REI_KEY_Y                  89
#define REI_KEY_Z                  90
#define REI_KEY_LEFT_BRACKET       91  /* [ */
#define REI_KEY_BACKSLASH          92  /* \ */
#define REI_KEY_RIGHT_BRACKET      93  /* ] */
#define REI_KEY_GRAVE_ACCENT       96  /* ` */
#define REI_KEY_WORLD_1            161 /* non-US #1 */
#define REI_KEY_WORLD_2            162 /* non-US #2 */


/* Function keys */
#define REI_KEY_ESCAPE             256
#define REI_KEY_ENTER              257
#define REI_KEY_TAB                258
#define REI_KEY_BACKSPACE          259
#define REI_KEY_INSERT             260
#define REI_KEY_DELETE             261
#define REI_KEY_RIGHT              262
#define REI_KEY_LEFT               263
#define REI_KEY_DOWN               264
#define REI_KEY_UP                 265
#define REI_KEY_PAGE_UP            266
#define REI_KEY_PAGE_DOWN          267
#define REI_KEY_HOME               268
#define REI_KEY_END                269
#define REI_KEY_CAPS_LOCK          280
#define REI_KEY_SCROLL_LOCK        281
#define REI_KEY_NUM_LOCK           282
#define REI_KEY_PRINT_SCREEN       283
#define REI_KEY_PAUSE              284
#define REI_KEY_F1                 290
#define REI_KEY_F2                 291
#define REI_KEY_F3                 292
#define REI_KEY_F4                 293
#define REI_KEY_F5                 294
#define REI_KEY_F6                 295
#define REI_KEY_F7                 296
#define REI_KEY_F8                 297
#define REI_KEY_F9                 298
#define REI_KEY_F10                299
#define REI_KEY_F11                300
#define REI_KEY_F12                301
#define REI_KEY_F13                302
#define REI_KEY_F14                303
#define REI_KEY_F15                304
#define REI_KEY_F16                305
#define REI_KEY_F17                306
#define REI_KEY_F18                307
#define REI_KEY_F19                308
#define REI_KEY_F20                309
#define REI_KEY_F21                310
#define REI_KEY_F22                311
#define REI_KEY_F23                312
#define REI_KEY_F24                313
#define REI_KEY_F25                314
#define REI_KEY_KP_0               320
#define REI_KEY_KP_1               321
#define REI_KEY_KP_2               322
#define REI_KEY_KP_3               323
#define REI_KEY_KP_4               324
#define REI_KEY_KP_5               325
#define REI_KEY_KP_6               326
#define REI_KEY_KP_7               327
#define REI_KEY_KP_8               328
#define REI_KEY_KP_9               329
#define REI_KEY_KP_DECIMAL         330
#define REI_KEY_KP_DIVIDE          331
#define REI_KEY_KP_MULTIPLY        332
#define REI_KEY_KP_SUBTRACT        333
#define REI_KEY_KP_ADD             334
#define REI_KEY_KP_ENTER           335
#define REI_KEY_KP_EQUAL           336
#define REI_KEY_LEFT_SHIFT         340
#define REI_KEY_LEFT_CONTROL       341
#define REI_KEY_LEFT_ALT           342
#define REI_KEY_LEFT_SUPER         343
#define REI_KEY_RIGHT_SHIFT        344
#define REI_KEY_RIGHT_CONTROL      345
#define REI_KEY_RIGHT_ALT          346
#define REI_KEY_RIGHT_SUPER        347
#define REI_KEY_MENU               348
#define REI_KEY_LAST               REI_KEY_MENU

enum class Key : int {
	Unknown = REI_KEY_UNKNOWN,
	Space = REI_KEY_SPACE,
	Apostrophe = REI_KEY_APOSTROPHE,
	Comma = REI_KEY_COMMA,
	Minus = REI_KEY_MINUS,
	Period = REI_KEY_PERIOD,
	Slash = REI_KEY_SLASH,
	Key0 = REI_KEY_0,
	Key1 = REI_KEY_1,
	Key2 = REI_KEY_2,
	Key3 = REI_KEY_3,
	Key4 = REI_KEY_4,
	Key5 = REI_KEY_5,
	Key6 = REI_KEY_6,
	Key7 = REI_KEY_7,
	Key8 = REI_KEY_8,
	Key9 = REI_KEY_9,
	Semicolon = REI_KEY_SEMICOLON,
	Equal = REI_KEY_EQUAL,
	A = REI_KEY_A,
	B = REI_KEY_B,
	C = REI_KEY_C,
	D = REI_KEY_D,
	E = REI_KEY_E,
	F = REI_KEY_F,
	G = REI_KEY_G,
	H = REI_KEY_H,
	I = REI_KEY_I,
	J = REI_KEY_J,
	K = REI_KEY_K,
	L = REI_KEY_L,
	M = REI_KEY_M,
	N = REI_KEY_N,
	O = REI_KEY_O,
	P = REI_KEY_P,
	Q = REI_KEY_Q,
	R = REI_KEY_R,
	S = REI_KEY_S,
	T = REI_KEY_T,
	U = REI_KEY_U,
	V = REI_KEY_V,
	W = REI_KEY_W,
	X = REI_KEY_X,
	Y = REI_KEY_Y,
	Z = REI_KEY_Z,
	LeftBracket = REI_KEY_LEFT_BRACKET,
	Backslash = REI_KEY_BACKSLASH,
	RightBracket = REI_KEY_RIGHT_BRACKET,
	GraveAccent = REI_KEY_GRAVE_ACCENT,
	World1 = REI_KEY_WORLD_1,
	World2 = REI_KEY_WORLD_2,
	Escape = REI_KEY_ESCAPE,
	Enter = REI_KEY_ENTER,
	Tab = REI_KEY_TAB,
	Backspace = REI_KEY_BACKSPACE,
	Insert = REI_KEY_INSERT,
	Delete = REI_KEY_DELETE,
	Right = REI_KEY_RIGHT,
	Left = REI_KEY_LEFT,
	Down = REI_KEY_DOWN,
	Up = REI_KEY_UP,
	PageUp = REI_KEY_PAGE_UP,
	PageDown = REI_KEY_PAGE_DOWN,
	Home = REI_KEY_HOME,
	End = REI_KEY_END,
	CapsLock = REI_KEY_CAPS_LOCK,
	ScrollLock = REI_KEY_SCROLL_LOCK,
	NumLock = REI_KEY_NUM_LOCK,
	PrintScreen = REI_KEY_PRINT_SCREEN,
	Pause = REI_KEY_PAUSE,
	F1 = REI_KEY_F1,
	F2 = REI_KEY_F2,
	F3 = REI_KEY_F3,
	F4 = REI_KEY_F4,
	F5 = REI_KEY_F5,
	F6 = REI_KEY_F6,
	F7 = REI_KEY_F7,
	F8 = REI_KEY_F8,
	F9 = REI_KEY_F9,
	F10 = REI_KEY_F10,
	F11 = REI_KEY_F11,
	F12 = REI_KEY_F12,
	F13 = REI_KEY_F13,
	F14 = REI_KEY_F14,
	F15 = REI_KEY_F15,
	F16 = REI_KEY_F16,
	F17 = REI_KEY_F17,
	F18 = REI_KEY_F18,
	F19 = REI_KEY_F19,
	F20 = REI_KEY_F20,
	F21 = REI_KEY_F21,
	F22 = REI_KEY_F22,
	F23 = REI_KEY_F23,
	F24 = REI_KEY_F24,
	F25 = REI_KEY_F25,
	Kp0 = REI_KEY_KP_0,
	Kp1 = REI_KEY_KP_1,
	Kp2 = REI_KEY_KP_2,
	Kp3 = REI_KEY_KP_3,
	Kp4 = REI_KEY_KP_4,
	Kp5 = REI_KEY_KP_5,
	Kp6 = REI_KEY_KP_6,
	Kp7 = REI_KEY_KP_7,
	Kp8 = REI_KEY_KP_8,
	Kp9 = REI_KEY_KP_9,
	KpDecimal = REI_KEY_KP_DECIMAL,
	KpDivide = REI_KEY_KP_DIVIDE,
	KpMultiply = REI_KEY_KP_MULTIPLY,
	KpSubtract = REI_KEY_KP_SUBTRACT,
	KpAdd = REI_KEY_KP_ADD,
	KpEnter = REI_KEY_KP_ENTER,
	KpEqual = REI_KEY_KP_EQUAL,
	LeftShift = REI_KEY_LEFT_SHIFT,
	LeftControl = REI_KEY_LEFT_CONTROL,
	LeftAlt = REI_KEY_LEFT_ALT,
	LeftSuper = REI_KEY_LEFT_SUPER,
	RightShift = REI_KEY_RIGHT_SHIFT,
	RightControl = REI_KEY_RIGHT_CONTROL,
	RightAlt = REI_KEY_RIGHT_ALT,
	RightSuper = REI_KEY_RIGHT_SUPER,
	Menu = REI_KEY_MENU,
	Last = REI_KEY_LAST,
};

inline std::string GetKeyName(Key key)
{
	switch (key) {
		case Key::Space :
			return "Space";
		case Key::Apostrophe :
			return "Apostrophe";
		case Key::Comma :
			return "Comma";
		case Key::Minus :
			return "Minus";
		case Key::Period :
			return "Period";
		case Key::Slash :
			return "Slash";
		case Key::Key0 :
			return "Key0";
		case Key::Key1 :
			return "Key1";
		case Key::Key2 :
			return "Key2";
		case Key::Key3 :
			return "Key3";
		case Key::Key4 :
			return "Key4";
		case Key::Key5 :
			return "Key5";
		case Key::Key6 :
			return "Key6";
		case Key::Key7 :
			return "Key7";
		case Key::Key8 :
			return "Key8";
		case Key::Key9 :
			return "Key9";
		case Key::Semicolon :
			return "Semicolon";
		case Key::Equal :
			return "Equal";
		case Key::A :
			return "A";
		case Key::B :
			return "B";
		case Key::C :
			return "C";
		case Key::D :
			return "D";
		case Key::E :
			return "E";
		case Key::F :
			return "F";
		case Key::G :
			return "G";
		case Key::H :
			return "H";
		case Key::I :
			return "I";
		case Key::J :
			return "J";
		case Key::K :
			return "K";
		case Key::L :
			return "L";
		case Key::M :
			return "M";
		case Key::N :
			return "N";
		case Key::O :
			return "O";
		case Key::P :
			return "P";
		case Key::Q :
			return "Q";
		case Key::R :
			return "R";
		case Key::S :
			return "S";
		case Key::T :
			return "T";
		case Key::U :
			return "U";
		case Key::V :
			return "V";
		case Key::W :
			return "W";
		case Key::X :
			return "X";
		case Key::Y :
			return "Y";
		case Key::Z :
			return "Z";
		case Key::LeftBracket :
			return "LeftBracket";
		case Key::Backslash :
			return "Backslash";
		case Key::RightBracket :
			return "RightBracket";
		case Key::GraveAccent :
			return "GraveAccent";
		case Key::World1 :
			return "World1";
		case Key::World2 :
			return "World2";
		case Key::Escape :
			return "Escape";
		case Key::Enter :
			return "Enter";
		case Key::Tab :
			return "Tab";
		case Key::Backspace :
			return "Backspace";
		case Key::Insert :
			return "Insert";
		case Key::Delete :
			return "Delete";
		case Key::Right :
			return "Right";
		case Key::Left :
			return "Left";
		case Key::Down :
			return "Down";
		case Key::Up :
			return "Up";
		case Key::PageUp :
			return "PageUp";
		case Key::PageDown :
			return "PageDown";
		case Key::Home :
			return "Home";
		case Key::End :
			return "End";
		case Key::CapsLock :
			return "CapsLock";
		case Key::ScrollLock :
			return "ScrollLock";
		case Key::NumLock :
			return "NumLock";
		case Key::PrintScreen :
			return "PrintScreen";
		case Key::Pause :
			return "Pause";
		case Key::F1 :
			return "F1";
		case Key::F2 :
			return "F2";
		case Key::F3 :
			return "F3";
		case Key::F4 :
			return "F4";
		case Key::F5 :
			return "F5";
		case Key::F6 :
			return "F6";
		case Key::F7 :
			return "F7";
		case Key::F8 :
			return "F8";
		case Key::F9 :
			return "F9";
		case Key::F10 :
			return "F10";
		case Key::F11 :
			return "F11";
		case Key::F12 :
			return "F12";
		case Key::F13 :
			return "F13";
		case Key::F14 :
			return "F14";
		case Key::F15 :
			return "F15";
		case Key::F16 :
			return "F16";
		case Key::F17 :
			return "F17";
		case Key::F18 :
			return "F18";
		case Key::F19 :
			return "F19";
		case Key::F20 :
			return "F20";
		case Key::F21 :
			return "F21";
		case Key::F22 :
			return "F22";
		case Key::F23 :
			return "F23";
		case Key::F24 :
			return "F24";
		case Key::F25 :
			return "F25";
		case Key::Kp0 :
			return "Kp0";
		case Key::Kp1 :
			return "Kp1";
		case Key::Kp2 :
			return "Kp2";
		case Key::Kp3 :
			return "Kp3";
		case Key::Kp4 :
			return "Kp4";
		case Key::Kp5 :
			return "Kp5";
		case Key::Kp6 :
			return "Kp6";
		case Key::Kp7 :
			return "Kp7";
		case Key::Kp8 :
			return "Kp8";
		case Key::Kp9 :
			return "Kp9";
		case Key::KpDecimal :
			return "KpDecimal";
		case Key::KpDivide :
			return "KpDivide";
		case Key::KpMultiply :
			return "KpMultiply";
		case Key::KpSubtract :
			return "KpSubtract";
		case Key::KpAdd :
			return "KpAdd";
		case Key::KpEnter :
			return "KpEnter";
		case Key::KpEqual :
			return "KpEqual";
		case Key::LeftShift :
			return "LeftShift";
		case Key::LeftControl :
			return "LeftControl";
		case Key::LeftAlt :
			return "LeftAlt";
		case Key::LeftSuper :
			return "LeftSuper";
		case Key::RightShift :
			return "RightShift";
		case Key::RightControl :
			return "RightControl";
		case Key::RightAlt :
			return "RightAlt";
		case Key::RightSuper :
			return "RightSuper";
		case Key::Menu :
			return "Menu";
		default:
			return "Unknown";
	}
}
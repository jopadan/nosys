#pragma once
#include <GLFW/glfw3.h>

enum key_event {
key_press                       = GLFW_PRESS,
key_release                     = GLFW_RELEASE,
key_repeat                      = GLFW_REPEAT,
};

enum keys {
key_unknown                     = GLFW_KEY_UNKNOWN,
key_space                       = GLFW_KEY_SPACE,
key_apostrophe                  = GLFW_KEY_APOSTROPHE,
key_comma                       = GLFW_KEY_COMMA,
key_minus                       = GLFW_KEY_MINUS,
key_period                      = GLFW_KEY_PERIOD,
key_slash                       = GLFW_KEY_SLASH,
key_0                           = GLFW_KEY_0,
key_1                           = GLFW_KEY_1,
key_2                           = GLFW_KEY_2,
key_3                           = GLFW_KEY_3,
key_4                           = GLFW_KEY_4,
key_5                           = GLFW_KEY_5,
key_6                           = GLFW_KEY_6,
key_7                           = GLFW_KEY_7,
key_8                           = GLFW_KEY_8,
key_9                           = GLFW_KEY_9,
key_semicolon                   = GLFW_KEY_SEMICOLON,
key_equal                       = GLFW_KEY_EQUAL,
key_a                           = GLFW_KEY_A,
key_b                           = GLFW_KEY_B,
key_c                           = GLFW_KEY_C,
key_d                           = GLFW_KEY_D,
key_e                           = GLFW_KEY_E,
key_f                           = GLFW_KEY_F,
key_g                           = GLFW_KEY_G,
key_h                           = GLFW_KEY_H,
key_i                           = GLFW_KEY_I,
key_j                           = GLFW_KEY_J,
key_k                           = GLFW_KEY_K,
key_l                           = GLFW_KEY_L,
key_m                           = GLFW_KEY_M,
key_n                           = GLFW_KEY_N,
key_o                           = GLFW_KEY_O,
key_p                           = GLFW_KEY_P,
key_q                           = GLFW_KEY_Q,
key_r                           = GLFW_KEY_R,
key_s                           = GLFW_KEY_S,
key_t                           = GLFW_KEY_T,
key_u                           = GLFW_KEY_U,
key_v                           = GLFW_KEY_V,
key_w                           = GLFW_KEY_W,
key_x                           = GLFW_KEY_X,
key_y                           = GLFW_KEY_Y,
key_z                           = GLFW_KEY_Z,
key_left_bracket                = GLFW_KEY_LEFT_BRACKET,
key_backslash                   = GLFW_KEY_BACKSLASH,
key_right_bracket               = GLFW_KEY_RIGHT_BRACKET,
key_grave_accent                = GLFW_KEY_GRAVE_ACCENT,
key_world_1                     = GLFW_KEY_WORLD_1,
key_world_2                     = GLFW_KEY_WORLD_2,
key_escape                      = GLFW_KEY_ESCAPE,
key_enter                       = GLFW_KEY_ENTER,
key_tab                         = GLFW_KEY_TAB,
key_backspace                   = GLFW_KEY_BACKSPACE,
key_insert                      = GLFW_KEY_INSERT,
key_delete                      = GLFW_KEY_DELETE,
key_right                       = GLFW_KEY_RIGHT,
key_left                        = GLFW_KEY_LEFT,
key_down                        = GLFW_KEY_DOWN,
key_up                          = GLFW_KEY_UP,
key_page_up                     = GLFW_KEY_PAGE_UP,
key_page_down                   = GLFW_KEY_PAGE_DOWN,
key_home                        = GLFW_KEY_HOME,
key_end                         = GLFW_KEY_END,
key_caps_lock                   = GLFW_KEY_CAPS_LOCK,
key_scroll_lock                 = GLFW_KEY_SCROLL_LOCK,
key_num_lock                    = GLFW_KEY_NUM_LOCK,
key_print_screen                = GLFW_KEY_PRINT_SCREEN,
key_pause                       = GLFW_KEY_PAUSE,
key_f1                          = GLFW_KEY_F1,
key_f2                          = GLFW_KEY_F2,
key_f3                          = GLFW_KEY_F3,
key_f4                          = GLFW_KEY_F4,
key_f5                          = GLFW_KEY_F5,
key_f6                          = GLFW_KEY_F6,
key_f7                          = GLFW_KEY_F7,
key_f8                          = GLFW_KEY_F8,
key_f9                          = GLFW_KEY_F9,
key_f10                         = GLFW_KEY_F10,
key_f11                         = GLFW_KEY_F11,
key_f12                         = GLFW_KEY_F12,
key_f13                         = GLFW_KEY_F13,
key_f14                         = GLFW_KEY_F14,
key_f15                         = GLFW_KEY_F15,
key_f16                         = GLFW_KEY_F16,
key_f17                         = GLFW_KEY_F17,
key_f18                         = GLFW_KEY_F18,
key_f19                         = GLFW_KEY_F19,
key_f20                         = GLFW_KEY_F20,
key_f21                         = GLFW_KEY_F21,
key_f22                         = GLFW_KEY_F22,
key_f23                         = GLFW_KEY_F23,
key_f24                         = GLFW_KEY_F24,
key_f25                         = GLFW_KEY_F25,
key_kp_0                        = GLFW_KEY_KP_0,
key_kp_1                        = GLFW_KEY_KP_1,
key_kp_2                        = GLFW_KEY_KP_2,
key_kp_3                        = GLFW_KEY_KP_3,
key_kp_4                        = GLFW_KEY_KP_4,
key_kp_5                        = GLFW_KEY_KP_5,
key_kp_6                        = GLFW_KEY_KP_6,
key_kp_7                        = GLFW_KEY_KP_7,
key_kp_8                        = GLFW_KEY_KP_8,
key_kp_9                        = GLFW_KEY_KP_9,
key_kp_decimal                  = GLFW_KEY_KP_DECIMAL,
key_kp_divide                   = GLFW_KEY_KP_DIVIDE,
key_kp_multiply                 = GLFW_KEY_KP_MULTIPLY,
key_kp_subtract                 = GLFW_KEY_KP_SUBTRACT,
key_kp_add                      = GLFW_KEY_KP_ADD,
key_kp_enter                    = GLFW_KEY_KP_ENTER,
key_kp_equal                    = GLFW_KEY_KP_EQUAL,
key_left_shift                  = GLFW_KEY_LEFT_SHIFT,
key_left_control                = GLFW_KEY_LEFT_CONTROL,
key_left_alt                    = GLFW_KEY_LEFT_ALT,
key_left_super                  = GLFW_KEY_LEFT_SUPER,
key_right_shift                 = GLFW_KEY_RIGHT_SHIFT,
key_right_control               = GLFW_KEY_RIGHT_CONTROL,
key_right_alt                   = GLFW_KEY_RIGHT_ALT,
key_right_super                 = GLFW_KEY_RIGHT_SUPER,
key_menu                        = GLFW_KEY_MENU,
key_last                        = GLFW_KEY_LAST,
mouse_button_1                  = GLFW_MOUSE_BUTTON_1,
mouse_button_2                  = GLFW_MOUSE_BUTTON_2,
mouse_button_3                  = GLFW_MOUSE_BUTTON_3,
mouse_button_4                  = GLFW_MOUSE_BUTTON_4,
mouse_button_5                  = GLFW_MOUSE_BUTTON_5,
mouse_button_6                  = GLFW_MOUSE_BUTTON_6,
mouse_button_7                  = GLFW_MOUSE_BUTTON_7,
mouse_button_8                  = GLFW_MOUSE_BUTTON_8,
mouse_button_last               = GLFW_MOUSE_BUTTON_LAST,
mouse_button_left               = GLFW_MOUSE_BUTTON_LEFT,
mouse_button_right              = GLFW_MOUSE_BUTTON_RIGHT,
mouse_button_middle             = GLFW_MOUSE_BUTTON_MIDDLE,
gamepad_button_a                = GLFW_GAMEPAD_BUTTON_A,
gamepad_button_b                = GLFW_GAMEPAD_BUTTON_B,
gamepad_button_x                = GLFW_GAMEPAD_BUTTON_X,
gamepad_button_y                = GLFW_GAMEPAD_BUTTON_Y,
gamepad_button_left_bumper      = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
gamepad_button_right_bumper     = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
gamepad_button_back             = GLFW_GAMEPAD_BUTTON_BACK,
gamepad_button_start            = GLFW_GAMEPAD_BUTTON_START,
gamepad_button_guide            = GLFW_GAMEPAD_BUTTON_GUIDE,
gamepad_button_left_thumb       = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
gamepad_button_right_thumb      = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
gamepad_button_dpad_up          = GLFW_GAMEPAD_BUTTON_DPAD_UP,
gamepad_button_dpad_right       = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
gamepad_button_dpad_down        = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
gamepad_button_dpad_left        = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,
gamepad_button_last             = GLFW_GAMEPAD_BUTTON_LAST,
gamepad_button_cross            = GLFW_GAMEPAD_BUTTON_CROSS,
gamepad_button_circle           = GLFW_GAMEPAD_BUTTON_CIRCLE,
gamepad_button_square           = GLFW_GAMEPAD_BUTTON_SQUARE,
gamepad_button_triangle         = GLFW_GAMEPAD_BUTTON_TRIANGLE,
gamepad_axis_left_x             = GLFW_GAMEPAD_AXIS_LEFT_X,
gamepad_axis_left_y             = GLFW_GAMEPAD_AXIS_LEFT_Y,
gamepad_axis_right_x            = GLFW_GAMEPAD_AXIS_RIGHT_X,
gamepad_axis_right_y            = GLFW_GAMEPAD_AXIS_RIGHT_Y,
gamepad_axis_left_trigger       = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
gamepad_axis_right_trigger      = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,
gamepad_axis_last               = GLFW_GAMEPAD_AXIS_LAST,
joystick_hat_centered           = GLFW_HAT_CENTERED,
joystick_hat_up                 = GLFW_HAT_UP,
joystick_hat_right              = GLFW_HAT_RIGHT,
joystick_hat_down               = GLFW_HAT_DOWN,
joystick_hat_left               = GLFW_HAT_LEFT,
joystick_hat_right_up           = GLFW_HAT_RIGHT_UP,
joystick_hat_right_down         = GLFW_HAT_RIGHT_DOWN,
joystick_hat_left_up            = GLFW_HAT_LEFT_UP,
joystick_hat_left_down          = GLFW_HAT_LEFT_DOWN,
};

void keys(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	printf("key: %s action: %d mods: %X\n", glfwGetKeyName(key, scancode), action, mods);

	if(action == key_press || action == key_repeat)
	{
		switch(key)
		{
			case key_escape:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
			case key_f12:
				grab();
				break;
			case key_up:
				zoom(win, 0.0,  1.0);
				break;
			case key_down:
				zoom(win, 0.0, -1.0);
				break;
			case key_left:
				zoom(win,  1.0, 0);
				break;
			case key_right:
				zoom(win,  -1.0, 0);
				break;
			default:
				break;
		}
	}
}

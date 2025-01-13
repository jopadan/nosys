#pragma once
#include <GLFW/glfw3.h>

enum keys {
key_unknown                     = GLFW_KEY_UNKNOWN,
gamepad_button_a                = OSL_KEY_CROSS,
gamepad_button_b                = OSL_KEY_CIRCLE,
gamepad_button_x                = OSL_KEY_SQUARE,
gamepad_button_y                = OSL_KEY_TRIANGLE,
gamepad_button_left_bumper      = OSL_KEY_L,
gamepad_button_right_bumper     = GSL_KEY_R,
gamepad_button_back             = OSL_KEY_HOME,
gamepad_button_start            = OSL_KEY_START,
gamepad_button_guide            = OSL_KEY_SELECT,
gamepad_button_left_thumb       = OSL_KEY_HOLD,
gamepad_button_right_thumb      = OSL_KEY_NOTE,
gamepad_button_dpad_up          = OSL_KEY_UP,
gamepad_button_dpad_right       = OSL_KEY_RIGHT,
gamepad_button_dpad_down        = OSL_KEY_DOWN,
gamepad_button_dpad_left        = OSL_KEY_LEFT,
gamepad_button_last             = OSL_KEY_NOTE,
gamepad_button_cross            = OSL_KEY_CROSS,
gamepad_button_circle           = OSL_KEY_CIRCLE,
gamepad_button_square           = OSL_KEY_SQUARE,
gamepad_button_triangle         = OSL_KEY_TRIANGLE,
};

void keys(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	printf("key: %s action: %d mods: %X\n", glfwGetKeyName(key, scancode), action, mods);

	if(key == key_escape && action == key_press)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if(key == key_f12 && action == key_press)
		grab();
}

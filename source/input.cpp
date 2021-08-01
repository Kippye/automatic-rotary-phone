#include <GLFW/glfw3.h>
#include <input.h>
#include <main.h>
#include <iostream>
#include <glm/gtx/norm.hpp>

class Program;
extern Program program;

void Input::setup(){ /* nothing to see here */ }

void Input::processInput(GLFWwindow* window)
{
	if (program.file_system.contextOpen) { return; }

	/// bools
	// modifiers, wait why do i even have these? oh right cus there aint no modifiers here...
	shift_down = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
	ctrl_down = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS);
	alt_down = (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS);

	lmb_down = (!program.gui.guiHovered && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
	rmb_down = (!program.gui.guiHovered && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
	/// drawing
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// did the cursor or the camera move?
		if (glm::length2(mouseMovement) > 1.0f || glm::length2(program.camera.lastMovement) > 1.0f)
		{
			// has the repetition delay passed?
			//if (program.render.mouse_repeat)
			//{
			//	program.render.mouse_button_delay = 0.0f;
			//}
			// if the cursor stopped for a while and then moved again, restart the delay
			//else if (program.render.mouse_button_delay > 100.0f)
			//{
			//	program.render.mouse_button_delay = 0.0f;
			//}
		}
	}
	/// drag deleting
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		// did the cursor or the camera move?
		if (glm::length2(mouseMovement) > 1.0f || glm::length2(program.camera.lastMovement) > 1.0f)
		{
			//// has the repetition delay passed?
			//if (program.render.mouse_repeat)
			//{
			//	program.render.mouse_button_delay = 0.0f;
			//}
			//// if the cursor stopped for a while and then moved again, restart the delay
			//else if (program.render.mouse_button_delay > 100.0f)
			//{
			//	program.render.mouse_button_delay = 0.0f;
			//}
		}
	}
}

void Input::key_event(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (program.file_system.contextOpen) { return; }
		switch (key)
		{
			case GLFW_KEY_BACKSPACE: // delete
				//if (program.gui.programGuiFocused) { return; }
				//program.editor.delete_selection();
			break;
			case GLFW_KEY_DELETE: // delete
				if (program.gui.programGuiFocused) { return; }
				program.editor.delete_selection();
			break;
			/// program manipulators
			case GLFW_KEY_ESCAPE: // exit the program
				if (program.gui.programGuiFocused) { return; }
				glfwSetWindowShouldClose(window, true);
			break;
			/// shortcut keys that should work even when UI is focused
			case GLFW_KEY_I:
				if (ctrl_down) // pop up open dialog
				{
					program.gui.openFileDialog(OPEN);
				}
			break;
			case GLFW_KEY_O:
				if (ctrl_down) // save
				{
					if (shift_down) // save as
					{
						program.gui.openFileDialog(SAVE_AS);
					}
					program.gui.openFileDialog(SAVE);
				}
			break;
		}
	}

	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
			case (GLFW_KEY_LEFT_SHIFT):
			shift_down = false;
			break;
			case (GLFW_KEY_LEFT_CONTROL):
			ctrl_down = false;
			break;
			case (GLFW_KEY_LEFT_ALT):
			alt_down = false;
			break;
			case (GLFW_MOUSE_BUTTON_LEFT):
			lmb_down = false;
			break;
			case (GLFW_MOUSE_BUTTON_RIGHT):
			rmb_down = false;
			break;
		}
	}
}

void Input::mouse_button_event(GLFWwindow* window, int key, int action, int mods)
{
	if (program.file_system.contextOpen || program.gui.guiHovered) { return; }

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_MOUSE_BUTTON_LEFT)
		{
			program.windowManager.hasFocus = true;

			if (program.gui.anyGuiFocused == false)
			{
				program.editor.set_selection_to(nullptr);
			}
		}
		else if (key == GLFW_MOUSE_BUTTON_RIGHT)
		{
			program.windowManager.hasFocus = true;
			// not doing much of anything here
		}
	}
}

void Input::cursor_pos_event(GLFWwindow* window, double xPos, double yPos)
{
	if (program.windowManager.hasFocus)
	{
		mouseMovement.x = xPos - mousePos.x;
		mouseMovement.y = yPos - mousePos.y;
		mousePos.x = xPos;
		mousePos.y = yPos;
		firstMouseMovement = false;
	}
}

void Input::scroll_event(GLFWwindow* window, double xOffset, double yOffset)
{
	if (program.file_system.contextOpen || program.gui.programGuiFocused) { return; }

	program.camera.moveCamera(program.camera.movementFront * (float)(yOffset * program.camera.zoomSpeed * program.deltaTime));
}

#include <main.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

/* TODO LIST:
 * // BUGS //
 * 
 * // UI //
 * [ ] Gui type selector
 * [ ] Remove remnants of blue from the properties panel
 * [ ] Remove texture selector feature and replace it with a file context opened by the properties panel
 * [ ] Add a check when trying to rename a GUI for any same-name GUIs that already exist, deny the change if there are any
 * // EDITOR //
 * [X] Remove all physics functionality
 * [ ] Replace "Tiles" with ImGui windows for elements, remove shaders and render.h / render.cpp
 * [ ] Make UI elements selectable by simply clicking them, with no multiselection
 * [ ] REMOVE tools entirely; simply use the window size draggers and dragging it instead
 * [ ] Add a check so EVERY gui wont get snapped if CTRL is pressed (use the selection to help?)
 * [ ] Snappable GUI movement
 * [ ] Overload the frame rounding style for RoundedGUIs
 * [ ] Monodirectional movement of GUIS (if pressing shift?)
 * [C] Create UI scaling by making the ui scalable only when the scale tool is selected
 * [C] Move tool with dragging and arrows
 * [C] Resize tool with squares
 * // OTHER //
 * [ ] Update blf converter object definitions and everything
 * [ ] Replace the trash texture atlas with just an array of every texture that has been loaded's data, and update it accordingly
 */

class GUI_Element;
class Location;
class Visuals;

Program program;

Program::Program()
{
	// setup and initialize components
	file_system.tryLoadConfigs();

	windowManager.initialize();
	input.setup();

	auto gui_1 = GUI_Element("Childless GUI :(");
	editor.elements.push_back(gui_1);
	editor.add_element(gui_1);

	auto gui_2 = GUI_Element("Cool GUI");
	gui_2.addSubElement(GUI_Element("Hi im a GUI object"));
	editor.elements.push_back(gui_2);
	editor.add_element(gui_2);

	// load UI textures
	std::cout << "Loading GUI textures..." << std::endl;
	file_system.loadGUITextures();
	std::cout << "Initializing GUI..." << std::endl;
	gui.guiInit(&windowManager);
}

void Program::loop()
{
	while (quitProgram == false)
	{
		// get delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		timeCounter += deltaTime;

		if (timeCounter >= 0.25f)
		{
			FPS = (int)((1.0f / deltaTime) + 0.5f);
			timeCounter = 0.0f;
		}

		glClearColor(40 / 255.0f, 120 / 255.0f, 20 / 255.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		input.processInput(windowManager.window);
		// update the camera view direction (not really needed but eh)
		camera.updateView();
		// window closing shenaniggas (I HATE THIS WORD)
		if (glfwWindowShouldClose(windowManager.window))
		{
			quitProgram = true;
			// show a confirmation window if unsaved progress exists
			if (program.editor.getDirtiness() == true)
			{
				quitProgram = false;
				program.gui.popupToggles[SAVE_CONTEXT] = true;
			}
		}
		gui.drawGui();
		glfwSwapBuffers(windowManager.window);
		glfwPollEvents();
	}
	
	std::cout << "Terminating program..." << std::endl;
	windowManager.terminate();
	file_system.trySaveConfigs();
}

int main()
{
	program.loop();
	return 0;
}

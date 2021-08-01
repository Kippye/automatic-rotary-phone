#include <program_gui.h>
#include <main.h>

#include <ImGuiFileDialog.h>
#include <glm/gtx/string_cast.hpp>
#include <libconfig.h++>

#include <iostream>
#include <iterator>
#include <algorithm>

class Program;
extern Program program;

// ran only once when either program starts
void Gui::guiInit(Window* windowManager)
{
	window = windowManager;

	IMGUI_CHECKVERSION();
	// imgui setup
	gui = ImGui::CreateContext();
	guiIO = &ImGui::GetIO();
	// set up platform / renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(windowManager->window, true);
	ImGui_ImplOpenGL3_Init("#version 430");

	// apply premade style
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4(0.03f, 0.09f, 0.05f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.05f, 0.14f, 0.08f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.04f, 0.11f, 0.06f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.10f, 0.27f, 0.14f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.03f, 0.07f, 0.04f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.24f, 0.10f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.05f, 0.30f, 0.11f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.40f, 0.17f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.24f, 0.11f, 0.94f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.09f, 0.44f, 0.18f, 0.94f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.04f, 0.20f, 0.09f, 0.94f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.07f, 0.27f, 0.13f, 0.94f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.04f, 0.11f, 0.05f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.03f, 0.20f, 0.07f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.04f, 0.25f, 0.09f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.04f, 0.34f, 0.12f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.05f, 0.24f, 0.10f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.05f, 0.30f, 0.11f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.09f, 0.44f, 0.18f, 0.94f);
	colors[ImGuiCol_Header] = ImVec4(0.05f, 0.24f, 0.10f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.05f, 0.30f, 0.11f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.09f, 0.44f, 0.18f, 0.94f);

	gd.normalButtonColor = colors[ImGuiCol_Button];
}

void Gui::Step(ImGuiSizeCallbackData* data) { float step = (float)(int)(intptr_t)data->UserData; data->DesiredSize = ImVec2((int)(data->DesiredSize.x / step + 0.5f) * step, (int)(data->DesiredSize.y / step + 0.5f) * step); }

void Gui::showGUIAdditionButtons()
{
	std::vector<GUI_TYPE>& gui_types = program.editor.GUI_types;

	for (int i = 0; i < gui_types.size(); i++)
	{
		if (ImGui::Button(program.editor.GUI_type_to_string[gui_types[i]].c_str()))
		{
			GUI_Element& addedGUI = GUI_Element();
			addedGUI.elementType = gui_types[i];
			addedGUI.name = std::string(program.editor.GUI_type_to_string[gui_types[i]]).append(" [").append(std::to_string(addedGUI.ID)).append("]");
			program.editor.elements.push_back(addedGUI);
			program.editor.add_element(addedGUI);
		}
		ImGui::SameLine();
	}
}

void Gui::showGUIObjectList()
{
	std::vector<GUI_TYPE>& guiObject_types = program.editor.GUIObject_types;

	for (int i = 0; i < guiObject_types.size(); i++)
	{
		if (ImGui::MenuItem(program.editor.GUI_type_to_string[guiObject_types[i]].c_str()))
		{
			GUI_Element& newGUIObject = GUI_Element(program.editor.GUI_type_to_string[guiObject_types[i]]);
			(*se.currentOpenedContext).addSubElement(newGUIObject);
		}
	}
}

void Gui::addPlacedGuis()
{
	std::vector<GUI_Element>& GUIs = program.editor.elements;

	for (int i = 0; i < GUIs.size(); i++)
	{
		if (program.input.ctrl_down) ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), Step, (void*)(intptr_t)100); // Fixed Step
		// make title bars only for WindowGUIs
		ImGuiWindowFlags windowFlags = GUIs[i].elementType == WindowGUI ? ImGuiWindowFlags_None : ImGuiWindowFlags_NoTitleBar;
		// set position
		ImGui::SetNextWindowPos(ImVec2(GUIs[i].location.Position.x, GUIs[i].location.Position.y), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(GUIs[i].location.Size.x, GUIs[i].location.Size.y), ImGuiCond_FirstUseEver);
		// draw rounded guis with rounded corners
		if (GUIs[i].elementType == RoundedGUI) ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);

		ImGui::Begin(GUIs[i].name.c_str(), (bool*)0, windowFlags | ImGuiWindowFlags_NoSavedSettings);
			// editing position or size through the properties panel
			if (!ImGui::IsWindowFocused())
			{
				if (GUIs[i].location.Position != glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y))
				{
					ImGui::SetWindowPos(ImVec2(GUIs[i].location.Position.x, GUIs[i].location.Position.y));
				}
				if (GUIs[i].location.Size != glm::vec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()))
				{
					ImGui::SetWindowSize(ImVec2(GUIs[i].location.Size.x, GUIs[i].location.Size.y));
				}
			}
			// dragging the window around or resizing it with the draggers
			else
			{
				// update position
				GUIs[i].location.Position.x = ImGui::GetWindowPos().x;
				GUIs[i].location.Position.y = ImGui::GetWindowPos().y;
				// update size
				GUIs[i].location.Size.x = ImGui::GetWindowWidth();
				GUIs[i].location.Size.y = ImGui::GetWindowHeight();
			}
			// select this in both the properties panel and the tree view
			if (ImGui::IsWindowFocused())
			{
				program.editor.set_selection_to(&GUIs[i]);
			}
		ImGui::End();

		if (GUIs[i].elementType == RoundedGUI) ImGui::PopStyleVar();
	}
}

void Gui::addPropertiesGui()
{
	bool* p_close = NULL;
	// Right side non-movable tab containing properties of selected tile
	ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(window->SCREEN_WIDTH / 4, -1));
	ImGui::SetNextWindowSize(ImVec2(s.propertiesPaneWidth, window->SCREEN_HEIGHT - s.bottomBarHeight));
	ImGui::SetNextWindowPos(ImVec2(window->SCREEN_WIDTH - s.propertiesPaneWidth, 0.0));
	ImGui::Begin("Element properties", p_close, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoNavInputs);
		s.propertiesPaneWidth = ImGui::GetWindowWidth();
		guiWantKeyboard = guiIO->WantCaptureKeyboard ? true : guiWantKeyboard;

		/// SELECTION SECTION
		if (program.editor.selection != nullptr) // we have something selected
		{
			/// general info
			// name input
			//TODO: FIX :(
			std::string inputText = program.editor.selection->name;
			if (ImGui::InputTextEx("Name", "Names must be unique", (char*)inputText.c_str(), 24, ImVec2(100, 40), ImGuiInputTextFlags_None))
			{
				program.editor.selection->name = inputText;
			}
			//ImGui::InputTextWithHint("Name", "You cannot use the same name twice", (char*)inputText.c_str(), 24, ImGuiInputTextFlags_CallbackCompletion, , inputText);
			/// location
			float pos[2] = { program.editor.selection->location.Position.x, program.editor.selection->location.Position.y };
			// position
			if (ImGui::InputFloat2("Position", pos))
			{
				program.editor.moveElement(program.editor.selection->ID, glm::vec2(pos[0], pos[1]));
			}
			float size[2] = { program.editor.selection->location.Size.x, program.editor.selection->location.Size.y };
			// size
			if (ImGui::InputFloat2("Size ", size))
			{
				program.editor.resizeElement(program.editor.selection->ID, glm::vec2(size[0], size[1]));
			}
			/// visual
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0.0f, 20.0f));
			// texture name TODO: replace with a file prompt to select an image file for the texture (then gets saved as just the texture filename)
			ImGui::Text(("Texture: " + program.editor.selection->visuals.textureName).c_str());
		}
		programGuiFocused = ImGui::IsWindowFocused() ? true : programGuiFocused;
	ImGui::End();
}

void Gui::addTreeGui()
{
	bool* p_close = NULL;

	ImGui::SetNextWindowSize(ImVec2(s.tileSelectorPaneWidth, window->SCREEN_HEIGHT - s.bottomBarHeight));
	ImGui::SetNextWindowSizeConstraints(ImVec2(-1, -1), ImVec2(-1, -1));
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));

	ImGui::Begin("GUI tree", p_close, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav); // Left side non-movable tab
		s.tileSelectorPaneWidth = ImGui::GetWindowWidth();

		int baseFlags = ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

		for (int i = 0; i < program.editor.elements.size(); i++)
		{
			GUI_Element& currentGUI = program.editor.elements[i];

			int usedFlags = baseFlags | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

			if (se.currentHierarchySelection == currentGUI.ID)
			{
				usedFlags |= ImGuiTreeNodeFlags_Selected;
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, s.guiTreeNodeHeight));

			bool parentOpen = ImGui::TreeNodeEx(currentGUI.name.c_str(), usedFlags);

			ImGui::PopStyleVar();

			if (ImGui::IsItemClicked())
			{
				se.currentHierarchySelection = currentGUI.ID;
			}

			bool itemHovered = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenOverlapped | ImGuiHoveredFlags_RectOnly);

			if (itemHovered) { se.currentHoveredNode = &currentGUI; }

			if (parentOpen)
			{
				std::vector<GUI_Element>& subElements = program.editor.elements[i].subElements;

				for (int j = 0; j < subElements.size(); j++)
				{
					GUI_Element subElement = subElements[j];

					usedFlags = baseFlags;
					if (se.currentHierarchySelection == subElement.ID)
					{
						usedFlags |= ImGuiTreeNodeFlags_Selected;
					}

					ImGui::TreeNodeEx(subElement.name.c_str(), usedFlags | ImGuiTreeNodeFlags_Leaf);
						if (ImGui::IsItemClicked())
						{
							se.currentHierarchySelection = subElement.ID;
						}
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
		}

		int index;

		program.editor.set_selection_to(program.editor.ID_to_element(se.currentHierarchySelection, index));

		// context menu when right clicking parent GUI nodes
		if (ImGui::BeginPopupContextWindow())
		{
			if (se.currentOpenedContext  != nullptr)
			{
				showGUIObjectList();
			}
			else if (se.currentHoveredNode != nullptr)
			{
				se.currentOpenedContext = se.currentHoveredNode;

				showGUIObjectList();
			}

			ImGui::EndPopup();
		}
		else
		{
			se.currentOpenedContext = nullptr;
		}

		// show all gui type addition buttons
		showGUIAdditionButtons();
		programGuiFocused = ImGui::IsWindowFocused() ? true : programGuiFocused;
	ImGui::End();
}

void Gui::addBottomBarGui()
{
	bool* p_close = NULL;

	// Bottom bar containing useful toggles and information
	ImGui::SetNextWindowSize(ImVec2(window->SCREEN_WIDTH, s.bottomBarHeight));
	//ImGui::SetNextWindowContentSize(ImVec2(32.0f, 16.0f));
	ImGui::SetNextWindowPos(ImVec2(0.0, window->SCREEN_HEIGHT - s.bottomBarHeight));
	ImGui::Begin("Bottom bar", p_close, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
		
		if (ImGui::Button("Load", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			openFileDialog(OPEN);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::Text("open a BLF file to edit ");
				ImGui::BulletText("shortcut: left CTRL + I ");
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::SetCursorPos(ImVec2(s.bottomBarButtonWidth + 20.0f, 0.0f));
		if (ImGui::Button("Save", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			openFileDialog(SAVE);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::Text("save changes to current file ");
				ImGui::BulletText("shortcut: left CTRL + O ");
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::SetCursorPos(ImVec2((s.bottomBarButtonWidth + 20.0f) * 2, 0.0f));
		if (ImGui::Button("Save as", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			openFileDialog(SAVE_AS);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::Text("save currently open blur level as a file ");
				ImGui::BulletText("shortcut: left CTRL + left SHIFT + O ");
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::SetCursorPos(ImVec2(window->SCREEN_WIDTH - ((s.bottomBarButtonWidth * 2) + 20), 0.0f));
		if (ImGui::Button("Reset camera", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			program.camera.cameraPos = glm::vec3(program.camera.cameraPos.x, program.camera.cameraPos.y, program.camera.blurZoomLevel);
		}
		ImGui::SetCursorPos(ImVec2(window->SCREEN_WIDTH - s.bottomBarButtonWidth, 0.0f));
		if (ImGui::Button("Status", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
		{
			popupToggles[STATUS] = !popupToggles[STATUS];
		}
		ImGui::SetCursorPos(ImVec2(window->SCREEN_WIDTH - s.bottomBarButtonWidth - 100, 0.0f));
		ImGui::Text(std::to_string(programGuiFocused).c_str());
	ImGui::End();
}

void Gui::addPopupGui()
{
	// Movable info panel showing information such as camera position, placed tile count and FPS
	if (popupToggles[STATUS])
	{
		if (ImGui::Begin("Info panel", &popupToggles[STATUS]))
		{
			guiWantKeyboard = guiIO->WantCaptureKeyboard ? true : guiWantKeyboard;
			programGuiFocused = ImGui::IsWindowFocused() ? true : programGuiFocused;

			ImGui::Text(("FPS: " + std::to_string(program.FPS)).c_str());
			ImGui::Text(("mouse pos: " + (glm::to_string(program.input.mousePos))).c_str());
			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
	}
	if (popupToggles[SAVE_CONTEXT])
	{
		if (ImGui::Begin("Unsaved changes", &popupToggles[SAVE_CONTEXT], ImGuiWindowFlags_AlwaysAutoResize))
		{
			guiWantKeyboard = guiIO->WantCaptureKeyboard ? true : guiWantKeyboard;
			programGuiFocused = ImGui::IsWindowFocused() ? true : programGuiFocused;

			if (ImGui::Button("Save and exit", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
			{
				openFileDialog(SAVE);
			}
			ImGui::SameLine();
			if (ImGui::Button("Exit", ImVec2(s.bottomBarButtonWidth, s.bottomBarHeight)))
			{
				popupToggles[SAVE_CONTEXT] = false;
				program.quitProgram = true;
			}
			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
	}
	// bla bla bla
}

void Gui::openFileDialog(GUI_PROMPT type)
{
	switch (type)
	{
		case DIR:
		{
			promptType = DIR;
			program.file_system.contextOpen = true;
			ImGuiFileDialog::Instance()->OpenDialog("ChooseContentFolder", "Select the content folder", nullptr, "C:\\", 1);
			// visual settings
			ImGuiFileDialog::Instance()->SetExtentionInfos(".", ImVec4(1, 1, 0.5f, 0.9f));
			break;
		}

		case OPEN:
		{
			promptType = OPEN;
			std::string startDir = "C:\\";
			if (program.file_system.blfDir != "")
			{
				startDir = program.file_system.blfDir;
			}
			else
			{
				startDir = program.file_system.scriptsDir == "" ? startDir : program.file_system.scriptsDir;
			}
			ImGuiFileDialog::Instance()->OpenDialog("OpenFile", "Load a BLF file", ".blf", startDir.c_str(), 1);
			break;
		}

		case SAVE:
		{
			if (program.file_system.blfDir != "") // we have a file open, save changes to it
			{
				program.blf_converter.write_file(program.file_system.blfDir.c_str());
			}
			else // we need to create a new file, open Save as dialog
			{
				openFileDialog(SAVE_AS);
			}
			break;
		}

		case SAVE_AS:
		{
			promptType = SAVE_AS;
			std::string startDir = "C:\\";
			if (program.file_system.blfDir != "")
			{
				startDir = program.file_system.blfDir;
			}
			else
			{
				startDir = program.file_system.scriptsDir == "" ? startDir : program.file_system.scriptsDir;
			}

			ImGuiFileDialog::Instance()->OpenDialog("SaveFileAs", "Save current BLF file as...", ".blf", startDir.c_str(), 1, nullptr, ImGuiFileDialogFlags_ConfirmOverwrite);
			break;
		}
	};
}

void Gui::checkFileDialog()
{
	// size constraints
	ImVec2 maxSize = ImVec2(window->SCREEN_WIDTH, window->SCREEN_HEIGHT);  // The full display area
	ImVec2 minSize = maxSize * 0.4f;

	switch (promptType)
	{
		case DIR:
		{
			if (ImGuiFileDialog::Instance()->Display("ChooseContentFolder", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
			{
				// action if OK
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					// change content dir
					program.file_system.scriptsDir = ImGuiFileDialog::Instance()->GetCurrentPath() + "\\";
					program.file_system.changeSetting<std::string>("dirs.content", program.file_system.scriptsDir, libconfig::Setting::TypeString);
				}

				// close
				program.file_system.contextOpen = false;
				ImGuiFileDialog::Instance()->Close();
			}
		}
		case OPEN:
		{
			// TODO: check if correct selection
			if (ImGuiFileDialog::Instance()->Display("OpenFile", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
			{
				// action if OK
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					std::string selectedFilename = ImGuiFileDialog::Instance()->GetCurrentFileName();
					if (selectedFilename.substr(selectedFilename.length() - 4, selectedFilename.length() - 3) != "." && selectedFilename.substr(selectedFilename.length() - 4) == ".blf")
					{
						// change currently selected blf
						program.file_system.blfDir = ImGuiFileDialog::Instance()->GetFilePathName();
						program.file_system.changeSetting<std::string>("dirs.blf", program.file_system.blfDir, libconfig::Setting::TypeString);
						// try to load the file
						program.blf_converter.load_file(ImGuiFileDialog::Instance()->GetFilePathName().c_str());

						// close
						program.file_system.contextOpen = false;
						ImGuiFileDialog::Instance()->Close();
					}
				}
				else
				{
					// close
					program.file_system.contextOpen = false;
					ImGuiFileDialog::Instance()->Close();
				}
			}
		}
		case SAVE_AS:
		{
			// TODO: check if correct selection
			if (ImGuiFileDialog::Instance()->Display("SaveFileAs", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
			{
				// action if OK
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					std::string selectedFilename = ImGuiFileDialog::Instance()->GetCurrentFileName();
					if (selectedFilename.substr(selectedFilename.length() - 4, selectedFilename.length() - 3) != "." && selectedFilename.substr(selectedFilename.length() - 4) == ".blf")
					{
						// change currently selected blf
						program.file_system.blfDir = ImGuiFileDialog::Instance()->GetFilePathName();
						program.file_system.changeSetting<std::string>("dirs.blf", program.file_system.blfDir, libconfig::Setting::TypeString);
						// try to save the file
						program.blf_converter.write_file(ImGuiFileDialog::Instance()->GetFilePathName().c_str());

						// close
						program.file_system.contextOpen = false;
						ImGuiFileDialog::Instance()->Close();
						// close the program if this was our last save
						if (glfwWindowShouldClose(program.windowManager.window))
						{
							program.quitProgram = true;
						}
					}
				}
				else
				{
					// close
					program.file_system.contextOpen = false;
					ImGuiFileDialog::Instance()->Close();
				}
			}
		}
	};
}

void Gui::drawGui()
{
	guiWantKeyboard = false;
	programGuiFocused = false;
	// new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// add guis to be rendered
	addPlacedGuis();
	addPropertiesGui();
	addTreeGui();
	addBottomBarGui();
	addPopupGui();

	ImGui::ShowDemoWindow();

	// check if any window is focused and/or hovered (for input capturing purposes)
	guiHovered = guiIO->WantCaptureMouse;
	anyGuiFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow);

	checkFileDialog();

	ImGui::Render();
	// check for right / middle click defocus
	if (gui->HoveredWindow == NULL && gui->NavWindow != NULL && (guiIO->MouseClicked[1] || guiIO->MouseClicked[2]) /* could cause issues, who cares? && GetFrontMostPopupModal() == NULL*/)
	{
		ImGui::FocusWindow(NULL);
	}

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

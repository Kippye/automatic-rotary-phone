#pragma once

#include <textures.h>
#include <window.h>
#include <gui_element.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <map>

enum GUI_POPUP
{
	STATUS,
	SAVE_CONTEXT
};

enum GUI_PROMPT
{
	DIR,
	OPEN,
	SAVE,
	SAVE_AS
};

struct GuiSizes
{
	public:
		// editor pane size variables
		float
		propertiesPaneWidth = 64.0f * 4,
		tileSelectorPaneWidth = 64.0f * 4,
		guiTreeNodeHeight = 6.0f,
		bottomBarButtonWidth = 100.0f,
		bottomBarHeight = 32.0f;
};

struct GuiSelections
{
	public:
		int currentTextureSelection = 0;
		int currentHierarchySelection = 0;
		char selectedGUINameBuffer[24] = {};
		GUI_Element* currentHoveredNode = nullptr;
		GUI_Element* currentOpenedContext = nullptr;
};

struct GuiData
{
	public:
		float f64_free = 0.1f,
		f64_snap = 1.0f;
		ImVec4 normalButtonColor;
		ImVec4 selectedButtonColor = ImVec4(0.00f, 0.51f, 1.00f, 1.00f);
};

class Gui
{
	private:
		// TEMP
		float lastPos[2] = { 0.0f, 0.0f };

		GuiSizes s;
		GuiData gd;
	public:
		GuiSelections se;
		GUI_PROMPT promptType = DIR;
		bool anyGuiFocused = false, programGuiFocused = false, guiHovered = false, guiWantKeyboard = false;
		std::map<std::string, unsigned int> guiTextures = {};
		std::map<GUI_POPUP, bool> popupToggles =
		{
			{ STATUS, false },
			{ SAVE_CONTEXT, false}
		};

		std::vector<E_Texture*>& tileTextures = std::vector<E_Texture*>{};
		ImGuiIO* guiIO;
		ImGuiContext* gui;
		Window* window;
	public:
		void guiInit(Window* windowManager);
		void drawGui();
		void openFileDialog(GUI_PROMPT type);
	private:
		static void Step(ImGuiSizeCallbackData* data);
		void showGUIAdditionButtons();
		void showGUIObjectList();

		void addPlacedGuis();
		void addTreeGui();
		void addPropertiesGui();
		void addBottomBarGui();
		void addPopupGui();

		void checkFileDialog();
};

#pragma once

#include <textures.h>
#include <gui_element.h>
#include <gui_object.h>
#include <glm.hpp>
#include <vector>
#include <map>

class GUI;
class GUI_Object;

struct GUI_Options
{
	public:
		Location location = Location(glm::vec2(0.0f), glm::vec2(1.0f));
		Visuals visuals = Visuals();
};

class Editor
{
	private:
		bool dirty = false; // are there any unsaved changes?
    public:
		std::vector<std::string> GUI_type_to_string =
		{
			"None",
			"GUI",
			"WindowGUI",
			"RoundedGUI",
			"Label",
			"Button",
			"Texture",
			"TextureButton",
			"Slider",
			"ProgressBar"
		};

		std::vector<GUI_TYPE> GUI_types = { GUI, WindowGUI, RoundedGUI };
		std::vector<GUI_TYPE> GUIObject_types = { Label, Button, Texture, TextureButton, Slider, ProgressBar };
		// editor data
        std::vector<GUI_Element> elements = {};
		// tool data
        glm::vec2& toolPos = glm::vec2(0.0f);
        glm::vec2 cachedToolPos = glm::vec2(0.0f);

		GUI_Element* selection = nullptr;
		GUI_Options nextElement;

		Editor();

		// properties
		bool getDirtiness();
		void setDirtiness(bool);

		// utility functions
		bool checkForOverlaps(Bounding_Box &box, glm::vec2 &pos);
		GUI_Element* positionToElement(glm::vec2 &pos, int &index, bool grid = false);
		GUI_Element* ID_to_element(int ID, int &index);
		std::vector<GUI_Element*>* getElementsInArea(Bounding_Box area, glm::vec2 &pos, std::vector<int> &indices);

	private:
		void Editor::update_element_selected(GUI_Element* element, int index, bool to);
		void Editor::update_element_selected(int ID, bool to);
	public:
		void set_selection_to(GUI_Element* element);
		void delete_selection();

        void moveElement(int index, glm::vec2 newPos);
        void moveElement(unsigned int ID, glm::vec2 newPos);
        void resizeElement(int index, glm::vec2 newSize);
		void resizeElement(unsigned int ID, glm::vec2 newSize);
		void changeTileVisuals(int index, Visuals visuals);
		void changeTileVisuals(unsigned int ID, Visuals visuals);

        void add_element(GUI_Element &element);
        void add_element(std::vector<GUI_Element> &elements);
        void remove_element(int index = -1);
        void remove_element(std::vector<int> &indices);
};

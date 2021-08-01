#pragma once

#include <util.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <vector>

enum GUI_TYPE
{
	None,
	GUI,
	WindowGUI,
	RoundedGUI,
	Label,
	Button,
	Texture,
	TextureButton,
	Slider,
	ProgressBar
};

class Location
{
	public:
		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec2 Size = glm::vec2(50.0f);
		Bounding_Box box;

		Location(glm::vec2 _Position = glm::vec2(0.0f), glm::vec2 _Size = glm::vec2(50.0f));
};

class Visuals
{
	// TODO: we don't use this garbage anymore, update to an array index or something.
	public:
		glm::vec2 atlasCoords = glm::vec2(0.0f);
		std::string textureName = "";

		Visuals(glm::vec2 _atlasCoords = glm::vec2(1.0f), std::string _textureName = "");
};


class GUI_Element
{
	private:
		static inline unsigned int lastID = 0;
	public:
		static glm::vec2 windowSize;
		unsigned int ID = 0;
		bool selected = false;
		Location location;
		Visuals visuals;

		GUI_TYPE elementType = None;
		std::string name = "";
		std::vector<GUI_Element> subElements = {}; // empty if not a GUI

		void addSubElement(GUI_Element element);
		void removeSubElement(int index);
		void removeSubElement(GUI_Element element);

		GUI_Element(std::string _name = "");
		GUI_Element(Location _location, Visuals _visuals, std::string _name = "");
};

// operators
inline bool operator==(const GUI_Element& element1, const GUI_Element& element2)
{
	if (&element1 != &element2)
	{
		return false;
	}
	
	if (&element1 == nullptr && &element2 == nullptr)
	{
		return true;
	}

	return element1.ID == element2.ID; 
}
inline bool operator!=(const GUI_Element& element1, const GUI_Element& element2) { return !(element1 == element2); }

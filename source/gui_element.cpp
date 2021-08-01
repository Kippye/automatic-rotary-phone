#include <gui_element.h>

glm::vec2 GUI_Element::windowSize;

// empty constructor - default tile
GUI_Element::GUI_Element(std::string _name)
{
	ID = lastID;
	lastID++;

	location.Position = windowSize / 2.0f;

	name = _name;
}

GUI_Element::GUI_Element(Location _location, Visuals _visuals, std::string _name)
{
	ID = lastID;
	lastID++;

	location = _location;
	visuals = _visuals;
	name = _name;
}

void GUI_Element::addSubElement(GUI_Element element)
{
	subElements.push_back(element);
}

void GUI_Element::removeSubElement(int index)
{
	subElements.erase(subElements.begin() + index);
}

void GUI_Element::removeSubElement(GUI_Element element)
{
	for (int i = 0; i < subElements.size(); i++)
	{
		if (subElements[i].name == element.name)
		{
			subElements.erase(subElements.begin() + i);
			return;
		}
	}
}

// initialize with positiong
Location::Location(glm::vec2 _Position, glm::vec2 _Size)
{
	Position = _Position;
	Size = _Size;
	box = Bounding_Box(Size);
}

Visuals::Visuals(glm::vec2 _atlasCoords, std::string _textureName) // every setting cus too lazy to make 1 by 1
{
	atlasCoords = _atlasCoords;
	textureName = _textureName;
}
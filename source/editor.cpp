#include <main.h>
#include <editor.h>
#include <iostream>
#include <algorithm>
#include <iterator>

// TEMP
#include <glm/gtx/string_cast.hpp>

class Program;
extern Program program;

Editor::Editor(){}

bool Editor::getDirtiness()
{
	return dirty;
}

void Editor::setDirtiness(bool to)
{
	dirty = to;

	program.windowManager.setTitle(dirty == true ? std::string(program.windowTitle).append(" *").c_str() : program.windowTitle);
}

bool Editor::checkForOverlaps(Bounding_Box &box, glm::vec2 &pos)
{
	for (int index = elements.size() - 1; index >= 0; index--)
	{
		if (box.overlaps(pos, elements[index].location.Position, elements[index].location.box))
		{
			return true;
		}
	}

	return false;
}

GUI_Element* Editor::positionToElement(glm::vec2 &pos, int &index, bool grid)
{
	// reverse loop so we get the top-most element and also speed up removing recently placed elements
	for (index = elements.size() - 1; index >= 0; index--)
	{
		if (elements[index].location.box.contains_position(elements[index].location.Position, pos))
		{
			return &elements[index]; // return so we only get 1 element each click, otherwise shit would be pretty weird, innit?
		}
	}
	return nullptr;
}

// for area tools like box selection
std::vector<GUI_Element*>* Editor::getElementsInArea(Bounding_Box area, glm::vec2 &pos, std::vector<int> &indices)
{
	std::vector<GUI_Element*>* elementsInArea = new std::vector<GUI_Element*>{};
	for (int i = elements.size() - 1; i >= 0; i--)
	{
		if (area.contains(pos, elements[i].location.Position, elements[i].location.box))
		{
			indices.push_back(i);
			elementsInArea->push_back(&elements[i]);
		}
	}

	return elementsInArea;
}

GUI_Element* Editor::ID_to_element(int ID, int &index)
{
	// reverse loop so we get the top-most element and also speed up removing recently placed elements
	for (index = elements.size() - 1; index >= 0; index--)
	{
		if (elements[index].ID == ID)
		{
			return &elements[index]; // return so we only get 1 element each click, otherwise shit would be pretty weird, innit?
		}
	}
	return nullptr;
}

void Editor::update_element_selected(GUI_Element* element, int index, bool to)
{
	element->selected = to;
	// TODO program.render.set_element_selection(index, to);
}

void Editor::update_element_selected(int ID, bool to)
{
	int index = 0;
	GUI_Element* element = ID_to_element(ID, index);
	update_element_selected(element, index, to);
}

void Editor::set_selection_to(GUI_Element* element)
{
	program.gui.se.currentHierarchySelection = element->ID;
	selection = element;
}

void Editor::delete_selection()
{
	if (selection == nullptr) { return; }

	int ID = selection->ID;

	selection = nullptr;

	int index = 0;
	ID_to_element(ID, index);

	remove_element(index);

	setDirtiness(true);
}

void Editor::moveElement(int index, glm::vec2 newPos)
{
	// snap position to the grid if needed
	if (program.input.ctrl_down)
	{
		newPos = mymath::round_to_grid(newPos);
	}

	elements[index].location.Position.x = newPos.x;
	elements[index].location.Position.y = newPos.y;
	// update visuals
	// TODO program.render.instanceTransformData[index].x = newPos.x;
	// program.render.instanceTransformData[index].y = newPos.y;
	// program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_1);

	setDirtiness(true);
}

void Editor::moveElement(unsigned int ID, glm::vec2 newPos)
{
	int index = 0;
	GUI_Element* element = ID_to_element(ID, index);

	// snap position to the grid if needed
	if (program.input.ctrl_down)
	{
		newPos = mymath::round_to_grid(newPos);
	}

	element->location.Position.x = newPos.x;
	element->location.Position.y = newPos.y;
	// update visuals
	// TODO program.render.instanceTransformData[index].x = newPos.x;
	// program.render.instanceTransformData[index].y = newPos.y;
	// program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_1);

	setDirtiness(true);
}

void Editor::resizeElement(int index, glm::vec2 newSize)
{
	// snap even the size to the grid if needed
	if (program.input.ctrl_down)
	{
		newSize = mymath::round_to_grid(newSize);
	}

	elements[index].location.Size.x = newSize.x;
	elements[index].location.Size.y = newSize.y;
	elements[index].location.box.update_size(newSize);
	// update visuals
	// TODO program.render.instanceTransformData[index].z = newSize.x;
	// program.render.instanceTransformData[index].w = newSize.y;
	// program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_1);

	setDirtiness(true);
}

void Editor::resizeElement(unsigned int ID, glm::vec2 newSize)
{
	int index = 0;
	GUI_Element* element = ID_to_element(ID, index);

	// snap even the size to the grid if needed
	if (program.input.ctrl_down)
	{
		newSize = mymath::round_to_grid(newSize);
	}

	element->location.Size.x = newSize.x;
	element->location.Size.y = newSize.y;
	elements[index].location.box.update_size(newSize);
	// update visuals
	// TODO program.render.instanceTransformData[index].z = newSize.x;
	// program.render.instanceTransformData[index].w = newSize.y;
	// program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_1);

	setDirtiness(true);
}

void Editor::changeTileVisuals(int index, Visuals visuals)
{
	elements[index].visuals = visuals;
	// TODO program.render.instanceAdditionalData[index].x = visuals.atlasCoords.x;
	// program.render.instanceAdditionalData[index].y = visuals.atlasCoords.y;
	// program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_2);

	setDirtiness(true);
}

void Editor::changeTileVisuals(unsigned int ID, Visuals visuals)
{
	int index = 0;
	GUI_Element* element = ID_to_element(ID, index);
	element->visuals = visuals;

	// TODO program.render.instanceAdditionalData[index].x = visuals.atlasCoords.x;
	// program.render.instanceAdditionalData[index].y = visuals.atlasCoords.y;
	// program.render.updateInstanceArray(INSTANCE_ARRAY_UPDATE_2);

	setDirtiness(true);
}

void Editor::add_element(GUI_Element &element)
{
	// program.render.add_to_render_list(element);
	setDirtiness(true);
}

void Editor::add_element(std::vector<GUI_Element> &elements)
{
	// program.render.add_to_render_list(elements);
	setDirtiness(true);
}

void Editor::remove_element(int index)
{
	// remove from the elements vector as well
	if (index != -1)
	// program.render.remove_from_render_list(index);
	elements.erase(std::begin(elements) + index);
	std::cout << elements.size() << std::endl;
	setDirtiness(true);
}

void Editor::remove_element(std::vector<int> &indices)
{
	if (indices.size() == 0) { return; } // don't dirty if we do nothing lol

	// program.render.remove_from_render_list(indices);

	for (int index : indices)
	{
		// remove from the elements vector as well
		if (index != -1)
		elements.erase(std::begin(elements) + index);
	}

	setDirtiness(true);
}

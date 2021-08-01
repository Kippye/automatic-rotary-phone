#include <blf_converter.h>
#include <main.h>
#include <iostream>
#include <chrono>

class Program;
extern Program program;

void BLF_Converter::load_file(const char* path)
{
	std::cout << "(BLF) Loading of file at " << path << " requested." << std::endl;

	// READING
	auto start_time = std::chrono::high_resolution_clock::now();

	BLFFile readFile;

	ObjectTable objects = {
		createDefinition<BLF_Element>(),
	};

	blf::readFile(path, readFile, objects);

	// we're not gonna copy because we're retarded :D
	DataTable& data = readFile.data;
	DataGroup<BLF_Element> elements = data.get<BLF_Element>();

	//program.render.instanceTransformData.clear();
	//program.render.instanceAdditionalData.clear();
	program.editor.elements.clear();

	for (BLF_Element* element : elements)
	{
		std::string texturePath = element->texture;
		//std::cout << texturePath << std::endl;
		// TODO if (program.textureLoader.getAtlasTextureCoords(program.render.textureAtlas, texturePath) == glm::vec2(-1, -1))

		// TODO program.editor.add_element(program.editor.elements.emplace_back(Location(glm::vec4(element->x, element->y, element->z, 0.0)), Visuals(program.textureLoader.getAtlasTextureCoords(program.render.textureAtlas, texturePath), texturePath)));
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
	std::cout << "(BLF) File read in: " << duration << " milliseconds." << std::endl;
}

void BLF_Converter::write_file(const char* path)
{
	// WRITING
	std::vector<BLF_Element> elements;

	DataTable data;

	// Maybe i have to add another element type here to separate GUI objects and GUIs
	ObjectTable objects = {
		createDefinition<BLF_Element>()
	};

	std::vector<GUI_Element>& editor_elements = program.editor.elements;

	std::cout << "(BLF) Writing " << editor_elements.size() << " UI elements." << std::endl;

	// dunno if i need this: if (editor_tiles.size() == 0) { std::cerr << "Tried to write 0 tiles" << std::endl; return; }

    elements.reserve(editor_elements.size());
	
	for (int i = 0; i < editor_elements.size(); i++)
	{
		// std::cout << program.textureLoader.getAtlasTexturePath(program.render.textureAtlas, editor_elements[i].visuals.atlasCoords) << std::endl;
		//elements.emplace_back(
		//	editor_elements[i].location.Position.x,
		//	editor_elements[i].location.Position.y,
		//	editor_elements[i].location.Position.z,
		//	editor_elements[i].location.Size.x,
		//	editor_elements[i].location.Size.y,
  //          program.textureLoader.getAtlasTexturePath(program.render.textureAtlas, editor_elements[i].visuals.atlasCoords)
  //      );

		data.addObject(&(elements[i]));
	}

	auto start_time = std::chrono::high_resolution_clock::now();

	writeFile(path, objects, data);

	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

	std::cout << "Write completed in: " << duration << " milliseconds." << std::endl;
	program.editor.setDirtiness(false);
}

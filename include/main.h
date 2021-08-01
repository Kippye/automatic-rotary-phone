#pragma once

#include <window.h>
#include <file_system.h>
#include <blf_converter.h>
#include <textures.h>
#include <camera.h>
#include <input.h>
#include <editor.h>
#include <program_gui.h>

class Window;
class FileSystem;
class BLF_Converter;
class TextureLoader;
class Camera;
class Input;
class Editor;

class Program
{
	public:
		Window& windowManager = *(new Window());
		FileSystem& file_system = *(new FileSystem());
		BLF_Converter& blf_converter = *(new BLF_Converter());
		TextureLoader& textureLoader = *(new TextureLoader());
		Camera& camera = *(new Camera());
		Input& input = *(new Input());
		Editor editor = *(new Editor());
		Gui& gui = *(new Gui());

		float deltaTime, lastFrame, timeCounter;
		int FPS;
		const char* windowTitle = "automatic rotary phone";
		bool quitProgram = false;

		Program(const Program&) = default;
		Program& operator=(const Program&) = default;
		Program();
		void loop();
};

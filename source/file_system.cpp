#include <file_system.h>
#include <main.h>

#include <filesystem>
#include <sstream>

#define DEBUG_FILE_LOADING false

class Program;
extern Program program;

std::vector<std::string>& FileSystem::getInDir(const char* directory, bool filesOnly, bool fullPath, bool extension, std::vector<std::string> acceptedExtensions)
{
	std::vector<std::string>* fileNames = new std::vector<std::string>();

	if (directory == "") { if (DEBUG_FILE_LOADING) std::cout << "Tried getting a directory without dir being set" << std::endl; return *fileNames; }

	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		if (filesOnly && entry.path().extension().string() == "") { continue; } // we only want files, not directories

		if (acceptedExtensions.size() > 0)
		{
			bool isAcceptedExtension = false;

			for (int i = 0; i < acceptedExtensions.size(); i++)
			{
				if (entry.path().extension().string() == acceptedExtensions[i])
				{
					isAcceptedExtension = true;
					break;
				}
			}
			// this file does not have an accepted extension
			if (!isAcceptedExtension)
			{
				continue;
			}
		}

		if (DEBUG_FILE_LOADING) std::cout << "Found file: " << entry.path() << "in " << directory << std::endl;

		if (fullPath) // dir/fn.ext
		{
			if (extension) // dir/fn.ext
			{
				fileNames->push_back(entry.path().string());
			}

			else // dir/fn
			{
				std::filesystem::path path = entry.path();
				path.replace_extension();
				fileNames->push_back(path.string());
			}
		}
		// add only filename + extension
		else
		{
			if (extension) // fn.ext
			{
				fileNames->push_back(entry.path().filename().string());
			}

			else // fn
			{
				fileNames->push_back(entry.path().stem().string());
			}
		}
	}

	return *fileNames;
}

std::vector<std::string>& FileSystem::getInDirRecursive(const char* directory, bool filesOnly, bool fullPath, bool extension, std::vector<std::string> acceptedExtensions)
{
	std::vector<std::string>* fileNames = new std::vector<std::string>();

	if (directory == "") { if (DEBUG_FILE_LOADING) std::cout << "Tried loading without dir being set" << std::endl; return *fileNames; }

	for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
	{
		if (filesOnly && entry.path().extension().string() == "") { continue; } // we only want files, ignore it

		if (acceptedExtensions.size() > 0)
		{
			bool isAcceptedExtension = false;

			for (int i = 0; i < acceptedExtensions.size(); i++)
			{
				if (entry.path().extension().string() == acceptedExtensions[i])
				{
					isAcceptedExtension = true;
					break;
				}
			}
			// this file does not have an accepted extension
			if (!isAcceptedExtension)
			{
				continue;
			}
		}

		if (DEBUG_FILE_LOADING) std::cout << "found file: " << entry.path() << std::endl;

		if (fullPath) // dir/fn.ext
		{
			if (extension) // dir/fn.ext
			{
				fileNames->push_back(entry.path().string());
			}

			else // dir/fn
			{
				std::filesystem::path path = entry.path();
				path.replace_extension();
				fileNames->push_back(path.string());
			}
		}
		// add only filename + extension
		else
		{
			if (extension) // fn.ext
			{
				fileNames->push_back(entry.path().filename().string());
			}

			else // fn
			{
				fileNames->push_back(entry.path().stem().string());
			}
		}
	}

	return *fileNames;
}

void FileSystem::loadGUITextures()
{
	if (DEBUG_FILE_LOADING) std::cout << "loading GUI textures" << std::endl;
	std::string guiTextureFolder = program.textureLoader.textureFolder + "gui/";
	std::vector<std::string>& filesInFolder = getInDirRecursive(guiTextureFolder.c_str(), true, false, true, imageExtensions);
	std::vector<std::string>& fileNames = getInDirRecursive(guiTextureFolder.c_str(), true, false, false, imageExtensions);
	std::vector<E_Texture*>& textures = program.textureLoader.loadTextures(filesInFolder, guiTextureFolder);

	for (int i = 0; i < fileNames.size(); i++)
	{
		program.gui.guiTextures.insert({ fileNames[i], textures[i]->ID });
	}
	if (DEBUG_FILE_LOADING) std::cout << "GUI textures successfully loaded" << std::endl;
}

void FileSystem::tryLoadConfigs()
{
	/// set file format options
	config.setOptions
	(
		Config::OptionFsync
		| Config::OptionSemicolonSeparators
		| Config::OptionColonAssignmentForGroups
		| Config::OptionOpenBraceOnSeparateLine
	);

	/// read file or error and quit
	try
	{
		config.readFile(config_file);
	}
	catch (const FileIOException& fioex)
	{
		std::cerr << "I/O error while reading user configuration." << std::endl;
		return;
	}
	catch (const ParseException& pex)
	{
		std::cerr << "parse error at " << pex.getFile() << ":" << pex.getLine()
			<< " - " << pex.getError() << std::endl;
		return;
	}

	/// find settings or set to defaults if they dont exist
	Setting& root = config.getRoot();

	if (!root.exists("dirs"))
		root.add("dirs", Setting::TypeGroup);

	Setting& dirs = root["dirs"];

	/// get saved directories
	if (!dirs.exists("scripts"))
	{
		dirs.add("scripts", Setting::TypeString);
		dirs["scripts"] = "";
	}
	if (!dirs.exists("blf"))
	{
		dirs.add("blf", Setting::TypeString);
		dirs["blf"] = "";
	}

	std::string stringScripts = dirs["scripts"].c_str();
	if (!std::filesystem::exists(std::filesystem::path(stringScripts)))
	{
		dirs["scripts"] = "";
		stringScripts = "";
	}
	scriptsDir = stringScripts;

	std::string stringBlf = dirs["blf"].c_str();
	if (!std::filesystem::exists(std::filesystem::path(stringBlf)))
	{
		dirs["blf"] = "";
		stringBlf = "";
	}
	blfDir = stringBlf;
}

void FileSystem::trySaveConfigs()
{
	// Write out the updated configuration.
	try
	{
		config.writeFile(config_file);
		std::cerr << "configuration successfully written to: " << config_file << std::endl;
	}
	catch (const FileIOException& fioex)
	{
		std::cerr << "I/O error while writing file: " << config_file << std::endl;
		return;
	}
}

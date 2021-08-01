#pragma once

#include <blf.hpp>
using namespace blf;

class BLF_Element : public TemplateObject
{
	public:
		double x;
		double y;
		double z;
		double sizeX;
		double sizeY;
		String texture;

		BLF_Element() {}

		BLF_Element(double x, double y, double z, double sizeX, double sizeY, String texture)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->sizeX = sizeX;
			this->sizeY = sizeY;
			this->texture = texture;
		}

		const char* getObjectName() const override
		{
			return "BLF_Element";
		};

		std::vector<ObjectAttribute> getAttributeMap() override
		{
			return
			{
				{"X", &x, TYPE_DOUBLE},
				{"Y", &y, TYPE_DOUBLE},
				{"Z", &z, TYPE_DOUBLE},
				{"SizeX", &sizeX, TYPE_DOUBLE},
				{"SizeY", &sizeY, TYPE_DOUBLE},
				{"Texture", &texture, TYPE_STRING}
			};
		}

		void storeForeignAttributes(ForeignAttributeTable table) override
		{
			// Nowhere to store these, instead we're gonna print them
			for (ObjectAttribute attr : table.attributes)
			{
				std::cout << "Attribute of type " << attr.attribType << " which has the name of: " << attr.name << std::endl;
			}
		}
};

class BLF_Converter
{
	public:
		void load_file(const char* path);
		void write_file(const char* path);
};

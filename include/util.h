#pragma once

#include <glm.hpp>

class mymath
{
	public:
		static glm::vec2 round_to_grid(glm::vec2 pos)
		{
			pos.x = floor(pos.x);
			pos.y = floor(pos.y);
			return pos;
		}
		static glm::vec4 round_to_grid(glm::vec4 pos)
		{
			pos.x = floor(pos.x);
			pos.y = floor(pos.y);
			return pos;
		}
};

class Bounding_Box
{
	public:
		glm::vec2 minimum, maximum;
		glm::vec2 size;

	public:
		Bounding_Box()
		{
			size = glm::vec2(1.0f);
		}

		Bounding_Box(glm::vec2 size)
		{
			update_size(size);
		}

		void update_size(glm::vec2 size)
		{
			this->size = size;
			// handle negative sizes correctly as well
			minimum.x = size.x > 0 ? 0.0f : size.x;
			minimum.y = size.y > 0 ? 0.0f : size.y;
			maximum.x = size.x > 0 ? size.x : 0.0f;
			maximum.y = size.y > 0 ? size.y : 0.0f;
		}

		bool contains_position(glm::vec2 &boxPos, glm::vec2 &sourcePos)
		{
			return sourcePos.x >= (minimum.x + boxPos.x) && sourcePos.y >= (minimum.y + boxPos.y)
			&& sourcePos.x <= (maximum.x + boxPos.x) && sourcePos.y <= (maximum.y + boxPos.y);
		}

		bool overlaps(glm::vec2 &boxPos, glm::vec2  &sourcePos, Bounding_Box &sourceBox)
		{
			return
			(
				contains_position(boxPos, sourcePos + sourceBox.minimum + glm::vec2(0.01f)) ||
				contains_position(boxPos, sourcePos + glm::vec2(sourceBox.minimum.x + sourceBox.size.x - 0.01f, sourceBox.minimum.y + 0.01f)) ||
				contains_position(boxPos, sourcePos + glm::vec2(sourceBox.minimum.x + 0.01f, sourceBox.minimum.y + sourceBox.size.y - 0.01f)) ||
				contains_position(boxPos, sourcePos + sourceBox.maximum - glm::vec2(0.01f))
			);
		}

		bool contains(glm::vec2 &boxPos, glm::vec2 &sourcePos, Bounding_Box &sourceBox)
		{
			return
			(
				contains_position(boxPos, sourcePos + sourceBox.minimum + glm::vec2(0.01f)) &&
				contains_position(boxPos, sourcePos + glm::vec2(sourceBox.minimum.x + sourceBox.size.x - 0.01f, sourceBox.minimum.y + 0.01f)) &&
				contains_position(boxPos, sourcePos + glm::vec2(sourceBox.minimum.x + 0.01f, sourceBox.minimum.y + sourceBox.size.y - 0.01f)) &&
				contains_position(boxPos, sourcePos + sourceBox.maximum - glm::vec2(0.01f))
			);
		}
};

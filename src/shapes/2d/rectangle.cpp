#include "../../../include/shapes/2d/rectangle.hpp"

Rectangle::Rectangle() {
	this->set_vbo_and_ebo(this->vertices, this->indices);
}

Rectangle::Rectangle(const float x, const float y, const float width, const float height, const float rotate)
	: Shape2D(x, y, width, height, rotate) {}

Rectangle::Rectangle(const glm::vec2 position, const glm::vec2 size, const float rotate)
	: Shape2D(position, size, rotate) {}

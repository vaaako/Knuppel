#pragma once

#include "../opengl/shader_program.hpp"
#include "../opengl/vao.hpp"
#include "../opengl/vbo.hpp"
#include "../opengl/ebo.hpp"
#include "../types/color.hpp"
#include <algorithm>
#include <memory>
#include <vector>


class BaseShape {
	public:
		BaseShape(const float x, const float y, const std::vector<unsigned int>& indices);
		BaseShape(const float x, const float y, const std::vector<unsigned int>& indices, const Color& color, const int vertices_count = 3);

		// Be sure colors has the right amount of vertices
		BaseShape(const float x, const float y, const std::vector<unsigned int>& indices, const std::vector<float>& colors);

		~BaseShape() = default;

		virtual inline void draw() {
			// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			// this->shader->use();
			// this->vao->bind();
			// this->draw_array();
			// this->vao->unbind();

			this->shader->use();
			this->vao->bind();
			this->draw_array();
			this->vao->unbind();
		}

	protected:
		const float x;
		const float y;
		
		// VBO* vbo = nullptr;
		std::unique_ptr<VBO> vbo; // VBO may use store_data after

		// Make VAO right away
		const std::unique_ptr<const VAO> vao = std::make_unique<VAO>(); 
		std::unique_ptr<const ShaderProgram> shader = nullptr; // If try to make ShaderProgram here, the shaders will not be initialized yet
		
		// const VAO vao = VAO();
		// const ShaderProgram* shader = nullptr;

		virtual inline void draw_array() const = 0;

	private:
		// const EBO* ebo = nullptr;
		std::unique_ptr<const EBO> ebo = nullptr;

		const char* vertexShaderSource = R"(
			#version 330 core

			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec3 aColor;
			
			out vec3 ourColor; // output a color to the fragment shader

			void main() {
				gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
				ourColor = aColor; // Set ourColor to the input color got from the vertex data
			}
		)";

		const char* fragmentShaderSource = R"(
			#version 330 core

			out vec4 FragColor;
			in vec3 ourColor; // Get ourColor from vertexShader

			void main() {
				// FragColor = vec4(1.0, 0.5, 0.2, 1.0);
				FragColor = vec4(ourColor, 1.0);
			}
		)";
};

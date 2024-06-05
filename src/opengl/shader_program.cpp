#include "Dynastinae/opengl/shader_program.hpp"
#include "Dynastinae/utils/log.hpp"
#include <cstddef>

ShaderProgram::ShaderProgram(const char* vertex_shader_source, const char* fragment_shader_source) {
	// Compile shaders
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
	glCompileShader(vertex_shader);

	// Check vertex shader error
	GLint success;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		GLchar info_log[512];
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		LOG_ERROR("\nVertex shader compilation failed: %s", info_log);
	}

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
	glCompileShader(fragment_shader);

	// Check fragment shader error
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		GLchar info_log[512];
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		LOG_ERROR("\nFragment shader compilation failed: %s", info_log);
	}


	// Create shader program
	this->id = glCreateProgram(); // Set program ID
	glAttachShader(this->id, vertex_shader);
	glAttachShader(this->id, fragment_shader);
	glLinkProgram(this->id);

	// Check link error
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if(!success) {
		GLchar info_log[512];
		glGetProgramInfoLog(this->id, 512, NULL, info_log);
		LOG_ERROR("\nHad error on linking shaders: %s", info_log);
	}

	// Not necessary anymore
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

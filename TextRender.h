#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;
using namespace glm;

class TextRender
{
public:
	TextRender(GLuint width, GLuint height);
	~TextRender();

	static void create(GLuint width, GLuint height);
	static void load(string fontFile, GLuint fontSize);
	static void render(string text, GLfloat x, GLfloat y, GLfloat scale, vec3 color, float alpha = 1.0f);

	struct Character {
		GLuint     TextureID;  // ID handle of the glyph texture
		glm::ivec2 Size;       // Size of glyph
		glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
		GLuint     Advance;    // Offset to advance to next glyph
	};

	map<GLchar, Character> Characters;

private:
	GLuint m_shader;
	GLuint VBO;
	GLuint VAO;

	static TextRender*	sm_singleton;
};


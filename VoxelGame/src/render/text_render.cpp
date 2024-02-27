//#include "text_render.hpp"
//
//#define FREETYPE_STATIC
//
//#include "ft2build.h"
//#include FT_FREETYPE_H
//
//#include <iostream>
//#include <map>
//#include <cstdio>
//
//#include <GL/glew.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "shader_handler.hpp"
//
//static FT_Library ft;
//static FT_Face face;
//
//static Shader textShader;
//
//static struct Character
//{
//	unsigned int Texture2D;
//	glm::ivec2 Size;
//	glm::ivec2 Bearing;
//	unsigned int Advance;
//};
//
//std::map<char, Character> Characters;
//
//GLuint VAO, VBO;
//
//void initialize_freetype()
//{
//	if (FT_Init_FreeType(&ft))
//	{
//		std::cout << "AAAAAAAA THE TEXT LIBRARY FUCKED UP" << std::endl;
//		return;
//	}
//	if (FT_New_Face(ft, "C:\\Users\\Augustus\\Desktop\\Projects\\Programming\\VoxelGame\\The Star in the Midnight.ttf", 0, &face))
//	{
//		std::cout << "AAAAAAAA THE TEXT LIBRARY FUCKED UP" << std::endl;
//		return;
//	}
//	FT_Set_Pixel_Sizes(face, 0, 48);
//
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
//
//	for (unsigned char c = 0; c < 128; c++)
//	{
//		// load character glyph 
//		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
//		{
//			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
//			continue;
//		}
//		// generate texture
//		unsigned int texture;
//		glGenTextures(1, &texture);
//		glBindTexture(GL_TEXTURE_2D, texture);
//		glTexImage2D(
//			GL_TEXTURE_2D,
//			0,
//			GL_RED,
//			face->glyph->bitmap.width,
//			face->glyph->bitmap.rows,
//			0,
//			GL_RED,
//			GL_UNSIGNED_BYTE,
//			face->glyph->bitmap.buffer
//		);
//		// set texture options
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		// now store character for later use
//		Character character = {
//			texture,
//			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//			face->glyph->advance.x
//		};
//		Characters.insert(std::pair<char, Character>(c, character));
//	}
//	textShader = Shader(
//		"C:\\Users\\Augustus\\Desktop\\Projects\\Programming\\VoxelGame\\shaders\\text.vert",
//		"C:\\Users\\Augustus\\Desktop\\Projects\\Programming\\VoxelGame\\shaders\\text.frag"
//	);
//
//	textShader.use();
//
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
//
//	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, 0, glm::value_ptr(projection));
//	glUniform3f(glGetUniformLocation(textShader.ID, "textColor"), 1.0f, 1.0f, 1.0f);
//
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
//
//	glVertexAttribPointer(0, 4, GL_FLOAT, 0, 4 * sizeof(float), 0);
//	glEnableVertexAttribArray(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//}
//
//
//void draw_text(const char* text, int x, int y)
//{
//	textShader.use();
//
//	const int scale = 5;
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindVertexArray(VAO);
//
//	// iterate through all characters
//	for (const char *c = text; *c != '\0'; c++)
//	{
//		Character ch = Characters[*c];
//
//		float xpos = x + ch.Bearing.x * scale;
//		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
//
//		float w = ch.Size.x * scale;
//		float h = ch.Size.y * scale;
//		// update VBO for each character
//		float vertices[6][4] = {
//			{ xpos,     ypos + h,   0.0f, 0.0f },
//			{ xpos,     ypos,       0.0f, 1.0f },
//			{ xpos + w, ypos,       1.0f, 1.0f },
//
//			{ xpos,     ypos + h,   0.0f, 0.0f },
//			{ xpos + w, ypos,       1.0f, 1.0f },
//			{ xpos + w, ypos + h,   1.0f, 0.0f }
//		};
//		// render glyph texture over quad
//		glBindTexture(GL_TEXTURE_2D, ch.Texture2D);
//		// update content of VBO memory
//		glBindBuffer(GL_ARRAY_BUFFER, VBO);
//		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		// render quad
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
//		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
//	}
//
//	glBindVertexArray(0);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//}
//
//char str[8];
//void draw_short(short num, int x, int y)
//{
//	sprintf_s(str, "%d", num);
//	draw_text(str, x, y);
//}
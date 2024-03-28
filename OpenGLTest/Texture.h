#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include "stb_image.h"
#include "Shader.h"

class Texture
{
	public:
		GLuint ID;
		GLenum type;
		GLenum slot;

		Texture(const char* image, GLenum textureType, GLenum slot, GLenum format, GLenum pixelType);

		void TextureUnit(Shader& shader, const char* uniform, GLuint unit);
		void Activate();
		void Bind();
		void Unbind();
		void Delete();
};
#endif 



#include "Texture.h"

Texture::Texture(const char* image, GLenum textureType, GLenum textureSlot, GLenum format, GLenum pixelType)
{
	type = textureType;
	slot = textureSlot;

	// load and generate the texture
	int width, height, nrChannels;

	glGenTextures(1, &ID);
	//glActiveTexture(textureSlot);
	glBindTexture(textureType, ID);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(image, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(textureType, 0, format, width, height, 0, format, pixelType, data);
		glGenerateMipmap(textureType);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	glBindTexture(textureType, 0);
}

void Texture::TextureUnit(Shader& shader, const char* uniform, GLuint unit)
{
	//GLuint texUnit = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	//glUniform1i(texUnit, unit);

	shader.setInt(uniform, unit);
}

void Texture::Activate() 
{
	glActiveTexture(slot);
	glBindTexture(type, ID);
}

void Texture::Bind()
{
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}

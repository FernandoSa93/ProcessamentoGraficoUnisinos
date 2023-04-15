#include "Layer.h"
#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>

Layer::Layer()
{
}

Layer Layer::carregaCamada(const char* imagem, Layer layer, int idTextura, float z, float scrollRateX) {
	int width, height, nrChannels;
	unsigned char *data;

	data = stbi_load(imagem, &width, &height, &nrChannels, 0);
	if (data)
	{
		layer.setWidth(width);
		layer.setHeight(height);
		layer.setTX((float)width);
		layer.setTY((float)height);
		layer.setPixels(data);
		layer.setTZ(z);
		layer.setTID(idTextura);
		layer.setScrollRateX(scrollRateX);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, layer.getWidth(), layer.getHeight(), 0,
			GL_RGB, GL_UNSIGNED_BYTE, layer.getPixels());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return layer;
}

Layer Layer::carregaCamadaAlpha(const char* imagem, Layer layer, int idTextura, float z, float scrollRateX) {
	int width, height, nrChannels;
	unsigned char *data;

	data = stbi_load(imagem, &width, &height, &nrChannels, 0);
	if (data)
	{
		layer.setWidth(width);
		layer.setHeight(height);
		layer.setTX((float)width);
		layer.setTY((float)height);
		layer.setPixels(data);
		layer.setTZ(z);
		layer.setTID(idTextura);
		layer.setScrollRateX(scrollRateX);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, layer.getWidth(), layer.getHeight(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, layer.getPixels());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return layer;
}

int Layer::getWidth() 
{
	return width;
}

int Layer::getHeight() 
{
	return height;
}

float Layer::getTX()
{
	return tx;
}

float Layer::getTY() 
{
	return ty;
}

float Layer::getTZ() 
{
	return tz;
}

unsigned char *Layer::getPixels()
{
	return pixels;
}

int Layer::getTID() 
{
	return tid;
}

float Layer::getScrollRateX()
{
	return scrollRateX;
}

void Layer::setWidth(int input_width) 
{
	width = input_width;
}

void Layer::setHeight(int input_height)
{
	height = input_height;
}

void Layer::setTX(float input_tx)
{
	tx = input_tx;
}

void Layer::setTY(float input_ty)
{
	ty = input_ty;
}

void Layer::setTZ(float input_tz)
{
	tz = input_tz;
}

void Layer::setPixels(unsigned char *input_pixel)
{
	pixels = input_pixel;
}

void Layer::setTID(int input_tid)
{
	tid = input_tid;
}

void Layer::setScrollRateX(float input_RateX)
{
	scrollRateX = input_RateX;
}
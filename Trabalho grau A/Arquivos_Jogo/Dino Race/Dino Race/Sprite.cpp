#include "Sprite.h"
#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>

Sprite::Sprite()
{
}

Sprite Sprite::carregaSprite(const char* imagem, Sprite sprite, int idTextura, float z) {
	int width, height, nrChannels;
	unsigned char *data;

	data = stbi_load(imagem, &width, &height, &nrChannels, 0);
	if (data)
	{
		sprite.setWidth(width);
		sprite.setHeight(height);
		sprite.setTX((float)width);
		sprite.setTY((float)height);
		sprite.setPosX(sprite.getTX());
		sprite.setPosY(sprite.getTY());
		sprite.setPixels(data);
		sprite.setTZ(z);
		sprite.setTID(idTextura);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sprite.getWidth(), sprite.getHeight(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, sprite.getPixels());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return sprite;
}

int Sprite::getWidth()
{
	return width;
}

int Sprite::getHeight()
{
	return height;
}

float Sprite::getTX()
{
	return tx;
}

float Sprite::getTY()
{
	return ty;
}

float Sprite::getTZ()
{
	return tz;
}

float Sprite::getPosX()
{
	return posX;
}

float Sprite::getPosY()
{
	return posY;
}

unsigned char *Sprite::getPixels()
{
	return pixels;
}

int Sprite::getTID()
{
	return tid;
}

float Sprite::getScrollRateX()
{
	return scrollRateX;
}

void Sprite::setWidth(int input_width)
{
	width = input_width;
}

void Sprite::setHeight(int input_height)
{
	height = input_height;
}

void Sprite::setTX(float input_tx)
{
	tx = input_tx;
}

void Sprite::setTY(float input_ty)
{
	ty = input_ty;
}

void Sprite::setTZ(float input_tz)
{
	tz = input_tz;
}

void Sprite::setPosX(float pos_tx)
{
	posX = pos_tx;
}

void Sprite::setPosY(float pos_ty)
{
	posY = pos_ty;
}

void Sprite::setPixels(unsigned char *input_pixel)
{
	pixels = input_pixel;
}

void Sprite::setTID(int input_tid)
{
	tid = input_tid;
}

void Sprite::setScrollRateX(float input_RateX)
{
	scrollRateX = input_RateX;
}

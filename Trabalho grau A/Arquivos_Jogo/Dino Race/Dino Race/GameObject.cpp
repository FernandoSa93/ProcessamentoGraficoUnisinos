#include "GameObject.h"
#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>

GameObject::GameObject()
{
}

GameObject GameObject:: carregaObjeto(const char* imagem, GameObject objeto, int idTextura, float z, float scrollRateX) {
	int width, height, nrChannels;
	unsigned char *data;

	data = stbi_load(imagem, &width, &height, &nrChannels, 0);
	if (data)
	{
		objeto.setWidth(width);
		objeto.setHeight(height);
		objeto.setTX((float)width);
		objeto.setTY((float)height);
		objeto.setPosX(objeto.getTX());
		objeto.setPosY(objeto.getTY());
		objeto.setPixels(data);
		objeto.setTZ(z);
		objeto.setTID(idTextura);
		objeto.setScrollRateX(scrollRateX);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, objeto.getWidth(), objeto.getHeight(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, objeto.getPixels());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return objeto;
}

int GameObject::getWidth()
{
	return width;
}

int GameObject::getHeight()
{
	return height;
}

float GameObject::getTX()
{
	return tx;
}

float GameObject::getTY()
{
	return ty;
}

float GameObject::getTZ()
{
	return tz;
}

float GameObject::getPosX()
{
	return posX;
}

float GameObject::getPosY()
{
	return posY;
}

unsigned char *GameObject::getPixels()
{
	return pixels;
}

int GameObject::getTID()
{
	return tid;
}

float GameObject::getScrollRateX()
{
	return scrollRateX;
}

void GameObject::setWidth(int input_width)
{
	width = input_width;
}

void GameObject::setHeight(int input_height)
{
	height = input_height;
}

void GameObject::setTX(float input_tx)
{
	tx = input_tx;
}

void GameObject::setTY(float input_ty)
{
	ty = input_ty;
}

void GameObject::setTZ(float input_tz)
{
	tz = input_tz;
}

void GameObject::setPosX(float pos_tx)
{
	posX = pos_tx;
}

void GameObject::setPosY(float pos_ty)
{
	posY = pos_ty;
}

void GameObject::setPixels(unsigned char *input_pixel)
{
	pixels = input_pixel;
}

void GameObject::setTID(int input_tid)
{
	tid = input_tid;
}

void GameObject::setScrollRateX(float input_RateX)
{
	scrollRateX = input_RateX;
}
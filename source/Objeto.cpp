
#include "Objeto.h"
#include "GlobalVars.h"

//Construtor

Objeto::Objeto() {}

Objeto::Objeto(int posX, int posY, int width, int height, int vel, const char *imgName)
{
	this->posX		= posX;
	this->posY		= posY;
	this->width		= width;
	this->height	= height;
	this->vel		= vel;
	this->initVel	= vel;
	
	this->setImagem(imgName);

}

//Setters and Getters

void Objeto::setAlive(bool alive)
{
	this->alive = alive;
}

bool Objeto::getAlive()
{
	return this->alive;
}

void Objeto::setPosX(int posX)
{
	this->posX = posX;
}

int Objeto::getPosX()
{
	return this->posX;
}

void Objeto::setPosY(int posY)
{
	this->posY = posY;
}

int Objeto::getPosY()
{
	return this->posY;
}

void Objeto::setPosXY(int posX, int posY)
{
	this->posX = posX;
	this->posY = posY;
}

void Objeto::setWidth(int width)
{
	this->width = width;
}

int Objeto::getWidth()
{
	return this->width;
}

void Objeto::setHeight(int height)
{
	this->height = height;
}

int Objeto::getHeight()
{
	return this->height;
}

void Objeto::setVel(int vel)
{
	this->vel = vel;
}

int Objeto::getVel()
{
	return this->vel;
}

void Objeto::setInitVel(int initVel)
{
	this->initVel = initVel;
}

int Objeto::getInitVel()
{
	return this->initVel;
}


void Objeto::setPosXAnterior(int posXAnterior)
{
	this->posXAnterior = posXAnterior;
}

int Objeto::getPosXAnterior()
{
	return this->posXAnterior;
}

void Objeto::setPosYAnterior(int posYAnterior)
{
	this->posYAnterior = posYAnterior;

}

int Objeto::getPosYAnterior()
{
	return this->posYAnterior;
}

void Objeto::setMaxFrame(int maxFrame)
{
	this->maxFrame = maxFrame;
}

int Objeto::getMaxFrame()
{
	return this->maxFrame;
}

void Objeto::setCurFrame(int curFrame)
{
	this->curFrame = curFrame;
}

int Objeto::getCurFrame()
{
	return this->curFrame;
}

void Objeto::setFrameCount(int frameCount)
{
	this->frameCount = frameCount;
}

int Objeto::getFrameCount()
{
	return this->frameCount;
}

void Objeto::setFrameDelay(int frameDelay)
{
	this->frameDelay = frameDelay;
}

int Objeto::getFrameDelay()
{
	return this->frameDelay;
}

void Objeto::atualizaFrames()
{
	//Gerencia a troca de frames do objeto
	this->setFrameCount(this->getFrameCount() + 1);
	if (this->getFrameCount() >= this->getFrameDelay()) {
		this->setCurFrame(this->getCurFrame() + 1);
		if (this->getCurFrame() >= this->getMaxFrame() && this->getAlive())
			this->setCurFrame(0);
		this->setFrameCount(0);
	}
}


void Objeto::setDirection(DIRECAO direction)
{
	this->direction = direction;

}

DIRECAO Objeto::getDirection()
{
	return this->direction;
}

void Objeto::draw()
{
	if (this->alive) {
		al_draw_bitmap_region(this->getImagem(), this->getWidth()*this->getCurFrame(),
			this->getHeight()* this->getDirection(),
			this->getWidth(), this->getHeight(), this->getPosX(), this->getPosY(), 0);
	}
}

bool Objeto::ColisaoCampo()
{
	// lógica inversa, verifica falso enquanto está DENTRO do quadrado da tela
	//Tela
	int telaX = 0 + TAM_BORDA;
	int telaY = 0 + TAM_BORDA;
	int telaW = SCREEN_W - TAM_BORDA;
	int telaH = SCREEN_H - TAM_BORDA;

	if (this->posX < telaX || this->posY < telaY ||
		this->posX+this->width > telaW || this->posY + this->height > telaH)

		return true;

	return false;
}

bool Objeto::Colisao(Objeto *obj)
{

	if (this->getPosX() + this->getWidth() > obj->getPosX() &&
		this->getPosX() < obj->getPosX() + obj->getWidth() &&
		this->getPosY() + this->getHeight() > obj->getPosY() &&
		this->getPosY() < obj->getPosY() + obj->getHeight())
		return true;

	return false;
}


// Destrutor
Objeto::~Objeto() {
}

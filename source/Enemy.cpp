#include "Enemy.h"


Enemy::Enemy() {

}

Enemy::Enemy(int posX, int posY, int width, int height, int vel, const char *imgName) {
	

	this->setAlive(true);
	this->setPosXY(posX, posY);
	this->setPosXAnterior(posX);
	this->setPosYAnterior(posY);
	this->setInitPosX(posX);
	this->setInitPosY(posY);

	timerAtual = 0;
	timerMax =  60;	// numero de vezes que o ALLEGRO_EVENT_TIMER deve ser chamado para alterar a direção

	this->setWidth(width);
	this->setHeight(height);
	this->setVel(vel);
	this->setInitVel(vel);
	
	this->setImagem(imgName);
	this->imgTemp = NULL;

	//relacionados a animação
	this->setMaxFrame(4);
	this->setCurFrame(0);
	this->setFrameCount(0);
	this->setFrameDelay(8);

	this->setDirection(BAIXO);

}


void Enemy::setInitPosX(int initPosX)
{
	this->initPosX = initPosX;
}

int Enemy::getInitPosX()
{
	return this->initPosX;
}
void Enemy::setInitPosY(int initPosY)
{
	this->initPosY = initPosY;
}

int Enemy::getInitPosY()
{
	return this->initPosY;
}

void Enemy::setImgTemp(ALLEGRO_BITMAP * imgTemp)
{
	this->imgTemp = imgTemp;
}
ALLEGRO_BITMAP * Enemy::getImgTemp() {
	return this->imgTemp;
}


void Enemy::setTimerMax(int timerMax)
{
	this->timerMax = timerMax;
}

void Enemy::inverteDirecao() {
	
		switch (this->getDirection())
		{
		case CIMA:
			this->setDirection(BAIXO);
			break;
		case BAIXO:
			this->setDirection(CIMA);
			break;
		case ESQUERDA:
			this->setDirection(DIREITA);
			break;
		case DIREITA:
			this->setDirection(ESQUERDA);
			break;
		default:
			break;
		}

}

void Enemy::atualizaPosicao() {
	switch (this->getDirection())
	{
	case CIMA:
		this->setPosY(this->getPosY() - this->getVel());
		break;
	case BAIXO:
		this->setPosY(this->getPosY() + this->getVel());
		break;
	case ESQUERDA:
		this->setPosX(this->getPosX() - this->getVel());
		break;
	case DIREITA:
		this->setPosX(this->getPosX() + this->getVel());
		break;
	default:
		break;
	}
}

void Enemy::alterarPosicao() {

	timerAtual++;
	
	if (timerAtual >= timerMax) {
		DIRECAO dirAnt = this->getDirection();
		do {
			this->setDirection((DIRECAO)(rand() % 4));
		} while (this->getDirection() == dirAnt);

		timerAtual = 0;
	}
}

void Enemy::resetStatus()
{
	this->setPosXY(this->getInitPosX(), this->getInitPosY());
	this->setVel(this->getInitVel());
}

Enemy::~Enemy() {
	//al_destroy_bitmap(imgTemp);
}


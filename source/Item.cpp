#include "Item.h"
#include "GlobalVars.h"





Item::Item(){

	this->setAlive(false);

	this->setWidth(TAM_ITEM);
	this->setHeight(TAM_ITEM);
	this->setPosXY(0, 0);
}

Item::~Item() {

}

void Item::setAliveTrue(Objeto *obj)
{
	do {
		this->setPosXY(rand() % (SCREEN_W), rand() % (SCREEN_H));
		this->setAlive(false);
	} while (this->ColisaoCampo() || this->Colisao(obj));

	this->setAlive(true);
}


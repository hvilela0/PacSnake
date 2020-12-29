#include "Imagem.h"



Imagem::Imagem() {

}

void Imagem::setImagem(const char *imgName) {

	img = al_load_bitmap(imgName);
}
void Imagem::setImagem(ALLEGRO_BITMAP * img)
{
	if (img == NULL)
		this->img = NULL;
	else
		this->img = img;
}
ALLEGRO_BITMAP * Imagem::getImagem() {
	return this->img;
}

Imagem::~Imagem()
{
	
}

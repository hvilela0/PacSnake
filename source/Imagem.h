#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>


class Imagem
{
	private:
		ALLEGRO_BITMAP	*img;

	public:
		Imagem();

		//modoos para setar imagens
		void setImagem(const char *imgName); // pelo nome do arquivo
		void setImagem(ALLEGRO_BITMAP *img); // por uso direto de imagem já carregada com o ALLEGRO

		ALLEGRO_BITMAP *getImagem();

		~Imagem();
};


#pragma once

// includes do Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

// minhas includes
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Tiles.h"
#include "GlobalVars.h"


enum TELA {
	TELA_INTRO, TELA_GAMEOVER, TELA_JOGO, TELA_MENU
};

class Jogo {
public: 
	Jogo();
	~Jogo();

	int initAll();
	void monitoraEventos();
	bool input();
	void update();
	void render();
	void destroyAll();


private:
	//funções auxiliares -------------

	//inicialização
	void gerarMapa();

	//input
	bool teclasDeJogo(int tecla);
	void alteraOpcoesMenu(int posicaoMenu);
	//update

	void timerFantasmaPiscando();


	//render
	void campo();
	void textos();
	void renderMenu();
	void renderIntro();
	void renderGameOver();
	void drawMap();

	//geral
	ALLEGRO_COLOR cores(int cor);

	void definePosFruta(Item *item);

};


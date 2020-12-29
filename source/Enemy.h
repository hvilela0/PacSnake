#pragma once
#include "Objeto.h"
class Enemy : public Objeto {
private:
	int initPosX;
	int initPosY;

	ALLEGRO_BITMAP *imgTemp;


	//tempo de atualização da direção do fantasma baseado no FPS
	int timerMax;
	int timerAtual;

public:

	//construtores
	Enemy();
	Enemy(int posX, int posY, int width, int height, int vel, const char *imgName);


	//setters and getters
	void setInitPosX(int initPosX);
	int getInitPosX();
	void setInitPosY(int initPosY);
	int getInitPosY();

	void setImgTemp(ALLEGRO_BITMAP * imgTemp);
	ALLEGRO_BITMAP * getImgTemp();

	void setTimerMax(int timerMax);

	void inverteDirecao();

	void atualizaPosicao();
	void alterarPosicao();

	void resetStatus();



	//destrutor
	~Enemy();

};


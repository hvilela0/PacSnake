#pragma once
#include "Objeto.h"
#include "GlobalVars.h"

enum KEYS {
	UP, DOWN, LEFT, RIGHT
};

class Player : public Objeto
{
private:
	int lifes;
	int initLifes;
	int pontos;
	int tamanho;

	int initMaxFrame;

public: 
	Player();
	Player(int posX, int posY, int width, int height, int vel, int initLifes, const char *imgName);
	~Player();

	void setLifes(int lifes);
	int getLifes();
	void setInitLifes(int initLifes);
	int getInitLifes();

	void setPontos(int pontos);
	int getPontos();
	void setTamanho(int tamanho);
	int getTamanho();

	void setPosXAnterior(int posXAnterior);
	int getPosXAnterior();
	void setPosYAnterior(int posYAnterior);
	int getPosYAnterior();
	

	void setInitMaxFrame(int initMaxFrame);
	int getInitMaxFrame();

	//funções auxiliares
	void atualizaPosicaoAnterior();
	bool verificaColisoes();

	void dead();

	void draw();

	bool autoColisao();
	void resetStatus();


	void setKeys(KEYS key);
	void atualizaPosicao();

};


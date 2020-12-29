#pragma once

#include "Imagem.h"

//includes C++
#include <stdlib.h>
#include <time.h>


enum DIRECAO {
	DIREITA, CIMA, ESQUERDA, BAIXO, NENHUMA
};

class Objeto :public Imagem
{
	private:
		bool	alive;
		int		posX, posY;
		int		width, height;
		int		vel, initVel;

		
		// características da frame e animação
		int		maxFrame;
		int		curFrame;
		int		frameCount;
		int		frameDelay;
		DIRECAO direction;
		

protected:
		int				posXAnterior;
		int				posYAnterior;



	public:

		//Construtor/Destrutor
		Objeto();

		Objeto(int posX, int posY, int width, int height, int vel, const char *imgName);
		~Objeto();

		//Setters and Getters
		void setAlive(bool alive);
		bool getAlive();
		void setPosX(int posX);
		int getPosX();
		void setPosY(int posY);
		int getPosY();
		void setPosXY(int posX, int posY);

		void setWidth(int width);
		int getWidth();
		void setHeight(int height);
		int getHeight();
		void setVel(int vel);
		int getVel();
		void setInitVel(int initVel);
		int getInitVel();

		void setPosXAnterior(int posXAnterior);
		int getPosXAnterior();
		void setPosYAnterior(int posYAnterior);
		int getPosYAnterior();



		void setMaxFrame(int maxFrame);
		int getMaxFrame();
		void setCurFrame(int curFrame);
		int getCurFrame();
		void setFrameCount(int frameCount);
		int getFrameCount();
		void setFrameDelay(int frameDelay);
		int getFrameDelay();

		void atualizaFrames();
		
		void setDirection(DIRECAO direction);
		DIRECAO getDirection();
		
		void draw();

		//Funções de Controle
		bool ColisaoCampo();
		bool Colisao(Objeto * obj);


};


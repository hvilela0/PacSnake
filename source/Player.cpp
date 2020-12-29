#include "Player.h"


// monitora status das teclas
bool keys[4] = { false, false, false, false };


//corpo do PAC-SNAKE
Objeto **body;

// faz tratamento da animação de morte do PAC-Snake
ALLEGRO_BITMAP *animMorte = NULL;
int animMorteMaxFrame = 0;
int MaxFrametemp = 0;


Player::Player() {

}

Player::Player(int posX, int posY, int width, int height, int vel, int initLifes, const char *imgName)
{
	this->setAlive(false);
	this->setPosXY(posX, posY);
	this->setPosXAnterior(posX);
	this->setPosYAnterior(posY);

	this->setWidth(width);
	this->setHeight(height);
	this->setVel(vel);
	this->setInitVel(vel);
	this->setInitLifes(initLifes);
	this->setLifes(initLifes);

	this->setPontos(0);
	this->setTamanho(0);

	this->setImagem(imgName);

	//relacionados a animação
	this->setMaxFrame(4);
	this->setCurFrame(0);
	this->setFrameCount(0);
	this->setFrameDelay(8);
	this->setInitMaxFrame(this->getMaxFrame());

	animMorte = al_load_bitmap("recursos/sprites/PacSnakeMorte.png");// não há teste de carregamento arquivo
	animMorteMaxFrame = 12;
	MaxFrametemp = this->getMaxFrame();
}

Player::~Player() {

	for (int i = 0; i < this->tamanho; i++)
		free(body[i]);

	al_destroy_bitmap(animMorte);
}

void Player::setLifes(int lifes)
{
	this->lifes = lifes;
}
int Player::getLifes()
{
	return lifes;
}

void Player::setInitLifes(int initLifes)
{
	this->initLifes = initLifes;
}

int Player::getInitLifes()
{
	return this->initLifes;
}

void Player::setPontos(int pontos)
{
	this->pontos = pontos;
}
int Player::getPontos()
{
	return this->pontos;
}

void Player::setTamanho(int tamanho)
{
	// libera memória caso tamanho volte para zero
	if (tamanho == 0) {
		for (int i = 0; i < this->tamanho; i++)
			free(body[i]);
	}
	
	//atribui novo tamanho
	this->tamanho = tamanho;

		if (this->tamanho > 0) {
			//cria instancias do corpo do PAC-SNAKE (dinâmicamente)
			body = (Objeto**)realloc(body, this->tamanho*sizeof(Objeto*));
			body[this->tamanho-1] = (Objeto*)malloc(sizeof(Objeto));

			//define os parãmtros iniciais do objeto body
			body[this->tamanho - 1]->setImagem("recursos/sprites/PacBody.png");
			body[this->tamanho - 1]->setWidth(al_get_bitmap_width(body[this->tamanho - 1]->getImagem()));
			body[this->tamanho - 1]->setHeight(al_get_bitmap_height(body[this->tamanho - 1]->getImagem()));

			body[this->tamanho - 1]->setVel(this->getVel());
	}
}

int Player::getTamanho()
{
	return this->tamanho;
}


void Player::setPosXAnterior(int posXAnterior)
{
	this->posXAnterior = posXAnterior;

	if (this->tamanho > 0) {

		//replica as posições das bolinhas nas anteriores // "pensar em calculo de convolução h(t)"
		for (int i = this->tamanho - 1; i > 0; i--) {
			body[i]->setPosX(body[i - 1]->getPosX());
		}
		body[0]->setPosX(posXAnterior);
		
		//distancia criação da primeira bolinha do corpo
		if (this->getDirection() == DIREITA)
			body[0]->setPosX(body[0]->getPosX() - this->getWidth());
		else if (this->getDirection() == ESQUERDA)
			body[0]->setPosX(body[0]->getPosX() + this->getWidth());

		//centraliza ela de acordo com a posição da cabeça
		body[0]->setPosX(body[0]->getPosX() + (this->getWidth() - body[0]->getWidth()) / 2);
	}

}

int Player::getPosXAnterior()
{
	return this->posXAnterior;
}

void Player::setPosYAnterior(int posYAnterior)
{
	this->posYAnterior = posYAnterior;

	if (this->tamanho > 0) {

		//replica as posições das bolinhas nas anteriores
		for (int i = this->tamanho - 1; i > 0; i--) {

			body[i]->setPosY(body[i - 1]->getPosY());
		}
		body[0]->setPosY(posYAnterior);
		
		//distancia criação da primeira bolinha do corpo
		if(this->getDirection() == BAIXO)
			body[0]->setPosY(body[0]->getPosY() - this->getHeight());
		else if (this->getDirection() == CIMA)
			body[0]->setPosY(body[0]->getPosY() + this->getHeight());

		//centraliza ela de acordo com a posição da cabeça
		body[0]->setPosY(body[0]->getPosY() + (this->getHeight() - body[0]->getHeight()) / 2);
		
	}

}

int Player::getPosYAnterior()
{
	return this->posYAnterior;
}

void Player::setInitMaxFrame(int initMaxFrame)
{
	this->initMaxFrame = initMaxFrame;
}

int Player::getInitMaxFrame()
{
	return this->initMaxFrame;
}

void Player::atualizaPosicaoAnterior()
{
	//Gerencia a troca de posição do this para atualizar a posição anterior
	if (this->getPosX() > (this->getPosXAnterior() + this->getWidth()) && this->getDirection() == DIREITA ||
		this->getPosX() < (this->getPosXAnterior() - this->getWidth()) && this->getDirection() == ESQUERDA ||
		this->getPosY() > (this->getPosYAnterior() + this->getHeight()) && this->getDirection() == BAIXO ||
		this->getPosY() < (this->getPosYAnterior() - this->getHeight()) && this->getDirection() == CIMA
		) {

		//pega posição "anterior" do this
		this->setPosXAnterior(this->getPosX());
		this->setPosYAnterior(this->getPosY());
	}
}

bool Player::verificaColisoes()
{
	// verifica colisao do player com as bordas da tela e próprio corpo
	if (this->ColisaoCampo() || this->autoColisao()) {

		this->dead();
		return true;
	}
	return false;
}

void Player::dead() {
		this->setLifes(this->getLifes() - 1);
		this->setVel(0);
		this->setTamanho(0);
		this->setAlive(false);
		this->setPosXY(this->getPosXAnterior(), this->getPosYAnterior());
}

void Player::draw()
{
	//desenha a cabeça do PAC-SNAKE
	if (this->getAlive()) {
		al_draw_bitmap_region(this->getImagem(), this->getWidth()*this->getCurFrame(),
			this->getHeight()* this->getDirection(),
			this->getWidth(), this->getHeight(), this->getPosX(), this->getPosY(), 0);
	}
	else { // desenha animação de morte do PAC
		MaxFrametemp = this->getInitMaxFrame();
		this->setDirection(DIREITA);
		this->setMaxFrame(animMorteMaxFrame);
		al_draw_bitmap_region(animMorte, this->getWidth()*this->getCurFrame(),0,
			this->getWidth(), this->getHeight(), this->getPosX(), this->getPosY(), 0);
		if (this->getCurFrame() >= this->getMaxFrame()) {
			this->setMaxFrame(MaxFrametemp);
			this->setPosXY(SCREEN_W / 2, SCREEN_H / 2);
			this->setAlive(true);
		}
	}
	// desenha corpo do PAC-SNAKE
	if (this->tamanho > 0) {

		for (int i = 0; i < this->tamanho; i++) {
			al_draw_bitmap(body[i]->getImagem(), body[i]->getPosX(), body[i]->getPosY(), 0);

		}
	}

}

bool Player::autoColisao()
{
	if (this->tamanho > 0){
		for (int i = 0; i < this->tamanho; i++) {
			if (this->Colisao(body[i]))
				return true;
		}
		return false;
	}
	else
		return false;
}

void Player::resetStatus()
{
	this->setPosXY(SCREEN_W / 2, SCREEN_H / 2);
	this->setLifes(this->getInitLifes());
	this->setPontos(0);
	this->setVel(0);
	this->setTamanho(0);
	this->setMaxFrame(MaxFrametemp);
	this->setAlive(true);
}


void Player::setKeys(KEYS key)
{
	for (int i = 0; i < 4; i++) {
		if (i == key) keys[i] = true;
		else		  keys[i] = false;

	}
}

void Player::atualizaPosicao() {
	this->setPosY(this->getPosY() - keys[UP] * this->getVel());
	this->setPosY(this->getPosY() + keys[DOWN] * this->getVel());
	this->setPosX(this->getPosX() - keys[LEFT] * this->getVel());
	this->setPosX(this->getPosX() + keys[RIGHT] * this->getVel());
}

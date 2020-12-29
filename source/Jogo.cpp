#include "Jogo.h"

//includes C++
#include <stdlib.h>
#include <time.h>
#include <string.h>


// cria ponteiros para uso geral
ALLEGRO_DISPLAY			*display		= NULL;
ALLEGRO_FONT			*fonte14		= NULL;
ALLEGRO_FONT			*fonte24		= NULL;
ALLEGRO_EVENT_QUEUE		*queue			= NULL;
ALLEGRO_TIMER			*timer			= NULL;

//SFX
ALLEGRO_SAMPLE			*comerFruta		= NULL;
ALLEGRO_SAMPLE			*comerFantasma	= NULL;
ALLEGRO_SAMPLE			*ganharVida		= NULL;
ALLEGRO_SAMPLE			*morte			= NULL;
ALLEGRO_SAMPLE			*speedUp		= NULL;
ALLEGRO_SAMPLE			*menuClick		= NULL;


//Musicas
ALLEGRO_SAMPLE			*musica01		= NULL;
ALLEGRO_SAMPLE			*musica02		= NULL;
ALLEGRO_SAMPLE			*musicaGO		= NULL;
ALLEGRO_SAMPLE			*musicaFant		= NULL;
ALLEGRO_SAMPLE			*musicaTemp		= NULL;

// Telas Principais
ALLEGRO_BITMAP			*telaIntro		= NULL;
ALLEGRO_BITMAP			*telaMenu		= NULL;
ALLEGRO_BITMAP			*telaGameOver	= NULL;

//Para pausar o jogo
bool pauseOn = false;

//para ativar/desativar fantasmas piscando
bool fantasmasPiscando = false;
bool fantasmaImgNormal = true;
//tempo de duração do fantasma piscando
int timerFantAtual = 0;
int timerFantMax = TMR_FANTASMA_PISCANDO;


//criação de evento para o jogo
ALLEGRO_EVENT	ev;
// controle de rendering
bool redraw = true; 

//variáveis para personalizar teclas do jogo
int teclaPac[4] = {ALLEGRO_KEY_W, ALLEGRO_KEY_S, ALLEGRO_KEY_A, ALLEGRO_KEY_D, }; // teclas padrão

// controla a opção do menu
int corOpcaoMenu[NUM_OPCOES_MENU] = {0,1,1,1};
int posMenu = 0;

//teclas de controle do PAC no Menu
char *teclaJogoMenu[4] = {  (char*)"W",
							(char*)"S",
							(char*)"A",
							(char*)"D" 
							};

// controla opções de efeitos sonoros
char *opcaoSFX[1] = { (char*)"ON" };
bool SFXOn = true;
// controla opções de efeitos sonoros
char *opcaoMusica[1] = { (char*)"Musica 01" };
int MusicOn = 1;
// controla opções de dificuldade do jogo
char *opcaoDificuldade[1] = { (char*)"Facil" };
int dificuldade = 1;


//tratamento de game_over
bool gameOver = false;
TELA tela = TELA_INTRO; 

//gera tiles para o campo de jogo
ALLEGRO_BITMAP *tileset = NULL;	
Tiles *tiles[MAX_TILES];

//cria instância do jogador
Player *player = NULL;

//Cria instâncias para o fantasmas
Enemy *ghost[4] = { NULL, NULL, NULL, NULL};
// cria bitmap para o fantasmas Piscando
ALLEGRO_BITMAP *fantasmaPiscandoImg = NULL;

// trabalha com a geração de frutas na tela
ALLEGRO_BITMAP *todasFrutas = NULL;	// memória para a imagem contendo todas as frutas
Item *frutas[MAX_ITENS];			//objetos separadamente

// geração de obstaculos na tela
Objeto *obstaculo[MAX_OBS] = {NULL};
int obsMap[MAP_V][MAP_H];


//Construtor & Destrutor
Jogo::Jogo() {}
Jogo::~Jogo() {}

// funções principais
int Jogo::initAll(){
	//inicializa aleatoriedade para selecionar o item a ser apresentado
	srand(time(NULL));

	//inicializa allegro
	if (!al_init()) {
		al_show_native_message_box(NULL, NULL, NULL,
			"Falha ao iniciar o allegro!", NULL, NULL);
		return 0;
	}

	//inicializa display
	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display) {
		al_show_native_message_box(NULL, NULL, NULL,
			"Falha ao iniciar o display!", NULL, NULL);
		return 0;
	}

	//inicializa audio
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(10);
	//carrega audios
	comerFruta = al_load_sample("recursos/sfx/comerFruta.wav");
	comerFantasma = al_load_sample("recursos/sfx/comerFantasma.wav");
	ganharVida = al_load_sample("recursos/sfx/ganharVida.wav");
	morte = al_load_sample("recursos/sfx/morte.wav");
	speedUp = al_load_sample("recursos/sfx/speedUp.wav");
	menuClick = al_load_sample("recursos/sfx/menuClick.wav");

	// carrega musicas  -- verificar carregamento de novo sample e play
	musica01   = al_load_sample("recursos/musicas/Musica01.wav");
	musicaTemp = al_load_sample("recursos/musicas/Musica01.wav");
	musica02   = al_load_sample("recursos/musicas/Musica02.wav");
	musicaGO   = al_load_sample("recursos/musicas/GameOverMusic.wav");
	musicaFant = al_load_sample("recursos/musicas/MusicaFantasmas.wav");

	al_play_sample(musica01, 0.2, 0, 1, ALLEGRO_PLAYMODE_LOOP, 0);


	// cria timer(s)
	timer = al_create_timer(1.0 / FPS);

	//inicialização e criação das fontes de texto
	al_init_font_addon();
	al_init_ttf_addon();
	fonte14 = al_load_font("recursos/fonts/bungee.ttf", 14, 0);
	fonte24 = al_load_font("recursos/fonts/bungee.ttf", 24, 0);

	//instala teclado como fonte de entrada
	al_install_keyboard();

	// inicializa o Queue (fila de eventos)
	queue = al_create_event_queue();

	//registra eventos para o queue
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	//inicializa primitives e image
	al_init_primitives_addon();
	al_init_image_addon();
	//inicializa o timer
	al_start_timer(timer);

	//inicia/ carrega telas
	telaIntro = al_create_bitmap(SCREEN_W, SCREEN_H);
	telaIntro = al_load_bitmap("recursos/telas/telaIntro.png");
	if (telaIntro == NULL) {
		al_show_native_message_box(NULL, NULL, NULL,
			"Falha ao carregar imagem da tela de introdução!", NULL, NULL);
		return 0;
	}
	// carrega tela de Menu...
	telaMenu = al_create_bitmap(SCREEN_W, SCREEN_H);
	telaMenu = al_load_bitmap("recursos/telas/telaMenu.png");
	if (telaMenu == NULL) {
		al_show_native_message_box(NULL, NULL, NULL,
			"Falha ao carregar imagem da tela de Game Over!", NULL, NULL);
		return 0;
	}

	// carrega tela de Game Over...
	telaGameOver = al_create_bitmap(SCREEN_W, SCREEN_H);
	telaGameOver = al_load_bitmap("recursos/telas/telaGameOver.png");
	if (telaGameOver == NULL) {
		al_show_native_message_box(NULL, NULL, NULL,
			"Falha ao carregar imagem da tela de Game Over!", NULL, NULL);
		return 0;
	}



	// inicializa tileset
	tileset = al_load_bitmap("recursos/tilesets/tileset.png");
	if (tileset == NULL) {
		al_show_native_message_box(NULL, NULL, NULL,
			"Falha ao carregar imagem com os tiles!", NULL, NULL);
		return 0;
	}
	for (int i = 0; i < MAX_TILES; i++) {
		tiles[i] = new Tiles();
		tiles[i]->setImagem(al_create_sub_bitmap(tileset, i*TAM_TILES, 0, TAM_TILES, TAM_TILES));
	}


	//inicializa player
	player = new Player(SCREEN_W / 2, SCREEN_H / 2, TAM_PAC, TAM_PAC, 3, 5, "recursos/sprites/PacSnake.png");
	if (player->getImagem() == NULL) {
		al_show_native_message_box(NULL, NULL, NULL,
			"Falha ao carregar imagem do PAC-Snake", NULL, NULL);
		return 0;
	}

	//inicializa fantasmas
		ghost[0] = new Enemy(64, 64, TAM_PAC, TAM_PAC, 2, "recursos/sprites/ghostRed.png");
		ghost[1] = new Enemy(SCREEN_W - 64, 64, TAM_PAC, TAM_PAC, 2, "recursos/sprites/ghostCyan.png");
		ghost[2] = new Enemy(SCREEN_W - 64, SCREEN_H - 64, TAM_PAC, TAM_PAC, 2, "recursos/sprites/ghostPink.png");
		ghost[3] = new Enemy(64, SCREEN_H - 64, TAM_PAC, TAM_PAC, 2, "recursos/sprites/ghostOrange.png");

		for (int i = 0; i < 4; i++) {
			ghost[i]->setTimerMax((3+(rand()%5))*10); // seta timer para os diferentes fantasmas de 60 a 100.
		}

	for (int i = 0; i < 4; i++) {
		if (ghost[i]->getImagem() == NULL) {
			al_show_native_message_box(NULL, NULL, NULL,
				"Falha ao carregar imagem do Fantasma!", NULL, NULL);
			return 0;
		}
	}

	//inicializa imagem fantasmaPsicandoImg
	fantasmaPiscandoImg = al_load_bitmap("recursos/sprites/fantasmasPiscando.png");
	if (fantasmaPiscandoImg == NULL) {
		al_show_native_message_box(NULL, NULL, NULL,
			"Falha ao carregar imagem com os tiles!", NULL, NULL);
		return 0;
	}

	// inicializa obstaculos
	for (int i = 0; i < MAX_OBS; i++) {
		obstaculo[i] = new Objeto(0, 0, TAM_OBS, TAM_OBS, 0, "recursos/sprites/obstaculo.png");
		if (obstaculo[i]->getImagem() == NULL) {
			al_show_native_message_box(NULL, NULL, NULL,
				"Falha ao carregar imagem dos Obstáculos(pedras)!", NULL, NULL);
			return 0;
		}
	}
	//inicializa mapa
	gerarMapa();
	//define posições das pedras no mapa
	drawMap();

	//inicializa todas frutas
	todasFrutas = al_load_bitmap("recursos/sprites/frutasPNG.png");
	if (todasFrutas == NULL) {
		al_show_native_message_box(NULL, NULL, NULL,
			"Falha ao carregar imagem com as Frutas!", NULL, NULL);
		return 0;
	}
	for (int i = 0; i < MAX_ITENS; i++) {
		frutas[i] = new Item();
		frutas[i]->setImagem(al_create_sub_bitmap(todasFrutas, i*TAM_ITEM,0, TAM_ITEM, TAM_ITEM));
	}
	//define posição da primeira fruta
	definePosFruta(frutas[MAX_ITENS-1]);
	

	return 1;
}
void Jogo::monitoraEventos() {
	al_wait_for_event(queue, &ev);
}
bool Jogo::input() {

	if (ev.type == ALLEGRO_EVENT_KEY_DOWN && player->getAlive()) {
		return teclasDeJogo(ev.keyboard.keycode);
	}
	else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		return true;
	}
	return false;
}
void Jogo::update(){
	if (ev.type == ALLEGRO_EVENT_TIMER) {
		if (!pauseOn) {

			switch (tela) {
			case TELA_INTRO:
				if (player->getAlive() == false) {
					player->resetStatus();
					gerarMapa();
					drawMap();
					for (int i = 0; i < MAX_ITENS; i++) {
						if (frutas[i]->getAlive()) {
							frutas[i]->setAlive(false);
							int frt = rand() % (MAX_ITENS);
							definePosFruta(frutas[frt]);
							break;
						}
						
					}
					for (int i = 0; i < 4; i++) {
						ghost[i]->resetStatus();
					}
				}
				break;
			case TELA_GAMEOVER:
				break;
			case TELA_JOGO:

				// --------------------------- ATUALIZA POSIÇÕES ------------------------

				//atualiza a posição do player na tela
				player->atualizaPosicao();

				//atualiza a posição do fantasma na tela
				if (player->getAlive()) {
					for (int i = 0; i < 4; i++) {
						if (ghost[i]->getAlive()) {
							ghost[i]->alterarPosicao();
							ghost[i]->atualizaPosicao();
						}
					}
				}

				//-------------------------------- COLISOES ------------------------------
				if (player->getAlive()) {

					// verifica colisao do player com as bordas da tela e próprio corpo
					if (player->verificaColisoes()) {
						for (int j = 0; j < 4; j++) {
							ghost[j]->resetStatus();
							if (SFXOn)
								al_play_sample(morte, 0.1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
						}
					}

					// verifica colisão do player com os fantasmas
					for (int i = 0; i < 4; i++) {
						if (ghost[i]->getAlive()) {
							if (player->Colisao(ghost[i])) {

								if (!fantasmasPiscando) {
									for (int j = 0; j < 4; j++) {
										ghost[j]->resetStatus(); //reseta posição de todos os fantasmas
									}
								}
								else {
									ghost[i]->resetStatus();	// reseta posição apenas do que colidiu 
									ghost[i]->setAlive(false);
									player->setPontos(player->getPontos() + (50 * dificuldade));
								}
								if (SFXOn) {
									if (!fantasmasPiscando)
										al_play_sample(morte, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
									else
										al_play_sample(comerFantasma, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
								}

								if (!fantasmasPiscando)
									player->dead();

								break;
							}
						}
					}

					// verifica colisao do fantasma com as bordas da tela
					for (int i = 0; i < 4; i++) {
						if (ghost[i]->ColisaoCampo())
							ghost[i]->inverteDirecao();
					}
					// fazer colisões fantasma-obstaculo
					for (int i = 0; i < 4; i++) {
						for (int j = 0; j < MAX_OBS; j++) {
							if (ghost[i]->Colisao(obstaculo[j])) {
								ghost[i]->inverteDirecao();
								break;

							}
						}
					}

					// verifica colisão do player com os obstaculos do campo
					for (int i = 0; i < MAX_OBS; i++) {
						if (player->Colisao(obstaculo[i])) {
							for (int j = 0; j < 4; j++) {
								ghost[j]->resetStatus();
							}
							player->dead();

							if (SFXOn)
								al_play_sample(morte, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
							break;
						}
					}

					// verifica colisão do player com as frutas
					for (int i = 0; i < MAX_ITENS; i++) {
						if (frutas[i]->getAlive()) {
							if (player->Colisao(frutas[i])) {
								frutas[i]->setAlive(false);
								player->setPontos(player->getPontos() + (10 * dificuldade));
								if (SFXOn)
									al_play_sample(comerFruta, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
								if ((player->getPontos() % 100 == 0)) {
									player->setLifes(player->getLifes() + 1);
									if (SFXOn)
										al_play_sample(ganharVida, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
								}
								if ((player->getTamanho() + 1) % 5 == 0) {
									player->setVel(player->getVel() + 1);
									if (SFXOn)
										al_play_sample(speedUp, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
								}

								player->setTamanho(player->getTamanho() + 1);

								if (i == 0) {// é o item 'fantasma azul'
									fantasmasPiscando = true;
								}
								int frt;
								if (fantasmasPiscando)
									frt = 1 + rand() % (MAX_ITENS - 1);
								else
									frt = rand() % (MAX_ITENS);

								definePosFruta(frutas[frt]);

								break;// i = MAX_ITENS;
							}
						}
					}
				}
				// ----------------------- OUTROS UPDATES ---------------------------

				//troca imagem do fantasma e ativa musica em modo Fantasma Piscando
				if (fantasmasPiscando && fantasmaImgNormal) {
					for (int i = 0; i < 4; i++) {
						ghost[i]->setImgTemp(ghost[i]->getImagem());
						ghost[i]->setImagem(fantasmaPiscandoImg);
						ghost[i]->setVel(ghost[i]->getVel() * 2);
					}
					fantasmaImgNormal = false;
					if (MusicOn != 0) {
						al_stop_samples();
						al_play_sample(musicaFant, 0.22, 0, 1, ALLEGRO_PLAYMODE_LOOP, 0);
					}

				}
				//Gerencia a troca de frames dos fantasmas
				if (fantasmasPiscando){
					for (int i = 0; i < 4; i++) {
						ghost[i]->atualizaFrames();
					}
				}

				//Verifica se já passou o tempo dos fantasmas piscando
				if(fantasmasPiscando)
					timerFantasmaPiscando();

				//Gerencia a troca de frames do player
				player->atualizaFrames();
				//Gerencia a troca de posição do player para atualizar a posição anterior
				player->atualizaPosicaoAnterior();

				//gerencia gameOver
				if (player->getLifes() <= 0 && player->getAlive()) {
					player->setLifes(player->getInitLifes());
					tela = TELA_GAMEOVER;
					al_stop_samples();
					if (MusicOn != 0) {
						al_play_sample(musicaGO, 0.2, 0, 0.8, ALLEGRO_PLAYMODE_ONCE, 0);
					}

				}
				break;
			case TELA_MENU:

				//colore opção marcada
				for (int i = 0; i < NUM_OPCOES_MENU; i++) {
					corOpcaoMenu[i] = COR_BRANCO;
				}
				corOpcaoMenu[posMenu] = COR_AMARELO;
				break;
			default:
				break;
			}
		}
		// "autoriza" a atualização da tela
		redraw = true;
	}
}
void Jogo::render(){
	if (redraw  && al_is_event_queue_empty(queue)) {

		redraw = false;

		switch (tela) {
		case TELA_INTRO:
			renderIntro();
			break;
		case TELA_MENU:
			renderMenu();
			break;
		case TELA_GAMEOVER:
			renderGameOver();
			break;
		case TELA_JOGO:
				//desenha campo de jogo
				this->campo();
				//desenha PAC_SNAKE e o corpo dele na tela
				player->draw();

				//desenha fantasma na tela
				for (int i = 0; i < 4; i++) {
					ghost[i]->draw();
				}

				//desenha frutas na tela
				for (int i = 0; i < MAX_ITENS; i++) {
					if (frutas[i]->getAlive())
						al_draw_bitmap(frutas[i]->getImagem(), frutas[i]->getPosX(), frutas[i]->getPosY(), 0);
				}

				// escreve todos os textos na tela, pontuação e vidas
				this->textos();

				//desenha obstáculos na tela
				for (int i = 0; i < MAX_OBS; i++) {
					al_draw_bitmap(obstaculo[i]->getImagem(), obstaculo[i]->getPosX(), obstaculo[i]->getPosY(), 0);
				}

				break;
		default:
			break;
		}

		// joga tudo que foi desenhado para a tela
		al_flip_display();

		//pausa na tela de game over
		

		


	}
} 

void Jogo::destroyAll(){
	// display
	al_destroy_display(display);

	//telas
	al_destroy_bitmap(telaIntro);
	al_destroy_bitmap(telaMenu);
	al_destroy_bitmap(telaGameOver);

	al_stop_samples();
	//SFX
	al_destroy_sample(comerFruta);
	al_destroy_sample(comerFantasma);
	al_destroy_sample(ganharVida);
	al_destroy_sample(morte);
	al_destroy_sample(speedUp);

	//Musicas
	al_destroy_sample(musica01);
	al_destroy_sample(musica02);
	al_destroy_sample(musicaGO);
	al_destroy_sample(musicaFant);
	al_destroy_sample(musicaTemp);



	// fontes - libera memória
	al_destroy_font(fonte14);
	al_destroy_font(fonte24);

	//queue
	al_destroy_event_queue(queue);

	//deleta instâncias de classes
	delete player;

	//destrói frutas
	al_destroy_bitmap(todasFrutas);
	for (int i = 0; i < MAX_ITENS; i++) {
		delete frutas[i];
	}
	// destrói tiles
	al_destroy_bitmap(tileset);
	for (int i = 0; i < MAX_TILES; i++) {
		delete tiles[i];
	}

	//destrói fantasmas
	for (int i = 0; i < 4; i++) {
		delete ghost[i];
	}
	al_destroy_bitmap(fantasmaPiscandoImg);

}


// funções auxiliares


void Jogo::gerarMapa()
{

	

	int mapaGerado0[MAP_V][MAP_H] = {
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
			{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
			{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
			{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
			{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
			{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
			{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
			{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
			{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
			{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};

	int mapaGerado1[MAP_V][MAP_H] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0}
	};
	int mapaGerado2[MAP_V][MAP_H] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};


	int mapSel = rand()% MAX_MAPS;
	switch (mapSel)
	{
	case 0:
		for (int i = 0; i < MAP_V; i++) {
			for (int j = 0; j < MAP_H; j++) {

				obsMap[i][j] = mapaGerado0[i][j];
			}
		}
		break;
	case 1:
		for (int i = 0; i < MAP_V; i++) {
			for (int j = 0; j < MAP_H; j++) {

				obsMap[i][j] = mapaGerado1[i][j];
			}
		}
		break;
	case 2:
		for (int i = 0; i < MAP_V; i++) {
			for (int j = 0; j < MAP_H; j++) {

				obsMap[i][j] = mapaGerado2[i][j];
			}
		}
		break;
	default:
		break;
	}

}

bool Jogo::teclasDeJogo(int tecla)
{
	//controla teclas quando em jogo
	if (tela == TELA_JOGO) {
		if (tecla == teclaPac[UP] && !pauseOn) {
			if (player->getDirection() == BAIXO) return false;
			player->setKeys(UP);
			if(player->getVel() == 0)
				player->setVel(player->getInitVel());
			player->setDirection(CIMA);
		}
		else if (tecla == teclaPac[DOWN] && !pauseOn) {
			if (player->getDirection() == CIMA) return false;
			player->setKeys(DOWN);
			if (player->getVel() == 0)
				player->setVel(player->getInitVel());
			player->setDirection(BAIXO);
		}
		else if (tecla == teclaPac[LEFT] && !pauseOn) {
			if (tela != TELA_JOGO) return false;
			if (player->getDirection() == DIREITA) return false;
			player->setKeys(LEFT);
			if (player->getVel() == 0)
				player->setVel(player->getInitVel());
			player->setDirection(ESQUERDA);
		}
		else if (tecla == teclaPac[RIGHT] && !pauseOn) {
			if (player->getDirection() == ESQUERDA) return false;
			player->setKeys(RIGHT);
			if (player->getVel() == 0)
				player->setVel(player->getInitVel());
			player->setDirection(DIREITA);
		}
		else if (tecla == ALLEGRO_KEY_P) {
			pauseOn = !pauseOn;
		}
	}
	
	//controla teclas quando no Menu
	else if (tela == TELA_MENU) {

		if (tecla == ALLEGRO_KEY_UP) {
			//cursor vai pra cima
			posMenu--;
			if (posMenu < 0) posMenu++;
			else {
				if (SFXOn)
					al_play_sample(menuClick, 0.7, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			}
		}
		else if (tecla == ALLEGRO_KEY_DOWN) {
			//cursor vai pra baixo
			posMenu++;
			if (posMenu >= NUM_OPCOES_MENU) posMenu--;
			else {
				if(SFXOn)
					al_play_sample(menuClick, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			}
		}
		else if (tecla == ALLEGRO_KEY_ENTER || tecla == ALLEGRO_KEY_PAD_ENTER) {

				alteraOpcoesMenu(posMenu);
		}

	}
	
	// demais teclas
	if (tecla == ALLEGRO_KEY_M) {
		if (tela == TELA_INTRO)
			tela = TELA_MENU;
	}
	else if (tecla == ALLEGRO_KEY_ENTER || tecla == ALLEGRO_KEY_PAD_ENTER) {
		switch (tela) {

		case TELA_INTRO:
			tela = TELA_JOGO;
			player->setAlive(true);
			break;
		case TELA_GAMEOVER:
			tela = TELA_INTRO;
			player->setPontos(0);
			al_stop_samples();
			if (MusicOn == 1) {
				al_play_sample(musica01, 0.2, 0, 1, ALLEGRO_PLAYMODE_LOOP, 0);
			}
			else if (MusicOn == 2) {
				al_play_sample(musica02, 0.2, 0, 1, ALLEGRO_PLAYMODE_LOOP, 0);
			}

			break;
		default:
			break;
		}
	}
	else if (tecla == ALLEGRO_KEY_ESCAPE) {
		if (tela == TELA_MENU)
			tela = TELA_INTRO;


		else if (tela == TELA_JOGO ) {
			if (!fantasmasPiscando) {
				tela = TELA_INTRO;
				player->setAlive(false);
			}
		}
		else if (tela == TELA_GAMEOVER) {
			tela = TELA_INTRO;
			player->setPontos(0);
			al_stop_samples();
			if (MusicOn == 1) {
				al_play_sample(musica01, 0.2, 0, 1, ALLEGRO_PLAYMODE_LOOP, 0);
			}
			else if (MusicOn == 2) {
				al_play_sample(musica02, 0.2, 0, 1, ALLEGRO_PLAYMODE_LOOP, 0);
			}
		}
		else
			return true;
	}
	
	return false;
}

void Jogo::alteraOpcoesMenu(int posicaoMenu)
{
	if (posicaoMenu <= 3) { // se esta na selecao de teclas
		bool out = false;
		al_draw_text(fonte24, cores(COR_BRANCO), SCREEN_W / 2, SCREEN_H - 48,
			ALLEGRO_ALIGN_CENTRE, "Aguardando tecla...");
		al_flip_display();
		while (!out) {
			al_wait_for_event(queue, &ev);
			if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
				int key = ev.keyboard.keycode;
				//evitas teclas repetidas e teclas reservadas
				if (key != teclaPac[0] && key != teclaPac[1] && key != teclaPac[2] && key != teclaPac[3] &&
					key != ALLEGRO_KEY_ENTER &&
					key != ALLEGRO_KEY_PAD_ENTER &&
					key != ALLEGRO_KEY_ESCAPE &&
					key != ALLEGRO_KEY_P) {
					teclaJogoMenu[posicaoMenu] = (char*)al_keycode_to_name(key);
					teclaPac[posicaoMenu] = key;
				}
				out = true;
			}
		}
	}
	// habilita SFX
	else if (posicaoMenu == 4) {
		if (SFXOn) {
			opcaoSFX[0] = (char*)"OFF";
			SFXOn = false;
		}
		else {
			opcaoSFX[0] = (char*)"ON";
			SFXOn = true;
		}
	}
	// habilita Musica
	else if (posicaoMenu == 5) {
		if (MusicOn == 1) {
			opcaoMusica[0] = (char*)"Musica 02";
			al_stop_samples();
			al_play_sample(musica02, 0.2, 0, 1, ALLEGRO_PLAYMODE_LOOP, 0);
			al_destroy_sample(musicaTemp);
			musicaTemp = al_load_sample("recursos/musicas/Musica02.wav");
			MusicOn = 2;
		}
		else if (MusicOn == 2) {
			opcaoMusica[0] = (char*)"OFF";
			al_stop_samples();

			MusicOn = 0;
		}
		else if (MusicOn == 0) {
			opcaoMusica[0] = (char*)"Musica 01";
			al_stop_samples();
			al_play_sample(musica01, 0.2, 0, 1, ALLEGRO_PLAYMODE_LOOP, 0);
			al_destroy_sample(musicaTemp);
			musicaTemp = al_load_sample("recursos/musicas/Musica01.wav");
			MusicOn = 1;
		}
	}
	// habilita Musica
	else if (posicaoMenu == 6) {
		if (dificuldade == 1) {
			opcaoDificuldade[0] = (char*)"Media";
			dificuldade = 2;
			player->setInitLifes(4);
			player->setLifes(player->getInitLifes());
			player->setInitVel(4);
			player->setVel(player->getInitVel());
			for (int i = 0; i < 4; i++) {
				ghost[i]->setInitVel(3);
				ghost[i]->setVel(ghost[i]->getInitVel());
			}

		}
		else if (dificuldade == 2) {
			opcaoDificuldade[0] = (char*)"Dificil";
			dificuldade = 3;
			player->setInitLifes(3);
			player->setLifes(player->getInitLifes());
			player->setInitVel(5);
			player->setVel(player->getInitVel());
			for (int i = 0; i < 4; i++) {
				ghost[i]->setInitVel(4);
				ghost[i]->setVel(ghost[i]->getInitVel());
			}
		}
		else if (dificuldade == 3) {
			opcaoDificuldade[0] = (char*)"Facil";
			dificuldade = 1;

			player->setInitLifes(5);
			player->setLifes(player->getInitLifes());
			player->setInitVel(3);
			player->setVel(player->getInitVel());
			for (int i = 0; i < 4; i++) {
				ghost[i]->setInitVel(2);
				ghost[i]->setVel(ghost[i]->getInitVel());
			}

		}
	}
}

void Jogo::timerFantasmaPiscando() {
	timerFantAtual++;

	if (timerFantAtual >= timerFantMax) {
		
		fantasmasPiscando = false;

		for (int i = 0; i < 4; i++) {
			ghost[i]->setImagem(ghost[i]->getImgTemp());
			ghost[i]->setAlive(true);
			ghost[i]->setCurFrame(0);
			ghost[i]->setVel(ghost[i]->getInitVel());

		}
		fantasmaImgNormal = true;
			
			if (MusicOn != 0) {
				al_stop_samples();
				al_play_sample(musicaTemp, 0.22, 0, 1, ALLEGRO_PLAYMODE_LOOP, 0);
			}

			timerFantAtual = 0;
	}

}

void Jogo::campo()
{
	//limpa a tela com a cor pintando o campo de jogo
	al_clear_to_color(cores(COR_BRANCO));

	//desenha bordas da tela
	for (int i = 0; i < SCREEN_W; i += TAM_TILES){
		al_draw_bitmap(tiles[BORDA]->getImagem(), i, 0, 0);
		al_draw_bitmap(tiles[BORDA]->getImagem(), i, SCREEN_H-TAM_TILES, 0);
		}
	for (int i = 0; i < SCREEN_H; i += TAM_TILES) {
		al_draw_bitmap(tiles[BORDA]->getImagem(), 0, i, 0);
		al_draw_bitmap(tiles[BORDA]->getImagem(),SCREEN_W - TAM_TILES, i, 0);
	}


	//desenha campo de fundo
	for(int i = TAM_TILES;i < SCREEN_W - TAM_TILES;i+= TAM_TILES)
		for (int j = TAM_BORDA; j < SCREEN_H - TAM_TILES; j += TAM_TILES)
			al_draw_bitmap(tiles[CAMPO]->getImagem(), i, j, 0);


}

void Jogo::textos()
{
	//TEXTO - título do jogo
	al_draw_text(fonte24, cores(COR_PRETO), SCREEN_W / 2 - 2, 5, ALLEGRO_ALIGN_CENTRE, "PAC-SNAKE");
	al_draw_text(fonte24, cores(COR_AMARELO), SCREEN_W / 2, 3, ALLEGRO_ALIGN_CENTRE, "PAC-SNAKE"); // sombra
	//TEXTO - Pontos
	al_draw_text(fonte14, cores(COR_BRANCO), SCREEN_W - 32, SCREEN_H - 31, ALLEGRO_ALIGN_RIGHT, "Points");

	//escreve pontos na tela
	al_draw_textf(fonte14, cores(COR_BRANCO), SCREEN_W - 32, SCREEN_H - 16, ALLEGRO_ALIGN_RIGHT, "%i", player->getPontos());

	//desenha vidas na parte inferior da tela (ou escreve se maior que 3)
	if (player->getLifes() <= 3) {
		for (int i = 0; i < player->getLifes(); i++)
			al_draw_bitmap_region(player->getImagem(), player->getWidth()*player->getCurFrame(), 0, player->getWidth(), player->getHeight(), 32 + 36 * i, SCREEN_H - 32, 0);
	}
	else {
		al_draw_bitmap_region(player->getImagem(), player->getWidth()*player->getCurFrame(), 0, player->getWidth(), player->getHeight(), 32, SCREEN_H - 32, 0);
		al_draw_textf(fonte24, cores(COR_BRANCO), 64 + 8, SCREEN_H - 29, ALLEGRO_ALIGN_LEFT, "x %i", player->getLifes());
	}

	if(pauseOn)
		al_draw_text(fonte24, cores(COR_AZUL), SCREEN_W/2, SCREEN_H/2, ALLEGRO_ALIGN_CENTRE, "JOGO PAUSADO!");
}

void Jogo::renderMenu()
{
	al_draw_bitmap(telaMenu, 0, 0, 0);

	//Títulos das teclas
	al_draw_text(fonte24, cores(corOpcaoMenu[0]), 48, 100, ALLEGRO_ALIGN_LEFT, "Tecla para cima:      ");
	al_draw_text(fonte24, cores(corOpcaoMenu[1]), 48, 130, ALLEGRO_ALIGN_LEFT, "Tecla para baixo:    ");
	al_draw_text(fonte24, cores(corOpcaoMenu[2]), 48, 160, ALLEGRO_ALIGN_LEFT, "Tecla para esquerda: ");
	al_draw_text(fonte24, cores(corOpcaoMenu[3]), 48, 190, ALLEGRO_ALIGN_LEFT, "Tecla para direita:  ");
	//Nomes das teclas
	al_draw_textf(fonte24, cores(corOpcaoMenu[0]), SCREEN_W - 80, 100, ALLEGRO_ALIGN_CENTRE, "%s", teclaJogoMenu[0]);
	al_draw_textf(fonte24, cores(corOpcaoMenu[1]), SCREEN_W - 80, 130, ALLEGRO_ALIGN_CENTRE, "%s", teclaJogoMenu[1]);
	al_draw_textf(fonte24, cores(corOpcaoMenu[2]), SCREEN_W - 80, 160, ALLEGRO_ALIGN_CENTRE, "%s", teclaJogoMenu[2]);
	al_draw_textf(fonte24, cores(corOpcaoMenu[3]), SCREEN_W - 80, 190, ALLEGRO_ALIGN_CENTRE, "%s", teclaJogoMenu[3]);

	//Habilita/desabilita Efeitos Sonoros
	al_draw_text(fonte24, cores(corOpcaoMenu[4]), 48, 250, ALLEGRO_ALIGN_LEFT, "Efeitos sonoros:  ");
	al_draw_textf(fonte24, cores(corOpcaoMenu[4]), SCREEN_W - 80, 250, ALLEGRO_ALIGN_CENTRE, "%s", opcaoSFX[0]);

	//Habilita/desabilita Música
	al_draw_text(fonte24, cores(corOpcaoMenu[5]), 48, 280, ALLEGRO_ALIGN_LEFT, "Musica:  ");
	al_draw_textf(fonte24, cores(corOpcaoMenu[5]), SCREEN_W - 80, 280, ALLEGRO_ALIGN_CENTRE, "%s", opcaoMusica[0]);

	//Seleciona Dificuldade
	al_draw_text(fonte24, cores(corOpcaoMenu[6]), 48, 340, ALLEGRO_ALIGN_LEFT, "Dificuldade:  ");
	al_draw_textf(fonte24, cores(corOpcaoMenu[6]), SCREEN_W - 80, 340, ALLEGRO_ALIGN_CENTRE, "%s", opcaoDificuldade[0]);

}

void Jogo::renderIntro(){

	al_draw_bitmap(telaIntro, 0, 0, 0);
	al_draw_text(fonte24, cores(COR_PRETO), SCREEN_W / 2, SCREEN_H / 2 + 228, ALLEGRO_ALIGN_CENTRE,
		"[ENTER] - Iniciar");
	al_draw_text(fonte24, cores(COR_PRETO), SCREEN_W / 2, SCREEN_H / 2 + 256, ALLEGRO_ALIGN_CENTRE,
		"[M] - menu");
}

void Jogo::renderGameOver(){
	al_draw_bitmap(telaGameOver, 0, 0, 0);
	al_draw_textf(fonte24, cores(COR_BRANCO), SCREEN_W / 2-14, 64,
		ALLEGRO_ALIGN_CENTRE, "Pontos: %d", player->getPontos());

	
	
}

void Jogo::drawMap()
{
	//obsMap;

	for (int i = 0; i < MAX_OBS; i++) {
		obstaculo[i]->setPosXY(0, 0);
	}

	int obsAtual = 0; // 0 até MAX_OBS

	for (int i = 0; i < MAP_V; i++) {
		for (int j = 0; j < MAP_H; j++) {
			if (obsMap[i][j] == 1) {
				obstaculo[obsAtual++]->setPosXY(TAM_BORDA + (j*TAM_OBS), TAM_BORDA + (i*TAM_OBS));
				
			}
			if (obsAtual == MAX_OBS) return;
			
			
		}
	}

}

ALLEGRO_COLOR Jogo::cores(int cor)
{
	switch (cor)
	{
	case COR_AMARELO:
		return al_map_rgb(0xff, 0xff, 0x00);
	case COR_BRANCO:
		return al_map_rgb(0xff, 0xff, 0xff);
	case COR_PRETO:
		return al_map_rgb(0x00, 0x00, 0x00);
	case COR_VERMELHO:
		return al_map_rgb(0xff, 0x00, 0x00);
	case COR_AZUL:
		return al_map_rgb(0x00, 0x00, 0xff);
	default:
		break;
	}


	return ALLEGRO_COLOR();
}

void Jogo::definePosFruta(Item *item)
{
	int frtX = 0;
	int frtY = 0;
	bool posOK = false;

	do
	{
		frtY = (rand() % (MAP_V));
		frtX = (rand() % (MAP_H));

		if (obsMap[frtY][frtX] == 1) {
			posOK = false;
		}
		else {
			posOK = true;
		}

	} while (!posOK);

	item->setPosXY(TAM_BORDA + (frtX*TAM_OBS + (TAM_OBS - TAM_ITEM) / 2),
		TAM_BORDA + (frtY*TAM_OBS + (TAM_OBS - TAM_ITEM) / 2));

	item->setAlive(true);
}


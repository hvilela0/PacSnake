// PacSnake.cpp

#include <Windows.h>

#include "PacSnake.h"

// cria instancia da classe jogo
Jogo *jogo = new Jogo();

// controla o looping do jogo
bool done	= false; 

int main() {

	// faz inicializações 
	if (!jogo->initAll()) return -1;

	// loopping do jogo
	while (!done) {

		jogo->monitoraEventos();	// aguarda por eventos no jogo // aguarda por eventos no jogo
		done = jogo->input();		// recebe entradas do jogo, se retornar true, encerra o jogo
		jogo->update();				// atualiza dados do jogo
		jogo->render();				// desenha para a tela
	}

	// libera memória
	jogo->destroyAll();
	delete jogo;

	return 0;
}

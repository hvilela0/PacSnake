#pragma once

#include "Colors.h"

/*
*	DEFINIÇÕES GLOBAIS DO JOGO
*/

// dimensões da tela  (razão 1,25)
#define SCREEN_W	800	
#define SCREEN_H	640

// frame por segundo
#define FPS			60


// tamanhos padrões 
#define TAM_BORDA	32
#define	TAM_PAC		32
#define TAM_OBS		32
#define TAM_ITEM	24

//itens
#define MAX_ITENS	 8

//obstaculos
#define MAX_OBS		52

//menu
#define NUM_OPCOES_MENU 7

// mapa de obstaculos
#define MAX_MAPS	3
#define MAP_H		(SCREEN_W-TAM_BORDA*2)/TAM_OBS //23
#define MAP_V		(SCREEN_H-TAM_BORDA*2)/TAM_OBS //18

//timers
#define TMR_FANTASMA_PISCANDO 600

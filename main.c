#include <stdio.h>
#include "player.h"
#include "tiro.h"
#include "enemy.h"
#include "pixel.h"
#include "victory.h"
#include "defeat.h"

#define TIROS_MAX 7

// HEX0 - HEX3 7-segment Display Adresses
// Base Adress = 0x10000020
// End Adress = 0x1000002F

int collision(int corrente, int pontos_player, int value);
int update_tiro_queue(int corrente);
void clear_tiro(int corrente);
void draw_tiro(int corrente);
void button_reset();
int button_detect(int mask);
void clear_screen();
void draw_pixel(int x, int y, short int color);
void draw_line(int x0,int x1, int y0, int y1, short int color);
void wait_vretrace();
void draw_victory_screen();
void draw_defeat_screen();
int switch_toggle(int mask);
void print_points_segments(int pontos);
void reset_switch();
int play_again();
void show_option_led();
void reset_option_led();
void get_mask(int mask);
void fila_tiro_led(int corrente);
void reset_fila_tiro_led();
void LCD_display(int row, char texto[]);
void LCD_reset();

struct tiro_t{
	int x;
	int y;
	int velocidade;
	int flag;
};

struct tiro_t tiros[TIROS_MAX];
struct tiro_t new_tiros[TIROS_MAX];


// SWITCH 0 = SPEED
// SWITCH 1 = DESCER
// SWITCH 2 = PODER TIRO
// SWITCH 3 = VELOCIDADE TIRO
// SIWTCH 4 = VIDA INIMIGOS 5
// SIWTCH 5 = VALOR PONTOS *2
int fmask = 0;

int main(){
	int play = 1;
	int x_novo = 158, x_antigo = 158, tiro_corrente = 0, pontos_player = 0, gambiarra = 0;
	int SHOT_POWER = 1, GAMBIARRA_SPEED = 200, DEFINED_SHOT_VELOCITY = 2, POINT_VALUE = 10;
	reset_switch();
	do{
		LCD_reset();
		LCD_display(0, "KEYS = 2 Dir");
		LCD_display(1, "3 Esq | 4 Tiro");
		x_novo = 158; x_antigo = 158; tiro_corrente = 0; pontos_player = 0; gambiarra = 0, fmask = 0;
		SHOT_POWER = 1; GAMBIARRA_SPEED = 200; DEFINED_SHOT_VELOCITY = 2; POINT_VALUE = 10;		
		clear_screen();
		draw_player(x_antigo);
		if(switch_toggle(0x1)){
			GAMBIARRA_SPEED /= 4;
			get_mask(0x1);
		}
		if(switch_toggle(0x2)){
			DOWN_SPACE *= 2;
			get_mask(0x2);
		}
		if(switch_toggle(0x4)){
			SHOT_POWER *= 2;
			get_mask(0x4);
		}
		if(switch_toggle(0x8)){
			DEFINED_SHOT_VELOCITY += 1;
			get_mask(0x8);
		}
		if(switch_toggle(0x10)){
			ENEMY_LIFE_TOTAL *= 2;
			get_mask(0x10);
		}
		if(switch_toggle(0x20)){
			POINT_VALUE *= 2;
			get_mask(0x20);
		}
		printf("Game Speed: %i\n", GAMBIARRA_SPEED);
		printf("Down Space: %i\n", DOWN_SPACE);
		printf("Shot Power: %i\n", SHOT_POWER);
		printf("Shot Velocity: %i\n", DEFINED_SHOT_VELOCITY);
		printf("Enemy Life Total: %i\n", ENEMY_LIFE_TOTAL);
		printf("Point Value: %i\n", POINT_VALUE);
		reset_switch();
		initialize_enemies();
		while(1){
			if (button_detect(0x4) && x_novo < 288){	// Detecta botão de movimento para direita
				x_antigo = x_novo;
				x_novo += 16;
				button_reset();
			}
			if (button_detect(0x8) && x_novo > 16){		// Detecta botão de movimento para esquerda
				x_antigo = x_novo;
				x_novo -= 16;
				button_reset();		
			}
			if (button_detect(0x2)){					// Detecta botão de atirar
				if(tiro_corrente < TIROS_MAX){
					int i;
					for(i = 0; i < SHOT_POWER; i++){
						tiros[tiro_corrente].x = x_novo + 8;
						tiros[tiro_corrente].y = 205 + 4*i;
						tiros[tiro_corrente].velocidade = DEFINED_SHOT_VELOCITY;
						tiros[tiro_corrente].flag = 1;
						tiro_corrente += 1;
					}
				}
				button_reset();
			}
			wait_vretrace();		
			reset_fila_tiro_led();
			clear_tiro(tiro_corrente);
			pontos_player = collision(tiro_corrente, pontos_player, POINT_VALUE);
			print_points_segments(pontos_player);
			tiro_corrente = update_tiro_queue(tiro_corrente);
			draw_tiro(tiro_corrente);
			fila_tiro_led(tiro_corrente);
			clear_player(x_antigo);
			if(gambiarra < GAMBIARRA_SPEED){
				move_enemy(0);
				gambiarra++;
			}
			else{
				move_enemy(1);
				gambiarra = 0;
			}
			draw_player(x_novo);
			if(victory_check()){
				draw_victory_screen();
				break;
			}
			if(lose_check()){
				draw_defeat_screen();
				break;
			}	
		}
		LCD_reset();
		LCD_display(0, "KEY2 = PlayAgain");
		LCD_display(1, "KEY3 = Exit");
		reset_option_led();
		reset_enemy();
		play = play_again();
	}while(play);
	clear_screen();
	return 0;
}

int update_tiro_queue(int corrente){
	int i, count = 0;
	
	for(i = 0; i < corrente; i++){
		if(tiros[i].flag){
			new_tiros[count].flag = tiros[i].flag;
			new_tiros[count].x = tiros[i].x;
			new_tiros[count].y = tiros[i].y;
			new_tiros[count].velocidade = tiros[i].velocidade;
			count++;
		}
	}
	memcpy(tiros, new_tiros, sizeof(struct tiro_t)*count);
	return count;
}

void draw_tiro(int corrente){
	int i;
	for(i = 0; i < corrente; i++){
		if(tiros[i].y <= 0){
			tiros[i].flag = 0;
			//printf("SAIU - %d\n", i);
		}
		if(tiros[i].flag){
			draw_pixel(tiros[i].x, tiros[i].y, 0x6B6E);
			//printf("%d %d\n ", i, tiros[i].y);
			tiros[i].y -= tiros[i].velocidade;
		}
	}
}

void fila_tiro_led(int corrente){
	volatile int *tl = (int *) 0x10000010;
	int i;
	int hex_mask[7] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40};
	unsigned int value_mask = 0;
	
	for(i = 0; i < corrente; i++){
		value_mask |= hex_mask[i];
	}
	*tl = value_mask;
}

void reset_fila_tiro_led(){
	volatile int *tl = (int *) 0x10000010;
	*tl = 0;
}

void clear_tiro(int corrente){
	int i;
	for(i = 0; i < corrente; i++){
		draw_pixel(tiros[i].x, tiros[i].y+tiros[i].velocidade, 0xFFFF);
	}
}

void LCD_reset(){
	volatile char *LCD = (char *) 0x10003050;
	*LCD = 1;
}

void LCD_display(int row, char texto[]){
	volatile char *LCD = (char *) 0x10003050;
	char * texto_ptr = texto;
	char instruction;
	instruction = row;
	if(row){
		instruction |= 0x40; 
	}
	instruction |= 0x80;
	*LCD = instruction;
	while(*texto_ptr){
		*(LCD + 1) = *texto_ptr;
		++texto_ptr;
	}		
}

int button_detect(int mask){
	volatile int *pb = (int *) 0x1000005C;
	return (*pb & mask);
}

void button_reset(){
	volatile int *pb = (int *) 0x1000005C;
	*pb = 0;
}

int switch_toggle(mask){
	volatile int *st = (int *) 0x10000040;
	show_option_led(mask);
	return (*st & mask);
}

void reset_switch(){
	volatile int *st = (int *) 0x10000040;
	*st = 0;
}

int collision(int corrente, int pontos_player, int value){
	int tiro_c, enemy_c;
	for(tiro_c = 0; tiro_c < corrente; tiro_c++){
		for(enemy_c = 0; enemy_c < ENEMY_MAX; enemy_c++){
			if((enemies[enemy_c].life != 0) && (tiros[tiro_c].x >= enemies[enemy_c].x) && (tiros[tiro_c].x <= enemies[enemy_c].x+16) 
				&& (tiros[tiro_c].y <= enemies[enemy_c].y) && (tiros[tiro_c].y >= enemies[enemy_c].y-16)){
				enemies[enemy_c].life--;
				tiros[tiro_c].flag = 0;
				if(!enemies[enemy_c].life){
					pontos_player += value;
					printf("Pontos: %d\n", pontos_player);				
				}
				//printf("COLIDIU - ");
			}
		}
	}
	return pontos_player;
}

void get_mask(mask){
	fmask |= mask;
}

void show_option_led(mask){	
	volatile int *rl = (int *) 0x10000000;
	*rl = fmask;
}

void reset_option_led(){
	volatile int *rl = (int *) 0x10000000;
	*rl = 0;
}

void print_points_segments(int pontos){
	int hex_mask[10] = {0x3F, 0x6, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x7, 0x7F, 0x6F};
	int i, posic;
	
	volatile int *sp = (int *) 0x10000020;

	unsigned int value_mask = 0;		
	for(i = 0; i < 3; i++){
		posic = pontos%10;
		pontos = pontos/10;
		
		value_mask |= hex_mask[posic] << (i*8);
	}
	*sp = value_mask;
}

void clear_screen(){
	int y, x;
	for(y = 0; y <= 320; y++){
		for(x = 0; x <= 240; x++){
			draw_pixel(y, x, 0xFFFF);
		}
	}
}

int play_again(){
	button_reset();
	printf("Deseja Continuar?\n");
	while(1){
		if(button_detect(0x4))	// Detecta botão de NAO
			return 1;
		if(button_detect(0x8))	// Detecta botão de SIM
			return 0;
	}
}

void draw_shot(int x0,int x1, int y0, int y1, short int color){
	if(x0 > x1){
		int aux = x0;
		x0 = x1;
		x1 = aux;
		aux = y0;
		y0 = y1;
		y1 = aux;
	}
	int x, deltax = (x1 - x0), deltay = abs(y1 - y0), y_step, y = y0;
	if(y0 < y1)
		y_step = 1;
	else
		y_step = -1;
	for(x = x0; x <= x1; x++){
		draw_pixel(x, y, 0x6B6E);
	}
}

void wait_vretrace(){
	volatile int *bvideo = (int *) 0x10003020;
	*bvideo = 1;
	while((*(bvideo + 3) & 1) == 1);
}

/****************************************************************/
/* FUJITSU MICROELECTRONICS EUROPE  -  BITMAP CONVERTER (V2.20) */
/* BITMAP FILE "enemy"                                          */
/* BITMAP SIZE : X=16 Y=16 PIXEL                                */
/* CREATED 25/05/2017 15:25:21                                  */
/* OUTPUT FORMAT : JASMINE 16BPP (565) UNCOMPRESSED             */
/****************************************************************/
#include "pixel.h"
#define ENEMY_MAX 12

void draw_enemy(int x, int y, unsigned short* enemy_canvas);
//void draw_enemy_lines();
void draw_enemy_lines(int x_s, int x_e, int y_s, int y_e);
void clear_enemy_lines();
void move_enemy(int still);
void move_line_right();
void move_line_left();
void move_line_down();
void initialize_enemies();
int victory_check();
int lose_check();
void reset_enemy();
void show_enemy_life_led(int total);

int DOWN_SPACE = 16, ENEMY_LIFE_TOTAL = 3;

int x_start = 64, x_end = 240, y_start = 10, y_end = 58, velocidade = 16, move_flag = 1;
int x_s_old = 64, x_e_old = 248, y_s_old = 10, y_e_old = 58;

void reset_enemy(){
	x_start = 64; x_end = 240; y_start = 10; y_end = 58; velocidade = 16; move_flag = 1;
	x_s_old = 64; x_e_old = 248; y_s_old = 10; y_e_old = 58;
	DOWN_SPACE = 16; ENEMY_LIFE_TOTAL = 3;
}

const unsigned short enemy[256] = { 
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xBE38,0xBDF8,0xBDF8,0xBE38,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xC658,0x68A5,0x58C6,0x58C6,0x68A5,0xC658,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xBE37,0x68C6,0x7804,0x3CC0,
0x3CC0,0x7804,0x68C6,0xBE37,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xBDF7,0x48ED,0x5828,0x5328,0x5FE2,0x5FE2,0x5328,0x5828,0x48ED,
0xBDF7,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x49CC,0x3850,0x2CC1,
0x86AA,0x67C2,0x67C2,0x86AA,0x2CC1,0x3850,0x49CC,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0x49EC,0x1847,0x47A0,0x85AD,0x6E66,0x6E66,0x85AD,
0x47A0,0x1847,0x49EC,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x5AAC,
0x38EF,0x2C22,0x6E85,0x7C8D,0x7C8D,0x6E85,0x2C22,0x38EF,0x5AAC,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xA535,0x41CE,0x3810,0x33E1,0x64AC,
0x64AC,0x33E1,0x3810,0x41CE,0xA535,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xBDB9,0x8491,0x596E,0x500E,0x3208,0x3208,0x500E,0x596E,0x8491,
0xBDB9,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xB4F8,0xAC17,0xD6B9,
0x514C,0x306F,0x306F,0x514C,0xD6B9,0xAC17,0xB4F8,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0x5232,0x5891,0xF71E,0xDF1A,0x292C,0x292C,0xDF1A,
0xF71E,0x5891,0x5232,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x5210,
0x382C,0xE6DC,0xFFFE,0x116B,0x116B,0xFFFE,0xE6DC,0x382C,0x5210,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xDE4D,0xC628,0xFFDD,0xFFBE,0x112B,
0x112B,0xFFBE,0xFFDD,0xC628,0xDE4D,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xE73B,0x190B,0x190B,0xE73B,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xA573,0x1948,0x1948,0xA573,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFDF,0x73B1,0x73B1,0xFFDF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF
};

const unsigned short blank[256] = { 
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF
};

struct enemy_t{
	int life;
	int x;
	int y;
};

struct enemy_t enemies[ENEMY_MAX];

void initialize_enemies(){
	int i;
	for(i = 0; i < ENEMY_MAX; i++){
		enemies[i].life = ENEMY_LIFE_TOTAL;
	}
	show_enemy_life_led(ENEMY_LIFE_TOTAL);
}

void show_enemy_life_led(total){
	int hex_mask[6] = {0x20000, 0x10000, 0x8000, 0x4000, 0x2000, 0x1000};
	int i;
	unsigned int value_mask = 0;
	
	volatile int *rl = (int *) 0x10000000;
	
	for(i = 0; i < total; i++){
		*rl |= hex_mask[i];
	}
}

int victory_check(){
	int i, count = 0;
	for(i = 0; i < ENEMY_MAX; i++){
		count += enemies[i].life;
	}
	if(!count)
		return 1;
	return 0;
}

int lose_check(){
	int i, count = 0;
	for(i = ENEMY_MAX/2; i < ENEMY_MAX; i++){
		count += enemies[i].life;
	}
	if(y_end >= 202 && count >= 1){
		return 1;
	}
	else if(y_end >= 250 && count == 0){
		return 1;
	}
	return 0;
}

/*void move_enemy(int move_flag){
	if(!move_flag){
		draw_enemy_lines(x_start, x_end, y_start, y_end);
	}
	else if(x_start > 32 && x_end < 288){ //enquanto n�o chega nas bordas
		x_s_old = x_start;
		x_e_old = x_end;
		x_start += velocidade;
		x_end += velocidade;
		draw_enemy_lines(x_start, x_end, y_start, y_end);
		
		printf("X_Start Mov Lateral: %i\n", x_start);
		printf("X_End Mov Lateral: %i\n", x_end);
	}
	else{ //chega nas bordas
		y_s_old = y_start;
		y_e_old = y_end;
		y_start += 48;
		y_end += 48;
		draw_enemy_lines(x_start, x_end, y_start, y_end);
		velocidade *= -1;
		x_start += velocidade;
		x_end += velocidade;
		clear_enemy_lines();	
		
		printf("Y_Start Mov Lateral: %i\n", y_start);
		printf("Y_End Mov Lateral: %i\n", y_end);
		printf("Velocidade: %i\n", velocidade);
	}
}*/

void move_enemy(still){
	if(!still){
		clear_enemy_lines();
		draw_enemy_lines(x_start, x_end, y_start, y_end);
	}
	else{
		clear_enemy_lines();
		y_s_old = y_start;
		y_e_old = y_end;
		if(move_flag == 1){	
			x_s_old = x_start;
			x_e_old = x_end;	
			move_line_right();
		}
		else if(move_flag == -1){
			x_s_old = x_start;
			x_e_old = x_end;
			move_line_left();
		}
	}
}

void move_line_right(){
	if(x_end < 288){
		x_start += velocidade;
		x_end += velocidade;
	}
	if(x_end == 288){
		move_flag = -1;
		move_line_down();
	}
	draw_enemy_lines(x_start, x_end, y_start, y_end);
}

void move_line_left(){
	if(x_start > 32){
		x_start -= velocidade;
		x_end -= velocidade;
	}
	if(x_start == 32){
		move_flag = 1;
		move_line_down();
	}
	draw_enemy_lines(x_start, x_end, y_start, y_end);
}

void move_line_down(){
	clear_enemy_lines();
	y_start += DOWN_SPACE;
	y_end += DOWN_SPACE;
	y_s_old = y_start;
	y_e_old = y_end;
}

void clear_enemy_lines(){
	int x, y;
	for(y = y_s_old; y <= y_e_old; y += 32){	
		for(x = x_s_old; x <= x_e_old; x += 32){
			draw_enemy(x, y, blank);
		}
	}
}


/*void draw_enemy_lines(){
	int x, y, counter = 0;
	for(y = 20; y < 60; y += 20){	
		for(x = 50; x < 300; x += 50){
			if(enemies[counter].life > 0){
				enemies[counter].x = x;
				enemies[counter].y = y;
				draw_enemy(x, y, enemy);
			}
			else{
				enemies[counter].x = x;
				enemies[counter].y = y;
				draw_enemy(x, y, blank);
			}
			counter++;
		}
	}
}*/

void draw_enemy_lines(int x_s, int x_e, int y_s, int y_e){
	int x, y, counter = 0;
	for(y = y_s; y <= y_e; y += 32){	
		for(x = x_s; x <= x_e; x += 32){
			if(enemies[counter].life > 0){
				enemies[counter].x = x;
				enemies[counter].y = y;
				draw_enemy(x, y, enemy);
			}
			else{
				enemies[counter].x = x;
				enemies[counter].y = y;
				draw_enemy(x, y, blank);
			}
			counter++;
		}
	}
}

void draw_enemy(int x, int y, unsigned short* enemy_canvas){
	int x_p, y_p, i = 0;
	for(y_p = y; y_p < y+16; y_p++){
		for(x_p = x; x_p < x+16; x_p++){
			draw_pixel(x_p, y_p, enemy_canvas[i]);
			i++;
		}
	}
}
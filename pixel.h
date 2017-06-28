void draw_pixel(int x, int y, short int color);

void draw_pixel(int x, int y, short int color){
	volatile short int *p = (short int *) 0x08000000;
	p += ((x << 1) + (y << 10))/2;
	*p = color;
}

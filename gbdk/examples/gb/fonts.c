/*
    fonts.c
    Simple example of how to use multiple fonts on the GB
    Michael Hope, 1999.
*/

#include <stdio.h>
#include <gb/font.h>
#include <gb/console.h>
#include <gb/drawing.h>

INT16 DespRight(INT16 a, INT8 b) {
	return a >> b;
}

void main(void)
{
	UINT8 i;
	INT16 tmp;
    mode(get_mode() | M_NO_SCROLL);

	
	tmp = 40;
	for(i = 0; i < 10; ++i) {
		printf("\n%d", (INT16)(DespRight(tmp + i * 4, 3)));
	}
}

    
    

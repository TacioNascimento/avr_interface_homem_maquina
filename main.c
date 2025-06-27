#define F_CPU   16000000L
#include <util/delay.h>
#include "lcd.h"
#include "teclado.h"


void main(void) 
{
    char tecla;

    lcd_init();
    teclado_init();

    while( 1 )
    {
        tecla = teclado_scan();

    }
}
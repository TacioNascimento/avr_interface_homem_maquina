#include <avr/io.h>
#include "bits.h"

/* -------------------------------------------------
 *          MAPA DE ENTRADAS E SAIDAS
 * -------------------------------------------------
 *  PORT    |Pino   |Conexão
 * ---------|-------|-------------------------------
 *  PB0     | 0     | Teclado Saída   L0
 *  PB1     | 1     | Teclado Saída   L1
 *  PB2     | 2     | Teclado Saída   L2
 *  PB3     | 3     | Teclado Saída   L3
 *  PB4     | 4     | Teclado Entrada C0
 *  PB5     | 5     | Teclado Entrada C1
 *  PB6     | 6     | Teclado Entrada C2
 *  PB7     | 7     | Teclado Entrada C3
 * -------------------------------------------------
 */

#define NUM_COL         4
#define NUM_LIN         4

#define L0_MASK         0x01
#define C0_MASK         0x10


void teclado_init( void )
{
    DDRD = 0x0F;    // L_Mask
    PORTD = 0xFF;   
}

const char tecladoMatriz[4][4] =  { {'1','2','3','A'},
                                    {'4','5','6','B'},
                                    {'7','8','9','C'},
                                    {'*','0','#','D'} };



unsigned char teclado_scan( void )
{
    unsigned char aux = 0;
    unsigned int l,c;

    for( l=0; l<NUM_LIN; l++ )
    {
        PORTD = ~(L0_MASK << l);
        for( c=0; c<NUM_COL; c++ )
        {
            if( (~PIND) & (C0_MASK<<c) )
            {
                aux = tecladoMatriz[l][c];
            }
        }
    }   
    return( aux );
}
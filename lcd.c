#define F_CPU   16000000L
#include <util/delay.h>
#include <avr/io.h>
#include "bits.h"


/* -------------------------------------------------
 *          MAPA DE ENTRADAS E SAIDAS
 * -------------------------------------------------
 *  PORT    | Pino  |Conexão
 * ---------|-------|-------------------------------
 *  PB0     | 8     | LCD_D4
 *  PB1     | 9     | LCD_D5
 *  PB2     | 10    | LCD_D6
 *  PB3     | 11    | LCD_D7
 *  PB4     | 12    | LCD_RS
 *  PB5     | 13    | LCD_EN
 * -------------------------------------------------
 */


//***************** Interface com PORTs/Pinos
#define LCD_ROWS        2
#define LCD_COLS        16
#define LCD_BUS(bus)    PORTB = ((PORTB & 0xF0) | ((bus) & 0x0F))
#define LCD_RS(bit)     (bit) ? (SETBIT(PORTB,4)) : (CLRBIT(PORTB,4))
#define LCD_EN(bit)     (bit) ? (SETBIT(PORTB,5)) : (CLRBIT(PORTB,5))

//***************** Definicao de Comandos ao LCD 
#define LCD_CLEAR_DISPLAY           0x01

#define LCD_RETURN_HOME             0x02

#define LCD_ENTRY_MODE_SET          0x04
#define LCD_EMS_CURSOR_RIGHT        0x02
#define LCD_EMS_CURSOR_LEFT         0x00
#define LCD_EMS_SHIFT_DISPLAY       0x01

#define LCD_DISPLAY_CONTROL         0x08
#define LCD_DC_DISPLAY_ON           0x04
#define LCD_DC_DISPLAY_OFF          0x00
#define LCD_DC_CURSOR_ON            0x02
#define LCD_DC_CURSOR_OFF           0x00
#define LCD_DC_BLINK_ON             0x01
#define LCD_DC_BLINK_OFF            0x00

#define LCD_CURSOR_SHIFT            0x10
#define LCD_CS_LEFT                 0x00
#define LCD_CS_RIGHT                0x04

#define LCD_DISPLAY_SHIFT           0x10
#define LCD_DS_LEFT                 0x08
#define LCD_DS_RIGHt                0x0C

#define LCD_FUNCTION_SET            0x20
#define LCD_FS_DATA_LENGTH_8        0x10
#define LCD_FS_DATA_LENGTH_4        0x00
#define LCD_FS_LINE_NUMBER_1        0x00
#define LCD_FS_LINE_NUMBER_2        0x08
#define LCD_FS_DOTS_FORMAT_5x8      0x00
#define LCD_FS_DOTS_FORMAT_5x11     0x04

#define LCD_SET_CGRAM_ADDRS( adrs ) (0x40+(adrs & 0x3F))

#define LCD_SET_DDRAM_ADDRS( adrs ) (0x80+(adrs & 0x7F))
#define LCD_ADDR_LINE_0             0x00
#define LCD_ADDR_LINE_1             0x40



// Escreve um comando no display (Instruction Register)
void lcd_instReg( unsigned char i )
{   
    LCD_RS(0);
    LCD_BUS(i>>4);

    LCD_EN(0);
    if( i == LCD_CLEAR_DISPLAY || i == LCD_RETURN_HOME )
        _delay_ms( 2 );
    else
        _delay_us( 40 );
    LCD_EN(1);

    if( (i & 0xF0) == (LCD_FUNCTION_SET | LCD_FS_DATA_LENGTH_4) )
    {
        LCD_BUS(i>>4);
        LCD_EN(0);
        _delay_us( 40 );
        LCD_EN(1);
    }

    LCD_BUS(i);
    LCD_EN(0);
    if( i == LCD_CLEAR_DISPLAY || i == LCD_RETURN_HOME )
        _delay_ms( 2 );
    else
        _delay_us( 40 );
    LCD_EN(1);
}



// Escreve um dado no display (Data Register))
void lcd_dataReg( unsigned char d )
{
    LCD_RS(1);
    LCD_BUS(d >> 4);
    LCD_EN(0);
    _delay_us( 40 );
    LCD_EN(1);

    LCD_BUS(d);
    LCD_EN(0);
    _delay_us( 40 );
    LCD_EN(1);
}

// Posicionar o cursor na coordenada: linha e coluna 
void lcd_lincol( unsigned char lin, unsigned char col)
{
    lcd_instReg( LCD_SET_DDRAM_ADDRS( ((LCD_ADDR_LINE_1 * lin) + (col + LCD_ADDR_LINE_0) ) ) );
}




// Inicializa os pinos conectados ao display
void lcd_init( void )
{
    DDRB |= 0x3F;
    PORTB &= ~0x3F;

    LCD_EN(1);
    lcd_instReg( LCD_FUNCTION_SET|LCD_FS_DATA_LENGTH_4|LCD_FS_LINE_NUMBER_2);
    lcd_instReg( LCD_DISPLAY_CONTROL|LCD_DC_DISPLAY_ON|LCD_DC_CURSOR_OFF|LCD_DC_BLINK_OFF );
    lcd_instReg( LCD_CLEAR_DISPLAY );
    lcd_instReg( LCD_RETURN_HOME );
}

// Escreve uma string no display
void lcd( unsigned char lin, unsigned char col, const char * str )
{
    char pos = col;
    lcd_lincol( lin, col );

    while( *str )
    {
        lcd_dataReg( *str );
        ++str;
        ++pos;
    }
}

// Escreve um número inteiro no display
void lcd_num(  unsigned char lin, unsigned char col,
                    int num, unsigned char tam )
{
    int div;
    unsigned char size;
    char sinal; // 0:+ 1:-

    sinal = (num < 0);
    if( sinal )
        num = (~num) + 1;

    lcd_lincol(lin, col);

    div=10000;
    size = 5;
    while( div > 1 )
    {
        if( num/div == 0 )
            --size;
        else
            break;
        div/=10;
    }

    while( tam > (size+sinal) && tam > 1 )
    {
        lcd_dataReg(' ');
        --tam;
    }  

    if( sinal )
        lcd_dataReg('-');

    do
    {
        lcd_dataReg( (num / div) + '0' );
        num = num % div;
        div/=10;
    }
    while( div >= 1 );
}

// Apaga todos os caracteres no display
void lcd_clr( void )
{
    lcd_instReg(LCD_CLEAR_DISPLAY);
}

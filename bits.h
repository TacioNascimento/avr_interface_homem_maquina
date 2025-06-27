#ifndef BITS_H
#define BITS_H

#define BIT(reg,bit)        (reg & (1<<bit))
#define SETBIT(reg,bit)     reg |=  (1<<bit)
#define CLRBIT(reg,bit)     reg &= ~(1<<bit)

#endif
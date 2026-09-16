#ifndef USART_H_
#define USART_H_

#include "macros.h"

#define SIZEBUFF 64

void usart_print(char *str);
void usart_println(char *str);
void usart_init(uint16_t baudrate, uint8_t fast);
void usart_write(char byte_tx);
char usart_read();
uint16_t usart_cnt();
char* usart_readString();

#endif

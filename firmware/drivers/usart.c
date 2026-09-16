#include "usart.h"

volatile uint8_t usart_run = 1;
volatile uint16_t rx_buff = 0;
volatile char rx_buffer[SIZEBUFF] = {0x00};
char stringRcv[SIZEBUFF] = {0x00};


ISR(USART_TX_vect){
    usart_run = 1;
}

ISR(USART_RX_vect){
    static uint16_t cnt_buff = 0;
    rx_buffer[cnt_buff] = UDR0;
    rx_buff++;
    cnt_buff++;

    if(cnt_buff>=SIZEBUFF)
        cnt_buff = 0;

}

void usart_init(uint16_t baudrate, uint8_t fast){
    if(fast){
        UCSR0A |= (1<<U2X0);
        UBRR0 = (F_CPU/(8UL*baudrate))-1;
    }else{
        UCSR0A &= ~(1<<U2X0);
        UBRR0 = (F_CPU/(16UL*baudrate))-1;
    }

    UCSR0B |= (1<<RXCIE0) |
              (1<<TXCIE0) |
              (1<<RXEN0)  |
              (1<<TXEN0);

    UCSR0C = 0X06;
}

void usart_write(char byte_tx){
    while(!usart_run);
    usart_run = 0;
    UDR0 = byte_tx;
}

void usart_print(char *str){
   for(uint16_t i=0; str[i]!='\0'; i++)
        usart_write(str[i]);
}

void usart_println(char *str){
    for(uint16_t i=0; str[i]!='\0'; i++)
        usart_write(str[i]);

    usart_write(0x0A);
}

uint16_t usart_cnt(){
    return rx_buff;
}

char usart_read(){
    static uint16_t buff_lo = 0;
    char byte_rx = 0;

    byte_rx = rx_buffer[buff_lo];
    buff_lo++;
    rx_buff--;

    if(buff_lo>=SIZEBUFF){
        buff_lo = 0;
    }

    return byte_rx;
}

char* usart_readString(){
    uint16_t c = 0;
    do{
        stringRcv[c] = usart_read();
        c++;
    }while(usart_cnt()!=0);

    return stringRcv;
}
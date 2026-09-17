#include "macros.h"
#include "drivers/usart.h"
#include "drivers/adc.h"

void uint16_to_string(uint16_t value, char *str);

int main(void) {
	cli();
	usart_init(9600, 0);
	adc_init();
    sei();
	char byte_rcv = 0x00;
	char string[SIZEBUFF] = {0X00};
	uint16_t i = 0;
	uint16_t adc_value = 12345;
	char byte0;
	char byte1;
	char str[6] = ""; // 5 dígitos + '\0'

    while (1){
		adc_value = adc_read(0);
		str[0] = (char) ((adc_value/10000) + '0');
		str[1] = (char) ((adc_value/1000) % 10 + '0');
		str[2] = (char) ((adc_value/100) % 10 + '0');
		str[3] = (char) ((adc_value/10) % 10 + '0');
		str[4] = (char) (adc_value % 10 + '0');
		str[5] = '\0';

		byte0 = (char)(adc_value & 0x00FF);
		byte1 = (char)((adc_value & 0xFF00) >> 8);
	

		for(i=0; usart_cnt()>0; i++){
			string[i] = usart_read();
		}
		for(i+1;i<=SIZEBUFF;i++){
			string[i] = 0x00;
		}

		for(uint16_t i=0; str[i]!='\0'; i++)
				usart_write(str[i]);
		usart_write(0x0A);
		
		_delay_ms(500);

		/*if(string[0] != 0x00){
			for(uint16_t i=0; str[i]!='\0'; i++)
				usart_write(str[i]);
		}*/

    }
}

void uint16_to_string(uint16_t value, char *str){
    char temp[6];
    uint8_t i = 0;
    uint8_t j = 0;

    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    while (value > 0) {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i > 0) {
        str[j++] = temp[--i];
    }

    str[j] = '\0';
}

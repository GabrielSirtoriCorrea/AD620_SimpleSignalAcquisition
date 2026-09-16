#include "adc.h"

volatile uint8_t adc_done = 1;

ISR(ADC_vect){
	adc_done = 1;
}

void adc_init(){
	ADMUX  |= (1<<REFS0);
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (0b111<<ADPS0);
}

uint16_t adc_read(uint8_t ch){
    adc_done = 0;
	DIDR0 |= (1<<ch);
	ADMUX &= 0xF0;
	ADMUX |= ch;
	ADCSRA |= (1<<ADSC);

	while (!adc_done);
	
	return ADC;
}
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void data(char character){
	PORTG |=(1<<3);
	PORTG &=~(1<<4);
	PORTH = character;
	latch();
}
void cmd(char command){
	PORTG &=~(1<<3); //command mode
	PORTG &=~(1<<4);
	PORTH = command;
	latch();
}
void latch(){
	PORTG |=(1<<5); //enable
	_delay_ms(50);
	PORTG &=~(1<<5);
	_delay_ms(50);
}
void wel(){
	cmd(0x01);
	char we[11] = {'C','A','R',' ','A','T',' ','G','A','T','E'};
	for (int i = 0; i <= 11; i ++){
		data(we[i]);
	}
}
void regi(){
	cmd(0x01);
	char re[8] = {'R','E','G','I','S','T','E','R'};
	for (int i = 0; i <= 8; i ++){
		data(re[i]);
	}
}

void numberPlate(){}
void under10(){
	char text[] = "Below 10 years";
	for (int i = 0; i <= strlen(text); i ++){
		data(text[i]);
	}
}
void over10(){
	char text[] = "Above 10 years";
	for (int i = 0; i <= strlen(text); i ++){
		data(text[i]);
	}
}

int main(void)
{
	DDRD=0b00000000;
	DDRG=0b11111111;
	DDRJ=0b00000111;
	DDRF=0b11111111;
	EIMSK |= (1 << INT0);
	sei();
	char str[10];
	
	cmd(0x0f);
	cmd(0x3f);
	
    while (1) 
    {    //keypad
		
		
		PORTJ = 0b11111011;
		
		//check 1
		if ((PINJ & 0b00001000) == 0){
			data('1');
			strncat(str, '1', 1);
			data(str[0]);
			
		}
		
		//check 4
		else if ((PINJ & 0b00010000) == 0){
			data('4');
		}
		
		//check 7
		else if ((PINJ & 0b00100000) == 0){
			data('7');
	
		}
		
		
		PORTJ = 0b11111101;
		//check 2
		if ((PINJ & 0b00001000) == 0){
			data('2');
		}
		
		//check 5
		else if ((PINJ & 0b00010000) == 0){
			
			data('5');
		}
		
		//check 8
		else if ((PINJ & 0b00100000) == 0){
			
			data('8');
		}
		
		PORTJ = 0b11111110;
		//check 3
		if ((PINJ & 0b00001000) == 0){
			data('3');
		}
		
		//check 6
		else if ((PINJ & 0b00010000) == 0){
			data('6');
		}
		
		//check 9
		else if ((PINJ & 0b00100000) == 0){
			data('9');
		}
		
		else if((PINJ & 0b01000000) == 0){
			cmd(0x01);
			data('#');
			data(str[0]);
		}
		_delay_ms(100);
		
		
    }
}
ISR(INT0_vect){
	PORTG ^=(1<<0);
	PORTG |=(1<<1);
	_delay_ms(400);
	PORTG &=~(1<<1);
	cmd(0b00001111);
	wel();
	_delay_ms(1200);
	cmd(0b00001111);
	regi();
	_delay_ms(1000);
	cmd(0x01);
	under10();
	cmd(0xC0);
	
}



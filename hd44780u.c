#include "PLL.h"
#include "port_init.h"
#include "hd44780u.h"

static const unsigned char switchEmoji[8][8] = {
	{0x0E,0x1F,0x15,0x1F,0x1F,0x1F,0x0E,0x0A}, //skull emo
	{0x00,0x0A,0x1F,0x15,0x1F,0x0E,0x04,0x00}, //square emo
};

void InitDisplayPort(void)
{
	InitPortValues();
	wait_us(17000); //wait for 17ms since datasince wait more then 15ms
	SendDisplayNibble(0x00, 0x03);
	wait_us(6100); //wait 6.1ms
	SendDisplayNibble(0x00, 0x03);
	wait_us(102); // 102us
	SendDisplayNibble(0x00, 0x03);
	wait_us(40); 
	SendDisplayNibble(0x00, 0x02);
	SendDisplayByte(0x00, 0x02, 0x08);
	SendDisplayByte(0x00, 0x00, 0x08);
	SendDisplayByte(0x00, 0x00, 0x01);
	SendDisplayByte(0x00, 0x00, 0x07);
}

void InitCustomCharacters(void)
{
	for(unsigned int i = 0; i < 2; i++)  
	{	
		unsigned int CGRAM_loc = 0x40+(i*8);
		SendDisplayByte(0, (0xF0 & CGRAM_loc) >> 4, 0x0F & CGRAM_loc);
		for(unsigned int j = 0; j < 8; j++)
			SendDisplayByte(1, (0xF0 & switchEmoji[i][j]) >> 4, 0x0F & switchEmoji[i][j]);
	}
}

void SendDisplayByte(unsigned long RS, unsigned long upperBitData, unsigned long lowerBitData)// 8 bit data
{ 
	wait_us(40);
	SendDisplayNibble(RS, upperBitData);
	wait_us(30);
	SendDisplayNibble(RS, lowerBitData);
}

// RS receive 1 or 0
void SendDisplayNibble(unsigned long RS, unsigned long data)// 4 bit data
{
	if(RS == 0x00)
		GPIO_PORTA_DATA_R &= ~PA3;
	else
		GPIO_PORTA_DATA_R |= PA3;

	GPIO_PORTB_DATA_R = data;
	
	ENPulseDelay();
}

void ENPulseDelay(void) {
  NVIC_ST_RELOAD_R = 37-1;  
  NVIC_ST_CURRENT_R = 0;       
	
	GPIO_PORTA_DATA_R |= PA2;
  while((NVIC_ST_CTRL_R&0x00010000)==0){ 
  }
	GPIO_PORTA_DATA_R &= ~PA2;
}

void clearDisplayScreen(void) {
	SendDisplayByte(0x00, 0x00, 0x01);
}

void printDisplay(unsigned char character) {
	unsigned char lowerbit = 0x0F & character;
	unsigned char upperbit = (0xF0 & character) >> 4 ;
	SendDisplayByte(0x01, upperbit, lowerbit);
}

void displayDeleteLastChar(void)
{
	SendDisplayByte(0x00, 0x01, 0x00);
	printDisplay(' ');
	SendDisplayByte(0x00, 0x01, 0x00);
}

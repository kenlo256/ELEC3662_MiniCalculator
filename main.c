#define MAXNUMCHARS 40 // maximum number that 2-line LCD display can handle
#define PASSWORDSIZE 8
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "UART.h"
#include "PLL.h"
#include "EEPROM.h"
#include "port_init.h"
#include "hd44780u.h"

void InitialisationStage(void);
void WelcomingStage(void);
void InitCursor(void);
void setPassword(void);

void appendCharacters(unsigned char input);
void deleteLastChar(void);
void printSentence(unsigned char string[80]);
void errorAction(void);
void printErrorMsg(unsigned char *errormsg);
void printPasswordSecondLine(unsigned char info);

void switchToggleEmoji(void);
void resetUserInputs(void);
void secondLinePasswordPrompt(void);

unsigned char GetKeyASCII(const char config[4][4]);
void mainCalculatorFunction(void) __attribute__((noreturn));

static unsigned char shiftToggle = 0x0;
static unsigned char userInputs[MAXNUMCHARS] = {}; 
static unsigned char tempInput = ' ';
static unsigned char numChars = 0;

static const unsigned char keyPadConfig[2][4][4] = {
	{
		{'1','2','3','+'}, // unshift config
		{'4','5','6','-'},
		{'7','8','9','.'},
		{'N','0','R','S'},
	},
	{
		{'1','2','3','x'}, // shifted config
		{'4','5','6','/'},
		{'7','8','9','E'},
		{'P','0','D','S'},
	}
};

int main(void){  
	InitialisationStage();
	InitCursor();
	EEPROM_Init();
	WelcomingStage();
	switchToggleEmoji();
	mainCalculatorFunction();
}

void mainCalculatorFunction(void)
{
	while(1)
	{
		switch(tempInput = GetKeyASCII(keyPadConfig[shiftToggle]))
		{
			case 'N':
				if(!numChars)
					break;
				if(strstr(userInputs, "0/"))
				{
					printErrorMsg("Undefined");
					switchToggleEmoji();
					break;
				}
				if(checkSyntax(userInputs, numChars))
				{
					tokenizeInputString(userInputs, numChars);
					strncpy(userInputs, doCalculation(), sizeof(userInputs));
					cleanTokenEntries();
					clearDisplayScreen();
					numChars = strlen(userInputs);
					printSentence(userInputs);
				}
				else
					printErrorMsg("Syntax Error");
				switchToggleEmoji();
				break;
			case 'D':
				clearDisplayScreen();
				resetUserInputs();		
				wait_us(1700);
				switchToggleEmoji();
				break;
			case 'S':
				shiftToggle = !shiftToggle;
				switchToggleEmoji();
				break;
			case 'R':
				deleteLastChar();
				break;
			case '0' ... '9': case '+': case '-': case 'x': case '/': case '.': case 'E': 
				if(numChars < MAXNUMCHARS-2)
				{
					printDisplay(tempInput);
					appendCharacters(tempInput);
				}
				break;
			case 'P':
				clearDisplayScreen();
				resetUserInputs();
				setPassword();
				switchToggleEmoji();
				break;
		}
	}
}
void setPassword(void)
{
	unsigned char infoStatus = 0;
	unsigned long ulAddr = 0x100; 
	
	printPasswordSecondLine(infoStatus);
	
	while(1)
	{
		switch(tempInput = GetKeyASCII(keyPadConfig[0]))
		{
			case '0' ... '9':
				if(numChars < PASSWORDSIZE)
				{
					printDisplay('*');
					appendCharacters(tempInput);
				}
				break;
			case 'N':
				
				if(numChars != PASSWORDSIZE)
				{
					printErrorMsg("Not in 8 digits");
					clearDisplayScreen();
					printPasswordSecondLine(infoStatus);
					resetUserInputs();
					break;
				}
								
				if(!infoStatus)
				{
					EEPROM_Write(userInputs, ulAddr);
					infoStatus = 1;
					clearDisplayScreen();
					resetUserInputs();
					printPasswordSecondLine(infoStatus);	
				}
				else
				{
					unsigned char EEPROM_Pw[8]= {};
					EEPROM_Read(EEPROM_Pw, ulAddr);
					
					if(strcmp(userInputs, EEPROM_Pw))
					{
						printErrorMsg("Wrong Pin!");
						infoStatus = 0;
						clearDisplayScreen();
						printPasswordSecondLine(infoStatus);
						resetUserInputs();
						break;
					}
					else
					{
						infoStatus = 2;
						clearDisplayScreen();
						resetUserInputs();
					}
				}	
				break;
			case 'R':
				deleteLastChar();
				break;
		}
		if(infoStatus == 2)
			break;
	}
}

void InitCursor(void)
{
	SendDisplayNibble(0x00, 0x02);
	SendDisplayByte(0x00, 0x02, 0x02);
	wait_us(1475);
	InitCustomCharacters();
	SendDisplayByte(0x00, 0x00, 0x0F);
}

void InitialisationStage(void)
{
	PLL_Init(80);//clock set to 80MHz
	PortA_Init();
	PortB_Init();
	PortD_Init();
	PortE_Init();
	SysTick_Init();
	UART_Init();
	InitDisplayPort();
}

void secondLinePasswordPrompt(void)
{
	wait_us(1700);
	SendDisplayByte(0,0x0A, 0x08);
	printSentence("Type password");
	SendDisplayByte(0,0x00, 0x02);
}

void WelcomingStage(void)
{
	unsigned long ulAddr = 0x100; 
	unsigned char EEPROM_Pw[8]= "";
	unsigned char status = 0;
	
	EEPROM_Read(EEPROM_Pw, ulAddr);
	
	if(isdigit(EEPROM_Pw[0]))
	{
		secondLinePasswordPrompt();
		while(1)
		{
			switch(tempInput = GetKeyASCII(keyPadConfig[0]))
			{
				case '0' ... '9':
					if(numChars < PASSWORDSIZE)
					{
						printDisplay('*');
						appendCharacters(tempInput);
					}
					break;
				case 'N':
				
					if(numChars != PASSWORDSIZE)
					{
						printErrorMsg("Not in 8 digits");
						clearDisplayScreen();
						secondLinePasswordPrompt();
						resetUserInputs();
						break;
					}
					
					if(strcmp(userInputs, EEPROM_Pw))
					{
						printErrorMsg("Wrong Pin!");
						clearDisplayScreen();
						secondLinePasswordPrompt();
						resetUserInputs();
						break;
					}
					else
					{
						status = 1;
						clearDisplayScreen();
						resetUserInputs();	
					}
				case 'R':
					deleteLastChar();
					break;
			}
			if(status)
				break;
		}
	}
	else
		setPassword();
}

void printPasswordSecondLine(unsigned char info)
{
	unsigned char *secondLineInfo[2] = {"Enter 8 digits", "Retype pin"};
	wait_us(1700);
	SendDisplayByte(0,0x0A, 0x08);
	printSentence(secondLineInfo[info]);
	SendDisplayByte(0,0x00, 0x02);
}

void switchToggleEmoji(void)
{
	wait_us(1700);
	SendDisplayByte(0,0x0C, 0x00);
	
	for(unsigned char j = 0; j < 40 ;j++)
		printDisplay(shiftToggle);
	
	SendDisplayByte(0,0x00, 0x02);
	printSentence(userInputs);
}

unsigned char GetKeyASCII(const char config[4][4])
{
	unsigned long counter = 0;
	for(unsigned char i = 0; i <= 3; i++)
	{
		GPIO_PORTD_DATA_R = (1 << i);
		for(unsigned char j = 0; j <= 3; j++)
		{
			if(GPIO_PORTE_DATA_R & (1 << j))
			{
				wait_us(20000);// bouncing time
				while(GPIO_PORTE_DATA_R & (1 << j)) {counter++;} // holding time
				GPIO_PORTD_DATA_R = 0x0;// reset the current port to low
				wait_us(22000); // debouncing time
				if(counter > 10) // a counter that can identify noise that was picked up by pressing a key
					return config[j][i];
			}
		}
	}
	return ' ';
}	

void appendCharacters(unsigned char input)
{
	userInputs[numChars++] = input;
	if(numChars > 16)
		SendDisplayByte(0x00, 0x01, 0x08);
}

void deleteLastChar(void)
{
	if(numChars > 0)
	{
		numChars--;
		userInputs[numChars] = '\0';
		displayDeleteLastChar();
		if(numChars > 15)
			SendDisplayByte(0x00, 0x01, 0x0C);
	}
}

void printErrorMsg(unsigned char *errormsg)
{
	clearDisplayScreen();
	printSentence(errormsg);
	errorAction();
}

void printSentence(unsigned char string[80]) {
	for(unsigned long i = 0; i < strlen(string); i++)
	{
		wait_us(2000);
		printDisplay(string[i]);
		if(i > 15)
			SendDisplayByte(0x00, 0x01, 0x08); 
	}
}

void errorAction(void)
{
	while(GetKeyASCII(keyPadConfig[0]) != 'N') {}
	clearDisplayScreen();
	printSentence(userInputs);
}

void resetUserInputs(void)
{
	memset(&userInputs[0], '\0', sizeof(userInputs));
	numChars = 0;
}
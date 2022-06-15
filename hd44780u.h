#ifndef TM4C123GH6PM_H
#define PA2 0x00000004
#define PA3 0x00000008

void SendDisplayNibble(unsigned long RS, unsigned long data);
void SendDisplayByte(unsigned long RS, unsigned long upperBitData, unsigned long lowerBitData);
void InitDisplayPort(void);
void InitCustomCharacters(void);

void clearDisplayScreen(void);
void displayDeleteLastChar(void);
void printDisplay(unsigned char character);

void ENPulseDelay(void);
#endif
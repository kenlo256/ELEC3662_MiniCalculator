#include "port_init.h"
#include "EEPROM.h"

void waitEEPROMDONE(void);

void EEPROM_Init(void) { 
	
	SYSCTL_RCGCEEPROM_R |= 0x00000001;
	waitEEPROMDONE();
	if((EEPROM_EESUPP_R&0x8) == 0x0 && (EEPROM_EESUPP_R&0x4) == 0x0)
	{
		SYSCTL_SREEPROM_R |= 0x00000001;
		SYSCTL_SREEPROM_R = 0x00000000;
		waitEEPROMDONE();
		if((EEPROM_EESUPP_R&0x8) == 0x0 && (EEPROM_EESUPP_R&0x4) == 0x0)
			return;	
	}
}

void waitEEPROMDONE(void) { volatile unsigned long delay; unsigned long status;
	delay = SYSCTL_RCGCEEPROM_R;
	delay = SYSCTL_RCGCEEPROM_R;
	delay = SYSCTL_RCGCEEPROM_R;
	delay = SYSCTL_RCGCEEPROM_R;
	delay = SYSCTL_RCGCEEPROM_R;
	delay = SYSCTL_RCGCEEPROM_R;
	while((EEPROM_EEDONE_R&0x01) != 0) {}
}

void EEPROM_Write(unsigned char* data, unsigned long addr)
{
	while((EEPROM_EEDONE_R&0x01) != 0) {}
	
	HWREG(EEPROM_EEBLOCK) = EEPROMBlockFromAddr(addr);
	HWREG(EEPROM_EEOFFSET) = OFFSET_FROM_ADDR(addr);
		
	unsigned char count = 8;
	
	while(count)
	{
		EEPROM_EERDWR_R = *data++;
		while((EEPROM_EEDONE_R&0x01) != 0) {}
		count--;
		HWREG(EEPROM_EEOFFSET)++;
		if(HWREG(EEPROM_EEOFFSET) == 0)
			HWREG(EEPROM_EEBLOCK) += 1;
	}
	
	if(!(EEPROM_EEDONE_R&0x01))
		return;
}

void EEPROM_Read(unsigned char* data, unsigned long addr)
{
	HWREG(EEPROM_EEBLOCK) = EEPROMBlockFromAddr(addr);
	HWREG(EEPROM_EEOFFSET) = OFFSET_FROM_ADDR(addr);
	
	unsigned char count = 8;
	
	while(count)
	{
		*data++ = EEPROM_EERDWR_R;
		count--;
		HWREG(EEPROM_EEOFFSET)++;
		if(HWREG(EEPROM_EEOFFSET) == 0)
			HWREG(EEPROM_EEBLOCK) += 1;
	}
}


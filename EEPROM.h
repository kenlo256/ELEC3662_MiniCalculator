#ifndef EEPROM_H

#define EEPROMBlockFromAddr(ulAddr) ((ulAddr)>> 6)
#define OFFSET_FROM_ADDR(x) (((x)>> 2) & 0x0F)
#define HWREG(x) (*((volatile unsigned long *)(x)))

#define SYSCTL_RCGCEEPROM_R     (*((volatile unsigned long *)0x400FE658))
#define SYSCTL_SREEPROM_R       (*((volatile unsigned long *)0x400FE558))

#define EEPROM_EEBLOCK        	((volatile unsigned long)0x400AF004)
#define EEPROM_EEOFFSET       	((volatile unsigned long)0x400AF008)
#define EEPROM_EESIZE_R       	(*((volatile unsigned long *)0x400AF000))
#define EEPROM_EERDWR_R         (*((volatile unsigned long *)0x400AF010))
#define EEPROM_EEDONE_R         (*((volatile unsigned long *)0x400AF018))
#define EEPROM_EESUPP_R         (*((volatile unsigned long *)0x400AF01C))


void EEPROM_Init(void);

void EEPROM_Write(unsigned char* data, unsigned long addr);
void EEPROM_Read(unsigned char* data, unsigned long addr);

unsigned long sizeofEEPROM(void);
#endif
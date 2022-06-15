#include "port_init.h"
	
void PortA_Init(void) { volatile unsigned long delay;

 SYSCTL_RCGC2_R |= 0x00000001; // 1) activate clock for Port A
 delay = SYSCTL_RCGC2_R; // allow time for clock to start
 GPIO_PORTA_AMSEL_R = 0x00; // 3) disable analog on PA
 GPIO_PORTA_PCTL_R = 0x00000000; // 4) PCTL GPIO on PA4-0
 GPIO_PORTA_DIR_R = 0x0C; // 5) PA2 PA3 out
 GPIO_PORTA_AFSEL_R = 0x00; // 6) disable alt funct on PA7-0
 GPIO_PORTA_DEN_R = 0x1F; // 7) enable digital I/O on PA4-PA0
}

void PortB_Init(void) { volatile unsigned long delay;

 SYSCTL_RCGC2_R |= 0x00000002; // 1) activate clock for Port B
 delay = SYSCTL_RCGC2_R; // allow time for clock to start
 GPIO_PORTB_AMSEL_R = 0x00; // 3) disable analog on PB
 GPIO_PORTB_PCTL_R = 0x00000000; // 4) PCTL GPIO on PB4-0
 GPIO_PORTB_DIR_R = 0x0F; // 5) PB0,PB1,PB2,PB3 out
 GPIO_PORTB_AFSEL_R = 0x00; // 6) disable alt funct on PB7-0
 GPIO_PORTB_DEN_R = 0x1F; // 7) enable digital I/O on PB4-PB0
}

void PortD_Init(void) { volatile unsigned long delay;

 SYSCTL_RCGC2_R |= 0x00000008; // 1) activate clock for Port D
 delay = SYSCTL_RCGC2_R; // allow time for clock to start
 GPIO_PORTD_LOCK_R = 0x4C4F434B; // unlock GPIO for PD
 GPIO_PORTD_AMSEL_R = 0x00; // 3) disable analog on PD
 GPIO_PORTD_PCTL_R = 0x00000000; // 4) PCTL GPIO on PD4-0
 GPIO_PORTD_DIR_R = 0x0F; // 5) 
 GPIO_PORTD_AFSEL_R = 0x00; // 6) disable alt funct on PD7-0
 GPIO_PORTD_DEN_R = 0x1F; // 7) enable digital I/O on PD4-PD0
 GPIO_PORTD_DATA_R = 0x0;
}

void PortE_Init(void) { volatile unsigned long delay;

 SYSCTL_RCGC2_R |= 0x00000010; // 1) activate clock for Port E
 delay = SYSCTL_RCGC2_R; // allow time for clock to start
 GPIO_PORTE_AMSEL_R = 0x00; // 3) disable analog on PE
 GPIO_PORTE_PCTL_R = 0x00000000; // 4) PCTL GPIO on PF4-0
 GPIO_PORTE_DIR_R = 0x00; // 5) 
 GPIO_PORTE_AFSEL_R = 0x00; // 6) disable alt funct on PE7-0
 GPIO_PORTE_PDR_R = 0x0F; // 7) enable pull-down resistor on PE
 GPIO_PORTE_DEN_R = 0x1F; // 8) enable digital I/O on PE4-PE0
 GPIO_PORTE_DATA_R = 0x0;
}

void InitPortValues(void)
{
	GPIO_PORTA_DATA_R = 0x00000000;
	GPIO_PORTB_DATA_R = 0x00000000;
}

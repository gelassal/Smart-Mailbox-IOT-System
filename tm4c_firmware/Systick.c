// SysTick.c
// Runs on TM4C123. 
// 
// Authors: 
// Date: August 28, 2018

#include "tm4c123gh6pm.h"

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void) {
	NVIC_ST_CTRL_R = 0x00; //DISABLE CLOCK TO INITIALIZE
	NVIC_ST_RELOAD_R = 5000; //MAX RELOAD VALUE
	NVIC_ST_CURRENT_R = 0; //CLEAR CURRENT
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF)|(0x00000000);
	NVIC_ST_CTRL_R = 0x07; // ENABLE AND USE CORE CLOCK

}

void SysTick_start(void)
{
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;
}
void SysTick_stop(void)
{
	NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE;
}

void SysTick_Set_Current_Note(unsigned long n_value)
{
  NVIC_ST_RELOAD_R = n_value-1;// update reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
}



void init_Servo(void){
    // Activate the clock for Port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2;  // Enable clock for Port c
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R2) == 0){};  // Wait for the clock to stabilize

    // GPIO configuration
    GPIO_PORTC_AMSEL_R &= ~0x20;       // Disable analog function for Pc5
    GPIO_PORTC_AFSEL_R &= ~0x20;       // Disable alternate function for Pc5
    GPIO_PORTC_PCTL_R &= ~0x00F00000;  // Clear PCTL register for pc5 selecting GPIO functionality
    GPIO_PORTC_DIR_R |= 0x20;          // Set PB0-PB2 as output (columns)
    GPIO_PORTC_DEN_R |= 0x20;          // Enable digital I/O for PB0-PB6
		//GPIO_PORTC_PUR_R |= 0x20;   // Enable pull-up resistors on PB3-PB6
		//GPIO_PORTC_PUR_R &= ~0x20; // Enable pull-down resistors on PB0-2
		//GPIO_PORTB_DR2R_R |= 0x20; // increase drive strength to PB0-2

}


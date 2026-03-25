////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 
// Example project for PinPad
// Description: 
// Hardware connections: 7 pins for rows and columns
// PB0 - FOR Col1 PB1 - FOR Col2  PB2- FOR Col3
// PB3 - FOR Row1 PB4 - FOR Row2 PB5 - For Row 3 PB6 - FOR Row4
// By George Elassal, Nathan Wong, Youssef Shafeek 8/28/2024
/////////////////////////////////////////////////////////////////////////////

//////////////////////1. Pre-processor Directives Section////////////////////
#include "tm4c123gh6pm.h"
#include "PinPad.h"
/////////////////////////////////////////////////////////////////////////////

//////////////////////2. Declarations Section////////////////////////////////

////////// Local Global Variables //////////

void init_PinPad(void){
    // Activate the clock for Port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;  // Enable clock for Port B
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R1) == 0){};  // Wait for the clock to stabilize

    // GPIO configuration
    GPIO_PORTB_AMSEL_R &= ~0xFF;       // Disable analog function for PB0-PB6
    GPIO_PORTB_AFSEL_R &= ~0xFF;       // Disable alternate function for PB0-PB6
    GPIO_PORTB_PCTL_R &= ~0xFFFFFFFF;  // Clear PCTL register for PB0-PB6, selecting GPIO functionality
    GPIO_PORTB_DIR_R |= 0x0F;          // Set PB0-PB2 as output (columns)
    GPIO_PORTB_DIR_R &= ~0xF0;         // Set PB3-PB6 as input (rows)
    GPIO_PORTB_DEN_R |= 0xFF;          // Enable digital I/O for PB0-PB6
		//GPIO_PORTB_PUR_R |= 0x78;   // Enable pull-up resistors on PB3-PB6
		//GPIO_PORTB_PUR_R &= ~0xF0; // Enable pull-down resistors on PB0-2
		//GPIO_PORTB_DR2R_R |= 0x07; // increase drive strength to PB0-2

}

void LED_Init(void){ 
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;	// Activate F clocks
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOF)==0){};
		
  GPIO_PORTF_AMSEL_R &= ~0x0E;      // 3) disable analog function
  GPIO_PORTF_PCTL_R &= ~0x0000FFF0; // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R |= 0x0E;         // 6) PF1-PF3 output
  GPIO_PORTF_AFSEL_R &= ~0x0E;      // 7) no alternate function     
  GPIO_PORTF_DEN_R |= 0x0E;         // 8) enable digital pins PF3-PF1
  LED = Dark;                       // Turn off all LEDs.
}
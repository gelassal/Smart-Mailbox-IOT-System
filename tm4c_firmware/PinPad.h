/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM controlled Car
// Description: 
/////////////////////////////////////////////////////////////////////////////

//////////////////////1. Pre-processor Directives Section////////////////////
#include "tm4c123gh6pm.h"

/////////////////////////////////////////////////////////////////////////////

#define LED (*((volatile unsigned long *)0x40025038))  // use onboard three LEDs: PF321

#define Dark    	0x00
#define RED     	0x02
#define BLUE    	0x04
#define GREEN   	0x08
#define YELLOW  	0x0A
#define CRAN      0x0C
#define WHITE  	0x0E
#define PURPLE  	0x06

void init_PinPad(void);
void LED_Init(void);
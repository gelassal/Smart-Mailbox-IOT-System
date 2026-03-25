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
#include <stdbool.h>
#include "UART0.h"
#include "PLL.h"
#include "Systick.h"
#include "UART3.h"
#include "Timer1A.h"

//#include "PWM.h"
/////////////////////////////////////////////////////////////////////////////

//////////////////////2. Declarations Section////////////////////////////////

#define MAX_STR_LEN 50

#define TRIGGER_PIN  (*((volatile unsigned long *)0x40024010))  // PE2 is the trigger pin	
#define ECHO_PIN     (*((volatile unsigned long *)0x40024008))  // PE1 is the echo pin
#define TRIGGER_VALUE 	0x04   // trigger at bit 7
#define ECHO_VALUE 			0x02   // trigger at bit 6
#define MC_LEN 					0.0625 // length of one machine cycle in microsecond for 16MHz clock
#define SOUND_SPEED 		0.0343 // centimeter per micro-second

#define OUTOFRANGE 90


////////// Local Global Variables //////////
int count = 0;
int help;

static volatile uint8_t done=0;
static volatile uint32_t distance=0;
bool flag = false;


signed int presses = 0;
int numbers[4];
int rowData;
//int period = 200;
int password[4] = {'1','5','9','0'};
bool safe = false;
int closed = 2;
int angle = 2;
int open = 6;
void SysTick_Handler(void);
void UART2_Handler(void);
uint8_t chr;
uint8_t str_idx=0;
uint8_t string[MAX_STR_LEN];  
bool end_of_str=false;
	
// Define the states
typedef enum {
    Col1,
    Col2,
    Col3,
		Col4
} State;

////////// Function Prototypes //////////
void handleCol1(int row);
void handleCol2(int row);
void handleCol3(int row);
void handleCol4(int row);
int readRows(void);
void Delay(void);
void UART3_InString(unsigned char *bufPt, unsigned short max);
void PortE_Init(void);
extern void GPIOPortE_Handler(void);
void Timer1A_Handler(void);



int main(void){
	PLL_Init();
	LED_Init();
	UART0_Init(false, false);
	UART3_Init(true, false);
	Delay();
	//PWM_PC5_Init();
	SysTick_Init();
	init_PinPad();
	State currentState = Col1; // Initial state
	SysTick_start();
	init_Servo();
	Timer1A_Init();
	Delay();
	PortE_Init();

	
	
	while(1){
		OutCRLF_0();
		UART0_OutString((uint8_t *)"Enter Your Password  -  ");		
		while (presses <= 3) {
				switch (currentState) {
						case Col1:
								GPIO_PORTB_DATA_R |= 0x01;
								handleCol1(readRows());
								// Transition logic: move to the next state
								currentState = Col2;
								break;

						case Col2:
								GPIO_PORTB_DATA_R |= 0x02;
								handleCol2(readRows());
								// Transition logic: move to the next state
								currentState = Col3;
								break;

						case Col3:
								GPIO_PORTB_DATA_R |= 0x04;
								handleCol3(readRows());
								// Transition logic: move to the first state or exit loop
								currentState = Col1;
								break;
						

						default:
								break;
				}
		}
		
		int i = 0;
		while(i < 4) {
			if (numbers[i] != password[i]) {
				safe = false;
				break;
			}
			else{
				safe = true;
			}
			i += 1;
		}
		
		
		if(safe){
			// OPEN LOCK
			angle = open;
			
			UART3_OutChar('3'); //update website
			
			OutCRLF_0();
			UART0_OutString((uint8_t *)"Unlocked");
		  OutCRLF_0();
			LED = GREEN;
			Delay();
			LED = Dark;
			Delay();
			LED = GREEN;
			Delay();
			LED = Dark;
			Delay();
		}else if(!safe & (angle == closed)){
			// CLOSE LOCK
			angle = closed;
			
			UART3_OutChar('4'); //update website
			
			OutCRLF_0();
			UART0_OutString((uint8_t *)"Wrong Passcode");
			OutCRLF_0();
			LED = RED;
			Delay();
			LED = Dark;
			Delay();
			LED = RED;
			Delay();
			LED = Dark;
			Delay();
		}
		presses = 0;
  }

	return 0;
}

int readRows(void) {
    rowData = GPIO_PORTB_DATA_R & 0xF0; // Mask bits 4 through 7
    if (rowData == 0x10) {  // Binary 00010000
			while((GPIO_PORTB_DATA_R & 0xF0) == 0x10){}
			//LED = RED;
      return 1;
    } 
    else if (rowData == 0x20) {  // Binary 00100000
			while((GPIO_PORTB_DATA_R & 0xF0) == 0x20){}
			//LED = BLUE;
		 return 2;
    } 
    else if (rowData == 0x40) {  // Binary 01000000
			while((GPIO_PORTB_DATA_R & 0xF0) == 0x40){}
			//LED = GREEN;
			return 3;
    }
		else if (rowData == 0x80) {   // Binary 10000000
			while((GPIO_PORTB_DATA_R & 0xF0) == 0x80){}
			//LED = YELLOW;
			return 4;
    } 
    else if (rowData == 0x00) {  // No bits set
			///////////////LED = PURPLE;  // Ensure LED is defined and set appropriately
			return 0;
    } 
    else {
			//LED = WHITE;
			return 0;  // No valid row detected
    }
}


void handleCol1(int row){
	int inposition = true;
	GPIO_PORTB_DATA_R |= 0x01;
	bool reset = false;
	if(row){
		if(row == 1){
			numbers[presses] = '1';	
			UART0_OutChar('1');
		}
		if(row == 2){
			numbers[presses] = '4';
			UART0_OutChar('4');			
		}
		if(row == 3){
			numbers[presses] = '7';	
			UART0_OutChar('7');
		}
		if(row == 4){	
			UART0_OutChar('*');
			presses = 0;
			//PUT HERE SONIC SENSOR
			done = 0;
			distance = 0;
			TRIGGER_PIN &= ~TRIGGER_VALUE; // send low to trigger
			GPTM_Wait1us(2);
			TRIGGER_PIN |= TRIGGER_VALUE; // send high to trigger
			GPTM_Wait1us(10);
			TRIGGER_PIN &= ~TRIGGER_VALUE; // send low to trigger
			while (!done);
			
			if(distance < 10){
				LED= BLUE;
				angle = closed;
			}else{
				LED = WHITE;
			}
			Delay();
			
			reset = true;
		//LED = YELLOW;
		}
		if(!reset){
			presses += 1;
		}
		Delay();
	}
	GPIO_PORTB_DATA_R &= ~0x07;
}

void handleCol2(int row){
	GPIO_PORTB_DATA_R = 0x02;
	if(row){
		if(row == 1){
			numbers[presses] = '2';	
			UART0_OutChar('2');
		}
		if(row == 2){
			numbers[presses] = '5';	
			UART0_OutChar('5');
		}
		if(row == 3){
			numbers[presses] = '8';	
			UART0_OutChar('8');
		}
		if(row == 4){
			numbers[presses] = '0';	
			UART0_OutChar('0');
		}
		presses += 1;
		Delay();
	}
	GPIO_PORTB_DATA_R &= ~0x07;
}

void handleCol3(int row){
	GPIO_PORTB_DATA_R = 0x04;
	if(row){
		if(row == 1){
			numbers[presses] = '3';	
			UART0_OutChar('3');
		}
		if(row == 2){
			numbers[presses] = '6';	
			UART0_OutChar('6');
		}
		if(row == 3){
			numbers[presses] = '9';	
			UART0_OutChar('9');
		}
		if(row == 4){
			numbers[presses] = '#';	
			UART0_OutChar('#');
		}
		presses += 1;
		Delay();
	}
	GPIO_PORTB_DATA_R &= ~0x07;
}

void handleCol4(int row){
	GPIO_PORTB_DATA_R |= 0x08;
	if(row){
		if(row == 1){
			numbers[presses] = 'A';	
			UART0_OutChar('A');
		}
		if(row == 2){
			numbers[presses] = 'B';	
			UART0_OutChar('B');
		}
		if(row == 3){
			numbers[presses] = 'C';	
			UART0_OutChar('C');
		}
		if(row == 4){
			numbers[presses] = 'D';	
			UART0_OutChar('D');
		}
		presses += 1;
		Delay();
	}
	GPIO_PORTB_DATA_R &= ~0x07;
}

void Delay(void){
	unsigned long volatile time;
  time = 727240*31/91;  // 0.25sec
  while(time){
		time--;
  }
}

//Interrupt service routine, 
//frequency is determined by current tone being played
void SysTick_Handler(void){
  //LED ^= GREEN;
	count++;
	if(count >= 64){
			count = 0;
	}else if(count <= (angle)){
		//LED = Dark;
		GPIO_PORTC_DATA_R  |= 0x20;
	}else	if(count > (angle)){
		//LED = GREEN;
		GPIO_PORTC_DATA_R  &= ~0x20;

	}
		
}

void UART3_InString(unsigned char *bufPt, unsigned short max) {
int length=0;
char character;
  character = UART3_InChar();
  while((character!=CR) && (length < max-1) ){
		*bufPt = character;
		bufPt++;
		length++;
    character = UART3_InChar();
	}
  *bufPt = 0; // adding null terminator to the end of the string.
}


void UART3_Handler(void){
		LED = PURPLE;
		//UART0_OutString((uint8_t *)"how tf is purple being hit");
	  OutCRLF_0();
		UART0_OutString((uint8_t *)"INPUT = ");
	  UART0_OutChar(chr);
		OutCRLF_0();
		for (uint32_t time=0;time<256000;time++) {}
		if(UART3_RIS_R&UART_RIS_RXRIS){       // received one item
			if ((UART3_FR_R&UART_FR_RXFE) == 0){ // if sending finished
				chr = UART3_DR_R&0xFF;
				if(chr == '1'){
					LED = RED;
					safe = true;
					angle = open;
					
				}
				else if(chr == '2'){
					LED = GREEN;
					OutCRLF_0();
					UART0_OutString((uint8_t *)"INPUT = ");
					UART0_OutChar(chr);
					OutCRLF_0();
					LED = GREEN;
								//PUT HERE SONIC SENSOR
					done = 0;
					distance = 0;
					TRIGGER_PIN &= ~TRIGGER_VALUE; // send low to trigger
					GPTM_Wait1us(2);
					TRIGGER_PIN |= TRIGGER_VALUE; // send high to trigger
					GPTM_Wait1us(10);
					TRIGGER_PIN &= ~TRIGGER_VALUE; // send low to trigger
					while (!done);
					
					if(distance < 10){
						LED= BLUE;
						angle = closed;
						//UART3_OutChar('3');
					}else{
						LED = WHITE;
						//UART3_OutChar('4');
					}
				}
				else if(chr == '3'){
					//LED = CYAN; 
					
				}
				else if(chr == '0'){
  				LED = YELLOW;
					OutCRLF_0();
					UART0_OutString((uint8_t *)"INPUT = ");
					UART0_OutChar(chr);
					OutCRLF_0();
					
				}
				else{
					LED = Dark;
				}
			}
			
		}
	
	UART3_ICR_R = UART_ICR_RXIC;        // acknowledge RX FIFO
}

void PortE_Init(void){ 
  SYSCTL_RCGCGPIO_R |= 0x10;  // Enable clock for GPIO Port E
	while ((SYSCTL_PRGPIO_R & 0x10) == 0)
		; // Wait until Port E is ready

                                    // 2) no need to unlock PE1 and PE2
  GPIO_PORTE_PCTL_R &= ~0x000FF0; // 3) regular GPIO for PE1 and PE2
  GPIO_PORTE_AMSEL_R &= ~0x06;      // 4) disable analog function on PE1 and PE2
  GPIO_PORTE_DIR_R &= ~0x02;        // 5) PE1: echo pin, input
  GPIO_PORTE_DIR_R |= 0x04;         // 5) PE2: trigger pin, output
  GPIO_PORTE_AFSEL_R &= ~0x06;      // 6) regular port function for PE1 and PE2
  GPIO_PORTE_DEN_R |= 0x06;         // 7) enable digital I/O on PE1 and PE2
  GPIO_PORTE_IS_R &= ~0x02;         // PE1 is edge-sensitive
  GPIO_PORTE_IBE_R |= 0x02;         // PE1 is both edges
  GPIO_PORTE_IEV_R &= ~0x02;        // PE1 both edge event
  GPIO_PORTE_ICR_R = 0x02;          // clear flag for PE1
  GPIO_PORTE_IM_R |= 0x02;          // arm interrupt on PE1
  NVIC_PRI1_R = (NVIC_PRI1_R & 0xFFFFFF1F) | 0x00000060; // (g) priority 3 for Port E
  NVIC_EN0_R |= 0x00000010;         // (h) enable Port E edge interrupt in NVIC
}

void GPIOPortE_Handler(void){
		if (ECHO_PIN==ECHO_VALUE){  // echo pin rising edge is detected, start timing
		//SysTick_Start();
		Timer1A_Start();
	}
	else { // echo pin falling edge is detected, end timing and calculate distance.
    // The following code is based on the fact that the HCSR04 ultrasonic sensor 
    // echo pin will always go low after a trigger with bouncing back
    // or after a timeout. The maximum distance can be detected is 400cm.
		// The speed of sound is approximately 340 meters per second,�
		// or �.0343 c/�S.
    // Distance = (echo pulse width * 0.0343)/2; = ((# of mc)*MC_LEN*SOUND_SPEED)/2
		//SysTick_Stop();
		Timer1A_Stop();
		//distance = (uint32_t)(SysTick_Get_MC_Elapsed()*MC_LEN*SOUND_SPEED)/2;
		if(flag){
			distance = (uint32_t)(GPTM_Get_MC_Elapsed()*MC_LEN*SOUND_SPEED)/2;		
			flag = false;
		}
		done = 1;
	}
  GPIO_PORTE_ICR_R = 0x02;      // acknowledge flag for pin 1
}

void Timer1A_Handler(void){
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
	distance = OUTOFRANGE;
	//LED = GREEN;
	//if(done == 0){
		flag = true;
		//LED = GREEN;
	//}
	//set flag 
}
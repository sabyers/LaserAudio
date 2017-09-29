//-------------------------------------------------------------------------------
//      Laser Modulate
//
//
//
//   Target: TI LaunchPad development board with MSP430G2553 device
//
//       Date:           September 17, 2017
//       Last Revision:  1.0
//       Assembler/IDE:  IAR Embedded Workbench 5.5
//
//       HW I/O assignments:
//       P1.0    (Output) Right Analog Output
//       P1.1    (Output) Left Analog Output
//       P1.2    Not Used
//       P1.3    Not Used
//       P1.4    Not Used
//       P1.5    Not Used
//       P1.6    Not Used
//       P1.7    Not Used
//
//       P2.0    (Input) Signal In
//       P2.1    Not Used 
//       P2.2    Not Used
//       P2.3    Not Used
//       P2.4    Not Used
//       P2.5    Not Used
//       P2.6    Not Used
//       P2.7    Not Used
//

#include <msp430g2553.h>

//----------------------------------------------------------------------
// CONSTANT DEFINITIONS

#define TIMER_A1_COUNT_1    110 // set count value for TimerA_1  
//Signal Out

//----------------------------------------------------------------------
// GLOBAL VARIABLE definition
unsigned int Delay = 0x0200u;   //Set Delay time to stabilize pushbutton and 
                                //the temperature display change rate 
unsigned int RawSignalRight;

unsigned int LeftRightSelect = 0; //Start with right audio

unsigned int SigIn;

void demodulator(void);

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  // set up Port 2 
  P2DIR &= ~BIT0; //P2.0 Input
  P2IFG &= ~0xFF;  // clear ALL the Int Flags on Port 2
  
  // set up Port 1 
  P1DIR = 0x1F;     // P1.0 (Right Audio) -> Output
  P1IFG &= ~0xFF;  // clear ALL the Int Flags on Port 1
  
  
  
//Configure Clocks
  DCOCTL = 0;                   //Select lowest DC0x and MODx Settings
  BCSCTL1 = CALBC1_12MHZ;        //Set range
  DCOCTL = CALDCO_12MHZ;         //Set DCO step + modulation
  
//SetupTimerA1
  TA1CCR0 = TIMER_A1_COUNT_1;   //load a count value into the counter
  TA1CTL = TASSEL_2 + ID_0 + MC_1;  //SMCLK/1, up mode
  TA1CCTL0 = CCIE;                  //enable the timer interrupt
  

   _BIS_SR(GIE);   // enable general interrupts 
  
  while(1)      
  {
    
  }
  
  
}


#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer_A1_ISR (void)
//void demodulator(void)
{
  if(LeftRightSelect == 0)
  {
    if(P2IN&0x1)
      P1OUT |= BIT0;
    else
      P1OUT &= ~BIT0;     
  }
  else {
    if(P2IN&0x1)
      P1OUT |= BIT1;
    else
      P1OUT &= ~BIT1;  
  }   
  
  LeftRightSelect ^= 0x1; //Toggles between 1 and 0 
}

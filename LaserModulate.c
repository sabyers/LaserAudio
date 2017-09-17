//-------------------------------------------------------------------------------
//      Laser Modulate
//
//
//
//   Target: TI LaunchPad development board with MSP430G2553 device
//
//       Date:           May 2, 2016
//       Last Revision:  1.0
//       Written by:     Saul Byers A.S., Texas Tech University
//       Adapted from:   Dr. Michael Helm, ECE dept, Texas Tech University
//       Assembler/IDE:  IAR Embedded Workbench 5.5
//
//       HW I/O assignments:
//       P1.0    (input) Right Analog Input
//       P1.1    (input) Left Analog Input
//       P1.2    Not Used
//       P1.3    Not Used
//       P1.4    Not Used
//       P1.5    Not Used
//       P1.6    Not Used
//       P1.7    Not Used
//
//       P2.0    Signal Out
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

#define TIMER_A1_COUNT_1    100 // set count value for TimerA_1  
//Signal Out

#define MonoEn          0

//----------------------------------------------------------------------
// GLOBAL VARIABLE definition
unsigned int RawSignalRight;

int LeftRightSelect = 0; //Start with right audio

unsigned int ADC[1];


int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  // set up Port 2 
  P2DIR = 0x1; //P2.0 Output
  P2IFG &= ~0xFF;  // clear ALL the Int Flags on Port 2
  
  // set up Port 1 
  P1DIR &= ~BIT0;     // P1.0 (Right Audio) -> Input
  P1DIR &= ~BIT0;     // P1.1 (Left Audio) -> Input
  P1IFG &= ~0xFF;  // clear ALL the Int Flags on Port 1
  
  
  
//Configure Clocks
  DCOCTL = 0;                   //Select lowest DC0x and MODx Settings
  BCSCTL1 = CALBC1_12MHZ;        //Set range
  DCOCTL = CALDCO_12MHZ;         //Set DCO step + modulation
  
//SetupTimerA1
  TA1CCR0 = TIMER_A1_COUNT_1;   //load a count value into the counter
  TA1CTL = TASSEL_2 + ID_0 + MC_1;  //SMCLK/1, up mode
  TA1CCTL0 = CCIE;                  //enable the timer interrupt
  
   
  
// setup ADC
  ADC10CTL0 = SREF_1+ADC10SHT_3+MSC+REFON+ADC10ON;//+ADC10IE; 
  ADC10CTL1 = INCH_1 + ADC10DIV_0 + CONSEQ_1 + ADC10SSEL_3;  // select pin A3
  ADC10AE0 |= BIT0 + BIT1;                   // Enable A0 and A1 which are P1.0,P1.1 
  ADC10DTC1 = 2;	//Number of conversions
  ADC10SA = (short)&ADC[0];           // ADC10 data transfer starting address.
    
  
   _BIS_SR(GIE);   // enable general interrupts 
  
  while(1)      
  {
    ADC10CTL0 |= ENC + ADC10SC;       //Starts ADC conversion
    ADC10SA = (short)&ADC[0]; // ADC10 data transfer starting address
    while(ADC10BUSY & ADC10CTL1 ); //Waits for conversion to finish 
  }
  
  
  return 0;
}


#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer_A1_ISR (void)
{
   if(LeftRightSelect == 0)
   {
    if(ADC[0] >= 850)
      P2OUT |= BIT0;    //P2.0 = 1
    else
      P2OUT &= ~BIT0;     //P2.0= 0
   }
  else //if(LeftRightSelect == 1)
  {
    if(ADC[1] >= 850)
      P2OUT |= BIT0;    //P2.0 = 1
    else
      P2OUT &= ~BIT0;     //P2.0 = 0
  }
  if(MonoEn == 0)
  {
    LeftRightSelect ^= 1;
  }
}



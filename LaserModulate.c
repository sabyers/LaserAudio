//-------------------------------------------------------------------------------
//      Project #6
//      This code utilizes the internal temperature sensor. Taking the analog
//      temperature value, convering it to digital, then converts it to BCD
//      and displays it on the four digit display. The temperature updates in
//      real time, taking the average of 128 readings about every 10 mS. The 
//      rightmost bushbutton changes the display from Fahrenheit to Celsius, 
//      however it defaults to Fahrenheit.
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
//       P2.0    Not Used
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

#define TIMER_A1_COUNT_1    5769 // set count value for TimerA_1  
//10 Khz is approximatly 100 uS - ADC takes 13 Clock Cycles with 16 x ADC10CLKs
//results in a 480 nS inturrupt timer

#define TIMER_A0_COUNT_0    600000 // set count value for TimerA_0
//20Khz


//----------------------------------------------------------------------
// GLOBAL VARIABLE definition
unsigned int Delay = 0x0200u;   //Set Delay time to stabilize pushbutton and 
                                //the temperature display change rate 
unsigned int RawSignalRight;

unsigned int LeftRightSelect = 0; //Start with right audio

unsigned int ADC[1];




int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  // set up Port 2 
  P2DIR |= BIT0; //P2.0 Output
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
  
 //SetupTimerA0
  TA0CCR0 = TIMER_A0_COUNT_0;   //load a count value into the counter
  TA0CTL = TASSEL_2 + ID_0 + MC_1;  //SMCLK/1, up mode
  TA0CCTL0 = CCIE;                  //enable the timer interrupt 
  
 
// setup ADC
  ADC10CTL0 = SREF_1+ADC10SHT_3+MSC+REFON+ADC10ON+ADC10IE; 
  ADC10CTL1 = INCH_1 + ADC10DIV_0 + CONSEQ_1 + ADC10SSEL_3;  // select pin A3
  ADC10AE0 |= BIT0 + BIT1;                   // Enable A0 and A1 which are P1.0,P1.1 
  ADC10DTC1 = 2;	//Number of conversions
  ADC10SA = (short)&ADC[0];           // ADC10 data transfer starting address.
  
   _BIS_SR(GIE);   // enable general interrupts 
  
  while(1)      
  {
  }
  
  
  return 0;
}



#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer_A1_ISR (void)
{
      ADC10CTL0 |= ENC + ADC10SC;       //Starts ADC conversion
      for (int dl = 0; dl < Delay; dl++); //Delay a bit to stabilize
}



#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer_A0_ISR (void)
{
  if(ADC[LeftRightSelect] >= 512)
    P2OUT |= BIT0;    //P2.0 = 1
  else
    P2OUT &= ~BIT0;     //P2.1 = 0
  
  LeftRightSelect ^= 0x1; //Toggles between 1 and 0
  
}

//ADC10 interrupt Service Routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void) {

          // ADC[0] has A1 value and ADC[1] has A0's value

	ADC10CTL0 &= ~ADC10IFG;  // clear interrupt flag

	ADC10SA = (short)&ADC[0]; // ADC10 data transfer starting address.
}









//-------------------------------------------------------------------------------
//      Individual Project
//      This code utilizes turns LED's on or off with two consecutive claps. 
//      Using a sound peak dectector this is accomplished.
//
//
//   Target: TI LaunchPad development board with MSP430G2553 device
//
//       Date:           Sep 4, 2017
//       Last Revision:  1.0
//       Written by:     Saul Byers A.S., Texas Tech University
//       Assembler/IDE:  IAR Embedded Workbench 5.5
//
//       HW I/O assignments:
//       P1.0    Not Used
//       P1.1    Not Used
//       P1.2    Not Used
//       P1.3    Not Used
//       P1.4    Not Used
//       P1.5    Sound Detector (Active High)
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
//

#include <msp430g2553.h>

//----------------------------------------------------------------------
// GLOBAL VARIABLE definition

int SoundFlag = 0;
int SoundCount = 0;

//-----------------------------------------------------------------------
//Begin Main Code
//-----------------------------------------------------------------------

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    P1DIR |= BIT0 + BIT6;			// P1.0 (Red) + P1.6 (Green) -> Output

    P1DIR &= ~BIT5;				// P1.5 (Sound Detector) -> Input
    P1REN |= BIT5;				// P1.5 Pull Up/Down Enable
    P1OUT |= BIT5;				// P1.5 Pull Up Enable

    while(1)
    {
      for (int i = 0; i <= 30000; i++)
      {
          
          if(P1IN & BIT5)			// If Sound Detected
          {
            SoundFlag = 1;                             //Set flag and delay               
            for (int j = 0; j <= 5000; j++);           //to avoid clap noise
          }          
          if (SoundCount >= 2)                  //claped twice?
          {
             P1OUT ^= BIT0;		// LED Switch states
             P1OUT ^= BIT6;                     
             SoundCount = 0;            //Reset clap counter and sound flag
             SoundFlag = 0;
             for (int d1 = 0; d1 <= 30000; d1++);       //Primitive delay to avoid rapid state changes
             for (int d2 = 0; d2 <= 30000; d2++);
             for (int d3 = 0; d3 <= 30000; d3++);
             for (int d4 = 0; d4 <= 30000; d4++);
          }
          else                                         //Not two claps
          {
            SoundCount = SoundCount + SoundFlag; //Increment clap count if flag set
            SoundFlag = 0;                      //reset flag
          }
      }
      SoundCount = 0;           //Clap counter reset
    }
}
//-----------------------------------------------------------------------
//End Main Code
//-----------------------------------------------------------------------






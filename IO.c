/*
 * File:   IO.c
 * Author: patsh
 *
 * Created on November 30, 2023, 9:06 PM
 */
#include "xc.h"
#include "IO.h"

#define PB1 PORTAbits.RA2
#define PB2 PORTAbits.RA4
#define PB3 PORTBbits.RB4

uint8_t CNFLAG2=0;
uint8_t TIMEOUT_FLAG= 0;
int last_state_PB2 = 1;


void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void) {
    IFS1bits.CNIF = 0; // Clear IF
    if (last_state_PB2 && PB2 == 0) {
        CNFLAG2 = 1;

    }
    last_state_PB2 = PB2;
}


void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    IFS0bits.T2IF=0; 
    AD1CON1bits.SAMP = 1;
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void){
    IFS0bits.T1IF=0; 
    T1CONbits.TON=0;
    TIMEOUT_FLAG= 1;
}

void IOinit(){
    AD1PCFG=0xFFFF;//set pins to Digital
    //Set RA2, RA4, RB4 to input, RB8 to output
    TRISAbits.TRISA2=1; 
    TRISAbits.TRISA4=1;
    TRISBbits.TRISB4=1;
    TRISBbits.TRISB8=0;
    TRISBbits.TRISB9=0;
    //Set pull-up resistors for all 3 buttons
    CNPU2bits.CN30PUE=1;
    CNPU1bits.CN0PUE=1;
    CNPU1bits.CN1PUE=1;
    //Enable interrupt on pins
    CNEN2bits.CN30IE=1;
    CNEN1bits.CN0IE=1;
    CNEN1bits.CN1IE=1;
    IPC4bits.CNIP=4;//Set Priority for input interrupt pins
    IFS1bits.CNIF=0;// Clear interrupt flag
    IEC1bits.CNIE=1;//Enable CN interrupts
    IFS1bits.CNIF = 0;//ensure cleared before start
}


void TIMER_init(){
 //Timer 2 Config
 T2CONbits.TCKPS=3;//set pre-scaler to 1:256
 T2CONbits.T32=0;// operate timer 2 as 16 bit timer
 T2CONbits.TCS=0; //use internal clock
 T2CONbits.TSIDL=0;//operate in idle mode
 IPC1bits.T2IP=4;//set priority
 IFS0bits.T2IF=0;// interrupt flag cleared
 IEC0bits.T2IE=1;//enable timer interrupt
 
  //Timer 1 Config
 T1CONbits.TCKPS=3;//set pre-scaler to 1:256
 T1CONbits.TCS=0; //use internal clock
 T1CONbits.TSIDL=0;//operate in idle mode
 IPC0bits.T1IP=4;//set priority
 IFS0bits.T1IF=0;// interrupt flag cleared
 IEC0bits.T1IE=1;//enable timer interrupt
}



void delayT2_ms(int time_ms){
//IFS0bits.T2IF = 0; // Clear interrupt flag
PR2 = time_ms*0.97656; //(time_ms)*(250 000 Hz)/(256)
T2CONbits.TON=1;
Idle();
//TMR2=0;
}

void delayT1_ms(int time_ms){
//IFS0bits.T2IF = 0; // Clear interrupt flag
PR1 = time_ms*0.97656; //(time_ms)*(250 000 Hz)/(256)
T1CONbits.TON=1;
//TMR2=0;
}

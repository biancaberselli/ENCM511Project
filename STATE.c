/*
 * File:   STATE.c
 * Author: patsh
 *
 * Created on November 30, 2023, 6:51 PM
 */

#include "xc.h"
#include "uart.h"
#include "STATE.h"

char char_1 = ',';
char char_2 = ',';
char char_3 = ',';
char pass1 = 'c';
char pass2 = 'g';
char pass3 = 'k';
extern uint8_t CNFLAG2;

typedef enum {
    START,
    FIRST_CODE,
    SECOND_CODE,
    THIRD_CODE,
    UNLOCKED,
    ERROR
} State;

State currentState = START;

void State_machine(void) {
    char char_1 = ',';
    char char_2 = ',';
    char char_3 = ',';
    if (currentState == START) {
        LATBbits.LATB8 = 1;
        escape_uart();
        Disp2String("Welcome! Enter 1st Character: ");
        char_1 = RecvUartChar();
        escape_uart();
        if (char_1 == pass1) {
            currentState = FIRST_CODE;
        } else if (char_1 != pass1) {
            currentState = ERROR;
        }
    }
    if (currentState == FIRST_CODE) {
        Disp2String("Correct 1st character. Enter 2nd character: ");
        char_2 = RecvUartChar();
        escape_uart();
        if (char_2 == pass2) {
            currentState = SECOND_CODE;
        } else if (char_2 != pass2) {
            currentState = ERROR;
        }
    }
    if (currentState == SECOND_CODE) {
        Disp2String("Correct 2rd character. Enter 3nd character: ");
        char_3 = RecvUartChar();
        escape_uart();
        if (char_3 == pass3) {
            currentState = THIRD_CODE;
        } else if (char_3 != pass3) {
            currentState = ERROR;
        }
    }

    if (currentState == ERROR) {
        Disp2String("ERROR: ACCESS DENIED");
        while (currentState == ERROR) {
            LATBbits.LATB9 = 1;
            LATBbits.LATB8 = 0;
            if (CNFLAG2 == 1) {
                CNFLAG2 = 0;
                currentState = START;
            }
            Idle();
        }
        LATBbits.LATB9 = 0;
    }
}

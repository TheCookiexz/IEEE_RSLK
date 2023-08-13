/*
 * Functions.c
 *
 *  Created on: Aug 8, 2023
 *      Author: User
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>//Exact-width integer types
#include <ti\devices\msp432p4xx\driverlib\driverlib.h>// Driver library

/*
 * Clock definitions
 */

#define CLK_HF      48e6                    // System clock: 48MHz
#define CLK_LF      32e3                    // External clock: 32kHz
#define CLK_PWM     1e3                     // PWM clock: 1kHz
#define CLK_BUT     2e2                     // Button clock: 200 Hz
#define CLK_RUN     7e5                     // Clock per automatic run
#define CLK_LED     2                       // LED clock: 2Hz

/*
 * Pin definitions
 */

#define RED         GPIO_PIN0               // Defines Red LED pin
#define GREEN       GPIO_PIN1               // Defines Green LED pin
#define BLUE        GPIO_PIN2               // Defines Blue LED pin

/*
 * Limit definitions
 */

#define NUM_DSP             5               // Lines displayed to terminal for default message
#define MAX_STR_BUFFER_LEN  200             // Max string buffer length
#define PWM_MIN             0               // Minimum PWM percent
#define PWM_MAX             1e2             // Maximum PWM percent
#define PWM_STEP            5               // Percent step
#define NUM_DEBOUNCE_CHECKS 10              // For debounce time.

/*
 * Motor definitions
 */

#define LEFT            0                   // Define left motor
#define RIGHT           1                   // Define right motor
#define FORWARD         1                   // Define motor forward direction
#define STOP            0                   // Define motor halt
#define BACKWARD        -1                  // Define motor reverse direction
#define INCREASE        true                // Define motor speed increase
#define DECREASE        false               // Define motor speed decrease

/*
 * Message Variant
 */

#define Intial_Message  0                   // Define Initial Message designator
#define Auto_Message    1                   // Define Automatic Mode Message designator
#define Manual_Message  2                   // Define Manual Mode Message designator
#define TX_Message      3                   // Define Error Message designator
#define Error_Message   4                   // Define Error Message designator

unsigned char received[1000];
int drone[1000];
int stop[1000];

void autoPilot(void);
void manuPilot(void);
void manualRemotePilot(void);
void remoteAutoPilot(void);
void CONTROL(void);

void uart0_transmitStr(const char *str);
void initDevice_HFXT(void);
void initLED(void);
void initGPIO(void);
void initTimer(void);
void initDebugUART(void);
void initMotors(void);
void initTachometers(void);
void startTimer(void);
void initSPI(void);
void initAA(void);
void initRF(void);
void RXaddress_P0(void);
void TXaddress_P0(void);
void RFch(void);
void initRETR(void);
void RX_PW0(void);
void RX_PW1(void);
void RX_PW2(void);
void RX_PW3(void);
void RX_PW4(void);
void RX_PW5(void);
void enterTX(void);
void enterRX(void);
void initRF24(void);

#endif /* FUNCTIONS_H_ */

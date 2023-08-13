/*
 * defenition.h
 *
 *  Created on: Apr 18, 2022
 *      Author: Erik Pineda-A
 *
 *  Contains global definition for the code
 */

#ifndef RSLK_H_
#define RSLK_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>//Exact-width integer types
#include <Functions.h>

#include <ti\devices\msp432p4xx\driverlib\driverlib.h>// Driver library


struct Motor_Info                           // Define structure with information for motors
{   int dutyCycle,
        direction;
    volatile int tachoTimerCount,
                 tachoDirection;
} Left, Right;                              // Allocates Left and Right motor

struct Random_Value                         // Holds generated values
{   int turns,
        distance;
} L_CTRL, R_CTRL;

/*
 * UART terminal Values
 */

char strBuffer[MAX_STR_BUFFER_LEN];        // Defines terminal string

void leftMotor_activate(int flag);
void rightMotor_activate(int flag);
void leftMotor_run(int direction, int dutyCycle);
void rightMotor_run(int direction, int dutyCycle);
float tacho_calcSpeed(int timerCount);
/*Transmit a string through UART0.
void uart0_transmitStr(const char *str);*/

void MSG(int msg_type);
void colorcycle(bool msg);
void dirChange(bool msg);
void MTR_SEL(int state,bool msg);
void PWM_CTL(bool state,bool msg);
void infoPrint(void);
void MTR_STOP(void);
void MTR_SYNC(void);
int Gen_Rand_Num(int control);
void PAUSE(int time,int pause);
void Ran_Path(void);
void Cal_Path(int Lt,int Ld,int Rt,int Rd);


#endif /* RSLK_H_ */

/*
 * function.h
 *
 *  Created on: Apr 18, 2022
 *      Author: Erik Pineda-A
 *
 *  Contains library and prototype functions of code
 */

#ifndef RSLK_C_
#define RSLK_C_

#include <RSLK.h>// contain predefined values and names

/*
 * Global clock variables
 */

uint32_t clockMCLK, clockSMCLK;             // Clock value placeholder

/*
 * Initial LED color
 */

uint8_t LED=RED;                           // Defines initial led to red

/*
 * Structure for information storing
 */

/*
 * S2 Config
 */

volatile uint32_t buttonS2_pressed = 0; //0: unpressed, 1: pressed

/*
 * Initial Motor selected
 */

struct MOTOR_INFO_LIVE
{   int direction,                          // Defines Direction
        motor;                              // Defines initial motor
} LIVE={0,LEFT};


const char *errorDisplayText[NUM_DSP] =     // Default message
{   "\r\n"
    "\r\n"
    " Invalid Entry, try again or press H for help :)\r\n\r\n"
    "       Entry:"
};

const char *txDisplayText[NUM_DSP] =    // Default message
{   "\r\n\r\n"
    " 1: Stop TX Mode\r\n"
    " 2: Start Slaves\r\n"
    " 3: Drone Slaves\r\n"
    " 4: Stop Slaves\r\n"
    " H: Help\r\n"
    "       Entry:"
};

const char *autoDisplayText[NUM_DSP] =      // Default message
{   "\r\n\r\n"
    " 1: Stop Auto Mode\r\n"
    " G: Generate Path\r\n"
    " H: Help\r\n"
    " W: Display Speed \r\n"
    "       Entry:"
};



const char *initDisplayText[NUM_DSP] =      // Default message
{   "\r\n\r\n\r\n\r\n\r\n"
    "TI-RSLK MAX Final Project\r\n"
    " 1: Auto Mode\r\n"
    " 2: Manual Mode\r\n"
    " 3: Send TX Info\r\n"
    "       Mode:"
};

const char *manualDisplayText[NUM_DSP] =    // Default message
{   "\r\n\r\n"
    " 1: Stop Manual Mode\r\n"
    " C: Change LED Color \r\n"
    " A: Left Motor \r\n"
    " F: Right Motor \r\n"
    " S: Change Direction\r\n"
    " R: Increase DutyCycle \r\n"
    " F: Decrease DutyCycle \r\n"
    " W: Display Speed \r\n"
    " H: Help\r\n"
    "       Entry:"
};

const char *fatalDisplayText[NUM_DSP] =  // Default message
{   "\r\n"
    "\r\n"
    " System Failure"
};


//flag = 1 to activate, 0 to sleep
void leftMotor_activate(int flag)
{   if(flag)
        {GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN7);}
    else
        {GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN7);}
}


//flag = 1 to activate, 0 to sleep
void rightMotor_activate(int flag)
{   if(flag)
        {GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);}
    else
        {GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);}
}


void leftMotor_run(int direction, int dutyCycle)
{   if(Left.direction != direction)
    {   Left.direction = direction;
        if(direction == FORWARD)
            {GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4);}
        else
            {GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN4);}
    }

    if(dutyCycle > PWM_MAX)
        {dutyCycle = PWM_MAX;}

    if(dutyCycle < PWM_MIN)
        {dutyCycle = PWM_MIN;}

    if(Left.dutyCycle != dutyCycle)
    {   Left.dutyCycle = dutyCycle;
        Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, clockSMCLK/(3*CLK_PWM)/100*Left.dutyCycle);
    }
}


void rightMotor_run(int direction, int dutyCycle)
{   if(Right.direction != direction)
    {   Right.direction = direction;
        if(direction  == FORWARD)
            {GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN5);}
        else
            {GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN5);}
    }

    if(dutyCycle > PWM_MAX)
        {dutyCycle = PWM_MAX;}

    if(dutyCycle < PWM_MIN)
        {dutyCycle = PWM_MIN;}

    if(Right.dutyCycle != dutyCycle)
    {   Right.dutyCycle = dutyCycle;
        Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, clockSMCLK/(3*CLK_PWM)/100*Right.dutyCycle);
    }
}


float tacho_calcSpeed(int timerCount)
{   //pulsePeriod = timerCount/timerClockFreq
    //speed (RPM) = 60/(360*pulsePeriod)
    //Hard-coded for Timer_A frequency 3MHz/6 = 600kHz.
    return 100000.0/timerCount;
}


/*Transmit a string through UART0.
void uart0_transmitStr(const char *str)
{   int len, i=0;

    len = strlen(str);
    while(i < len)
    {   UART_transmitData(EUSCI_A0_BASE, str[i++]);
        while(!UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT_FLAG));
        UART_clearInterruptFlag(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT_FLAG);
    }
}*/

void MSG(int msg_type)
{   int i;

    if(msg_type==Intial_Message)
    {   for(i=0; i<NUM_DSP; i++)
        {uart0_transmitStr(initDisplayText[i]);}
    }
    else if(msg_type==Auto_Message)
    {   for(i=0; i<NUM_DSP; i++)
        {uart0_transmitStr(autoDisplayText[i]);}
    }
    else if(msg_type==Manual_Message)
    {   for(i=0; i<NUM_DSP; i++)
        {uart0_transmitStr(manualDisplayText[i]);}
    }
    else if(msg_type==TX_Message)
    {   for(i=0; i<NUM_DSP; i++)
        {uart0_transmitStr(txDisplayText[i]);}
    }
    else if(msg_type==Error_Message)
    {   for(i=0; i<NUM_DSP; i++)
        {uart0_transmitStr(errorDisplayText[i]);}
    }
    else
    {   for(i=0; i<NUM_DSP; i++)
        {uart0_transmitStr(fatalDisplayText[i]);}
    }
}

void colorcycle(bool msg)
{   LED = (LED == BLUE) ? RED : (LED << 1); //cycle through three colors

    if(msg==true)
        {uart0_transmitStr("\r\n       Change blinking LED color.\r\n       Entry:");}
}

void dirChange(bool msg)
{   if(LIVE.motor == LEFT)
    {   LIVE.direction = (Left.direction == FORWARD) ? BACKWARD : FORWARD;
        leftMotor_run(LIVE.direction, Left.dutyCycle);

        if(msg==true)
                {uart0_transmitStr("\r\n       Change Left motor direction.\r\n       Entry:");}
    }
    else
    {   LIVE.direction = (Right.direction == FORWARD) ? BACKWARD : FORWARD;
        rightMotor_run(LIVE.direction, Right.dutyCycle);

        if(msg==true)
                {uart0_transmitStr("\r\n       Change Right motor direction.\r\n       Entry:");}
    }
}

void MTR_SEL(int state,bool msg)
{   if(state==LEFT)
    {   LIVE.motor = LEFT;

        if(msg==true)
            {uart0_transmitStr("\r\n       Switched control to Left motor.\r\n       Entry:");}
    }
    if(state==RIGHT)
    {   LIVE.motor = RIGHT;

        if(msg==true)
            {uart0_transmitStr("\r\n       Switched control to Right motor.\r\n       Entry:");}
    }
}

void PWM_CTL(bool state,bool msg)
{   if(state==true)
    {   if(LIVE.motor == LEFT)
            {leftMotor_run(Left.direction, Left.dutyCycle + PWM_STEP);}
        else
            {rightMotor_run(Right.direction, Right.dutyCycle + PWM_STEP);}

        if(msg==true)
            {uart0_transmitStr("\r\n       Increase PWM duty cycle.\r\n       Entry:");}
    }

    if(state==false)
    {   if(LIVE.motor == LEFT)
            {leftMotor_run(Left.direction, Left.dutyCycle - PWM_STEP);}
        else
            {rightMotor_run(Right.direction, Right.dutyCycle - PWM_STEP);}

        if(msg==true)
            {uart0_transmitStr("\r\n       Decrease PWM duty cycle.\r\n       Entry:");}
    }
}

void infoPrint(void)
{   snprintf(strBuffer, MAX_STR_BUFFER_LEN, "\r\n       Left RPM = %.1f, Left Dir = %d; Right RPM = %.1f, Right Dir = %d\r\n       Entry:",
    tacho_calcSpeed(Left.tachoTimerCount), Left.tachoDirection,
    tacho_calcSpeed(Right.tachoTimerCount), Right.tachoDirection);
    uart0_transmitStr(strBuffer);
}

void MTR_STOP(void)
{   int i=PWM_MAX/PWM_STEP;
    do
    {   MTR_SEL(RIGHT,false);
        PWM_CTL(DECREASE,false);
        MTR_SEL(LEFT,false);
        PWM_CTL(DECREASE,false);
        i--;
    }while(i!=0);
}

void MTR_SYNC(void)
{   Left.direction=FORWARD;
    Right.direction=Left.direction;

    Left.dutyCycle=PWM_MIN;
    Right.dutyCycle=Left.dutyCycle;

    Left.tachoTimerCount=0;
    Right.tachoTimerCount=Left.tachoTimerCount;

    Left.tachoDirection=FORWARD;
    Right.tachoTimerCount=Left.tachoDirection;
}

int Gen_Rand_Num(int control)
    {return (rand() % control + 1);}

void PAUSE(int time,int pause)
{   int timer_CTRL;
    for(timer_CTRL=0; timer_CTRL!=time; timer_CTRL++){}
    MTR_STOP();
    for(timer_CTRL=0; timer_CTRL!=pause; timer_CTRL++){}
}

void Ran_Path(void)
{   int count_CTRL;

    colorcycle(false);

    L_CTRL.turns=Gen_Rand_Num(8);
    R_CTRL.turns=Gen_Rand_Num(8);
    L_CTRL.distance=Gen_Rand_Num(8);
    R_CTRL.distance=Gen_Rand_Num(8);

    for(count_CTRL=0; count_CTRL!=L_CTRL.turns; count_CTRL++)
    {   MTR_SEL(LEFT,false);
        PWM_CTL(INCREASE,false);
    }

    PAUSE(CLK_RUN,CLK_RUN);

    for(count_CTRL=0; count_CTRL!=L_CTRL.distance; count_CTRL++)
    {   MTR_SEL(RIGHT,false);
        PWM_CTL(INCREASE,false);
        MTR_SEL(LEFT,false);
        PWM_CTL(INCREASE,false);
    }

    PAUSE(L_CTRL.distance*CLK_RUN,CLK_RUN);

    for(count_CTRL=0; count_CTRL!=R_CTRL.turns; count_CTRL++)
    {   MTR_SEL(RIGHT,false);
        PWM_CTL(INCREASE,false);
    }

    PAUSE(CLK_RUN,CLK_RUN);

    for(count_CTRL=0; count_CTRL!=R_CTRL.distance; count_CTRL++)
    {   MTR_SEL(RIGHT,false);
        PWM_CTL(INCREASE,false);
        MTR_SEL(LEFT,false);
        PWM_CTL(INCREASE,false);
    }

    PAUSE(R_CTRL.distance*CLK_RUN,CLK_RUN);
}

void Cal_Path(int Lt,int Ld,int Rt,int Rd)
{   int count_CTRL;

    colorcycle(false);

    L_CTRL.turns=Lt;
    R_CTRL.turns=Rt;
    L_CTRL.distance=Ld;
    R_CTRL.distance=Rd;

    for(count_CTRL=0; count_CTRL!=L_CTRL.turns; count_CTRL++)
    {   MTR_SEL(LEFT,false);
        PWM_CTL(INCREASE,false);
    }

    PAUSE(CLK_RUN,CLK_RUN);

    for(count_CTRL=0; count_CTRL!=L_CTRL.distance; count_CTRL++)
    {   MTR_SEL(RIGHT,false);
        PWM_CTL(INCREASE,false);
        MTR_SEL(LEFT,false);
        PWM_CTL(INCREASE,false);
    }

    PAUSE(L_CTRL.distance*CLK_RUN,CLK_RUN);

    for(count_CTRL=0; count_CTRL!=R_CTRL.turns; count_CTRL++)
    {   MTR_SEL(RIGHT,false);
        PWM_CTL(INCREASE,false);
    }

    PAUSE(CLK_RUN,CLK_RUN);

    for(count_CTRL=0; count_CTRL!=R_CTRL.distance; count_CTRL++)
    {   MTR_SEL(RIGHT,false);
        PWM_CTL(INCREASE,false);
        MTR_SEL(LEFT,false);
        PWM_CTL(INCREASE,false);
    }

    PAUSE(R_CTRL.distance*CLK_RUN,CLK_RUN);
}

#endif /* RSLK_C_ */

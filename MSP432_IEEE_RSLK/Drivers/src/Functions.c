/*
 * mode_CTRL.h
 *
 *  Created on: Apr 23, 2022
 *      Author: Erika
 */

#ifndef FUNCTIONS_C_
#define FUNCTIONS_C_

#include <Functions.h>
#include <NRF24.h>
#include <RSLK.h>

bool SEND_ROUTE=false;
int timed=0,
    Mode=0;

/*
 * Button control
 */

struct BUTTON_INFO_LIVE
{   volatile uint32_t stateIndex,
                      pressed;              //0: unpressed, 1: pressed
} S1={0,0};

/*
 * RX info
 */

bool TX_RX=false,
     CALC_ROUTE=false,
     SEND=false;

/*
 * Global clock variables
 */

extern uint32_t clockMCLK, clockSMCLK;             // Clock value placeholder

/*
 * Initial Led color
 */

uint8_t Led=RED;                           // Defines initial led to red

/*
 * Structure for information storing
 */

int ITTERATIONS = 2;                        // Define auto path iteration

int start[] = {1,12,0,0,0,0,0,2};


#define BUTTON_S1   GPIO_PIN1               // Defines S1 Button

// Randomizes the path taken
void autoPilot(void)
{   bool looped=true;

    MSG(Auto_Message);

    while(looped)
    {   if(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG))
        {   uint8_t data = UART_receiveData(EUSCI_A0_BASE);
            UART_clearInterruptFlag(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);

            switch(data)
            {   case '1':
                        looped=false;
                        MSG(Intial_Message);
                        MTR_STOP();
                    break;

                case 'G':
                case 'g':
                        Ran_Path();

                    break;

                case 'H':
                case 'h':
                        MSG(Auto_Message);
                    break;

                case 'W':
                case 'w':
                        infoPrint();
                    break;

                default:
                        MSG(Error_Message);
                    break;
            }
        }
    }
}

void manuPilot()
{   bool looped=true;

    MSG(Manual_Message);

    while(looped)
    {   if(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG))
        {   uint8_t data = UART_receiveData(EUSCI_A0_BASE);
            UART_clearInterruptFlag(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);

            switch(data)
            {   case '1':
                        MTR_STOP();
                        looped=false;
                        MSG(Intial_Message);
                    break;

                case 'H':
                case 'h':
                        MSG(Manual_Message);
                    break;

                case 'C':
                case 'c':
                        colorcycle(true);
                    break;

                case 'A':
                case 'a':
                        MTR_SEL(LEFT,true);
                    break;

                case 'D':
                case 'd':
                        MTR_SEL(RIGHT,true);
                    break;

                case 'S':
                case 's':
                        dirChange(true);
                    break;

                case 'R':
                case 'r':
                        PWM_CTL(INCREASE,true);
                    break;

                case 'F':
                case 'f':
                        PWM_CTL(DECREASE,true);
                    break;

                case 'W':
                case 'w':
                        infoPrint();
                    break;

                default:
                        MSG(Error_Message);
                    break;
            }// end of switch
        }// end of input
    }// end of loop
}

void manualRemotePilot()
{   bool looped=true;
    int i;

    MSG(TX_Message);

    while(looped)
    {   if(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG))
        {   uint8_t data = UART_receiveData(EUSCI_A0_BASE);
            UART_clearInterruptFlag(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);

            switch(data)
            {   case '1':
                        MTR_STOP();
                        looped=false;
                        MSG(Intial_Message);
                    break;

                case '2':
                        for(i=0;i!=3;i++)
                            {TXNRF24(start);}
                    break;

                case '3':
                        for(i=0;i!=3;i++)
                            {TXNRF24(drone);}
                    break;

                case '4':
                        for(i=0;i!=3;i++)
                            {TXNRF24(stop);}
                    break;

                case 'H':
                case 'h':
                        MSG(TX_Message);
                    break;

                default:
                        MSG(Error_Message);
                    break;
            }// end of switch
        }// end of input
    }// end of loop
}

void remoteAutoPilot(void)
{   int cycle_CTRL=0;

    if(received[0]==1)
    {   if(received[1]==0)
        {   uart0_transmitStr("\r\n       Path Generating.\r\n       Entry:");
            for(cycle_CTRL=0;cycle_CTRL!=received[7];cycle_CTRL++)
                {Ran_Path();}
            received[0]=0;
        }

        else if(received[1]==1)
        {   if(Mode!=1)
            {   if(received[2]==0)
                    {SEND=true;}

                else if(received[2]==1)
                    {CALC_ROUTE=true;}
            }

            else if(Mode==1)
                {SEND_ROUTE=true;}
        }
            received[0]=0;
            received[1]=0;
            received[2]=0;
            received[3]=0;
            received[4]=0;
            received[5]=0;
            received[6]=0;
            received[7]=0;
    }

        else
        {colorcycle(false);}
}

void CONTROL(void)
{   int i;
    bool looped=true;

    if(SEND==true)
    {   //enterTX();
        //for(i=0;i!=3;i++)
        //    {TXNRF24(acknowledge);};
        //enterRX();

        while(looped)
        {   RXNRF24();

            if(received[2]==1)
            {   Cal_Path(received[6],received[5],received[4],received[3]);
                looped=false;
            }
        }

        SEND=false;
    }

    else if(CALC_ROUTE==true)
    {   //enterTX();
        //for(i=0;i!=3;i++)
        //        {TXNRF24(stop);}
        //enterRX();

//        Cal_Path(received[6],received[5],received[4],received[3]);

        while(looped)
        {   RXNRF24();

            if(received[2]==1)
            {   Cal_Path(received[6],received[5],received[4],received[3]);
                looped=false;
            }
        }

        CALC_ROUTE=false;
    }

    else if(SEND_ROUTE==true)
    {   Ran_Path();

        int cord[]={1,1,1,R_CTRL.distance,L_CTRL.distance,R_CTRL.turns,L_CTRL.turns,1};

        enterTX();
        for(i=0;i!=25;i++)
            {TXNRF24(cord);}
        enterRX();

        SEND_ROUTE=false;
    }
}

void uart0_transmitStr(const char *str)
{   int len, i=0;

    len = strlen(str);
    while(i < len)
    {   UART_transmitData(EUSCI_A0_BASE, str[i++]);
        while(!UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT_FLAG));
        UART_clearInterruptFlag(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT_FLAG);
    }
}

void initDevice_HFXT(void)
{   WDT_A_holdTimer();  //Stop watchdog timer.

    //Change VCORE to 1 to support a frequency higher than 24MHz.
    //See MSP432 Data Sheet, Section 5.8 for Flash wait-state requirements for active frequency.
    PCM_setPowerState(PCM_AM_DCDC_VCORE1);
    FlashCtl_setWaitState(FLASH_BANK0, 1);
    FlashCtl_setWaitState(FLASH_BANK1, 1);

    FPU_enableModule();
    FPU_enableLazyStacking(); //Required to use FPU within ISR.

    //Configure PJ.2 and PJ.3 in HFXT mode.
    //Initialize external clock sources HFXT.
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ, GPIO_PIN2|GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    CS_setExternalClockSourceFrequency(CLK_LF, CLK_HF);
    CS_startHFXT(false);

    CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);  //48MHz
    CS_initClockSignal(CS_HSMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_8); //6Mhz
    CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_16);  //3MHz

    clockMCLK = CS_getMCLK();
    clockSMCLK = CS_getSMCLK();
}


void initLED(void)
{   //Configure P2.0, P2.1, P2.2 as output.
    //P2.0, P2.1, P2.2 are connected to a RGB tri-color Led on LaunchPad.
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
}

void initGPIO(void)
{    //Configure P1.1 (button S1) as an input and enable interrupts.
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, BUTTON_S1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, BUTTON_S1);
    GPIO_interruptEdgeSelect(GPIO_PORT_P1, BUTTON_S1, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_enableInterrupt(GPIO_PORT_P1, BUTTON_S1);
    Interrupt_enableInterrupt(INT_PORT1);

    //Configure P1.2 (button S2) as an input and enable interrupts.
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, BUTTON_S2);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, BUTTON_S2);
    GPIO_interruptEdgeSelect(GPIO_PORT_P1, BUTTON_S2, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_enableInterrupt(GPIO_PORT_P1, BUTTON_S2);
    Interrupt_enableInterrupt(INT_PORT4);
}

void initTimer(void)
{   // TIMER32_0 to blink CLK Led
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_setCount(TIMER32_0_BASE, clockMCLK/CLK_LED);
    Timer32_enableInterrupt(TIMER32_0_BASE);
    Interrupt_enableInterrupt(INT_T32_INT1); // Enable Timer32_0 interrupt in the interrupt controller.

    //TIMER32_1 to clock signal out to SPI at sample frequency
    Timer32_initModule(TIMER32_1_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_setCount(TIMER32_1_BASE, clockSMCLK/48e3);
    Timer32_enableInterrupt(TIMER32_1_BASE);
    Interrupt_enableInterrupt(INT_T32_INT2); //Enable Timer32_1 interrupt in the interrupt controller.

    SysTick_setPeriod(clockMCLK/CLK_BUT);
}

void initDebugUART(void)
{   // Configuration for 3MHz SMCLK, 9600 baud rate.
    // http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
    const eUSCI_UART_ConfigV1 config =
    {   EUSCI_A_UART_CLOCKSOURCE_SMCLK, //SMCLK Clock Source
        19, //BRDIV = 19
        8, //UCxBRF = 8
        0, //UCxBRS = 0
        EUSCI_A_UART_NO_PARITY, //No Parity
        EUSCI_A_UART_LSB_FIRST, //MSB First
        EUSCI_A_UART_ONE_STOP_BIT, //One stop bit
        EUSCI_A_UART_MODE, //UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION, //Oversampling
        EUSCI_A_UART_8_BIT_LEN
    };

    //Configure GPIO pins for UART. RX: P1.2, TX:P1.3.
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2|GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    UART_initModule(EUSCI_A0_BASE, &config);
    UART_enableModule(EUSCI_A0_BASE);
}


void initMotors(void)
{   Timer_A_PWMConfig leftMotor_pwmConfig =
    {   TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_3,
        clockSMCLK/(3*CLK_PWM),
        TIMER_A_CAPTURECOMPARE_REGISTER_4,
        TIMER_A_OUTPUTMODE_RESET_SET,
        clockSMCLK/(3*CLK_PWM)/100*Left.dutyCycle
    };

    Timer_A_PWMConfig rightMotor_pwmConfig =
    {   TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_3,
        clockSMCLK/(3*CLK_PWM),
        TIMER_A_CAPTURECOMPARE_REGISTER_3,
        TIMER_A_OUTPUTMODE_RESET_SET,
        clockSMCLK/(3*CLK_PWM)/100*Right.dutyCycle
    };

    //P3.7: 1 to activate left motor;
    //P3.6: 1 to activate right motor
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6|GPIO_PIN7);

    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN7);

    //P5.4: left motor, 0 for forward, 1 for backward;
    //P5.5: right motor, 0 for forward, 1 for backward
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN4|GPIO_PIN5);

    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN5);

    //P2.7/TA0.4: left motor PWM;
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);

    Timer_A_generatePWM(TIMER_A0_BASE, &leftMotor_pwmConfig);
    Timer_A_generatePWM(TIMER_A0_BASE, &rightMotor_pwmConfig);
}

void initTachometers(void)
{   Timer_A_ContinuousModeConfig Timer_A_config =
    {   TIMER_A_CLOCKSOURCE_SMCLK,  //3MHz
        TIMER_A_CLOCKSOURCE_DIVIDER_5,  //600kHz
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_DO_CLEAR
    };

    Timer_A_CaptureModeConfig leftTacho_captureConfig =
    {   TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_CAPTUREMODE_RISING_EDGE,
        TIMER_A_CAPTURE_INPUTSELECT_CCIxA,
        TIMER_A_CAPTURE_SYNCHRONOUS,
        TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,
        TIMER_A_OUTPUTMODE_OUTBITVALUE
    };

    Timer_A_CaptureModeConfig rightTacho_captureConfig =
    {   TIMER_A_CAPTURECOMPARE_REGISTER_0,
        TIMER_A_CAPTUREMODE_RISING_EDGE,
        TIMER_A_CAPTURE_INPUTSELECT_CCIxA,
        TIMER_A_CAPTURE_SYNCHRONOUS,
        TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,
        TIMER_A_OUTPUTMODE_OUTBITVALUE
    };

    Timer_A_configureContinuousMode(TIMER_A3_BASE, &Timer_A_config);

    //P10.5/TA3.1: left tachometer
    //P10.4/TA3.0: right tachometer
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P10, GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P10, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);

    //TA3.1: left tachometer pulse period (i.e., speed) measurement
    //TA3.0: right tachometer pulse period (i.e., speed) measurement
    Timer_A_initCapture(TIMER_A3_BASE, &leftTacho_captureConfig);
    Timer_A_initCapture(TIMER_A3_BASE, &rightTacho_captureConfig);

    //P5.2: left tachometer direction
    //P5.0: right tachometer direction
    GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN0|GPIO_PIN2);

    Interrupt_enableInterrupt(INT_TA3_0);
    Interrupt_enableInterrupt(INT_TA3_N);
}

void startTimer(void)
{   Interrupt_enableMaster();   // Enable device interrupts
    Timer32_startTimer(TIMER32_0_BASE, false);    //Start timer for heart beat Led blinking.
    Timer32_startTimer(TIMER32_1_BASE, false);      // Start timer for SPI communications
    Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_CONTINUOUS_MODE);//Start timer for tachometer speed measurement.
}

void initSPI(void)
{   //P4DIR &= 0x00 ;
    P4OUT &= 0x00;
    P4DIR |= MOSI + SCLK + CE + CSN ;  //Output Pins
    P4DIR &= ~MISO;
    CE_Off();
    CSN_On();
}

void initAA(void)//EN_AA  -- enabling AA in all pipes
{   CSN_Off();
    Instruction_Byte_MSB_First(W_REGISTER | EN_AA);
    Write_Byte_MSB_First(en_aa_register,1);
    CSN_On();
}

void initRF(void) //RF_SETUP
{   CSN_Off();
    Instruction_Byte_MSB_First(W_REGISTER | RF_SETUP);
    Write_Byte_MSB_First(rf_setupregister,1);
    CSN_On();
}

void RXaddress_P0(void)//RX_ADDR_P0
{   CSN_Off();
    Instruction_Byte_MSB_First(W_REGISTER | RX_ADDR_P0);
    Write_Byte_MSB_First(address_P0,5); // write 5 bytes address
    CSN_On();
}

void TXaddress_P0(void)//TX_ADDR
{   CSN_Off();
    Instruction_Byte_MSB_First(W_REGISTER | TX_ADDR);
    Write_Byte_MSB_First(address_P0,5); // write 5 bytes address
    CSN_On();
}

void RFch(void)//RF_CH
{   CSN_Off();
    Instruction_Byte_MSB_First(W_REGISTER | RF_CH);
    Write_Byte_MSB_First(rf_chanregister,1);
    CSN_On();
}

void initRETR(void)//SETUP_RETR
{   CSN_Off();
    Instruction_Byte_MSB_First(W_REGISTER | SETUP_RETR);
    Write_Byte_MSB_First(setup_retr_register,1);
    CSN_On();
}

void RX_PW0(void)//RX_PW0
{   CSN_Off();
    Instruction_Byte_MSB_First(W_REGISTER | RX_PW_P0);
    Write_Byte_MSB_First(rx_pw_register,1);
    CSN_On();
}

void RX_PW1(void)//RX_PW1
{   CSN_Off();
    Instruction_Byte_MSB_First(W_REGISTER | RX_PW_P1);
    Write_Byte_MSB_First(rx_pw_register,1);
    CSN_On();
}

void RX_PW2(void)//RX_PW2
{   CSN_Off();
    Instruction_Byte_MSB_First(W_REGISTER | RX_PW_P2);
    Write_Byte_MSB_First(rx_pw_register,1);
    CSN_On();
}

void RX_PW3(void)//RX_PW3
{   CSN_Off();
    Instruction_Byte_MSB_First(W_REGISTER | RX_PW_P3);
    Write_Byte_MSB_First(rx_pw_register,1);
    CSN_On();
}

void RX_PW4(void)//RX_PW4
{   CSN_Off();
    Instruction_Byte_MSB_First(W_REGISTER | RX_PW_P4);
    Write_Byte_MSB_First(rx_pw_register,1);
    CSN_On();
}

void RX_PW5(void)//RX_PW5
{   CSN_Off();
    Instruction_Byte_MSB_First(W_REGISTER | RX_PW_P5);
    Write_Byte_MSB_First(rx_pw_register,1);
    CSN_On();
}

void enterTX(void)//CONFIG
{   CSN_Off();
    Instruction_Byte_MSB_First(W_REGISTER | CONFIG);
    Write_Byte_MSB_First(configregisterTX,1);
    CSN_On();
}

void enterRX(void)//CONFIG
{   CSN_Off();
    Instruction_Byte_MSB_First(W_REGISTER | CONFIG);
    Write_Byte_MSB_First(configregisterRX,1);
    CSN_On();
}

void initRF24(void)
{   initAA();
    initRF();
    RXaddress_P0();
    TXaddress_P0();
    RFch();
    initRETR();

    RX_PW0();
    RX_PW1();
    RX_PW2();
    RX_PW3();
    RX_PW4();
    RX_PW5();

    enterTX();
}

/*******************************************************************/
//SysTick ISR
void SysTick_Handler(void)
{   uint8_t buttonState;

    //P1.1 is configured with a pull-up resistor, so P1.1 = 0 when pressed.
    //buttonState = 1 when pressed, which is opposite to P1.1 value.
    buttonState = GPIO_getInputPinValue(GPIO_PORT_P1, BUTTON_S1);
    buttonState = BUTTON_S1 & (~buttonState);
    S1.stateIndex++;

    if((buttonState & BUTTON_S1) == 0)
    {   SysTick_disableInterrupt();
        SysTick_disableModule();
    }
    else if(S1.stateIndex >= NUM_DEBOUNCE_CHECKS)//Button S1 is pressed
    {   SysTick_disableInterrupt();
        SysTick_disableModule();
        S1.pressed = 1;
    }
}

// Button interrupt handler
void PORT1_IRQHandler(void)
{   uint8_t status;

    status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    if(status & BUTTON_S1)
    {   //Start SysTick for debouncing button
        uart0_transmitStr("\r\n     Button S1 interrupt triggered.\r\n ");

        int i;
        for(i=ITTERATIONS;i!=0;i--)
            {Ran_Path();}

        SysTick->VAL = 0; //To cause immediate reload to SysTick counter.
        SysTick_enableModule(); //Enable and start the SysTick counter.
        SysTick_enableInterrupt();
    }
    if(status & BUTTON_S2)
    {   //Start SysTick for debouncing button
        uart0_transmitStr("\r\n     Button S2 interrupt triggered.\r\n ");

        TX_RX = !TX_RX;

        Mode++;
        if(Mode>3)
            {Mode=0;}

        if(TX_RX==false)
        {   enterTX();
            uart0_transmitStr("\r\n     TX Mode.\r\n ");
        }
        else
        {   enterRX();
            uart0_transmitStr("\r\n     RX Mode.\r\n ");
        }

        SysTick->VAL = 0; //To cause immediate reload to SysTick counter.
        SysTick_enableModule(); //Enable and start the SysTick counter.
        SysTick_enableInterrupt();
    }
}

//Timer32_0 ISR
void T32_INT1_IRQHandler(void)
{   Timer32_clearInterruptFlag(TIMER32_0_BASE);

    if(GPIO_getInputPinValue(GPIO_PORT_P2, RED|GREEN|BLUE))
        {GPIO_setOutputLowOnPin(GPIO_PORT_P2, RED|GREEN|BLUE);}
    else
        {GPIO_setOutputHighOnPin(GPIO_PORT_P2, Led);}

    if(TX_RX==true)
    {   RXNRF24();
        remoteAutoPilot();
    }
}

//Timer32_1 ISR
void T32_INT2_IRQHandler(void)
    {Timer32_clearInterruptFlag(TIMER32_1_BASE);}

//Left tachometer pulse period measurement
void TA3_N_IRQHandler(void)
{   static uint_fast16_t lastCount = 0, currentCount = 0;

    Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);

    currentCount = Timer_A_getCaptureCompareCount(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);

    Left.tachoTimerCount = currentCount - lastCount;
    if(Left.tachoTimerCount < 0)
        {Left.tachoTimerCount += 0xFFFF;}

    if(Left.dutyCycle==0)
        {Left.tachoTimerCount=0;}

    lastCount = currentCount;

    //P5.2: 1 for forward, 0 for backward
    if(GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN2))
        {Left.tachoDirection = FORWARD;}
    else
        {Left.tachoDirection = BACKWARD;}
}

//Right tachometer pulse period measurement
void TA3_0_IRQHandler(void)
{   static uint_fast16_t lastCount = 0, currentCount = 0;

    Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

    currentCount = Timer_A_getCaptureCompareCount(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

    Right.tachoTimerCount = currentCount - lastCount;
    if(Right.tachoTimerCount < 0)
        {Right.tachoTimerCount += 0xFFFF;}

    if(Right.dutyCycle==0)
        {Right.tachoTimerCount=0;}

    lastCount = currentCount;

    //P5.0: 1 for forward, 0 for backward
    if(GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN0))
        {Right.tachoDirection = FORWARD;}
    else
        {Right.tachoDirection = BACKWARD;}
}


#endif /* FUNCTIONS_C_ */

/*
 * FINAL_B.c
 *
 *  Created on: Apr 18, 2022
 *      Author: Erika
 */

#include <Functions.h>
#include <RSLK.h>
#include <NRF24.h>

void main(void)
{   MTR_SYNC();

    initDevice_HFXT();
    initLED();
    initGPIO();
    initTimer();
    initDebugUART();

    initMotors();
    initTachometers();

    initSPI();
    initRF24();

    startTimer();   // Starts timers

    MSG(Intial_Message);      // Initial display on terminal.

    while(1)
    {   MTR_STOP();
        if(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG))
        {   uint8_t data = UART_receiveData(EUSCI_A0_BASE);
            UART_clearInterruptFlag(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);

            switch(data)
            {   case '1':
                        autoPilot();
                    break;

                case '2':
                        manuPilot();
                    break;

                case '3':
                        manualRemotePilot();
                    break;

                case 'H':
                case 'h':
                        MSG(Intial_Message);
                    break;

                default:
                        MSG(Error_Message);
                    break;
            }// end of switch
        }// end of input

        CONTROL();
    }// end of while
}


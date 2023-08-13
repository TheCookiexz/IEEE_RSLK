/*
 * SPI_Function.h
 *
 *  Created on: Apr 26, 2022
 *      Author: ep0316
 */

#ifndef NRF24_C_
#define NRF24_C_

#include <NRF24.h>
#include <Functions.h>


/*
 * RF24 Settings
 */

void SCLK_Pulse (void)
{   P4OUT |= SCLK;//set high with OR 1
      P4OUT ^= SCLK;//toggle with XOR 1
}

void Send_Bit (unsigned int value)
{   if (value != 0)
        {P4OUT |= MOSI;}
    else
        {P4OUT &= ~MOSI;}
}

void CE_On (void)
{P4OUT |= CE;}

void CE_Off (void)
{P4OUT &= ~CE;}

void CSN_On (void)
{P4OUT |= CSN;}

void CSN_Off (void)
{P4OUT &= ~CSN;}

void Write_Byte(int content)  //Not ued in this application
{   int i;
    for (i=0;i<8;i++)
    {   Send_Bit(content & (1 << i));//Write to Address
        SCLK_Pulse();
    }
}

void Instruction_Byte_MSB_First(int content)
{   int i;
    for (i=7;i>=0;--i)
    {   status_reg <<= 1;
        Send_Bit(content & (1 << i));

        if ((P4IN & MISO) == 0x02)
            {status_reg |= 0b00000001;}
        else
            {status_reg  &= 0b11111110;}

        SCLK_Pulse();
    }
}

void Read_Byte_MSB_First(int index, unsigned char regname[])
{   int i,k;
    for (i=0;i<=(index-1);i++)
    {   for (k=0;k<8;k++)
        {   regname[i] <<= 1;

            if ((P4IN & MISO) == 0x02)
            {    //read_reg |= 0b10000000;
                regname[i] |= 0b00000001;
            }

            else
            {    //read_reg  &= 0b01111111;
                regname[i]  &= 0b11111110;
            }

            SCLK_Pulse();
        }
    }
}

void Write_Byte_MSB_First(unsigned char content[], int index2)
{   int i,k;
    for (i=0;i<=(index2-1);i++)
    {   for (k=7;k>=0;--k)
        {   Send_Bit(content[i] & (1 << k));//Write to Address
            SCLK_Pulse();
        }
    }
}

void Write_Payload_MSB_First(int pyld[], int index3)
{   int pd_i,pd;
    for (pd_i=0;pd_i<=(index3-1);pd_i++)
    {   for (pd=7;pd>=0;--pd)
        {   Send_Bit(pyld[pd_i] & (1 << pd));//Write to Address
            SCLK_Pulse();
        }

        for (pd=15;pd>=8;--pd)
        {   Send_Bit(pyld[pd_i] & (1 << pd));  //Write to Address
            SCLK_Pulse();
        }
    }
}

void RXNRF24(void)
{   int i;
    for(i=NUM_DSP;i!=0;i--)
    {   CE_On();
        __delay_cycles(150); //settling RX
        CSN_Off();

        Instruction_Byte_MSB_First(NOP);  //to get the status reg..

        CSN_On();

        if ((status_reg & BIT6) == 0x40)
        {   CE_Off();
            CSN_Off();

            Instruction_Byte_MSB_First(R_RX_PAYLOAD);
            Read_Byte_MSB_First(32,read_PAYLOAD);

            CSN_On();

            int j=0,i;

            for (i=0;i<=14;i+=2)
            {   received[j]=read_PAYLOAD[i] | (read_PAYLOAD[i+1] << 8);
                j++;
            }

            CSN_Off();

            Instruction_Byte_MSB_First(W_REGISTER | STATUS);
            Write_Byte_MSB_First(clr_status,1);

            CSN_On();
        }
    }
}

void TXNRF24(int payload[])
{   //STDBY-I

    __delay_cycles(1500);

    CSN_Off();
    Instruction_Byte_MSB_First(W_TX_PAYLOAD);
    Write_Payload_MSB_First(payload,Package_Size);
    Write_Payload_MSB_First(payload,Package_Size);
    CSN_On();

    CE_On();
    __delay_cycles(50); //min pulse >10usec
    CE_Off();

    //TX settling 130 usec
    __delay_cycles(150);
    //TX MODE

    //__delay_cycles(20000);

    //STDBY-I
    CSN_Off();
    Instruction_Byte_MSB_First(NOP);
    CSN_On();

    if ((status_reg & SCLK) == 0x10)
    {   CSN_Off();
        Instruction_Byte_MSB_First(W_REGISTER | STATUS);
        Write_Byte_MSB_First(clr_status,1);
        CSN_On();

        CSN_Off();
        Instruction_Byte_MSB_First(FLUSH_TX);
        CSN_On();
    }


    __delay_cycles(1500);
}

#endif /* NRF24_C_ */

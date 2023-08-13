#ifndef NRF24_H_
#define NRF24_H_

#include "msp.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Register Commands
 */

#define R_REGISTER       (0b00000000)       // Read Registers
#define W_REGISTER       (0b00100000)       // Write Registers
#define R_RX_PAYLOAD     (0b01100001)       // Read RX-payload: 1 – 32 bytes. A read operation will always start at byte 0. Payload will be deleted from FIFO after it is read. Used in RX mode
#define W_TX_PAYLOAD     (0b10100000)       // Used in TX mode. Write TX-payload: 1 – 32 bytes. A write operation will always start at byte 0.
#define FLUSH_TX         (0b11100001)       // Flush TX FIFO, used in TX mode
#define FLUSH_RX         (0b11100010)       // Flush RX FIFO, used in RX mode Should not be executed during transmission of acknowledge, i.e. acknowledge package will not be completed.
#define REUSE_TX_PL      (0b11100011)       // Used for a PTX device Reuse last sent payload. Packets will be repeatedly resent as long as CE is high.
#define NOP              (0b11111111)

/*
 * Register Addresses
 */

#define CONFIG           (0x00)             // Configuration Register
#define EN_AA            (0x01)             // Enable ‘Auto Acknowledgment’ Function Disable this functionality to be compatible with nRF2401
#define EN_RXADDR        (0x02)             // Enabled RX Addresses
#define SETUP_AW         (0x03)             // Setup of Address Widths
#define SETUP_RETR       (0x04)             // Setup of Automatic Transmission
#define RF_CH            (0x05)             // RF Channel
#define RF_SETUP         (0x06)             // RF Setup Register (Bit 3 -> data rate 0:1 Mbps
#define STATUS           (0x07)             // Status Register
#define OBSERVE_TX       (0x08)             // Transmit Observe Register
#define CD               (0x09)             // Carrier Detect
#define RX_ADDR_P0       (0x0A)             // Receive address data pipe 0. 5 Bytes maximum length LSByte is written first. Write the number of bytes defined by SETUP_AW)
#define RX_ADDR_P1       (0x0B)             // Receive address data pipe 1. 5 Bytes maximum length LSByte is written first. Write the number of bytes defined by SETUP_AW)
#define RX_ADDR_P2       (0x0C)             // Receive address data pipe 2. Only LSB. MSBytes will be equal to RX_ADDR_P1[39:8]
#define RX_ADDR_P3       (0x0D)             // Receive address data pipe 3. Only LSB. MSBytes will be equal to RX_ADDR_P1[39:8]
#define RX_ADDR_P4       (0x0E)             // Receive address data pipe 4. Only LSB. MSBytes will be equal to RX_ADDR_P1[39:8]
#define RX_ADDR_P5       (0x0F)             // Receive address data pipe 5. Only LSB. MSBytes will be equal to RX_ADDR_P1[39:8]
#define TX_ADDR          (0x10)             // Transmit Used for a PTX device only. (LSByte is written first) Set RX_ADDR_P0 equal to this address to handle automatic acknowledge if this is a PTX device with Enhanced ShockBurst™ enabled
#define RX_PW_P0         (0x11)
#define RX_PW_P1         (0x12)
#define RX_PW_P2         (0x13)
#define RX_PW_P3         (0x14)
#define RX_PW_P4         (0x15)
#define RX_PW_P5         (0x16)
#define FIFO_STATUS      (0x17)             // FIFO Status register
#define Package_Size     8


#define clr_status (0x70)                 // clr status reg
#define rf_setupregister (0b00000001)// Data Rate -> '0' (1 Mbps) & PA_MIN
#define configregisterTX (0b00001110)       // CRC '1'-> (2 bytes)  & Power ON & Enable CRC & TRANSEIVER -------1
#define configregisterRX (0b00001111)       // CRC '1'-> (2 bytes)  & Power ON & Enable CRC & RECEIVER -------1
#define rf_chanregister (0b01001100)      // Channel '1001100'
#define address_P0 ("00001")                   // write to RX_ADDR_P0 and TX_ADDR
#define address_P1 ("00002")
#define setup_retr_register (0b01011111)  // retry values
#define en_aa_register (0b00111111)
#define rx_pw_register (0b00100000)       // RX_ payload width register -->32


/*
 * Pin definitions
 */

#define SAMPLE_FREQ    1e2                  // Frequency in Hz, common sampling rates for digital audio: 8000, 32000, 44100, 48000
#define MOSI        BIT0                    // P4.0
#define MISO        BIT1                    // P4.1
#define CSN         BIT3                    // P4.3
#define SCLK        BIT4                    // P4.4
#define CE          BIT5                    // P4.5
#define BUTTON_S2   GPIO_PIN4               // Defines S2 Button



unsigned char status_reg;
unsigned char read_PAYLOAD[32];



void SCLK_Pulse (void);
void Send_Bit (unsigned int value);
void CE_On (void);
void CE_Off (void);
void CSN_On (void);
void CSN_Off (void);
void Write_Byte(int content) ;
void Instruction_Byte_MSB_First(int content);
void Read_Byte_MSB_First(int index, unsigned char regname[]);
void Write_Byte_MSB_First(unsigned char content[], int index2);
void Write_Payload_MSB_First(int pyld[], int index3);
void RXNRF24(void);
void TXNRF24(int payload[]);

#endif

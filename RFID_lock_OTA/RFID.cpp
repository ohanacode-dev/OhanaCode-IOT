#include "RFID.h"
#include "main.h"
#include "spi.h"


void AntennaOn(void);								// Open antennas, each time you start or shut down the natural barrier between the transmitter should be at least 1ms interval
void Write_RFID(uint8_t addr, uint8_t val);		// To a certain RFID register to write a uint8_t of data
uint8_t RFID_ToCard(uint8_t *sendData, uint8_t sendLen);	// RC522 and ISO14443 card communication

uint8_t RxBits;			                            // The number of received data bits
/*
 * Function Name: RFID_Init
 * Description: Initialize the RFID module
 * Input: None
 * Return value: None
*/
void RFID_Init(void)
{
	slaveSelect();		// Selects the SPI connected RFID module
	pwrUp();
	_delay_ms(50);          // Allows for 37.74us oscillator start-up delay

	 	
	//Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
    Write_RFID(TModeReg, 0x8D);		//Tauto=1; f(Timer) = 6.78MHz/TPreScaler
    Write_RFID(TPrescalerReg, 0x3E);	//TModeReg[3..0] + TPrescalerReg
    Write_RFID(TReloadRegL, 30);
    Write_RFID(TReloadRegH, 0);
	
	Write_RFID(TxAutoReg, 0x40);		//100%ASK
	Write_RFID(ModeReg, 0x3D);		//CRC Initial value 0x6363	???

	Write_RFID(RFCfgReg, (0x07<<4)); // Set Rx Gain to max

	AntennaOn();		//Open the antenna
}


/*
 * Function Name: Write_RFID
 * Function Description: To a certain RFID register to write a uint8_t of data
 * Input Parameters: addr - register address; val - the value to be written
 * Return value: None
 */
void Write_RFID(uint8_t addr, uint8_t val)
{

	spi_enable(true);

	//Address Format: 0XXXXXX0, the left most "0" indicates a write
	spi((addr<<1)&0x7E);
	spi(val);
	
	spi_enable(false);
}


/*
 * Function Name: Read_RFID
 * Description: From a certain RFID read a uint8_t of data register
 * Input Parameters: addr - register address
 * Returns: a uint8_t of data read from the
 */
uint8_t Read_RFID(uint8_t addr)
{
	uint8_t val;

	spi_enable(true);

	//Address Format: 1XXXXXX0, the first "1" indicates a read
	spi(((addr<<1)&0x7E) | 0x80);
	val =spi(0);

	spi_enable(false);
	
	return val;	
}

/*
 * Function Name: SetBitMask
 * Description: Set RC522 register bit
 * Input parameters: reg - register address; mask - set value
 * Return value: None
 */
void SetBitMask(uint8_t reg, uint8_t mask)
{
    uint8_t tmp;
    tmp = Read_RFID(reg);
    Write_RFID(reg, tmp | mask);  // set bit mask
}


/*
 * Function Name: ClearBitMask
 * Description: clear RC522 register bit
 * Input parameters: reg - register address; mask - clear bit value
 * Return value: None
*/
void ClearBitMask(uint8_t reg, uint8_t mask)
{
    uint8_t tmp;
    tmp = Read_RFID(reg);
    Write_RFID(reg, tmp & (~mask));  // clear bit mask
} 


/*
 * Function Name: AntennaOn
 * Description: Open antennas, each time you start or shut down the natural barrier between the transmitter should be at least 1ms interval
 * Input: None
 * Return value: None
 */
void AntennaOn(void)
{
	uint8_t temp;

	temp = Read_RFID(TxControlReg);
	if (!(temp & 0x03))
	{
		SetBitMask(TxControlReg, 0x03);
	}
}



/*
 * Function Name: RFID_Request
 * Description: Find cards, read the card type number
 * Input parameters: reqMode - find cards way
 *			 TagType - Return Card Type
 *			 	0x4400 = Mifare_UltraLight
 *				0x0400 = Mifare_One(S50)
 *				0x0200 = Mifare_One(S70)
 *				0x0800 = Mifare_Pro(X)
 *				0x4403 = Mifare_DESFire
 * Return value: the successful return MI_OK
 */
uint8_t RFID_Request(uint8_t *TagType)
{
	uint8_t status;

	Write_RFID(BitFramingReg, 0x07);		//TxLastBists = BitFramingReg[2..0]	???
	
	TagType[0] = PICC_REQIDL;
	status = RFID_ToCard(TagType, 1);
	
	if ((status != MI_OK) || (RxBits != 0x10))
	{    
		status = MI_ERR;
	}
   
	return status;
}


/*
 * Function Name: RFID_ToCard
 * Description: RC522 and ISO14443 card communication
 * Input Parameters:
 *			 sendData--RC522 sent to the card by the data
 *			 sendLen--Length of data sent
 *			 backData--Data returned from the card
 *			 backLen--Returned data bit length
 * Return value: the successful return MI_OK
 */
uint8_t RFID_ToCard(uint8_t *sendData, uint8_t sendLen)
{
    uint8_t status = MI_ERR;
    uint8_t irqEn = 0x77;
    uint8_t waitIRq = 0x30;
    uint8_t lastBits;
    uint8_t n;
    uint16_t i;

    Write_RFID(ComIrqReg, irqEn|0x80);	//Interrupt request
    ClearBitMask(ComIrqReg, 0x80);			//Clear all interrupt request bit
    SetBitMask(FIFOLevelReg, 0x80);			//FlushBuffer=1, FIFO Initialization

	Write_RFID(CommandReg, PCD_IDLE);	//NO action; Cancel the current command???

	//Writing data to the FIFO
    for (i=0; i<sendLen; i++)
    {
		Write_RFID(FIFODataReg, sendData[i]);
	}

	//Execute the command
	Write_RFID(CommandReg, PCD_TRANSCEIVE);

	SetBitMask(BitFramingReg, 0x80);		//StartSend=1,transmission of data starts


	//Waiting to receive data to complete
	i = 2000;	//i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms???
    do
    {
		//ComIrqReg[7..0]
		//Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
        n = Read_RFID(ComIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitIRq));

    ClearBitMask(BitFramingReg, 0x80);			//StartSend=0

    if (i != 0)
    {
        if(!(Read_RFID(ErrorReg) & 0x1B))	//BufferOvfl Collerr CRCErr ProtecolErr
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {
				status = MI_NO_TAG_ERR;			//??
			}

			n = Read_RFID(FIFOLevelReg);
			lastBits = Read_RFID(ControlReg) & 0x07;
			if (lastBits)
			{
				RxBits = (n-1)*8 + lastBits;
			}
			else
			{
				RxBits = n*8;
			}

			if (n == 0)
			{
				n = 1;
			}
			if (n > RFID_MSG_MAX_LEN)
			{
				n = RFID_MSG_MAX_LEN;
			}

			//Reading the received data in FIFO
			for (i=0; i<n; i++)
			{
				sendData[i] = Read_RFID(FIFODataReg);  /* return to the same buffer */
			}

        }
        else
        {
			status = MI_ERR;
		}

    }


    return status;
}

/*
 * Function Name: RFID_Anticoll
 * Description: Anti-collision detection, reading selected card serial number card
 * Input parameters: serNum - returns 4 bytes card serial number, the first 5 bytes for the checksum byte
 * Return value: the successful return MI_OK
 */
uint8_t RFID_Anticoll(uint8_t *serNum)
{
    uint8_t status;
    uint8_t i;
	uint8_t serNumCheck=0;
    //uint unLen;

    //ClearBitMask(Status2Reg, 0x08);		//TempSensclear
    //ClearBitMask(CollReg,0x80);			//ValuesAfterColl
	Write_RFID(BitFramingReg, 0x00);		//TxLastBists = BitFramingReg[2..0]

    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    status = RFID_ToCard(serNum, 2);

    if (status == MI_OK)
	{
		//Check card serial number
		for (i=0; i<4; i++)
		{
		 	serNumCheck ^= serNum[i];
		}
		if (serNumCheck != serNum[i])
		{
			status = MI_ERR;
		}
    }

    //SetBitMask(CollReg, 0x80);		//ValuesAfterColl=1

	//memcpy(serNum, str, 5);
    return status;
}

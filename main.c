#include <C:\WinAVR-20100110\avr\include\avr\io.h>
#include <C:\WinAVR-20100110\avr\include\compat\ina90.h>
#include <C:\WinAVR-20100110\avr\include\avr\interrupt.h>
#include <C:\WinAVR-20100110\avr\include\stdlib.h>
#include <C:\WinAVR-20100110\avr\include\string.h>
#include <C:\WinAVR-20100110\avr\include\stdio.h>
#include "TWI_Master.h"

#define TWI_GEN_CALL         				0x00
#define TWI_SLAVE_DIOD_OFF_DISP_OFF 		0x02		
#define TWI_SLAVE_DIOD_ON_DISP_OFF  		0x03
#define TWI_SLAVE_DIOD_OFF_DISP_ON  		0x04
#define TWI_SLAVE_DIOD_ON_DISP_ON   		0x05
#define TWI_SLAVE_DIOD_OFF_DISP_BLINK	  	0x06
#define TWI_SLAVE_DIOD_BLINK_DISP_ON	  	0x07

volatile unsigned char SysStatusFlags;
volatile unsigned int Measure1,
					  Measure2,
					  Measure3,
					  Measure4,
					  Measure5,
					  Measure6, k, k1, k2;
volatile unsigned int DispCounter;

unsigned int Measure[7];
unsigned int MaxMeasure[7];

void main(void)
{
	unsigned char messageBuf[4];
  	unsigned long int ProgrButtCounter, TimeCounter,i,temp1;
  	unsigned char TWI_targetSlaveAddress, temp,
                  pressedButton, myCounter=0;
	unsigned int j,temp2;

	SysStatusFlags   = 0x00;
	
	Measure1	     = 0x00;
	Measure2	     = 0x00;
	Measure3	     = 0x00;
	Measure4	     = 0x00;
	Measure5	     = 0x00;
	Measure6	     = 0x00;

	DispCounter  	 = 0x00;
	ProgrButtCounter = 0x00;
	TimeCounter	     = 0x00;
	temp1 = 0x00;
/* ------------------------- Ñòîðîæåâîé òàéìåð ----------------------------- */

	WDTCR = (1<<WDCE) | (1<<WDE);
	WDTCR = 0x00;
	_NOP();_NOP();_NOP();_NOP();
	WDTCR = (1<<WDP2) | (1<<WDP0)|(1<<WDE);
	_WDR();
	_SEI();

/* ------------------------------------------------------------------------- */

	k = 0; k1 = 0; k2 = 0;
  	DDRA  |=  0x80;
	PORTA &=~ 0x80;
    TWI_Master_Initialise();                                           

			TWI_targetSlaveAddress = 1;
			messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
          	messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_BLINK;
          	messageBuf[2] = 888 >> 8;
			messageBuf[3] = 888 - (messageBuf[2] << 8);
       		TWI_Start_Transceiver_With_Data(messageBuf,4);

			TWI_targetSlaveAddress = 2;
			messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
          	messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_BLINK;
          	messageBuf[2] = 888 >> 8;
			messageBuf[3] = 888 - (messageBuf[2] << 8);
       		TWI_Start_Transceiver_With_Data(messageBuf,4);

			TWI_targetSlaveAddress = 3;
			messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
          	messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_BLINK;
          	messageBuf[2] = 888 >> 8;
			messageBuf[3] = 888 - (messageBuf[2] << 8);
       		TWI_Start_Transceiver_With_Data(messageBuf,4);

			TWI_targetSlaveAddress = 4;
			messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
          	messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_BLINK;
          	messageBuf[2] = 888 >> 8;
			messageBuf[3] = 888 - (messageBuf[2] << 8);
       		TWI_Start_Transceiver_With_Data(messageBuf,4);

			TWI_targetSlaveAddress = 5;
			messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
          	messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_BLINK;
          	messageBuf[2] = 888 >> 8;
			messageBuf[3] = 888 - (messageBuf[2] << 8);
       		TWI_Start_Transceiver_With_Data(messageBuf,4);

			TWI_targetSlaveAddress = 6;
			messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
          	messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_BLINK;
          	messageBuf[2] = 888 >> 8;
			messageBuf[3] = 888 - (messageBuf[2] << 8);
       		TWI_Start_Transceiver_With_Data(messageBuf,4);

	TIFR  |= 0x10;                                             
	TIMSK |= 0x10;

	OCR3A=2500;                                            
    TCCR3A=0;                                               
    ETIFR|=0x10;                                             
    TCCR3B=0x0D;			// prescaler 64
    ETIMSK=0x10;

	while(!k2) {}

  	DDRA  |= 0x80;			
  	for(i=0;i<200000;i++) {PORTA |= 0x80; _WDR();}
  	PORTA &=~ 0x80;	

	DDRC  = 0x00;
	PORTC = 0xFF;

  	TWI_Master_Initialise();
  	_SEI();
	_WDR();  
  	TWI_targetSlaveAddress = DispCounter;

	for(DispCounter = 1; DispCounter < 7; DispCounter++)
	{
		MaxMeasure[DispCounter] = 0x00;
		Measure[DispCounter] = 10;
	}
	
	for(DispCounter = 1; DispCounter < 7; DispCounter++)
	{

		MaxMeasure[DispCounter] = DispCounter*150;
		TWI_targetSlaveAddress = DispCounter;
		messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
    	messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_ON;
    	messageBuf[2] = Measure[DispCounter] >> 8;
		messageBuf[3] = Measure[DispCounter] - (messageBuf[2] << 8);
     	TWI_Start_Transceiver_With_Data(messageBuf,4);
	}

	i = 0;
  	while(1)
  	{  
		_WDR();
		if(PINC == 0xFE) 
		{
			if (temp1 == 0)
			{
				DDRA  |= 0x80;			
  				for(i=0;i<15000;i++) {PORTA |= 0x80; _WDR();}
  				PORTA &=~ 0x80;
			}
		temp1++;
		}
		else temp1 = 0x00;

		if ((temp1 > 700000) && (!(SysStatusFlags&0x01)))	
		{	_WDR();
			temp1 = 0x00;
			for(i=0;i<50000;i++) {PORTA |= 0x80; _WDR();}
			PORTA &=~ 0x80;
			for(i=0;i<60000;i++) {PORTA &=~ 0x80; _WDR();}
			PORTA |= 0x80;
			for(i=0;i<50000;i++) {PORTA |= 0x80; _WDR();}
			PORTA &=~ 0x80;
			for(i=0;i<60000;i++) {PORTA &=~ 0x80; _WDR();}
			PORTA |= 0x80;
			for(i=0;i<100000;i++) {PORTA |= 0x80; _WDR();}
			PORTA &=~ 0x80;
			i=0;
			SysStatusFlags |= 0x01;	
		}


		if (SysStatusFlags & 0x01)
		{
          	_WDR();
			messageBuf[0] = TWI_GEN_CALL;					     
          	messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_OFF;    
          	TWI_Start_Transceiver_With_Data(messageBuf,2);

			for(i=0;i<1000;i++)
			{ _NOP();_NOP();_NOP();_WDR();}
	
			for(DispCounter = 1; DispCounter < 7; DispCounter++)
			{

				MaxMeasure[DispCounter] = DispCounter*150;
				TWI_targetSlaveAddress = DispCounter;
				messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
    			messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_ON;
    			messageBuf[2] = MaxMeasure[DispCounter] >> 8;
				messageBuf[3] = MaxMeasure[DispCounter] - (messageBuf[2] << 8);
     			TWI_Start_Transceiver_With_Data(messageBuf,4);
			}
			
       		DispCounter = 0x01;
			TWI_targetSlaveAddress = DispCounter;
			messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
          	messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_BLINK;
          	messageBuf[2] = MaxMeasure[DispCounter] >> 8;
			messageBuf[3] = MaxMeasure[DispCounter] - (messageBuf[2] << 8);
       		TWI_Start_Transceiver_With_Data(messageBuf,4);
			
			while (SysStatusFlags & 0x01)
			{
				_WDR();
				if(PINC == 0xFE) 	
				{
					if (temp1 == 0)
					{
						DDRA  |= 0x80;			
  						for(i=0;i<15000;i++) {PORTA |= 0x80; _WDR();}
  						PORTA &=~ 0x80;
					}
					temp1++;
				}
				else 				
				{
					_WDR();
					temp1 = 0x00;
    				pressedButton = ~PINC;
    				if (pressedButton)
    				{
      				do{temp = ~PINC;_WDR();}
      				while (temp);
      				switch ( pressedButton ) 
      				{

/* ------------------------ Êíîïêà "ÂÂÎÄ" ------------------------------------------- */

						case (1<<PC1):
							if (!(SysStatusFlags&0x02))
							{
								_WDR();
								messageBuf[0] = TWI_GEN_CALL;					     
          						messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_OFF;    
          						TWI_Start_Transceiver_With_Data(messageBuf,2);

							  	DDRA  |= 0x80;			
  								for(i=0;i<15000;i++) {PORTA |= 0x80; _WDR();}
  								PORTA &=~ 0x80;
								
								TWI_targetSlaveAddress = DispCounter;
								messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
          						messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_ON;
          						messageBuf[2] = MaxMeasure[DispCounter] >> 8;
								messageBuf[3] = MaxMeasure[DispCounter] - (messageBuf[2] << 8);
       							TWI_Start_Transceiver_With_Data(messageBuf,4);
								SysStatusFlags |= 0x02;
							}
							else
							{
								_WDR();
								for(i = 1; i < 7; i++)
								{
									_WDR();
									TWI_targetSlaveAddress = i;
									messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
    								messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_ON;
    								messageBuf[2] = MaxMeasure[i] >> 8;
									messageBuf[3] = MaxMeasure[i] - (messageBuf[2] << 8);
     								TWI_Start_Transceiver_With_Data(messageBuf,4);
								}

								DDRA  |= 0x80;			
  								for(i=0;i<15000;i++) {PORTA |= 0x80; _WDR();}
  								PORTA &=~ 0x80;

								TWI_targetSlaveAddress = DispCounter;
								messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
          						messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_BLINK;
          						messageBuf[2] = MaxMeasure[DispCounter] >> 8;
								messageBuf[3] = MaxMeasure[DispCounter] - (messageBuf[2] << 8);
       							TWI_Start_Transceiver_With_Data(messageBuf,4);
								SysStatusFlags &=~ 0x02;
							}
						break;

/* ------------------------ Êíîïêà "ÂÂÅÐ?" ------------------------------------------ */

						case (1<<PC2):
							if (!(SysStatusFlags&0x02))
							{
								_WDR();
								
								for(i = 1; i < 7; i++)
								{
									_WDR();
									TWI_targetSlaveAddress = i;
									messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
    								messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_ON;
    								messageBuf[2] = MaxMeasure[i] >> 8;
									messageBuf[3] = MaxMeasure[i] - (messageBuf[2] << 8);
     								TWI_Start_Transceiver_With_Data(messageBuf,4);
								}

								DispCounter++;
								if (DispCounter > 0x06) DispCounter = 0x01;        						
							
							  	DDRA  |= 0x80;			
  								for(i=0;i<15000;i++) {PORTA |= 0x80; _WDR();}
  								PORTA &=~ 0x80;
							
								TWI_targetSlaveAddress = DispCounter;
								messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
          						messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_BLINK;
          						messageBuf[2] = MaxMeasure[DispCounter] >> 8;
								messageBuf[3] = MaxMeasure[DispCounter] - (messageBuf[2] << 8);
       							TWI_Start_Transceiver_With_Data(messageBuf,4);
							}
							else
							{
								_WDR();
								MaxMeasure[DispCounter] = MaxMeasure[DispCounter] + 10;
								if (MaxMeasure[DispCounter] > 999) 
									MaxMeasure[DispCounter] = MaxMeasure[DispCounter] -10;
								TWI_targetSlaveAddress = DispCounter;
								messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
          						messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_ON;
          						messageBuf[2] = MaxMeasure[DispCounter] >> 8;
								messageBuf[3] = MaxMeasure[DispCounter] - (messageBuf[2] << 8);
       							TWI_Start_Transceiver_With_Data(messageBuf,4);							
							  	DDRA  |= 0x80;			
  								for(i=0;i<15000;i++) {PORTA |= 0x80; _WDR();}
  								PORTA &=~ 0x80;
							}
						break;
						
/* ------------------------ Êíîïêà "ÂÍÈÇ" ------------------------------------------- */						

						case (1<<PC3):
							if (!(SysStatusFlags&0x02))
							{
								_WDR();					
								for(i = 1; i < 7; i++)
								{
									_WDR();
									TWI_targetSlaveAddress = i;
									messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
    								messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_ON;
    								messageBuf[2] = MaxMeasure[i] >> 8;
									messageBuf[3] = MaxMeasure[i] - (messageBuf[2] << 8);
     								TWI_Start_Transceiver_With_Data(messageBuf,4);
								}
								DispCounter--;
								if (DispCounter < 0x01) DispCounter = 0x06;

							  	DDRA  |= 0x80;			
  								for(i=0;i<15000;i++) {PORTA |= 0x80; _WDR();}
  								PORTA &=~ 0x80;
									
								TWI_targetSlaveAddress = DispCounter;
								messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
          						messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_BLINK;
          						messageBuf[2] = MaxMeasure[DispCounter] >> 8;
								messageBuf[3] = MaxMeasure[DispCounter] - (messageBuf[2] << 8);
       							TWI_Start_Transceiver_With_Data(messageBuf,4);
							}
							else
							{
								_WDR();
								MaxMeasure[DispCounter] = MaxMeasure[DispCounter] - 10;
								if (MaxMeasure[DispCounter] < 1) 
									MaxMeasure[DispCounter] = MaxMeasure[DispCounter] + 10;
								TWI_targetSlaveAddress = DispCounter;
								messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
          						messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_ON;
          						messageBuf[2] = MaxMeasure[DispCounter] >> 8;
								messageBuf[3] = MaxMeasure[DispCounter] - (messageBuf[2] << 8);
       							TWI_Start_Transceiver_With_Data(messageBuf,4);
							  	DDRA  |= 0x80;			
  								for(i=0;i<15000;i++) {PORTA |= 0x80; _WDR();}
  								PORTA &=~ 0x80;
							}
						break;

/* ---------------------------------------------------------------------------------- */
					}      
					}
				}
				if (temp1 > 700000)	
				{	
					_WDR();
					temp1 = 0x00;
					messageBuf[0] = TWI_GEN_CALL;					     
					messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_OFF;    
					TWI_Start_Transceiver_With_Data(messageBuf,2);
										
					for(i=0;i<50000;i++) {PORTA |= 0x80; _WDR();}
					PORTA &=~ 0x80;
					for(i=0;i<60000;i++) {PORTA &=~ 0x80; _WDR();}
					PORTA |= 0x80;
					for(i=0;i<50000;i++) {PORTA |= 0x80; _WDR();}
					PORTA &=~ 0x80;
					for(i=0;i<60000;i++) {PORTA &=~ 0x80; _WDR();}
					PORTA |= 0x80;
					for(i=0;i<100000;i++) {PORTA |= 0x80; _WDR();}
					PORTA &=~ 0x80;
					
					for(DispCounter = 1; DispCounter < 7; DispCounter++)
					{
						TWI_targetSlaveAddress = DispCounter;
						messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS)|(FALSE<<TWI_READ_BIT); 
    					messageBuf[1] = TWI_SLAVE_DIOD_OFF_DISP_ON;
    					messageBuf[2] = Measure[DispCounter] >> 8;
						messageBuf[3] = Measure[DispCounter] - (messageBuf[2] << 8);
     					TWI_Start_Transceiver_With_Data(messageBuf,4);
					}
					i=0;
					SysStatusFlags &=~ 0x01; 		
				}				
			}
		}
  	}

}



ISR (TIMER1_COMPA_vect)
    {
		switch(k)
		{
			case 1:
				k = 0;
				PORTA |= 0x80;
			break;
			case 0:
				k = 1;
				PORTA &=~ 0x80;
			break;
		}
    } 

ISR (TIMER3_COMPA_vect)
    {
			switch(k1)
			{
				case 0:
					
					
					OCR1A=300;                                            
    				TCCR1B=0x0A;			// prescaler 64
					OCR3A=2500;		
				break;

				case 1:
					k = 0;
					PORTA &=~ 0x80;
    				OCR1A  = 0;
					TCCR1B = 0;
					OCR3A=2500;	                                               
				break;

				case 2:
					OCR1A=252;                                            
                    TCCR1B=0x0A;			// prescaler 64
					OCR3A=2500;		
				break;

				case 3:
    				k = 0;
					PORTA &=~ 0x80;
					OCR1A  = 0;
					TCCR1B = 0;
					OCR3A=2100;                                               
				break;

				case 4:
					OCR1A=230;                                            
					TCCR1B=0x0A;			// prescaler 64
					OCR3A=6000;
				break;

				case 5:
    				k = 0;
					PORTA &=~ 0x80;
					OCR1A  = 0;
					TCCR1B = 0;
					OCR3A=2800;                                               
				break;

				case 6:
					OCR1A=300;                                            
    				TCCR1B=0x0A;			// prescaler 64
					OCR3A=2500;		
				break;

				case 7:
					k = 0;
					PORTA &=~ 0x80;
    				OCR1A  = 0;
					TCCR1B = 0;
					OCR3A=2500;	                                               
				break;

				case 8:
					OCR1A=252;                                            
					TCCR1B=0x0A;			// prescaler 64
					OCR3A=2500;		
				break;

				case 9:
    				k = 0;
					PORTA &=~ 0x80;
					OCR1A  = 0;
					TCCR1B = 0;
					OCR3A=2500;
				break;

				case 10:
					OCR1A=212;                                            
					TCCR1B=0x0A;			// prescaler 64
					OCR3A=2500;		
				break;

				case 11:
    				k = 0;
					PORTA &=~ 0x80;
					OCR1A  = 0;
					TCCR1B = 0;
					OCR3A=400;
				break;


				case 12:
					OCR1A=230;                                            
					TCCR1B=0x0A;			// prescaler 64
					OCR3A=5000;		
				break;

				case 13:
    				k = 0;
					PORTA &=~ 0x80;
					OCR1A  = 0;
					TCCR1B = 0;
					OCR3A=5000;                                               
				break;

				case 14:
					OCR1A=300;                                            
					TCCR1B=0x0A;			// prescaler 64
					OCR3A=2700;		
				break;

				case 15:
    				k = 0;
					PORTA &=~ 0x80;
					OCR1A  = 0;
					TCCR1B = 0;
					OCR3A=2000;                                               
				break;

				case 16:
					OCR1A=252;                                            
					TCCR1B=0x0A;			// prescaler 64
					OCR3A=3000;
				break;

				case 17:
    				k = 0;
					PORTA &=~ 0x80;
					OCR1A  = 0;
					TCCR1B = 0;
					OCR3A=2500;                                               
				break;

				case 18:
					OCR1A=230;                                            
					TCCR1B=0x0A;			// prescaler 64
					OCR3A=5000;		
				break;

				case 19:
    				k = 0;
					PORTA &=~ 0x80;
					OCR1A  = 0;
					TCCR1B = 0;
					OCR3A=3000;
				break;

				case 20:
					OCR1A=252;                                            
					TCCR1B=0x0A;			// prescaler 64
					OCR3A=2500;
				break;

				case 21:
					k = 0;
					PORTA &=~ 0x80;
    				OCR1A  = 0;
					TCCR1B = 0;
					OCR3A=2500;                                               
				break;

				case 22:
					OCR1A=300;                                            
					TCCR1B=0x0A;			// prescaler 64
					OCR3A=3500;		
				break;

				case 23:
    				OCR1A  = 0;
					TCCR1B = 0;                                               
					TCCR3B = 0;
					_CLI();
					PORTA &=~ 0x80;
					k2 = 1;
				break;
			}
		k1++;
    }

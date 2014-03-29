#include "derivative.h" // include peripheral declarations

//Pins stuff
#define GPIO_PIN_MASK 0x1Fu
#define GPIO_PIN(x) (((1)<<(x & GPIO_PIN_MASK)))
#define	Set_GPIOB_PDOR(x)	(GPIOB_PDOR |= (1 << (x-1)))

//Time definitions
#define nt15_msec	5500
#define nt40_usec	 100

//LCD Control
#define nIns	0
#define nData	1
#define PortLCD    	GPIOC_PDOR

//Enable connected to portb_01
#define Enable_1	GPIOB_PDOR |= 0x01
#define Enable_0	GPIOB_PDOR &= 0xFE
#define RS_1   		GPIOB_PDOR |= 0x02
#define RS_0   		GPIOB_PDOR &= 0xFD

//Data and clock
//Zero is low, One is high
#define clock_out_1		GPIOC_PDOR |= 0x04
#define clock_out_0 	GPIOC_PDOR &= 0xFB
#define data_out_1		GPIOC_PDOR |= 0x08
#define data_out_0		GPIOC_PDOR &= 0xF7

#define clock_in		(GPIOC_PDIR & 0x0F) == 0x02
#define data_in			(GPIOC_PDIR & 0x0F) == 0x01

//Declare Variables

int int_Temp;
int i=0;
int cap_data=0;
int bit_weigh=0;

//Cursor Blink off initialization
const unsigned char InitializeLCD[5] = {0x38, 0x38, 0x38, 0x0C, 0x01};
//--------------------------------------------------------------
//Normal Functions Prototypes
void cfgPorts(void);
void initLCD(void);
void delay(long time);
void sendCode(int Code, int Data);
//Program Functions Prototypes
void host_capture_data(void);
void ms_clock_initialization(void);
void host_send_F4_code(void);

int main(void)
{	cfgPorts();
	initLCD();

	//ms_clock_initialization();
	
	for(;;)
	{
	}
	return 0;
}
//---------------------------------------
//PROGRAM FUNCTIONS..!!
void host_capture_data(void)
{	if(!data_in)
	{	while(!clock_in)//Start bit
		{}
		for(i=8;i<1;i--)
		{	while(clock_in)
			{}
			if(!data_int == 1)
			{	cap_data = cap_data | bit_weigh;
				bit_weigh = bit_weigh * 2;
			}
			else
			{
				bit_weigh = bit_weigh * 2;
			}
			while(!clock_in)
			{}		
		}
		bit_weigh = 0;
	}
}

void host_send_F4_code(void)
{	if(clock_in)
	{	while(!clock_in)
		{	data_out_0;
		}		
		while(clock_in)
		{}
		while(!clock_in)
		{	data_out_0;
		}
		while(clock_in)
		{}
		while(!clock_in)
		{	data_out_1;
		}
		while(clock_in)
		{}
		while(!clock_in)
		{	data_out_0;
		}
		while(clock_in)
		{}
		while(!clock_in)
		{	data_out_1;
		}
		while(clock_in)
		{}
		while(!clock_in)
		{	data_out_1;
		}
		while(clock_in)
		{}
		while(!clock_in)
		{	data_out_1;
		}
		while(clock_in)
		{}
		while(!clock_in)
		{	data_out_1;
		}
		while(clock_in)
		{}
		while(!clock_in)
		{	data_out_1;
		}
		data_out_1;
		GPIOC_PDDR = 0xFFF0; //Data disable 
	}
}
void ms_clock_initialization(void)
{	clock_out_0;
	data_out_0;
	delay(400);
	GPIOC_PDDR = 0xFFF8; //Clock disable	
}
//---------------------------------------
//CONFIGURE FUNCTIONS..!!
void cfgPorts(void)
{	//Turn on clock for portb
	SIM_SCGC5 = SIM_SCGC5_PORTB_MASK;
	//Turn on clock for portd
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;	
	////Turn on clock for portc
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	/* Set pins of PORTB as GPIO */
	PORTB_PCR0= PORT_PCR_MUX(1);
	PORTB_PCR1= PORT_PCR_MUX(1);
	PORTB_PCR2= PORT_PCR_MUX(1);
	PORTB_PCR3= PORT_PCR_MUX(1);
	PORTB_PCR4= PORT_PCR_MUX(1);
	PORTB_PCR5= PORT_PCR_MUX(1);
	PORTB_PCR6= PORT_PCR_MUX(1);
	PORTB_PCR7= PORT_PCR_MUX(1);
	
	/* Set pins of PORTC as GPIO */
	PORTC_PCR0= PORT_PCR_MUX(1);
	PORTC_PCR1= PORT_PCR_MUX(1);
	PORTC_PCR2= PORT_PCR_MUX(1);
	PORTC_PCR3= PORT_PCR_MUX(1);
	PORTC_PCR4= PORT_PCR_MUX(1);
	PORTC_PCR5= PORT_PCR_MUX(1);
	PORTC_PCR6= PORT_PCR_MUX(1);
	PORTC_PCR7= PORT_PCR_MUX(1);
	PORTC_PCR8= PORT_PCR_MUX(1);
	PORTC_PCR9= PORT_PCR_MUX(1);
	PORTC_PCR10= PORT_PCR_MUX(1);
	PORTC_PCR11= PORT_PCR_MUX(1);
	PORTC_PCR12= PORT_PCR_MUX(1);
	PORTC_PCR13= PORT_PCR_MUX(1);
	
	/* Set pins of PORTD as GPIO */
	PORTD_PCR0= PORT_PCR_MUX(1);
	PORTD_PCR1= PORT_PCR_MUX(1);	
	
//Initialize Ports
	GPIOB_PDOR = 0x00;
	GPIOC_PDOR = 0xFF;
	GPIOD_PDOR = 0x00;

//Configure PortBs
	GPIOB_PDDR = 0xFF;
	GPIOC_PDDR = 0xFFFC; //Is necessary define in and out
	GPIOD_PDDR = 0xFF;
}

void initLCD(void)
{
	int i;
	delay(nt15_msec);
	
	/* Send initialization instructions */
	/* Loop for sending each character from the array */
	for(i=0;i<5;i++)
	{	sendCode(nIns, InitializeLCD[i]);	// send initialization instructions 			
	}	
}

void sendCode(int Code, int Data)
{	//Assign a value to pin RS
	/*HINT: When RS is 1, then the LCD receives a data
	when RS is 0, then the LCD receives an instruction */
	// Initialize RS and Enable with 0
	RS_0;
	Enable_0;
	//Assign the value we want to send to the LCD
	PortLCD = (Data << 6);	
	
	//We make the algorithm to establish if its an instruction we start with 0 on RS value, otherwise if its a data command we start with RS as 1;
	if (Code == nIns)
	{	Enable_1;
		delay(nt40_usec);
		Enable_0;
		RS_0;
	}		
	else if(Code == nData)
	{	RS_1;
		Enable_1;
		delay(nt40_usec);
		Enable_0;
		RS_0;
	}
}
void delay(long time)
{	while (time > 0)
	{	time--;
	}	
}	

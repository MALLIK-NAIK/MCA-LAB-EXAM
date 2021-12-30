#include <lpc214x.h>

#define SIZE 10
int d1[SIZE]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F}; 
int d2[SIZE]={0x003F0000, 0x00060000, 0x005B0000, 0x004F0000, 0x00660000, 0x006D0000, 0x007D0000, 0x00070000, 0x007F0000, 0x006F0000}; 

void pll();
void msecdelay(int x);
int main(void)
{
	int i = 0, j = 0;
	PINSEL0 = 0x00000000; 		// Cong P0L as GPIO
	PINSEL1 = 0x00000000; 		// Cong P0H as GPIO
	IO0DIR = 0x00FF00FF; 			// P0.0 to P0.6 D2 and P0.16 to P0.22 D1
	
	while(1)
	{
					
			for(i=0;i<SIZE;i++) 
			{ 
				
				IO0SET = d1[i];
										
				for(j=0; j<SIZE; j++)
				{
						IO0PIN = (d1[i] | d2[j]);
						msecdelay(20);
						IO0PIN =  (d1[i] & 0x00FF0000);
				}
							
			}				
			
	}

	return 0;
}


void pll()
{
	PLL0CON = 0x01; 			//PLL Enable
	PLL0CFG = 0x24;				//Fosc=12Mhz,CCLK=60Mhz,PCLK=60MHz  ==> Calculated M = 5 (Value to be loaded 4) and P = 2.5 (Value to be loaded 2)
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
	
	while(!(PLL0STAT&(1<<10)));		// Wait till PLL gets lock
	PLL0CON = 0x03;				// PLL EN and Connect
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
	VPBDIV = 0x01;			// PCLK = CCLK
}

void msecdelay(int x)
{
    T0CTCR=0x0;                                 //Select Timer Mode
    T0TCR=0x00;                 //Timer off
    T0PR=59999;                 //Prescaler value for 1ms
    T0TCR=0x02;                 //Timer reset
    T0TCR=0x01;                 //Timer ON
    while(T0TC < x);           
    T0TCR=0x00;                 //Timer OFF
    T0TC=0;                     //Clear the TC value. This is Optional.

}
/*
 * main.c
 */
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"


#define LED1_H	GpioDataRegs.GPASET.bit.GPIO25 = 1
#define LED1_L	GpioDataRegs.GPACLEAR.bit.GPIO25 = 1
#define LED2_H	GpioDataRegs.GPASET.bit.GPIO24 = 1
#define LED2_L	GpioDataRegs.GPACLEAR.bit.GPIO24 = 1
#define LED1_T	GpioDataRegs.GPATOGGLE.bit.GPIO25 = 1
#define LED2_T	GpioDataRegs.GPATOGGLE.bit.GPIO24 = 1

void Gpio_select(void);
void delay(unsigned char i);
void delay2(unsigned char i);
void main(){

	InitSysCtrl();

	DINT;

	Gpio_select();

	LED1_L; // 25
	LED2_L; //24



	for(;;){

		delay2(0x0100);
		delay(0x0100);
	}
}
void delay2(unsigned char i){ //24번 핀 주기 빨라졌다 느려지게 점등하는 함수
	int k;
	for(k=0;k<=i;k++)
	{
		LED2_T;
		DELAY_US(1000*k);
	}
}
void delay(unsigned char i){ //25번 핀 주기 느려졌다 빨라지게 점등하는 함수
	int k;
	for(k=i;k>=0;k--)
	{
		LED1_T;
		DELAY_US(1000*k);
	}
}

void Gpio_select(void)
{
	EALLOW;

	GpioCtrlRegs.GPAMUX2.all = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;
	
    EDIS;

}

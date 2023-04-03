/*
 * main.c
 */
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"


#define LED1_H	GpioDataRegs.GPASET.bit.GPIO25 = 1 //LED1
#define LED1_L	GpioDataRegs.GPACLEAR.bit.GPIO25 = 1
#define LED2_H	GpioDataRegs.GPASET.bit.GPIO24 = 1 //LED2
#define LED2_L	GpioDataRegs.GPACLEAR.bit.GPIO24 = 1
#define LED1_T	GpioDataRegs.GPATOGGLE.bit.GPIO25 = 1
#define LED2_T	GpioDataRegs.GPATOGGLE.bit.GPIO24 = 1

//dip 레지스터
#define DIP1 GpioDataRegs.GPBDAT.bit.GPIO48
#define DIP2 GpioDataRegs.GPBDAT.bit.GPIO49

void Gpio_select(void);
void Gpio_Fnd_out(unsigned char da);
void main(){

	unsigned char counter=0;
	unsigned char direction=1;
	InitSysCtrl();
	DINT;
	Gpio_select();

	LED1_L; // 25
	LED2_L; //24


	int i=2;
	while(i>0){
	for(;;){
		Gpio_Fnd_out(counter);
		DELAY_US(1000000);
		//0~9
		if(direction){ //dir=1일때
			counter++;}
		else{          //dir=0일때
			counter--;}

		if(counter==9){
			direction=0; //거꾸로 가기위한
		}
		else if(counter==0){
			LED1_T;
			LED2_T;
			DELAY_US(1000000);
			LED1_T;
			LED2_T;
			direction=1;
			i--;
			Gpio_Fnd_out(counter);
			break;
		}

		}

	}

}

void Gpio_Fnd_out(unsigned char da){
	if(da&0x01) GpioDataRegs.GPBSET.bit.GPIO50=1;
	else GpioDataRegs.GPBCLEAR.bit.GPIO50=1;

	if(da&0x02) GpioDataRegs.GPBSET.bit.GPIO51=1;
	else GpioDataRegs.GPBCLEAR.bit.GPIO51=1;

	if(da&0x04) GpioDataRegs.GPBSET.bit.GPIO52=1;
	else GpioDataRegs.GPBCLEAR.bit.GPIO52=1;

	if(da&0x08) GpioDataRegs.GPBSET.bit.GPIO53=1;
	else GpioDataRegs.GPBCLEAR.bit.GPIO53=1;

}
void Gpio_select(void)
{
	EALLOW;

	GpioCtrlRegs.GPBMUX2.all=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO51 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO52 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO53 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;
    EDIS;

}


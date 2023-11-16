/*
 * main.c
 */
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

//입출력용 레지스터(GpioDataRegs)
#define LED1_H	GpioDataRegs.GPASET.bit.GPIO25 = 1 //LED1 GPxSET 레지스터에 '1'을 써 넣으면 출력 값을 '1'(High)로 설정
#define LED1_L	GpioDataRegs.GPACLEAR.bit.GPIO25 = 1 //GPxCLEAR 레지스터에 ‘1’을 써 넣으면 출력 값을 ‘0’(Low)으로 설정
#define LED2_H	GpioDataRegs.GPASET.bit.GPIO24 = 1 //LED2
#define LED2_L	GpioDataRegs.GPACLEAR.bit.GPIO24 = 1
#define LED1_T	GpioDataRegs.GPATOGGLE.bit.GPIO25 = 1 //GPxTOGGLE 레지스터를 사용하면 GPIO 핀의 출력 상태를 반전시킬 수 있
#define LED2_T	GpioDataRegs.GPATOGGLE.bit.GPIO24 = 1

//dip 레지스터
#define DIP1 GpioDataRegs.GPBDAT.bit.GPIO48
#define DIP2 GpioDataRegs.GPBDAT.bit.GPIO49

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
		if(DIP1&&DIP2){
			//LED1,2 on
			LED1_H;
			LED2_H;
		}
		else if(DIP1&&(!DIP2)){
			//LED1 toggle
			LED1_T;
			LED2_L;
		}
		else if((!DIP1)&&DIP2){
			//LED2 toggle
			LED2_T;
			LED1_L;
		}
		else { //DIP1,2 off
			LED1_L;
			LED2_L;
		}

		DELAY_US(100000);
		}
}


void Gpio_select(void)
{
	//입출력 방향 지정용 레지스터(GpioCtrlRegs)
	//GPxMUX1, GPxMUX2와 GPxDIR 레지스터들은 보호(EALLOW Protected)되어 있음
	//이들 레지스터를 조작할 때는 필히 EALLOW란 코드로 먼저 보호를 해제하고, 레지스터의 설정을
	//변경한 후에는 EDIS란 코드를 사용하여 다시 보호를 설정해 주어야 함
	EALLOW;

	//I/O 핀에 '내부 풀업 저항의 사용할 것인지의 여부'는 GPIO Pullup Disable 레지스터인 GPAPUD, GPBPUD와 GPCPUD를 사용하여 지정
	//레지스터 내의 해당 GPIO 비트를 ‘1’로 하면 풀업 저항을 사용하지 않게 됨
	GpioCtrlRegs.GPBPUD.bit.GPIO48=0;
	GpioCtrlRegs.GPBPUD.bit.GPIO49=0;

	//GPxMUXn GPIO핀의 사용 용도에 대한 설정 (0 : GPIO, 1 : 핀기능 PWM, SCI…)
	GpioCtrlRegs.GPAMUX2.all = 0;
	GpioCtrlRegs.GPBMUX2.all=0;

	//GPIO 핀의 입출력 방향 설정
	//GPIO핀에 해당하는 비트에 0을 써 넣으면 입력, 1을 써 넣으면 출력으로 설정
	GpioCtrlRegs.GPADIR.bit.GPIO25 = 1; //입출력 포트 출력 으로 선언
	GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;
	
	GpioCtrlRegs.GPBDIR.bit.GPIO48=0; //입출력 포트 입력 으로 선언
	GpioCtrlRegs.GPBDIR.bit.GPIO49=0;
	//
    EDIS;

}

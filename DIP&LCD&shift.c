/*
 * main.c
 */
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"


//lcd
#define LCDD0_H GpioDataRegs.GPBSET.bit.GPIO57=1
#define LCDD1_H GpioDataRegs.GPBSET.bit.GPIO56=1
#define LCDD2_H GpioDataRegs.GPBSET.bit.GPIO61=1
#define LCDD3_H GpioDataRegs.GPBSET.bit.GPIO60=1
#define LCDE_H GpioDataRegs.GPBSET.bit.GPIO55=1

#define LCDRS_H GpioDataRegs.GPBSET.bit.GPIO54=1
#define LCDD0_L GpioDataRegs.GPBCLEAR.bit.GPIO57=1
#define LCDD1_L GpioDataRegs.GPBCLEAR.bit.GPIO56=1
#define LCDD2_L GpioDataRegs.GPBCLEAR.bit.GPIO61=1
#define LCDD3_L GpioDataRegs.GPBCLEAR.bit.GPIO60=1
#define LCDE_L GpioDataRegs.GPBCLEAR.bit.GPIO55=1
#define LCDRS_L GpioDataRegs.GPBCLEAR.bit.GPIO54=1


//dip 레지스터
#define DIP1 GpioDataRegs.GPBDAT.bit.GPIO48
#define DIP2 GpioDataRegs.GPBDAT.bit.GPIO49

void Gpio_select(void);
void Gpio_Fnd_out(unsigned char da);

//lcd
void lcdprint_data(char *str); // 문자열 출력
void lcd_write(char data,unsigned char Rs); // 한 문자씩 출력
void lcd_Gpio_data_out(unsigned char da); // GPIO 데이터 설정
void lcd_init(void); // LCD 초기화

void main(){


	InitSysCtrl();
	DINT;
	Gpio_select();


	lcd_init();



	for(;;){

		//lcdprint_data("39311168");
		if(DIP1&&DIP2){ //제자리
			lcd_write(0x80,0);
			lcdprint_data("39311168");
		}
		else if(DIP1&&(!DIP2)){ //좌측 시프트
			lcd_write(0x18,0);

		}
		else if((!DIP1)&&DIP2){ //우측 시프트
			lcd_write(0x1C,0);
		}
		else { //DIP1,2 off    display clear
			lcd_write(0x01,0); //Display Clear
		}

		DELAY_US(1000000);

	}



}
void lcd_init(void)
{
	lcd_write(0x28,0);// 4bit data mode, 2 line, 5x7 dot
	lcd_write(0x28,0);
	lcd_write(0x0C,0); //display on
	lcd_write(0x01,0); //Display Clear
	DELAY_US(1960);
	lcd_write(0x06,0); //Entry mode
}

void lcd_write(char data,unsigned char Rs) // 한 문자씩 출력
{
	if(Rs) LCDRS_H;          //제어명령
	else   LCDRS_L;
	lcd_Gpio_data_out((data>>4)&0x0f);
	//상위 4bit 출력
	DELAY_US(1);
	LCDE_H;
	//제어신호E 읽기
	DELAY_US(1);
	LCDE_L;
	//제어신호E 쓰기
	DELAY_US(1);
	//하위 4bit 출력

	lcd_Gpio_data_out(data&0x0f);
	DELAY_US(1);
	LCDE_H;
	//제어신호E 읽기
	DELAY_US(1);
	//제어신호E 쓰기
	LCDE_L;
	DELAY_US(41);
}

void lcd_Gpio_data_out(unsigned char data)
{
   //GPIO 입력에 따라 4bit의 데이터 설정
	if(data&0x1) LCDD0_H;
	else LCDD0_L;
	if(data&0x2) LCDD1_H;
	else LCDD1_L;
	if(data&0x4) LCDD2_H;
	else LCDD2_L;
	if(data&0x8) LCDD3_H;
	else LCDD3_L;

}

void lcdprint_data(char *str)// 문자열 출력
{
   //문자를 하나씩 읽어서 끝문자(\0)가 나올때 까지 출력
	unsigned char i=0;
	while(str[i]!='\0'){
		lcd_write(str[i++],1);
	}
}

void Gpio_select(void)
{
	EALLOW;
	GpioCtrlRegs.GPBPUD.bit.GPIO48=0;
	GpioCtrlRegs.GPBPUD.bit.GPIO49=0;
	//
	GpioCtrlRegs.GPBMUX2.all=0;
	//LCD
	GpioCtrlRegs.GPBDIR.bit.GPIO54 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO55 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO56 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;

	//
	GpioCtrlRegs.GPBDIR.bit.GPIO48=0; //입출력 포트 입력 으로 선언 48 49
	GpioCtrlRegs.GPBDIR.bit.GPIO49=0;
	//
	EDIS;

}

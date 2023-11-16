
/*
 * main.c
 */
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

//led
#define LED1_H	GpioDataRegs.GPASET.bit.GPIO25 = 1 //LED1
#define LED1_L	GpioDataRegs.GPACLEAR.bit.GPIO25 = 1
#define LED2_H	GpioDataRegs.GPASET.bit.GPIO24 = 1 //LED2
#define LED2_L	GpioDataRegs.GPACLEAR.bit.GPIO24 = 1
#define LED1_T	GpioDataRegs.GPATOGGLE.bit.GPIO25 = 1
#define LED2_T	GpioDataRegs.GPATOGGLE.bit.GPIO24 = 1

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
void lcd_write(unsigned char data,unsigned char Rs); // 한 문자씩 출력
void lcd_Gpio_data_out(unsigned char da); // GPIO 데이터 설정
void lcd_init(void); // LCD 초기화

void main(){



	char min[2]={0};
	char sec[2]={0};
    char mil=0;
	InitSysCtrl();
	DINT;
	Gpio_select();

	lcd_init();

	LED1_L; // 25
	LED2_L; //24

	for(;;){


		if(DIP1&&DIP2){ // pause 기능 --LED 일시정지
			lcd_write(0x80,0); //1번째 열
			lcdprint_data("StopWatch Pause");
			lcd_write(0xC0,0);   //2번째 열



			lcd_write(min[0]+'0',1);
			lcd_write(min[1]+'0',1);
			lcd_write(':',1);
			lcd_write(sec[0]+'0',1);
			lcd_write(sec[1]+'0',1);
			lcd_write(':',1);
			lcd_write(mil+'0',1);


		}
		else if(DIP1&&(!DIP2)){ //1초 단위 표시 --LED 동시 점멸

			lcd_write(0x80,0);
			lcdprint_data("Play 1s Mode   ");


			lcd_write(0xC0,0);
			lcdprint_data("  ");
			lcd_write(min[0]+'0',1);
			lcd_write(min[1]+'0',1);
			lcd_write(':',1);
			lcd_write(sec[0]+'0',1);
			lcd_write(sec[1]+'0',1);
			lcd_write(' ',1);
			lcd_write(' ',1);

			sec[1]++;
			if(sec[1]==10){
				sec[1]=0;
				sec[0]++;
				if(sec[0]==6){
					sec[0]=0;
					sec[1]=0;
					min[1]++;
				}
			}

			LED1_T;
			LED2_T;
		}
		else if((!DIP1)&&DIP2){ //0.1초 단위 표시 --LED 1,2 번갈아
			lcd_write(0x80,0);
			lcdprint_data("Play o.1s Mode   ");
			lcd_write(0xC0,0);//2열


			LED1_H;
			DELAY_US(100000);
			LED1_L;
			DELAY_US(100000);
			LED2_H;
			DELAY_US(100000);
			LED2_L;

			lcd_write(0xC0,0);

			lcd_write(min[0]+'0',1);
			lcd_write(min[1]+'0',1);
			lcd_write(':',1);
			lcd_write(sec[0]+'0',1);
			lcd_write(sec[1]+'0',1);
			lcd_write(':',1);
			lcd_write(mil+'0',1);
			mil++;
			if(mil==10){
				sec[1]++;
				mil=0;
				if(sec[1]==10){
								sec[1]=0;
								sec[0]++;
								if(sec[0]==6){
									sec[0]=0;
									sec[1]=0;
									min[1]++;
								}
							}
			}

		}
		else { //DIP1,2 off   스톱워치 off  --LED 0ff
			lcd_write(0x80,0);
			lcdprint_data("StopWatch OFF  ");

			min[0]=0;
			min[1]=0;
			sec[0]=0;
			sec[1]=0;
			mil=0;
			lcd_write(0xC0,0);//2열
			lcd_write(min[0]+'0',1);
			lcd_write(min[1]+'0',1);
			lcd_write(':',1);
			lcd_write(sec[0]+'0',1);
			lcd_write(sec[1]+'0',1);
			lcd_write(':',1);
			lcd_write(mil+'0',1);




			LED1_L;
			LED2_L;

		}

		DELAY_US(1000000);

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
void lcd_init(void)
{
	lcd_write(0x28,0);// 4bit data mode, 2 line, 5x7 dot
	lcd_write(0x28,0);
	lcd_write(0x0C,0); //display on
	lcd_write(0x01,0); //Display Clear
	DELAY_US(1960);
	lcd_write(0x06,0); //Entry mode
}

void lcd_write(unsigned char data,unsigned char Rs) // 한 문자씩 출력
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
	//led
	GpioCtrlRegs.GPAMUX2.all = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;
	//
	GpioCtrlRegs.GPBMUX2.all=0;
	GpioCtrlRegs.GPBPUD.bit.GPIO48=0;
	GpioCtrlRegs.GPBPUD.bit.GPIO49=0;
	//

	//Fnd
	GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO51 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO52 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO53 = 1;
	//

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

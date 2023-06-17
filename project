#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include <time.h>
#include <stdlib.h>
#define LED1_H GpioDataRegs.GPASET.bit.GPIO25 = 1
#define LED2_H GpioDataRegs.GPASET.bit.GPIO24 = 1
#define LED1_L GpioDataRegs.GPACLEAR.bit.GPIO25 = 1
#define LED2_L GpioDataRegs.GPACLEAR.bit.GPIO24 = 1

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

//LCD
void Gpio_Fnd_out(unsigned char da);
void lcdprint_data(char *str); // 문자열 출력
void lcd_write(char data,unsigned char Rs); // 한 문자씩 출력
void lcd_Gpio_data_out(unsigned char da); // GPIO 데이터 설정
void lcd_init(void); // LCD 초기화

void display_LCD1s(void);
void time_check(void);
//timer
interrupt void cpu_timer0_isr(void);
interrupt void cpu_timer1_isr(void);
void shift_data(char *str2,int a);
char min1 = '1';
char min2 = '5';
char sec1 = '0';
char sec2 = '0';

int led_stat = 0;
int fnd_counter = 0;
int check__=0;


char randset[3]={0}; //랜덤 셋 생성
char innum[3]; //입력으로 넣을 3자리
char strike='0';
char ball='0';


void Gpio_select(void);
void Gpio_Fnd_out(unsigned char da);
interrupt void XINT1_isr(void); // 인터럽트 루틴
interrupt void XINT2_isr(void); // 인터럽트 루틴

interrupt void cpu_timer0_isr(void);
void f();
void f(){
	lcd_init();
	lcd_write(0x80,0);
	lcdprint_data("Your Win!");
	lcd_write(0xC0,0);
	lcdprint_data("Restart?");
}
interrupt void XINT1_isr(void) //첫번째 인터럽트 동작 sw1
{
	DINT;
	lcd_init();

	//스톱워치 on
	if(DIP1&&DIP2){
		lcd_init();
		strike='0';
		ball='0';
		min1='1';
		min2='5';
		sec1='0';
		sec2='0';
		//랜덤 숫자 3개 생성 + 타이머 설정


		while(1){ //중복없이 랜덤 숫자 뽑기
			randset[0]=rand()%10+'0';
			randset[1]=rand()%10+'0';
			randset[2]=rand()%10+'0';

			if(randset[0]!=randset[1]&&randset[1]!=randset[2]&&randset[0]!=randset[2]) break;
		}
		//타이머 15분 제한

		Gpio_Fnd_out(0);
		lcd_write(0x80,0);
		lcdprint_data("Baseball Game!  ");
		lcd_write(0xC0,0);
		lcdprint_data("     Enter");



	}
	else if(DIP1&&!DIP2){

		lcd_init();

		lcd_write(0x80,0);
		lcdprint_data(" ");
		lcd_write(innum[0],1);
		lcdprint_data(" ");
		lcd_write(innum[1],1);
		lcdprint_data(" ");
		lcd_write(innum[2],1);
		lcdprint_data("   ");

		display_LCD1s();
			sec2--;
			time_check();



			if(strike=='3') f();
			else{
			lcd_write(0xC0,0);
			lcd_write(strike,1);
			lcdprint_data(" Strike ");
			lcd_write(ball,1);
			lcdprint_data(" Ball");
			strike='0';
				ball='0';
			}


	}
	else if(!DIP1&&DIP2){

		lcd_init();
		lcd_write(0x80,0);
				lcdprint_data(" ");
				lcd_write(innum[0],1);
				lcdprint_data(" ");
				lcd_write(innum[1],1);
				lcdprint_data(" ");
				lcd_write(innum[2],1);
				lcdprint_data("   ");

		display_LCD1s();
			sec2--;
			time_check();


			if(strike=='3') f();
			else{
					lcd_write(0xC0,0);
					lcd_write(strike,1);
					lcdprint_data(" Strike ");
					lcd_write(ball,1);
					lcdprint_data(" Ball");
					strike='0';
						ball='0';
			}


	}
	else{ //둘다 off

		lcd_init();
		lcd_write(0x80,0);
				lcdprint_data(" ");
				lcd_write(innum[0],1);
				lcdprint_data(" ");
				lcd_write(innum[1],1);
				lcdprint_data(" ");
				lcd_write(innum[2],1);
				lcdprint_data("   ");
		display_LCD1s();
			sec2--;
			time_check();


			if(strike=='3') f();
			else{
				lcd_write(0xC0,0);
				lcd_write(strike,1);
				lcdprint_data(" Strike ");
				lcd_write(ball,1);
				lcdprint_data(" Ball");
				strike='0';
					ball='0';
			}

	}


	if(min1<'0'){
			lcd_init();
			lcd_write(0x80,0);
			lcdprint_data("Time Over");

		}
	EINT;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
	PieCtrlRegs.PIEIER1.bit.INTx7=1;
}

interrupt void XINT2_isr(void) //두번째 인터럽트 동작 sw2
{
	DINT;
	//스톱워치 off

	if(DIP1&&DIP2){
		//숫자 던지기 시작
		lcd_init();
		innum[0]='0';
		innum[1]='0';
		innum[2]='0';


		}
	else if(DIP1&&!DIP2){

		innum[0]++;
		if(innum[0]>'9')
			innum[0]='0';
		//판정
						unsigned char i=0;
						unsigned char j=0;
						for(i=0;i<3;i++){
							for(j=0;j<3;j++){
								if(randset[i]==innum[j])
								{
								if(i==j)
									strike++;
								else
									ball++;
								}

							}
						}
						Gpio_Fnd_out(innum[0]);
	}
	else if (!DIP1&&DIP2){

		innum[1]++;
		if(innum[1]>'9')
			innum[1]='0';

		//판정
								unsigned char i=0;
								unsigned char j=0;
								for(i=0;i<3;i++){
									for(j=0;j<3;j++){
										if(randset[i]==innum[j])
										{
										if(i==j)
											strike++;
										else
											ball++;
										}

								}
								}
								Gpio_Fnd_out(innum[1]);
	}

	else{ //둘다 off
		innum[2]++;
		if(innum[2]>'9')
			innum[2]='0';

		//판정
								unsigned char i=0;
								unsigned char j=0;
								for(i=0;i<3;i++){
									for(j=0;j<3;j++){
										if(randset[i]==innum[j])
										{
										if(i==j)
										{
											strike++;

										}
										else
											ball++;
									}

								}
								}
								Gpio_Fnd_out(innum[2]);
	}



	//display_LCD1s();
	sec2--;
	//time_check();
	EINT;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
	PieCtrlRegs.PIEIER1.bit.INTx7=1;
}
void Gpio_select(void)
{
	EALLOW;
	GpioCtrlRegs.GPAMUX2.all = 0x0000;
	GpioCtrlRegs.GPBMUX2.all=0;

	//dip
	GpioCtrlRegs.GPBPUD.bit.GPIO48=0;
	GpioCtrlRegs.GPBPUD.bit.GPIO49=0;
	//
	GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;
	//LCD
	GpioCtrlRegs.GPBDIR.bit.GPIO54 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO55 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO56 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;
	//fnd
	GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO51 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO52 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO53 = 1;
	//dip
	GpioCtrlRegs.GPBDIR.bit.GPIO48=0; //입출력 포트 입력 으로 선언
	GpioCtrlRegs.GPBDIR.bit.GPIO49=0;
	//
	EDIS;
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

//LCD

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

//timer
void display_LCD1s(void)
{
	lcd_write(min1,1);
	lcd_write(min2,1);
	lcd_write(':',1);
	lcd_write(sec1,1);
	lcd_write(sec2,1);
}

void time_check(void)
{

	if(sec2<'0'){
		sec1--;
		sec2='9';
	}

	if(sec1<'0'){
		min2--;
		sec1='5';
	}

	if(min2<'0')
	{
		min1--;
		min2 = '9';
	}


}




void main(void)
{
	InitSysCtrl();
	Gpio_select();
	lcd_init();
	DINT;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();
	EALLOW;
	PieVectTable.TINT0 = &XINT1_isr;//timer 1초

	PieVectTable.XINT1 = &XINT1_isr;
	PieVectTable.XINT2 = &XINT2_isr;
	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0; //스위치 sw1
	GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0; // sw2
	GpioCtrlRegs.GPADIR.bit.GPIO26 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO27 = 0;
	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL=27;
	GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL=26;
	EDIS;

	InitCpuTimers();
	ConfigCpuTimer(&CpuTimer0,150000000,1);

	XIntruptRegs.XINT1CR.bit.POLARITY = 0;
	XIntruptRegs.XINT2CR.bit.POLARITY = 0;
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;
	XIntruptRegs.XINT2CR.bit.ENABLE = 1;
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;

	//PieCtrlRegs.PIEIER1.bit.INTx7=1;

	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;
	PieCtrlRegs.PIEIER1.bit.INTx5 = 1;
	IER |= M_INT1;
	EINT;

	LED1_L;
	LED2_L;

	srand((unsigned)time(NULL));

	for(;;){

	}






}

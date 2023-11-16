#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

#define LED1_T	GpioDataRegs.GPATOGGLE.bit.GPIO25 = 1
#define LED2_T	GpioDataRegs.GPATOGGLE.bit.GPIO24 = 1
#define LED1_L	GpioDataRegs.GPACLEAR.bit.GPIO25 = 1
#define LED2_L	GpioDataRegs.GPACLEAR.bit.GPIO24 = 1

void Gpio_select(void);
interrupt void cpu_timer0_isr(void);
interrupt void cpu_timer1_isr(void);

interrupt void cpu_timer0_isr(void)
{
	DINT;

	LED1_T;

	EINT;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void cpu_timer1_isr(void)
{
	DINT;

	LED2_T;

	EINT;
}


void Gpio_select(void)
{
	EALLOW;
	GpioCtrlRegs.GPAMUX2.all = 0x0000;

	GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;

	EDIS;
}

void main(void)
{

	InitSysCtrl();

	Gpio_select();

	DINT;

	InitPieCtrl();

	IER = 0x0000;
	IFR = 0x0000;

	InitPieVectTable();

	EALLOW;
	PieVectTable.TINT0 = &cpu_timer0_isr;
	PieVectTable.XINT13 = &cpu_timer1_isr;
	EDIS;

	InitCpuTimers();

	ConfigCpuTimer(&CpuTimer0, 150000000, 1);
	ConfigCpuTimer(&CpuTimer1, 30000000, 10);

	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	IER |= M_INT1 | M_INT13;
	EINT;

	LED1_L;
	LED2_L;

	for(;;);

}

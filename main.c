/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* XILINX CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xgpio.h"
#include "xtmrctr.h"
#include "xparameters.h"
#include "xuartlite.h"
#include "xil_io.h"

#define	AXI_GPIO_DEVICE_ID		XPAR_GPIO_0_DEVICE_ID
#define AXI_UARTLITE_DEVICE_ID	XPAR_AXI_UARTLITE_0_DEVICE_ID
#define	XGPIO_BANK1			1
#define	XGPIO_BANK2			2

#define	LED34_R_PIN			0x01
#define	LED34_G_PIN			0x02
#define	LED34_B_PIN			0x04

//void print(char *str);

static void delay(int dly)
{
	int i, j;
	for (i = 0; i < dly; i++) {
		for (j = 0; j < 0xffff; j++) {
			;
		}
	}
}

int main()
{

	print("test...\n\r");

	/*
	 * definitions.
	 */
	XGpio_Config *XGpioCfg;
	XGpio XGpio;
	//XTmrCtr_Config *tconfig;
	XTmrCtr timer;

	int Status1, Status2;
    unsigned int time1, time2, cum_time;
//    float time_sec;

    init_platform();

	/*
	 * Initialize the AXI TmrCtr driver.
	 */
    Status1 = XTmrCtr_Initialize(&timer, XPAR_AXI_TIMER_0_DEVICE_ID);
        if (Status1 == XST_SUCCESS){
        	print("true initialize timer...\n\r");
        }
   /*
    * start timing procedure.
    */
    time1 = XTmrCtr_GetValue(&timer, 0);
    XTmrCtr_Start(&timer, 0);

    	/*
    	 * timing codes.
    	 */
    	//sleep(1);
    	print("This is Mohammad Khalili...\n\r");

    XTmrCtr_Stop(&timer, 0);
    time2 = XTmrCtr_GetValue(&timer, 0);

    cum_time = time2 - time1;
    xil_printf("Clock.NO (PL freq = 50 MHz) = %d\n\r",cum_time);
//    time_sec = (cum_time*(1/50000000))*10e+6;
//    xil_printf("requested time is = %1.4f \n\r",time_sec);

	/*
	 * Initialize the AXI GPIO driver.
	 */
	XGpioCfg = XGpio_LookupConfig(AXI_GPIO_DEVICE_ID);
	Status2= XGpio_CfgInitialize(&XGpio, XGpioCfg, XGpioCfg->BaseAddress);
	if (Status2 != XST_SUCCESS) {
		print("axi gpio cfg error\n\r");
		return XST_FAILURE;
	}

	/*
	 * clear GPIO.
	 */
	XGpio_SetDataDirection(&XGpio, XGPIO_BANK1, ~(LED34_R_PIN | LED34_G_PIN | LED34_B_PIN));
	XGpio_DiscreteWrite(&XGpio, XGPIO_BANK1, LED34_R_PIN | LED34_G_PIN | LED34_B_PIN);

	/*
	 * blink LEDs.
	 *
	 *
	 */
//	int Status3;
	XUartLite_Config *XuartCfg;
	XUartLite Xuart;
//
	XuartCfg = XUartLite_LookupConfig(AXI_UARTLITE_DEVICE_ID);
//
	XUartLite_CfgInitialize(&Xuart, XuartCfg, 0x42C00000);



//	if (Status3 == XST_SUCCESS) {
//		print("axi uart cfg \n\r");
//		return XST_FAILURE;
//	}




	while (1) {
		XGpio_DiscreteWrite(&XGpio, XGPIO_BANK1, ~LED34_R_PIN);
		delay(100);
		XGpio_DiscreteWrite(&XGpio, XGPIO_BANK1, ~(LED34_R_PIN | LED34_G_PIN));
		delay(100);
		XGpio_DiscreteWrite(&XGpio, XGPIO_BANK1, ~(LED34_R_PIN | LED34_G_PIN | LED34_B_PIN));
		delay(100);
		XGpio_DiscreteWrite(&XGpio, XGPIO_BANK1, ~(LED34_G_PIN | LED34_B_PIN));
		delay(100);
		XGpio_DiscreteWrite(&XGpio, XGPIO_BANK1, ~(LED34_B_PIN));
		delay(1000);
		XGpio_DiscreteWrite(&XGpio, XGPIO_BANK1, LED34_R_PIN | LED34_G_PIN | LED34_B_PIN);
		delay(1000);


		XUartLite_Send(&Xuart, "PL uart start...\n\r" , 30);
		delay(100);
	}
    cleanup_platform();
    return 0;
}

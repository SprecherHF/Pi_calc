/*
 * U_PiCalc_FS2022
 *
 * Created: 20.03.2018 18:32:07
 * Author : Andreas Sprecher
 */ 

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "avr_compiler.h"
#include "pmic_driver.h"
#include "TC_driver.h"
#include "clksys_driver.h"
#include "sleepConfig.h"
#include "port_driver.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "stack_macros.h"

#include "mem_check.h"

#include "init.h"
#include "utils.h"
#include "errorHandler.h"
#include "NHD0420Driver.h"

#include "ButtonHandler.h"
#include "semphr.h"

float pi4;
float pisqr;
float pieuler;
float pileibniz;



void leibniz_task(void* pvParameters);
void euler_task(void* pvParameters);
void compare_Pi(void* pvParameters);
void controllerTask(void* pvParameters);
void vButtonTask(void *pvParameters);

//Eventgroup und definitionen f�r PI Berechnungen

#define PICALC_ACTIVE					1 << 0	// diverse sachen entfernen und die resets einf�gen
#define PICALC_ALGO1					1 << 1
#define PICALC_ALGO2					1 << 2
#define PICALC_BUTTON1_SHORT			1 << 3
#define PICALC_BUTTON1_LONG				1 << 4
#define PICALC_BUTTON2_SHORT			1 << 5
#define PICALC_BUTTON2_LONG				1 << 6
#define PICALC_BUTTON3_SHORT			1 << 7
#define PICALC_BUTTON3_LONG				1 << 8
#define PICALC_BUTTON4_SHORT			1 << 9
#define PICALC_BUTTON4_LONG				1 << 10
#define PICALC_BUTTON_ALL

EventGroupHandle_t Picalculating;
SemaphoreHandle_t ubergabe;
uint8_t meinevariable = 0;



int main(void)
{
    vInitClock();
	vInitDisplay();
	ubergabe = xSemaphoreCreateMutex();
	
	Picalculating = xEventGroupCreate();
	
	xTaskCreate( controllerTask, (const char *) "control_tsk", configMINIMAL_STACK_SIZE+150, NULL, 1, NULL);
	xTaskCreate( leibniz_task, (const char *) "Leibniz_PI", configMINIMAL_STACK_SIZE+150, NULL, 1,NULL);
	xTaskCreate( euler_task, (const char *) "Euler_PI", configMINIMAL_STACK_SIZE+150, NULL, 1,NULL);
	xTaskCreate( compare_Pi, (const char *) "compare_PI", configMINIMAL_STACK_SIZE+150, NULL, 1,NULL);
	xTaskCreate(vButtonTask, (const char *) "buttonTask", configMINIMAL_STACK_SIZE+10, NULL, 3, NULL);
	vDisplayClear();
	vDisplayWriteStringAtPos(0,0,"%s", pistring);
	
	
	vTaskStartScheduler();
	return 0;
}

void controllerTask(void* pvParameters) {
	(void) pvParameters;
	static MENU_MAINSCREEN;
	static menuMode;
	char pistring[12];
	uint16_t buttonstate = 0x0000;
	PORTE.DIRSET = 0x08;
	while(Picalculating == NULL) {
			vTaskDelay(1);
	}
	
	for(;;) {
			PORTE.OUTCLR = 0x08;
			buttonstate = xEventGroupGetBits(Picalculating);
			vDisplayClear();
			sprintf(&pistring[0], "PI: %.8f", M_PI);
			vDisplayWriteStringAtPos(1,0, "%s", pistring); // irgendwo hier k�men die 500ms hin, fragen wo und ob taskdelay
			switch(menuMode) {
					case MENU_MAINSCREEN
						if(buttonstate & PICALC_BUTTON1_SHORT) {
							
						}
	


void leibniz_task(void* pvParameters) {
	pi4 = 1;
	uint32_t zaehler = 3;
	float pi = 0;
	for(;;) {
		pi4 = pi4 - (1.0 / zaehler);
		zaehler += 2;
		pi4 = pi4 + (1.0 / zaehler);
		zaehler += 2;
		pileibniz = pi4 * 4;
		if(xSemaphoreTake(ubergabe, 10/portTICK_RATE_MS) == pdTRUE; { // hier die mutexes einf�gen
		meinevariable = pileibniz;
		xSemaphoreGive(ubergabe);
		vTaskDelay(10/portTICK_RATE_MS);
		}
}

void euler_task(void* pvParameters) {
	uint32_t x = 1;
	float zaehler;
	float pi = 0;
	for(;;) {
		zaehler = pow(x,2);
		pisqr = pisqr + (1.0 / zaehler);	// dito mutexes
		x += 1;
		pieuler = sqrt(pisqr * 6);
		}
}	


void compare_Pi(void* pvParameters) {
	uint32_t x = 1;
	float zaehler;
	float pi = 0;
	for(;;) {
		zaehler = pow(x,2);
		pisqr = pisqr + (1.0 / zaehler);	// dito mutexes
		x += 1;
		pieuler = sqrt(pisqr * 6);
	}
}


void vButtonTask(void * pvParameters) {
		initButtons();
		for(;;) {
			updateButtons();
			if(getButtonPress(BUTTON1) == SHORT_PRESSED) {
				xEventGroupSetBits(Picalculating, PICALC_BUTTON1_SHORT); // algo sets von oben und weiterer knopf mit clearbits
			}
			if(getButtonPress(BUTTON2) == SHORT_PRESSED) {
				xEventGroupSetBits(Picalculating, PICALC_BUTTON2_SHORT);
			}
			if(getButtonPress(BUTTON3) == SHORT_PRESSED) {
				xEventGroupSetBits(Picalculating, PICALC_BUTTON3_SHORT);
			}
			if(getButtonPress(BUTTON4) == SHORT_PRESSED) {
				xEventGroupSetBits(Picalculating, PICALC_BUTTON4_SHORT);
			}
			if(getButtonPress(BUTTON1) == LONG_PRESSED) {
				xEventGroupSetBits(Picalculating, PICALC_BUTTON1_LONG);
			}
			if(getButtonPress(BUTTON2) == LONG_PRESSED) {
				xEventGroupSetBits(Picalculating, PICALC_BUTTON2_LONG);
			}
			if(getButtonPress(BUTTON3) == LONG_PRESSED) {
				xEventGroupSetBits(Picalculating, PICALC_BUTTON3_LONG);
			}
			if(getButtonPress(BUTTON4) == LONG_PRESSED) {
				xEventGroupSetBits(Picalculating, PICALC_BUTTON4_LONG);
			}
			vTaskDelay(10/portTICK_RATE_MS);
		}
	}
	
	
	

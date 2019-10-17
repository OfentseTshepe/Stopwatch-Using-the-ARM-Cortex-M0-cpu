//********************************************************************
//*                    EEE2046F C example                            *
//*==================================================================*
//* WRITTEN BY:    	                 		                         *
//* DATE CREATED:                                                    *
//* MODIFIED:                                                        *
//*==================================================================*
//* PROGRAMMED IN: Eclipse Neon 3 Service Release 1 (4.4.1)          *
//* DEV. BOARD:    UCT STM32 Development Board                       *
//* TARGET:	   STMicroelectronics STM32F051C6                        *
//*==================================================================*
//* DESCRIPTION:                                                     *
//*                                                                  *
//********************************************************************
// INCLUDE FILES
//====================================================================

#include "lcd_stm32f0.h"
#include "stm32f0xx.h"
//====================================================================
// SYMBOLIC CONSTANTS
//====================================================================

//====================================================================
// GLOBAL VARIABLES
//====================================================================

//====================================================================
// FUNCTION DECLARATIONS
//====================================================================
void init_ports();
void	init_NVIC();
void init_TIM14();
void check_pb();
void display();



//====================================================================
// MAIN FUNCTION
//====================================================================
int hundereth_of_second=0;//holds the hudreeds of a second values
int seconds=0;//holds seconds
int minutes=0;//holds minutes
int SW0_pressed =0;
int SW1_pressed=0;
int SW2_presed=0;

char printout[16];
char str_second[2];
char str_min[2];
char str_hun_sec[2];
void main (void)
{
	init_ports();
   init_NVIC();
   init_TIM14();
   init_LCD();


   lcd_putstring("Stop Watch");
   lcd_command(LINE_TWO);
   lcd_putstring("Press SW0");
   //free(str_hun_sec);

   while(1){
	   check_pb();

   }



}										// End of main

//====================================================================
// FUNCTION DEFINITIONS
//====================================================================
void init_ports()
{
	RCC->AHBENR |= 0b1<<18|0b1<<17;
	GPIOB->MODER  |= 0x5555;
	GPIOA->MODER |=0b00000000;
	GPIOA -> MODER |= 0b00000000;
	GPIOA->PUPDR|=0b01010101;
	GPIOB->ODR =0;
}

void init_NVIC(){//Initializing NVIC for timer  14
	NVIC_EnableIRQ(TIM14_IRQn);
}

void init_TIM14(){//Initializing timer 14
	RCC -> APB1ENR |= RCC_APB1ENR_TIM14EN;
	TIM14 -> PSC = 7;
	TIM14 -> ARR = 59999;
	TIM14 -> DIER |= TIM_DIER_UIE;
	TIM14 -> CR1 |= TIM_CR1_CEN;
}
void TIM14_IRQHandler(void) {
	TIM14 -> SR &= ~TIM_SR_UIF;
	if(SW0_pressed==1){

	hundereth_of_second++; //if hundrends of seconds overflow reset to 0 and incriment seconts
	if (hundereth_of_second>99){
		hundereth_of_second=0;
		seconds++;
	}

	if(seconds>59){
		seconds=0;
		minutes++;
	}
	if (minutes>60){
		minutes=0;
	}


	display();}

	else if (SW1_pressed==1){

		hundereth_of_second++; //if hundrends of seconds overflow reset to 0 and incriment seconts
		if (hundereth_of_second>99){
			hundereth_of_second=0;
			seconds++;
		}

		if(seconds>59){
			seconds=0;
			minutes++;
		}
		if (minutes>60){
			minutes=0;
		}


		}





}
void check_pb(){// for checking which button was pressed
	if(!(GPIOA->IDR&1)){
		SW0_pressed=1;
		SW1_pressed=0;
		GPIOB->ODR=1;
}
	if(!(GPIOA->IDR&2)){//SW1
		SW0_pressed=0; //stops the LCD from counting but counts in the background
		SW1_pressed=1;
		GPIOB->ODR=2;
}


	if(!(GPIOA->IDR&4)){
		SW0_pressed=0; // SW2 stops the LCD from counting and displays the current value
		SW2_presed=1;
		GPIOB->ODR=4;
		display();
		SW1_pressed=0;
}

	if(!(GPIOA->IDR&8)){//SW3 resets the couter and shows the well come message
		SW0_pressed=0; //
		SW1_pressed=0;
		GPIOB->ODR=0;
		seconds =0;
		minutes =0;
		SW1_pressed=0;
		SW2_presed=0;
		hundereth_of_second=0;
		lcd_command(CLEAR);
		main();
}

}

void display(){
	// if hundreth's of seconds are less than 10 the the display will show  a hundreth's of seconds  number with 0 in front of it
	if(SW0_pressed==1||SW2_presed==1){

		 lcd_command(CLEAR);
		if(hundereth_of_second<10){

		   strcpy(str_hun_sec,"0");


	   }
	   else{
		   strcpy(str_hun_sec,"");

	   }
	//================================================================================
	   // if seconds are less than 10 the the display will show  a secons number with 0 in front of it
	   if(seconds<10){

	   	   strcpy(str_second,"0");


	      }
	      else{
	   	   strcpy(str_second,"");

	      }
	  //==============================================================================

	   if(minutes<10){    // if minutes are less than 10 the the display will show  a minute number with 0 in front of it

	     	   strcpy(str_min,"0");


	        }
	        else{
	     	   strcpy(str_min,"");

	        }





	   //=============================================================================

	 //the format of how the LCD will output the time
	  snprintf(printout,16,"%s%d%s%s%d%s%s%d",str_min,minutes,":",str_second,seconds,".",str_hun_sec,hundereth_of_second);
	  delay(300);//gives the lcd enough time to update
	  lcd_putstring("TIME");
	  lcd_command(LINE_TWO);
	  lcd_putstring(printout);}

}


//********************************************************************
// END OF PROGRAM
//********************************************************************


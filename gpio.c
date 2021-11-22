/*******************************************************************************
  * File Name          : assignment1.c
  * Description        : Controlling GPIOA pins
  *			 Only Pin 2 and Pin 3 is occupied for UART
  *			 The usage of the code is leda <index> <0 - 1>
  * 
  * Author:              Group 1
  *			 Sinan KARACA
  *			 Mohammed Al Bunde
  * Date:                22.09.2021				 
  ******************************************************************************
  */
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#include "common.h"

/* This include will give us the CubeMX generated defines */
#include "main.h"


ParserReturnVal_t assignmentFirst(int mode)
{
  uint32_t led,ledControl;

  int rc;
  int shiftNumber, tempCheck;
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;



  // First input for pin index <1-16>
  rc = fetch_uint32_arg(&led);
  
  
  
  if(rc || led < 0 || led > 16 || led == 2|| led == 3) {
  
    //Check if user entered and input
    //Check if GPIOA <index> is inside 0 to 16
    //Check if <index> is 2 or 3 (Checking for uart pins)
    
    printf("Choose Pin Between 1-15\n");
    printf("Pin 2 and Pin 3 are reserved for UART\n");
    printf("Please choose other pins!\n");
    return CmdReturnBadParameter1;
    
  
  } else if(led == 16) {
    
    //If it is 16 reset all the pins to low
    
    printf("All pins are being reseted.\n");
    // ODR is the registers for setting or resetting bits
    // ODR = 0x0000000 means all the bits are resetted.
    GPIOA->ODR = 0x00000000;
    return CmdReturnBadParameter1;
  
  //
  } else {
    
    //If everything is fine just display the selected pin to the terminal
    printf("Selected Pin: %ld\n",led);
  }
  
  // To select the correspound pin to high or low
  // Register right of the value 1 with the amount of index
  shiftNumber = 1 << led;

  // Second input for low/high the correspounding pin. 0-1 is selectable.
  rc = fetch_uint32_arg(&ledControl);
  
  if(rc || ledControl < 0 || ledControl > 1) {
  
    // Check if the second input is 0 or 1
    printf("Please supply 1 | 0 to turn on or off the LED\n");
    printf("Please supply 2 to turn off all the LEDs\n");
    return CmdReturnBadParameter1;
    
  }else if (ledControl == 1) {
  
    // if second input is high.
    // Set the ODR correspounding bit. With Or operator.
    GPIOA->ODR = GPIOA->ODR | shiftNumber;
    printf("Pin %ld is turning on.\n",led);
    
    HAL_Delay(100);
    
    
    // Check if the correspounding pin is working with IDR.
    tempCheck = GPIOA->IDR >> led;
    tempCheck = tempCheck % 2;

    if(tempCheck){
      printf("Pin %ld is turned on correctly.\n",led);
    } else {
      printf("There is a problem!\n");
    }
   
  }else if (ledControl == 0){
  
    // if second input is low.
    // Set the ODR correspounding bit. With And operator.
    GPIOA->ODR = GPIOA->ODR & ~shiftNumber;
    printf("Pin %ld is turning off.\n",led);
    
    HAL_Delay(100);
    
    // Check if the correspounding pin is turned off with IDR.
    tempCheck = GPIOA->IDR >> led;
    tempCheck = tempCheck % 2;
    if(!tempCheck){
      printf("Pin %ld is turned off correctly.\n",led);
    } else {
      printf("There is a problem!\n");
    }
    
  }
 
  
  return CmdReturnOk;
}

//Control the terminal with Leda command Leda <index> <high/low>
ADD_CMD("Leda",assignmentFirst,"index - state   Control Pins")





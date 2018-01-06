

//=================================================================
/* 4-Bit mode 16x2 LCD Module Software      */
/* 2nd Dec 2005                             */
/* Copyright 2005 Circuits4You.com          */
/* WWW - http://www.circuits4you.com        */
/* Email - info@circuits4you.com            */

/* LCD Pin-5(R/W) must be connected to ground*/
//=================================================================
#include <avr/io.h>
#include <string.h>

#include "lcd.h"

#define E   PC5
#define RS   PC4

//=================================================================
//        LCD Display Initialization Function
//=================================================================
void dispinit(void)
{
 int count;
 char init[]={0x43,0x03,0x03,0x02,0x28,0x01,0x0C,0x06,0x02,0x02};

 PORTC &= ~(1<<RS);           // RS=0
 for (count = 0; count <= 9; count++)
  {
 displaybyte(init[count]);
  }
 PORTC |= 1<<RS;    //RS=1
}


//=================================================================
//        Enable Pulse Function
//=================================================================
void epulse(void)
{
 PORTC |= 1<<E;
  delay_ms(1); //Adjust delay if required
 PORTC &= ~(1<<E);
 delay_ms(1); //Adjust delay if required
}


//=================================================================
//        Send Single Byte to LCD Display Function
//=================================================================
void displaybyte(char D)
{
 //data is in Temp Register
  char K1;
  K1=D;
  K1=K1 & 0xF0;
  K1=K1 >> 4;  //Send MSB

  PORTC &= 0xF0;
  PORTC |= (K1 & 0x0F);
 epulse();

  K1=D;
  K1=K1 & 0x0F;  //Send LSB
        PORTC &= 0xF0;
  PORTC |= K1;
 epulse();
}

//=================================================================
//        Display Line on LCD at desired location Function
//=================================================================
void display(char string[16], char LineNo)
{
 int len,count;

 PORTC &= ~(1<<RS);           // RS=0 Command Mode

 if(LineNo==1)
 {
  displaybyte(0x80);   //Move Coursor to Line 1
 }
 else
 {
  displaybyte(0xC0);   //Move Coursor to Line 2
 }
 PORTC |= (1<<RS);           // RS=1 Data Mode

        len = strlen(string);
   for (count=0;count<len;count++)
  {
    displaybyte(string[count]);
 }
   int blank_len = 16-len;
   for (count=0;count<blank_len;count++)
     {
       displaybyte(' ');
    }

}


//=================================================================
//        Delay Function
//=================================================================
void delay_ms(unsigned int de)
{
unsigned int rr,rr1;
   for (rr=0;rr<de;rr++)
   {

  for(rr1=0;rr1<30;rr1++)   //395
  {
   asm("nop");
  }

   }
}



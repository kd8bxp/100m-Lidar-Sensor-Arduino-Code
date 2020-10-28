/*
Copyright (c) 2018 LeRoy Miller

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  

https://github.com/kd8bxp
https://www.youtube.com/channel/UCP6Vh4hfyJF288MTaRAF36w  
https://kd8bxp.blogspot.com/  
*/

#include <Wire.h>
#include "src/LiquidCrystal/LiquidCrystal.h"

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
boolean laseron = 0;
int previousButton = 5;
String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

void setup() {
  Serial.begin(19200);
  inputString.reserve(200);
  lcd.begin(16, 2);        
  lcd.setCursor(0, 0);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
}

void loop() {
  lcd_key = readButtons();  // read the buttons

  switch (lcd_key)               // depending on which button was pushed, we perform an action
  {
    case btnRIGHT:
    {
      
      Serial.println("M"); //Take Slow Reading
      
      break;
    }
    case btnLEFT:
    {
      
      Serial.println("S"); //Show Status
      
      break;
    }
    case btnUP:
    {
      
      Serial.println("D"); //Take Normal Reading
      
      break;
    }
    case btnDOWN:
    {
      
      Serial.println("F"); //Take Fast Reading
      
      break;
    }
    case btnSELECT:
    {
      //Toggle Laser ON/OFF
      
      laseron = !laseron;
      if (laseron == 1) {
        Serial.println("O");
      } 
      if (laseron == 0) { 
        Serial.println("C");
      }
      
      break;
    }
    case btnNONE:
    {
      
      break;
    }
  }
  if (stringComplete) {
    if (inputString.indexOf("OK")>0) { 
      lcd.clear();
      lcd.setCursor(0,0);
      if (laseron == 1) {
       lcd.print("Laser On");
      } 
      if (laseron == 0) { 
        lcd.print("Laser Off");
      }
    } else if (inputString.indexOf("Er")>0) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error: ");
      lcd.setCursor(7,0);
      inputString.remove(0,inputString.indexOf("Er")-1);
      inputString.remove(inputString.length()-2);
      lcd.print(inputString); 
    } else if (inputString.indexOf("V")>0) {
     lcd.clear();
     lcd.setCursor(0,0);
     inputString.remove(inputString.length()-2);
     String volt = inputString;
     volt.remove(0,volt.indexOf("V")-3);
     volt.remove(volt.indexOf("V"));
     char arr[volt.length()];
     volt.toCharArray(arr, volt.length()+1);
     float volts = atof(arr);
     inputString.remove(0,3);
     inputString.remove(inputString.indexOf("V")-6);
     char arr1[inputString.length()];
     inputString.toCharArray(arr1, inputString.length()+1);
     float temp = atof(arr1);
     lcd.print(temp);
     lcd.print("'C ");
     lcd.setCursor(10, 0);
     lcd.print(volts);
     lcd.print("v");
     lcd.setCursor(0,1);
     lcd.print((temp * 9/5) + 32);
     lcd.print("'F");   
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      inputString.remove(inputString.length()-2);
      inputString.remove(inputString.indexOf("m"));
      inputString.remove(0,3);
      char arr[inputString.length()];
      inputString.toCharArray(arr, inputString.length()+1);
      float meter = atof(arr);
      lcd.print(meter); lcd.print("m");
      lcd.setCursor(8,0);
      lcd.print(meter*100); lcd.print("cm");
      lcd.setCursor(0,1);
      lcd.print(meter * 3.281); lcd.print("f");
      lcd.setCursor(8,1);
      lcd.print((meter*3.281)*12); lcd.print("in");
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  
delay(200);
}


int readButtons()
{
  adc_key_in = analogRead(0);      
  if (adc_key_in > 1000) {previousButton = btnNONE; return btnNONE; }// We make this the 1st option for speed reasons since it will be the most likely result

  if (adc_key_in < 50 && previousButton != btnRIGHT) { previousButton = btnRIGHT;  return btnRIGHT; }
  if (adc_key_in < 150 && previousButton != btnUP)  { previousButton = btnUP; return btnUP; }
  if (adc_key_in < 265 && previousButton != btnDOWN) { previousButton = btnDOWN;  return btnDOWN; }
  if (adc_key_in < 450 && previousButton != btnLEFT) { previousButton = btnLEFT;  return btnLEFT; }
  if (adc_key_in < 690 && previousButton != btnSELECT) { previousButton = btnSELECT; return btnSELECT; }

  previousButton = btnNONE;
  return btnNONE;  
}



void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

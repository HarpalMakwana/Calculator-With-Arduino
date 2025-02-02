// ****************************************************************************************************************************
// *                                       *Future Science Hub *
// *                                  Project: Arduino Calculator                                                           *
// *                                        Author: Makwana Harpal                                                           *
// *                                    Contact: 9313758130 (WhatsApp)                                                       *
// *                                Instagram: @Future Science Hub                                                      *
// *                                                                                                                         *
// *                     Description: This project implements a calculator using a                                          *
// *                         4x4 matrix keypad and an OLED display. The user can                                             *
// *                         perform basic arithmetic operations such as addition,                                           *
// *                     subtraction, multiplication, and division. The results are                                          *
// *                          displayed on the OLED display. It also supports reset                                          *
// *                                   functionality to clear calculations.                                                  *
// ***************************************************************************************************************************

/* 
*  Harpal Makwana
*  for Future Science Hub (YouTube)
*  Subscribe for more Arduino projects and tutorials
*  https://www.youtube.com/@Harpal9313
*   
*/

#include <Keypad.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad

//keymap defines the key pressed according to the row and columns just as appears on the keypad
char customKey;
double first = 0;
long second = 0;
double total = 0;
char Operator;
bool equalPressed=false;
bool showFirst = false;

char keymap[numRows][numCols]= 
{
{'1', '2', '3', '+'}, 
{'4', '5', '6', '-'}, 
{'7', '8', '9', 'x'},
{'C', '0', '=', '/'}
};

//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {9,8,7,6}; //Rows 0 to 3
byte colPins[numCols]= {5,4,3,2}; //Columns 0 to 3

//initializes an instance of the Keypad class
Keypad customKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void showSplash() {
  String splashString="Arduino Calculator by";
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setCursor(64-(splashString.length()*3),0);
  display.print(splashString);
  display.setTextSize(2);
  splashString="Harpal ";
  display.setCursor(64-(splashString.length()*6),16);
  display.print(splashString);
  display.setTextSize(2);
  splashString="Makwana";
  display.setCursor(64-(splashString.length()*6),40);
  display.print(splashString);
  display.display();
  delay(3000);
}

void setup()
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  showSplash();
  display.print("Arduino Calculator Harpal Makwana");
  display.setTextSize(2);
  display.clearDisplay();
  display.display();
}

void loop(){
customKey = customKeypad.getKey();
  switch(customKey) 
  {
  case '0' ... '9': // This keeps collecting the first value until a operator is pressed "+-*/"
    showFirst=true;
    first = first * 10 + (customKey - '0');
    showDisplay();  
    break;

  case '+':
    showFirst=true;
    Operator='+';
    showDisplay();
    second = SecondNumber(); // get the collected the second number
    total = first + second;
    showDisplay(); 
    first = total,  // reset values back to zero for next use
    second = 0;   
    break;

  case '-':
    showFirst=true;
    Operator='-';
    showDisplay();
    second = SecondNumber();
    total = first - second;
    showDisplay();
    first = total, second = 0;
    break;

  case 'x':
    showFirst=true;
    Operator='x';
    showDisplay();
    second = SecondNumber();
    total = first * second;
    showDisplay();
    first = total, second = 0;
    break;

  case '/':
    showFirst=true;
    Operator='/';
    showDisplay();
    second = SecondNumber();
    second == 0 ? display.print("Invalid") : total = (float)first / (float)second;
    showDisplay();
    first = total, second = 0;    
    break;

  case 'C':
    total = 0;
    first=0;
    second=0;
    Operator='\0';
    showFirst=false;
    equalPressed=false;
    display.clearDisplay();
    display.display();    
    break;
  }
}

void showDisplay()
{
  display.clearDisplay();
  display.setCursor(110,0);
  display.println(Operator);
  if (showFirst) {
    Serial.print(first);
    display.println(first);
  } else
  {
    display.println("");
  }
  if (second>0) {
    display.println(second);
  } else
  {
    display.println("");
  }
  if (equalPressed) {
    display.println(total);
    equalPressed=false;
  }
  display.display();
}
long SecondNumber()
{
  while( 1 )
  {
    customKey = customKeypad.getKey();
    if(customKey >= '0' && customKey <= '9')
    {
      second = second * 10 + (customKey - '0');
      showDisplay();
    }

    if(customKey == '=') {
      equalPressed=true;
      break;  //return second;
    }
  }
 return second; 
}

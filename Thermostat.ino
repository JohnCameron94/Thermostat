
/**********************************************************************************
 * Program by Johnathon Cameron
 * Version : 1.0
 * Hardware: LCD Display,Thermistor,Potentimeter
 * Controller: Arduino Uno/ Teensy 3.2 and YwRobot powersupply
 * 
 * Description: Program is created to display Temperature and act as a thermostate. 
 * On a button click the user will change the display to Fahrenheit. 
 * 
 * Default Temperature: is in Celsius.
 **********************************************************************************/
#include <LiquidCrystal.h>
#include <TimerOne.h>

/**PIN Values **/
#define FAN_PIN 10
#define THERMISTOR 0
#define PUSH_BUTTON 13
#define CELSIUS 1
#define FAHRENHEIT 0

/*************************************
 * Default Temp Unit at start of program. 
 * Changes upon user clicking the push 
 * button for 5 seconds
 ************************************/
int  TEMP_UNIT = CELSIUS;

/***********************
 * TEMPERATURE VARIABLES
 ***********************/
  int code;
  float celsius;//Different Units
  float fahrenheit;

LiquidCrystal lcd(12,11,5,4,3,2);

/*SETUP MODE*/
void setup() {
  /*Interrupt driven, to toggle between Temperature Units*/
  //Timer1.initialize(digitalRead(PUSH_BUTTON));
  //Timer1.attachInterrupt(changeDefaultUnit);
  pinMode(PUSH_BUTTON,INPUT);
  pinMode(FAN_PIN,OUTPUT);
  lcd.print("John's Thermostat");
  Serial.begin(9600);
}
/**************************************
 * Function used to change default unit
 **************************************/
void changeDefaultUnit(){
  int success = 0;//Boolean to check if user kept finger on button for 5 seconds

  //Loop for 5 seconds
  for(int i = 5; i > 0 ; i--){
      if(digitalRead(PUSH_BUTTON) == LOW){
        lcd.clear();//refresh after every second
        
        /**Print to display */
        lcd.print("Hold:");
        lcd.print(i);
        lcd.print(" Second");
        delay(1000);//1 Second
        
        /**********************************
         * If user kept finger on button for 
         * 5 seconds, then success is true
         **********************************/
        if(i==1){ 
         lcd.clear(); 
         success = 1;
       
        }
      }  
  } 
  /************************************************
   * If success change the default temp unit
   * print to display the default delay for 1 second
   * before going back to main display of temps
   *************************************************/
  if(success){
    if(TEMP_UNIT){//if default is C
      TEMP_UNIT = FAHRENHEIT;//change to F
       lcd.print("Default:F");
       delay(1000);// 1 second
      
    }else{//If default is F
      TEMP_UNIT = CELSIUS;//change to C
       lcd.print("Default:C");
       delay(1000);// delay 1 second
    }
   }
  
  lcd.clear();//refresh display with main Display of temps
  
}

float readTemp(){
  
  /**Temp Unit Holders*/
  float celsius;//C
  float fahrenheit;//F
  
  //check if user has clicked button
  int state = digitalRead(PUSH_BUTTON);
  
 /*read input from thermistor and do accurate conversion */ 
  int code = analogRead(THERMISTOR);
  if (code <= 289) {
    celsius = 5 + (code - 289) / 9.82;
  }
  if (code > 289 && code <= 342) {
    celsius = 10 + (code - 342) / 10.60;
  }
  if (code > 342 && code <= 398) {
    celsius = 15 + (code - 398) / 11.12;
  }
  if (code > 398 && code <= 455) {
    celsius = 20 + (code - 455) / 11.36;
  }
  if (code > 455 && code <= 512) {
    celsius = 25 + (code - 512) / 11.32;
  }
  if (code > 512 && code <= 566) {
    celsius = 30 + (code - 566) / 11.00;
  }
  if (code > 566 && code <= 619) {
    celsius = 35 + (code - 619) / 10.44;
  }
  if (code > 619 && code <= 667) {
    celsius = 40 + (code - 667) / 9.73;
  }
  if (code > 667) {
    celsius = 45 + (code - 712) / 8.90;
  }

  fahrenheit = celsius * 1.8000 +32.00;//convert to fahrenheit
 
 /**Check for button click**/
  if(!state){
   changeDefaultUnit();
  }//Change the default temp unit

  //Check which temp unit is default
  //C = 1, F = 0
  if(TEMP_UNIT)
    return celsius;
  else
    return fahrenheit;
}

void loop() {
 
  float temp = readTemp();//read temp

  //DEBUG uncomment to see serial print
  //Serial.print("Temp ");
  //Serial.println(temp);
  
 /********************
  * LCD Display prints
  ********************/
  lcd.print("Temp:");//print to LCD Display

  // LCD Display will display the correct Temp Unit
  if(TEMP_UNIT == CELSIUS){
    //TEMP Check if to hot turn on fan else turn it off
    if(temp > 26)
      analogWrite(FAN_PIN,255);
    else
      analogWrite(FAN_PIN,10); 
      
    lcd.print(temp); 
    lcd.print("(C)");
    
  }else{
    
    //TEMP Check if to hot turn on fan else turn it off
    if(temp > 77)
      analogWrite(FAN_PIN,255);
    else
      analogWrite(FAN_PIN,0);

      
    lcd.print(temp);
    lcd.print("(F)");
  }
  //Delay for 1 second before reading new temp
  delay(1000);
  //refresh display with new values
  lcd.clear();

}

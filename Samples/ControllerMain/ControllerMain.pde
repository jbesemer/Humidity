/*
  The circuit:
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */

#include <LiquidCrystal.h>
#include <Sensirion.h>
#include "Encoder.h"

const int DRED = A4;
const int DGRN = A5;

const int  dataPin = A1;  // 2.red
const int  clockPin = A2;  // 3.orange
Sensirion Sensor(dataPin, clockPin);

const int LCD_RS = 12;
const int LCD_Enable = 11;
const int LCD_D4 = 10;
const int LCD_D5 = 9;
const int LCD_D6 = 8;
const int LCD_D7 = 7;

const int LCD_ROwS = 2;
const int LCD_COLUMNS = 16;

LiquidCrystal lcd(LCD_RS, LCD_Enable, LCD_D4, LCD_D5, LCD_D6, LCD_D7 );

const int EncoderA = 2;
const int EncoderB = 3;
const int Button = 4;

volatile int Counter = 0;
int PrevCounter = 0;

const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 6; // Analog output pin that the LED is attached to

int DisplayPwm = 145;

int InterruptCount = 0;

void setup() {
	Serial.begin(38400); // Open serial connection to report values to host

	lcd.begin(LCD_COLUMNS,LCD_ROwS);

   analogWrite(analogOutPin, DisplayPwm);

	pinMode(EncoderA, INPUT);
	digitalWrite(EncoderA, HIGH);
	pinMode(EncoderB, INPUT);
	digitalWrite(EncoderB, HIGH);
	pinMode(Button, INPUT);
	digitalWrite(Button, HIGH);

	attachInterrupt(0, updateEncoder, CHANGE);
	attachInterrupt(1, updateEncoder, CHANGE);

      pinMode( DRED, OUTPUT);
      pinMode( DGRN, OUTPUT);
	digitalWrite(DRED, LOW);
	digitalWrite(DGRN, LOW);
  
      Serial.println("Ready...");
}

const int UpdateIntensityInterval = 100;
const int UpdateSensorInterval = 5000;
const int UpdateDisplayInterval = 250;

unsigned long prevUpdateIntensity = 0;
unsigned long prevUpdateSensor = -UpdateSensorInterval;  // force refresh at startup
unsigned long prevUpdateDisplay = 0;


void loop() {
  //updateEncoder();
   
   if( Counter != PrevCounter ){
     Serial.print( "Count: ");
     Serial.print(PrevCounter);
     Serial.print(" -> ");
     Serial.println( Counter);
     PrevCounter = Counter;
   }

  int ms = millis();

  //if( ms - prevUpdateIntensity > UpdateIntensityInterval ){
  //  prevUpdateIntensity = ms;
  //  updateLcdLedIntensity();
  //}

  if( ms - prevUpdateDisplay > UpdateDisplayInterval ){
    prevUpdateDisplay = ms;
	UpdateDisplay();
  }

  if( ms - prevUpdateSensor > UpdateSensorInterval ){
    prevUpdateSensor = ms;
    UpdateSensor();
  }
}


float temp_c;
float temp_f;
float humidity;
float dewPoint;

void UpdateSensor()
{
    digitalWrite(DRED, HIGH);
    lcd.setCursor(14, 1);
    lcd.print("S");
  
    // Read values from the sensor
    int err = Sensor.measure( &temp_c, &humidity, &dewPoint);

    // Print the values to the serial port
    Serial.print("Temperature: ");
    Serial.print(temp_c );
    Serial.print("C / ");
    //Serial.print(temp_f );
    Serial.print(" Humidity: ");
    Serial.print(humidity);
    Serial.println("%    ");

    lcd.setCursor(0, 0);
    //lcd.print("Temperature: ");
    lcd.print(temp_c );
    lcd.print("C / ");
    //lcd.print(temp_f );
    //lcd.print("F "); //Humidity: ");
    lcd.print(humidity);
    lcd.print("%    ");
    
    lcd.setCursor(14, 1);
    lcd.print(" ");
    digitalWrite(DRED, LOW);
}

void UpdateDisplay(){
    digitalWrite(DGRN, HIGH);
    lcd.setCursor(14, 1);
    lcd.print("D");

    lcd.setCursor(0, 1);
    //lcd.print(digitalRead(EncoderA));
    //lcd.print(" ");
    //lcd.print(digitalRead(EncoderB));
    //lcd.print(" ");
    lcd.print(InterruptCount);
    lcd.print(" ");
    lcd.print(Counter); //millis()/1000);
    lcd.print("  ");
    
    lcd.setCursor(14, 1);
    lcd.print(" ");
    digitalWrite(DGRN, LOW);
}

void updateLcdLedIntensity()
{
   int sensorValue = analogRead(analogInPin);
   DisplayPwm = map(sensorValue, 0, 1023, 0, 255);
   analogWrite(analogOutPin, DisplayPwm);
}

void updateEncoder(){
}


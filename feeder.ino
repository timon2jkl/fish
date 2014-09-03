#include <Wire.h>
#include "RTClib.h"
#include "LiquidCrystal.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 4 on the Arduino
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
RTC_DS1307 RTC;

// Connect via i2c, default address #0 (A0-A2 not jumpered)
LiquidCrystal lcd(0);

const int motor1Pin = 8;    // H-bridge leg 1 (pin 2, 1A) (L293D pin numbers)
const int motor2Pin = 7;    // H-bridge leg 2 (pin 7, 2A)
const int enablePin1 = 6;    // H-bridge enable pin 1
const int motor3Pin = 2;    // H-bridge leg 3 (pin 15, 1B)
const int motor4Pin = 3;    // H-bridge leg 4 (pin 10, 2B)
const int enablePin2 = 5;    // H-bridge enable pin 9

void setup () {

  Serial.begin(115200);
  Wire.begin();
  RTC.begin();
  sensors.begin();

  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);

  //RTC.adjust(DateTime(__DATE__, __TIME__));

  // set all the other pins you're using as outputs:
  pinMode(motor1Pin, OUTPUT); 
  pinMode(motor2Pin, OUTPUT); 
  pinMode(enablePin1, OUTPUT);
  pinMode(motor3Pin, OUTPUT); 
  pinMode(motor4Pin, OUTPUT); 
  pinMode(enablePin2, OUTPUT);


  digitalWrite(motor1Pin, LOW);   // set leg 1 of the H-bridge low PUMP 1
  digitalWrite(motor2Pin, LOW);  // set leg 2 of the H-bridge high  EXTRA
  digitalWrite(motor3Pin, LOW);   // set leg 3 of the H-bridge low PUMP 2
  digitalWrite(motor4Pin, LOW);  // set leg 4 of the H-bridge high FEEDER

  //set enablePin high so that motor can turn on:
  digitalWrite(enablePin1, LOW);
  digitalWrite(enablePin2, LOW);  
}

void loop() {
  {
    sensors.requestTemperatures(); // Send the command to get temperatures
    DateTime now = RTC.now();   //get time
    lcd.setCursor(0, 0); 

    //set row and  space to start word
    switch(now.dayOfWeek())
    {
    case 0:  lcd.print("Sun");  break;
    case 1:  lcd.print("Mon");  break;
    case 2:  lcd.print("Tue");  break;
    case 3:  lcd.print("Wed");  break;
    case 4:  lcd.print("Thu"); break;
    case 5:  lcd.print("Fri");  break;
    case 6:  lcd.print("Sat");  break;
    default: lcd.print("Error");
    }
    lcd.print(' ');

    switch(now.month())
    {
    case 1:  lcd.print("Jan "); break;    
    case 2:  lcd.print("Feb "); break;
    case 3:  lcd.print("Mar "); break;
    case 4:  lcd.print("Apr "); break;
    case 5:  lcd.print("May "); break;
    case 6:  lcd.print("Jun "); break;
    case 7:  lcd.print("Jul "); break;
    case 8:  lcd.print("Aug "); break;    
    case 9:  lcd.print("Sep "); break;
    case 10: lcd.print("Oct "); break;
    case 11: lcd.print("Nov "); break;
    case 12: lcd.print("Dec "); break;
    default: lcd.print("Error ");
    }
    lcd.print(now.day(), DEC);

    lcd.print(' ');
    lcd.print(now.year(), DEC);

    lcd.setCursor(0, 1);      // start row 2 (1)
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    lcd.print(now.second(), DEC);
    lcd.setCursor(9, 1);      //move temp to 9 space row 2(1)
    lcd.print(' ');
    lcd.print(sensors.getTempCByIndex(0) * 1.8 + 32.0);   //converts celc to fraenh  C to F
    lcd.print('F');

    delay(30);


    // Set the time you want the motors to START
    if((now.hour () == 2)&&(now.minute () == 23 )&&(now.second () == 00)&&(now.dayOfWeek()==5))

    {
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print(" TRUE");
      lcd.print(" ");
      lcd.print("Pump 1");
      digitalWrite(enablePin1, HIGH);
      digitalWrite(motor1Pin, HIGH);
      delay(8500); // set how long you want the motor to run... 1000 = aprox 1ml
      digitalWrite(motor1Pin, LOW);
      digitalWrite(enablePin1, LOW);

      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print(" TRUE");
      lcd.print(" ");
      lcd.print("Pump 2");
      digitalWrite(enablePin2, HIGH);
      digitalWrite(motor3Pin, HIGH);
      delay(8500); // set how long you want the motor to run... 1000 = aprox 1ml
      digitalWrite(motor3Pin, LOW);
      digitalWrite(enablePin2, LOW);

      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print(" TRUE");
      lcd.print(" ");
      lcd.print("FEEDER 1");
      digitalWrite(enablePin2, HIGH);
      digitalWrite(motor4Pin, HIGH);
      delay(8500); // set how long you want the motor to run... 1000 = aprox 1ml
      digitalWrite(motor4Pin, LOW);
      digitalWrite(enablePin2, LOW);
    }
  }

}

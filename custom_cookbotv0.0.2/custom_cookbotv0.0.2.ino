/*

   Created By MD Borhan uddin 02/01/2017
*/
#include <Servo.h>
#include <LiquidCrystal.h>
#include "Wire.h"
#include <SoftwareSerial.h>
#include <Adafruit_PWMServoDriver.h>
SoftwareSerial blSerial(2, 3);
Servo bigServo, heatServo;

Servo chickenServo;

int cBigCup_1 = 52, cBigCup_2 = 50, cSmallCup_1 = 48, cSmallCup_2 = 46, cSmallCup_3 = 44, cSmallCup_4 = 42;

boolean OnionServoStatus = false, ChiliServoStatus = false, SaltServoStatus = false, GingerServoStatus = false, RiceServoStatus = false;

int  ovenPin = 12;//, waterPin = 26 ;// waterPin willbe 40
int OnionServoPin = 28, ChiliServoPin = 27, GingerServoPin = 29, RiceServoPin = 31, SaltServoPin = 33;
int ledPin = 8;
boolean spiceServoStatus, deviceEnabled = false, ChickenServoStatus = false, OvenActionStatus = false, OilActionStatus = false, spudHandaleStatus = false, waterActionStatus = false;
boolean oneCycle = false;
long previousTime, previousTimeOven;
String dateofDay = "", siparator = "/", timeofDay = "", timeSiparator = ":", fullLineBlank = "                    ";
///Buletooth
char receiveData;
int pos = 0;
//RTC
#define DS3231_I2C_ADDRESS 0x68
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMINR  600 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAXR  150 // this is the 'maximum' pulse length count (out of 4096)
uint8_t servonum = 0;
int spudPin = 5;//44
int oilPin = 7;
int waterPin = 11; //#####################################water pin
int allConnectionOn = 4;
//Coocking Item
boolean chiken = false, beef = false, rice = false, nudoles = false;
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int totalInput = 0, isServo = 0;
String inputString = "";
boolean stringComplete = false;
// Physica equvalent of lite jessy, we need to an operating system. thats the great idea when is the product of mechanism
//LiquidCrystal lcd(51, 53, 46, 48, 50, 52);
LiquidCrystal lcd(51, 53, 43, 45, 47, 49);
void setup() {
  Wire.begin();
  pinMode(oilPin, OUTPUT);
  pinMode(waterPin, OUTPUT);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  yield();
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("  COOK BOT v0.0.1 ");
  Serial1.begin(38400);
  Serial.begin(38400);
  Serial.println("I am here");
  Serial1.println("Bluetooth");

  spiceServoStatus = false;
  ChickenServoStatus = false;
  //cBigCup_1=52,cBigCup_2=50,cSmallCup_1=48,cSmallCup_2=46,cSmallCup_3=44,cSmallCup_4=42;
  pinMode(cBigCup_1, OUTPUT);
  pinMode(cBigCup_2, OUTPUT);
  pinMode(cSmallCup_1, OUTPUT);
  pinMode(cSmallCup_2, OUTPUT);
  pinMode(cSmallCup_3, OUTPUT);
  pinMode(cSmallCup_4, OUTPUT);

  pinMode(OnionServoPin, OUTPUT);
  pinMode(ChiliServoPin, OUTPUT);
  pinMode(GingerServoPin, OUTPUT);
  pinMode(RiceServoPin, OUTPUT);
  pinMode(SaltServoPin, OUTPUT);

  pinMode(ovenPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(spudPin, OUTPUT);

  pinMode(allConnectionOn, OUTPUT);

  digitalWrite(cBigCup_1, HIGH);
  digitalWrite(cBigCup_2, HIGH);
  digitalWrite(cSmallCup_1, HIGH);
  digitalWrite(cSmallCup_2, HIGH);
  digitalWrite(cSmallCup_3, HIGH);
  digitalWrite(cSmallCup_4, HIGH);

  digitalWrite(oilPin, HIGH);
  digitalWrite(waterPin, HIGH);

  digitalWrite(OnionServoPin, HIGH);
  digitalWrite(ChiliServoPin, HIGH);
  digitalWrite(GingerServoPin, HIGH);
  chickenServo.attach(37);
  chickenServo.write(0);
  heatServo.attach(36);
  heatServo.write(10);

  bigServo.attach(8, 0, 180);
}
void loop() {
  if (stringComplete) {
    Serial.println(inputString);
    String inputCounter = getValueA(inputString, ':', 8000);
    String CheckTemp = getValueA(inputString, '!', 1);
    if (CheckTemp.toInt() > 0)
    {
      //Serial.println(CheckTemp.toInt());
      if (CheckTemp.toInt() >= 0 && CheckTemp.toInt() <= 40)
      {
        //heatServoAction(1);
      } if (CheckTemp.toInt() > 40 && CheckTemp.toInt() <= 70)
      {
        //heatServoAction(2);
      }
      if (CheckTemp.toInt() > 70 && CheckTemp.toInt() <= 100)
      {
        //heatServoAction(3);
      }
    }
    if (totalInput > 0)
    {
      String space = " ";
      String nameOfCookie  = space + getValue(inputString, ':', 0);
      lcd.setCursor(0, 1);
      lcd.print(fullLineBlank);
      lcd.setCursor(0, 1);
      lcd.print(nameOfCookie);
      //digitalWrite(ovenPin, HIGH);
      digitalWrite(allConnectionOn, HIGH);
      for (int i = 0; i <= totalInput; i++)
      {
        //Serial.println(getValue(inputString, ':', i));
        String eachValue = getValue(inputString, ':', i);

        String checkServo = getValue(eachValue, '#', 1); // is it servo
        String checkSpudFri = getValue(eachValue, '@', 1); //is it spud
        String checkOil = getValue(eachValue, '+', 1); //is it Oil
        String bigServi = getValue(eachValue, '$', 1); //is it big servo
        String spudTimeDelay = getValue(eachValue, '*', 1); //is it Oil
        String checkWater = getValue(eachValue, '^', 1); //is it Water
        String checkHeat = getValue(eachValue, '%', 1); //is it heat
        String longDelayCheck = getValue(eachValue, '&', 1); //is it long deay spud
        String onlyTimeDelay = getValue(eachValue, '`', 1); //is it Only Time delay

        String ovenOnCheck = getValueA(eachValue, '=', 1);
        String ovenOffCheck = getValueA(eachValue, '|', 1);
        if (ovenOnCheck.toInt() > 0)
        {
          Serial.println("HIGH");
          digitalWrite(ovenPin, HIGH);
        }
        if (ovenOffCheck.toInt() > 0)
        {
          Serial.println("LOW");
          digitalWrite(ovenPin, LOW);
        }


        if (onlyTimeDelay.toInt() > 0)
        {
          OnlyTimeDelayF(onlyTimeDelay.toInt());
        }

        if (longDelayCheck.toInt() > 0)
        {
          //Serial.println(longDelayCheck);
          longTimeOparation(longDelayCheck.toInt());
        }

        if (checkHeat.toInt() > 0)
        {
          //Serial.println(checkHeat);
          heatServoAction(checkHeat.toInt());
        }

        if (checkServo.toInt() > 0)
        {
          //servo action with number
          //Serial.println(checkServo);
          servoAction(checkServo.toInt() - 1);
        }
        if (checkSpudFri.toInt() > 0)
        {
          //spud handle with timming
          //Serial.println(checkSpudFri);
          spudOparation(checkSpudFri.toInt());
        }
        if (checkOil.toInt() > 0)
        {
          //Put oil with timming
          //Serial.println(checkOil);
          oilOparation(checkOil.toInt());
        }
        if (bigServi.toInt() > 0)
        {
          //Put oil with timming
          //Serial.println(bigServi);
          bigServo.attach(8, 0, 180);
          bigServoStart();
        }
        if (spudTimeDelay.toInt() > 0)
        {
          //Put oil with timming
          //Serial.println(spudTimeDelay);
          timeOparation(spudTimeDelay.toInt());
        }
        if (checkWater.toInt() > 0)
        {
          //Put oil with timming
          //Serial.println(checkWater);
          waterOparation(checkWater.toInt());
        }
        delay(100);
      }
      digitalWrite(allConnectionOn, LOW);
      //digitalWrite(ovenPin, LOW);
      heatServo.write(10);
      delay(15);
      lcd.setCursor(0, 2);
      lcd.print(fullLineBlank);
      lcd.setCursor(0, 2);
      lcd.print(" COOK COMPLETED");
      sendAndroidValues(" COOK COMPLETED");
      //sendAndroidValues("@@");
    }
    totalInput = 0;
    inputString = "";
    stringComplete = false;
  }
}
void heatServoAction(int val)
{
  if (val == 1)
  {

    heatServo.write(10);
    Serial.println("111");
    delay(15);

  } else if (val == 2)
  {
    Serial.println("112");
    heatServo.write(100);
    delay(15);
  } else if (val == 3)
  {
    Serial.println("113");
    heatServo.write(175);
    delay(15);
  }

}
void bigServoStart()
{
  Serial.println("211");
  bigServo.attach(8, 0, 180);
  lcd.setCursor(0, 2);
  lcd.print(fullLineBlank);
  lcd.setCursor(0, 2);
  lcd.print(" SPUD GRINDING");
  sendAndroidValues(" SPUD GRINDING");
  setPosition(400);
  delay(6000);
  setPosition(500); //500
  delay(6000);
  Serial.println("211");
  Serial.println();
  bigServo.detach();


}
void servoAction(int n)
{
  servoDisplay(n + 1);

  if (n == 6)
  {

    chickenCup1();
  }
  else if (n == 7)
  {
    chickenCup2();
  }
  //cBigCup_2 = 50, cSmallCup_1 = 48, cSmallCup_2 = 46, cSmallCup_3 = 44, cSmallCup_4 = 42
  else if (n == 0)
  {
    digitalWrite(cSmallCup_1, LOW);
    delay(500);
    digitalWrite(cSmallCup_1, HIGH);
  }
  else if (n == 1)
  {
    digitalWrite(cSmallCup_2, LOW);
    delay(500);
    digitalWrite(cSmallCup_2, HIGH);
  }
  else if (n == 2)
  {
    digitalWrite(cSmallCup_3, LOW);
    delay(500);
    digitalWrite(cSmallCup_3, HIGH);
  }
  else if (n == 8)
  {
    digitalWrite(cSmallCup_4, LOW);
    delay(500);
    digitalWrite(cSmallCup_4, HIGH);
  }
  else
  {
    for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
      pwm.setPWM(n, 0, pulselen);
    }

    delay(500);
    for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
      pwm.setPWM(n, 0, pulselen);
    }

    delay(500);
  }
}

void servoActionReverse(int n)
{



  //Serial.println(servonum);
  servoDisplay(n + 1);
  for (uint16_t pulselen = SERVOMINR; pulselen < SERVOMAXR; pulselen++) {
    pwm.setPWM(n, 0, pulselen);
  }

  delay(500);
  for (uint16_t pulselen = SERVOMAXR; pulselen > SERVOMINR; pulselen--) {
    pwm.setPWM(n, 0, pulselen);
  }

  delay(500);

}
void chickenCup1()
{
  /*chickenServo.write(179);              // tell servo to go to position in variable 'pos'
    delay(1500);
    chickenServo.write(0);              // tell servo to go to position in variable 'pos'
    delay(1500);      // waits 15ms for the servo to reach the position
  */


  //For new version

  digitalWrite(cBigCup_1, LOW);
  delay(500);
  digitalWrite(cBigCup_1, HIGH);

}

void chickenCup2()
{
  digitalWrite(cBigCup_2, LOW);
  delay(500);
  digitalWrite(cBigCup_2, HIGH);
}

void servoDisplay(int n)
{
  if (n == 1)
  {
    Serial.println("311");
    Serial.println();
    sendAndroidValues(" BEING ONION ");
    lcd.setCursor(0, 2);
    lcd.print(fullLineBlank);
    lcd.setCursor(0, 2);
    lcd.print(" BEING ONION");
  } else if (n == 2)
  {
    Serial.println("312");
    Serial.println();
    sendAndroidValues(" BEING CHILI ");
    lcd.setCursor(0, 2);
    lcd.print(fullLineBlank);
    lcd.setCursor(0, 2);
    lcd.print(" BEING CHILI");
  } else if (n == 3)
  {
    Serial.println("313");
    Serial.println();
    sendAndroidValues(" BEING SALT ");
    lcd.setCursor(0, 2);
    lcd.print(fullLineBlank);
    lcd.setCursor(0, 2);
    lcd.print(" BEING SALT");
  } else if (n == 4)
  {

  } else if (n == 5)
  {

  } else if (n == 6)
  {

  }
  else if (n == 7)
  {
    Serial.println("314");
    Serial.println();
    sendAndroidValues(" BEING CHICKEN 1");
    lcd.setCursor(0, 2);
    lcd.print(fullLineBlank);
    lcd.setCursor(0, 2);
    lcd.print(" BEING CHICKEN 1");

  }
  else if (n == 8)
  {
    Serial.println("314");
    Serial.println();
    sendAndroidValues(" BEING CHICKEN 2");
    lcd.setCursor(0, 2);
    lcd.print(fullLineBlank);
    lcd.setCursor(0, 2);
    lcd.print(" BEING CHICKEN 2");

  }
  else if (n == 9)
  {
    Serial.println("315");
    Serial.println();
    sendAndroidValues(" BEING MIXED SPICE ");
    lcd.setCursor(0, 2);
    lcd.print(fullLineBlank);
    lcd.setCursor(0, 2);
    lcd.print(" BEING MIXED SPICE");
  }


}
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;

  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period");
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit");
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}
void timeOparation(int t)
{
  Serial.println(t);
  digitalWrite(spudPin, HIGH);

  for (int i = 0; i <= t; i++)
  {
    if (i % 20 == 0)
    {
      //spudOparation(3);
      bigServoStart();
      i = i + 12;
    } else
    {
      sendAndroidValues(" DELAY TIME");
      lcd.setCursor(0, 2);
      lcd.print(fullLineBlank);
      lcd.setCursor(0, 2);
      lcd.print(" DELAY TIME");
      delay(1000);
    }

  }
  digitalWrite(spudPin, LOW);

}
void longTimeOparation(int t)
{
  //Serial.println(t);
  digitalWrite(spudPin, HIGH);

  for (int i = 0; i <= t; i++)
  {
    if (i % 40 == 0)
    {
      //spudOparation(3);
      bigServoStart();
      i = i + 12;
    } else
    {
      sendAndroidValues(" DELAY TIME");
      lcd.setCursor(0, 2);
      lcd.print(fullLineBlank);
      lcd.setCursor(0, 2);
      lcd.print(" DELAY TIME");
      delay(1000);
    }

  }
  digitalWrite(spudPin, LOW);

}
void OnlyTimeOparation(int t)
{
  //Serial.println(t);
  sendAndroidValues(" DELAY TIME");
  digitalWrite(spudPin, HIGH);
  lcd.setCursor(0, 2);
  lcd.print(fullLineBlank);
  lcd.setCursor(0, 2);
  lcd.print(" DELAY TIME");
  for (int i = 0; i <= t; i++)
  {
    delay(1000);
  }
  digitalWrite(spudPin, LOW);

}
void temp_control()
{
  //Temp control sensore attached and run
}
void OnlyTimeDelayF(int t)
{
  //Serial.println(t);
  sendAndroidValues(" ONLY DELAY TIME");
  lcd.setCursor(0, 2);
  lcd.print(fullLineBlank);
  lcd.setCursor(0, 2);
  lcd.print(" ONLY DELAY TIME");
  for (int i = 0; i <= t; i++)
  {
    delay(1000);
  }
}
void oilOparation(int t)
{
  digitalWrite(oilPin, LOW);
  sendAndroidValues(" BEING OIL");
  long ms = t * 1000;
  lcd.setCursor(0, 2);
  lcd.print(fullLineBlank);
  lcd.setCursor(0, 2);
  lcd.print("  BEING OIL");
  delay(ms);
  digitalWrite(oilPin, HIGH);
}

void waterOparation(int t)
{
  digitalWrite(waterPin, LOW);
  Serial.println("Water Being");
  sendAndroidValues(" BEING WATER");
  long ms = t * 1000;
  lcd.setCursor(0, 2);
  lcd.print(fullLineBlank);
  lcd.setCursor(0, 2);
  lcd.print("  BEING WATER");
  delay(ms);
  digitalWrite(waterPin, HIGH);
}
/*void spudOparation(int t)
  {
  Serial.println(t);
  unsigned long ms = t * 1000;
  digitalWrite(spudPin, HIGH);
  Serial.println(ms);
  delay(ms);
  digitalWrite(spudPin, LOW);
  }*/

void spudOparation(int t)
{
  Serial.println(t);
  sendAndroidValues("  SPUD GRINDING");
  digitalWrite(spudPin, HIGH);
  lcd.setCursor(0, 2);
  lcd.print(fullLineBlank);
  lcd.setCursor(0, 2);
  lcd.print(" SPUD GRINDING");
  for (int i = 0; i <= t; i++)
  {
    delay(1000);
  }
  digitalWrite(spudPin, LOW);

}
void serialEvent1() {
  while (Serial1.available()) {
    char inChar = (char)Serial1.read();
    //for new receipie
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
String getValueA(String data, char separator, int index)
{
  int maxIndex = data.length() - 1;
  int j = 0;
  String chunkVal = "";
  int k = 0;
  for (int i = 0; i <= maxIndex;  i++)
  {
    chunkVal.concat(data[i]);

    if (data[i] == separator)
    {
      j++;
      k++;
      totalInput++;
      if (j > index)
      {
        chunkVal.trim();
        return chunkVal;
      }

      chunkVal = "";
    }
    else if ((i == maxIndex) && (j < index)) {
      chunkVal = "";
      return chunkVal;
    }
  }
}
void setPosition(int val)
{
  if (val >= 500)
  {
    Serial.print("writing Microseconds: ");
    Serial.println(val);
    bigServo.writeMicroseconds(val);
  }
  else
  {
    Serial.print("writing Angle: ");
    Serial.println(val);
    bigServo.write(val);

  }
}





void spudHandale(int ms)
{
  if (oneCycle == false)
  {

    digitalWrite(spudPin, HIGH);
    delay(ms);
    digitalWrite(spudPin, LOW);
  }

}
void sendAndroidValues(String value)
{
  Serial1.print('#');
  /*for(int k=0; k<4; k++)
    {
    //Serial1.print(voltageValue[k]);
    Serial1.print("A");
    //Serial1.print('+');
    }*/
  Serial1.print(value);
  Serial1.print('~');
  Serial1.println();
  //delay(5000);
}

void sendAndroidValuesForProcessing(String value)
{
  Serial1.print('#');
  Serial1.print(value);
  Serial1.print('~');
  Serial1.println();
  //delay(5000);
}
void ovenAction()
{
  digitalWrite(ovenPin, HIGH);
  lcd.setCursor(0, 2);
  lcd.print(fullLineBlank);
  lcd.setCursor(0, 2);
  lcd.print("  Oven Heating up");
}


//########################################### RTC Clock  ############### START
void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
void displayTime()
{

  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

  timeofDay = hour + timeSiparator + minute + timeSiparator + second;
  lcd.setCursor(2, 3);
  lcd.print(timeofDay);

  dateofDay = dayOfMonth + siparator + month + siparator + year;
  lcd.setCursor(10, 3);
  lcd.print("  ");
  lcd.setCursor(11, 3);
  lcd.print(dateofDay);
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(dayOfWeek));
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();
}
byte decToBcd(byte val)
{
  return ( (val / 10 * 16) + (val % 10) );
}
byte bcdToDec(byte val)
{
  return ( (val / 16 * 10) + (val % 16) );
}
//########################################### RTC CLOCK  ############### END

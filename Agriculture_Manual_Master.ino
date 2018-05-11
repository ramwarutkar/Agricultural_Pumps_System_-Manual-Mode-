#include <Wire.h>
#include <EEPROM.h>
#include <DS3231.h>
DS3231  rtc(SDA, SCL);
Time  t;
int Hor;
int Min;
int Sec;
int Date;
int Month;
int Year;
int Day;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
}

void loop() {
 // getting_time();
  commonregisters();

}

void commonregisters()
{
  static int register_address = 0;
  static int start_bit = 0;
  Wire.requestFrom(5, 1);
  while (Wire.available())
  {
    int c = Wire.read();
    if (start_bit == 0)
    {
      if (c == 255)
      {
        start_bit = 1;
        Serial.print("Start Bit Received  - ");
        Serial.println(start_bit);
      }
    }
    else if (start_bit == 1)
    {
      EEPROM.write(register_address, c);
      Serial.print("Register Address - ");
      Serial.println(register_address);
      Serial.print("EEPROM Value - ");
      Serial.println(EEPROM.read(register_address));
      register_address++;
      if (register_address == 109)
      {
        register_address = 0;
        start_bit = 0;
      }
    }
  }
}

void alternate_days()
{

}

void getting_time()
{
  static int set_time = 1;
  t = rtc.getTime();
  Hor = t.hour;
  Min = t.min;
  Sec = t.sec;
  Date = t.date;
  Month = t.mon;
  Year = t.year;
  Day = t.dow;
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");
  Serial.println(rtc.getTimeStr());
  if (set_time = 1)
  {
    rtc.setTime(EEPROM.read(103), EEPROM.read(104), EEPROM.read(105));
    rtc.setDate(EEPROM.read(108), EEPROM.read(107), 2000 + EEPROM.read(106));
    switch (EEPROM.read(102))
    {
      case 1:
        {
          rtc.setDOW(MONDAY);
          break;
        }
      case 2:
        {
          rtc.setDOW(TUESDAY);
          break;
        }
      case 3:
        {
          rtc.setDOW(WEDNESDAY);
          break;
        }
      case 4:
        {
          rtc.setDOW(THURSDAY);
          break;
        }
      case 5:
        {
          rtc.setDOW(FRIDAY);
          break;
        }
      case 6:
        {
          rtc.setDOW(SATURDAY);
          break;
        }
      case 7:
        {
          rtc.setDOW(SUNDAY);
          break;
        }
      default: break;
    }
    set_time = 0;
  }
}


#include <Wire.h> 
#include <OneWire.h> 
#include <LiquidCrystal_I2C.h>

int DS18S20_Pin = 10; //DS18S20 подключаем на 10 пин
OneWire ds(DS18S20_Pin);  // on digital pin 10
#define sensor A0 // Присваиваем имя “sensor” выврлу Arduino A0 
int tempPin = 0;
int pin = 7; // пин подсветки экрана
int pinin = 2;// пин кнопки

float Vin; //переменная измерения АЦП
float Volt; // переменная для вовода на LCD
int flag=0; // переменная для кнопки
long previousMillis = 0; // время для вычесления частоты вывода
int interval = 1500;  // частота вывода показаний на экран
long y = 0; //вермя горения экрана миллисикунд
long off = 60000; //вермя отключения экрана до 32 767

long x = 0;
long z = 0;
int pause = 1100 ;// пауза удержания клавиши для вкл экрана

float temperature = 0;


LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

byte man_with_hat[8] =
{
  B00011,
  B00011,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  };
// символ градуса 

byte m1[8] =
{
  B00100,
  B11111,
  B00001,
  B01001,
  B11101,
  B01001,
  B00001,
  B11111,
  };
  byte m2[8] =
{
  B00100,
  B11111,
  B10000,
  B10000,
  B10110,
  B10000,
  B10000,
  B11111,
  };
// символы АКБ
 
void setup(void) {

  pinMode(pin, OUTPUT);
  pinMode(pinin, INPUT);
  
  lcd.init();  // initialize the lcd 
   lcd.createChar(1, man_with_hat);
lcd.setCursor(3, 0);
lcd.print("\1");         //вывод символа градуса
lcd.setCursor(4, 0);
lcd.print("C");
                        //вывод символа АКБ
lcd.createChar(2, m1);
lcd.setCursor(13, 0);
lcd.print("\2");
lcd.createChar(3, m2);
lcd.setCursor(12, 0);
lcd.print("\3");
                  
lcd.setCursor(8, 0); 
lcd.write(124); // вертикальная полоса
    
lcd.setCursor(8, 1);
lcd.write(124); // вертикальная полоса
     
lcd.backlight();         //включение подсветки 
digitalWrite(pin,HIGH); //включение подсветки при старте 

}

void loop(void) {

 float temperature = getTemp();

//Вывод показаний на экран НАЧАЛО
 unsigned long currentMillis = millis();
 if(currentMillis - previousMillis > interval) {
      lcd.setCursor(2, 1); 
   lcd.print(temperature, 1);
   lcd.setCursor(11, 1);
   lcd.print(Volt, 1); 
   previousMillis = currentMillis;
       }
//Вывод показаний на экран КОНЕЦ




Vin = analogRead(sensor); /* Arduino измеряет напряжение на входе порта датчика и сохраняет его в переменной “Vin” */
 Volt = Vin/23.3 ;      // определение напряжения 

//Таймер отключения экрана НАЧАЛО
if(digitalRead(pin)==LOW)   
       { y = millis(); }
 if(millis() - y  >  off )   
       { digitalWrite(pin,LOW); }           
//Таймер отключения экрана КОНЕЦ


// Включение экрана с задержкой нажатия на клавиже НАЧАЛО
 if(digitalRead(pinin)==HIGH)   
       { x = millis(); }
        if(digitalRead(pinin)==LOW )   
      { z = millis(); }
      
if(digitalRead(pinin)==HIGH&&x - z > pause&&flag==0 )
{ digitalWrite(pin,!digitalRead(pin));
flag=1;
}
if(digitalRead(pinin)==LOW&&flag==1)
{  flag=0; }
// Выключение экрана с задержкой нажатия на клавиже НАЧАЛО

  
//Вывод на экран реакции нажатия кнопки НАЧАЛО
//if(digitalRead(pinin)==HIGH)   
 //      {           
 //        lcd.setCursor(0, 0);
 //        lcd.write(165);
 //              }
 //   if(digitalRead(pinin)==LOW)//если кнопка нажата     // и перемення flag равна 0 , то ...
 //      {           
 //        lcd.setCursor(0, 0);
//  lcd.write(32);
 //              }
//    //Вывод на экран реакции нажатия кнопки КОНЕЦ
  
 
 }


float getTemp(){
 
  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad

  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  
  ds.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
   return TemperatureSum;
    
}


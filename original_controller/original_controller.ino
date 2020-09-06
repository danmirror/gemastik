
#include<DHT.h> 

#include <Adafruit_Sensor.h> 
#define DHTPIN A0  // suhu    
#define DHTTYPE DHT11
 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); 

#include <Wire.h> 
#include <Servo.h>
Servo myServo ;  
DHT dht(DHTPIN, DHTTYPE);

int sensor_pin = A1;  // soil mois
int output_value ;
int analogInPin = A2 ;  // pH tanah
int sensor = 0;      
float output = 0.0; 
int relay = 12;
int humidity_1 = dht.readHumidity();
int celcius_1 = dht.readTemperature();
int fahrenheit_1 = dht.readTemperature(true);
int output_value2 = map(output_value,1023,0,0,100);
   

void setup() 
{ 
  uint32_t baud =115200;
  Serial.begin(baud);

  
  dht.begin(); 
  lcd.begin();
  lcd.backlight();
  pinMode (relay,OUTPUT);
  myServo.attach(4); 

} 
void loop(){Serial.println("output");

  delay(2000);  
  if (isnan(humidity_1) || isnan(celcius_1) || isnan(fahrenheit_1)) {
    Serial.println("Pembacaan data dari module sensor gagal!");
    return;
  }
//==========================================DHT11===================================  
  
  lcd.setCursor(0,0);
  lcd.print("HUMADITY:");
  lcd.setCursor(9,0);
  lcd.print(humidity_1);
  lcd.print("%");
  lcd.setCursor(13,0);
  
  
  
  lcd.setCursor(0,1);
  lcd.print("TEMPERATURE:");
  lcd.setCursor(12,1);
  lcd.print(celcius_1);
  lcd.setCursor(14,1);
  lcd.print("C");
//  delay(3000);
  lcd.clear();

  
  
  //Serial.println("");
  output_value= analogRead(sensor_pin);
//==================================SOIL_MOISTURE===================================    
  
 // Serial.println("");
//  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("S.MOISTURE:");
  lcd.setCursor(11,0);
  lcd.print(output_value2);
  lcd.setCursor(12,0);
  lcd.print("%");
//  delay(3000);
  lcd.clear(); 

  if (output_value2 <= 6 )
    {
     digitalWrite(relay,LOW);  // Hidup 15 detik
     lcd.setCursor(0,0);
     lcd.print("POMPA ON");
//     delay(15000);
     lcd.clear(); 
     }
  else { 
     digitalWrite(relay,HIGH) ;
     lcd.setCursor(0,0);
  }
//==========================================pH_Tanah=================================
  sensor = analogRead(analogInPin);
  output = (-0.0693*sensor)+7.3855;
  
//  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("pH SOIL:");
  lcd.setCursor(8,0);
  lcd.print(output);
//  delay(3000);
  lcd.clear(); 
//==========================================Wadah pupuk=================================   
  if (output >= 5.5 ) 
    {
     myServo.write(90);
//     delay (3000);
     lcd.setCursor(0,0);
     lcd.print("PUPUK");
     lcd.clear(); 
     }
  else { 
      myServo.write(11); 
      delay(2000);
  }
//  delay(1000); 

//  Serial.print("Kelembaban: ");
  Serial.print(humidity_1);
  Serial.print(",");
//  Serial.print("Suhu : "); 
  Serial.print(celcius_1);
  Serial.print(",");
//  Serial.print("ºCelcius ");
//  Serial.print(fahrenheit_1); 
//  Serial.print("ºFahrenheit\t");

//  Serial.print("Kelembaban Tanah : ");
  Serial.print(output_value2);
  Serial.print(",");
//  Serial.print("%");
//  Serial.print(" \t");
//  Serial.print("sensor ADC= ");
//  Serial.print(sensor);
//  Serial.print("  output Ph: ");
  Serial.println(output);
  
}


 

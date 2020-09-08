
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); 

//----------------DHT----------------
#include<DHT.h> 
#include <Adafruit_Sensor.h> 
#define DHTPIN A1  // suhu    
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);




#include <Servo.h>
Servo myServo ;  

int soil_pin = A0;  // soil mois
int soil_val ;

int analogInPin = A2 ;  // pH tanah
int ph_val = 0;      
float ph_out = 0.0; 
int relay = 3;


void setup() 
{ 
  uint32_t baud =115200;
 Serial.begin(baud);

  
  dht.begin(); 
  lcd.begin();
  lcd.backlight();
  pinMode (relay,OUTPUT);
  myServo.attach(4); 
  lcd.print("GEMASTIK");
  lcd.clear();
} 
void loop(){
  
//==========================================DHT11===================================  
  int humidity_1 = dht.readHumidity();
  int celcius_1 = dht.readTemperature();
  int fahrenheit_1 = dht.readTemperature(true);

  
 
//==================================SOIL_MOISTURE===================================  
  soil_val= analogRead(soil_pin);  
  int soil_val2 = map(soil_val,1023,0,0,100);
  
  if (soil_val <= 6 )
  {
    digitalWrite(relay,LOW);  // Hidup 15 detik
  }
  else { 
    digitalWrite(relay,HIGH) ;
  }
//==========================================pH_Tanah=================================
  ph_val = analogRead(analogInPin);
  ph_out = (-0.0693*ph_val)+7.3855;
//  Serial.print(ph_val);/
//==========================================Wadah pupuk=================================   
  if (ph_out >= 5.5 ) 
  {
     myServo.write(90);
  }
  else { 
      myServo.write(90); 
      //delay(2000);
  }


//------------------LCD -----------------------------
  lcd.setCursor(0,0);
  lcd.print("Hum :");
  //lcd.setCursor(4,0);
  lcd.print(humidity_1);
  lcd.print("%");
  lcd.setCursor(9,0);
  lcd.print("Tmp:");
  lcd.print(celcius_1);
  lcd.print("*");

  lcd.setCursor(0,1);
  lcd.print("Soil:");
  lcd.print(soil_val2);
  lcd.setCursor(9,1);
  lcd.print("Ph :");
  lcd.print(ph_out);


//-----------------send to raspberry------------------
  Serial.print(humidity_1);
  Serial.print(",");

  Serial.print(celcius_1);
  Serial.print(",");

  Serial.print(soil_val);
  Serial.print(",");

  Serial.println(ph_out);
  delay(50);
}


 

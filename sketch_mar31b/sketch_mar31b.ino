#include <Servo.h>
// include the library code:
#include <LiquidCrystal.h>
#define kpa2atm 0.00986923267


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// pin defs


// variables
int value;
float pkPa; // pressure in kPa
float pAtm; // pressure in Atm

unsigned long time;

int tempPin = A0;
int GasSensor = A1;
int PirSensorPIN = A2;
int pressurePin = A3;
int motionRead;
int gasRead;
int tempVal;
int pos=0;
int kg;
Servo myservo;

void setup() {
  // put your setup code here, to run once:

pinMode(2,OUTPUT);
pinMode(3,OUTPUT);
pinMode(4,OUTPUT);
pinMode(5,OUTPUT);
pinMode(6, OUTPUT);
pinMode(GasSensor,INPUT);
pinMode (PirSensorPIN,INPUT);
myservo.attach(13);
lcd.begin(16, 4);
// Print a message to the LCD.
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
motor();
motion();
Serial.println();
Serial.println();
Serial.println("___________________________________________________________________");

}

void motor(){

  gasRead = (digitalRead(GasSensor));
  //Serial.println(gasRead);
  tempVal=analogRead(tempPin);
  float mv=(tempVal/1024.0)*5000;
  float cel = mv/100;

 //float mv=(tempVal);
 // float cel =mv;
  float far=(cel*9)/5+32;
  Serial.print("Temperature:");
  Serial.print(cel);
  Serial.print("* celcius");
  Serial.println("");

  if (gasRead == HIGH){ 
  Serial.println(" CH4 Detected"); 
  digitalWrite(4,100);
  digitalWrite(5,0);
  digitalWrite(6,HIGH);
  lcd.setCursor(0,0);
  lcd.print(">200ppm CH4");  
  // print the number of seconds since reset: 
  lcd.setCursor(0,1);
  lcd.print("FAN2:ON Clean:M");
  Serial.println("CH4 gas detected::Fan 2 ON :: Cleaning the waste");
  delay(1000);
  }else if(gasRead == LOW)
  {
     digitalWrite(4,0);
     digitalWrite(5,0);
    }
  if (cel<8 ){
  Serial.println("_______ICE Temperature______");
  Serial.println("_______Heater is On______");
  digitalWrite(2,0);
  digitalWrite(3,0); 
  digitalWrite(4,0);
  digitalWrite(5,0);
  digitalWrite(6,HIGH);
  lcd.setCursor(0,0);
  lcd.print(cel);
  lcd.print("*Temp:Low");
  // print the number of seconds since reset: 
  lcd.setCursor(0,1);
  lcd.print("Heater ON");
  delay(500);
  }

if (cel<25 && cel>8 ){
  Serial.println("_______Temperature Normal______");
  digitalWrite(2,0);
  digitalWrite(3,0); 
  digitalWrite(4,0);
  digitalWrite(5,0);
  digitalWrite(6,LOW);
  lcd.setCursor(0,0);
  lcd.print("Temp:Normal");
  // print the number of seconds since reset: 
  lcd.setCursor(0,1);
  lcd.print(cel);

  delay(500);
  }
  if(cel>25 && cel <40){
    Serial.println("_______Temperature Higher______");
    digitalWrite(2,300);
    digitalWrite(3,0); 
    digitalWrite(6,LOW);
    lcd.setCursor(0,0);
    lcd.print(cel);
    lcd.print("*Temp:High");
  // print the number of seconds since reset: 
    lcd.setCursor(0,1);
    lcd.print("Single Fan ON");
    delay(500);  
    }
  else if (cel >40 ){ 
  Serial.println("_______Temperature So Hot______");
  Serial.println("FAN1::ON & FAN2:: ON");
  digitalWrite(2,300);
  digitalWrite(3,0);
  digitalWrite(4,100);
  digitalWrite(5,0);
  digitalWrite(6,LOW);
  lcd.setCursor(0,0);
  lcd.print("Temp:Hot:");
  lcd.print(cel);
  // print the number of seconds since reset: 
  lcd.setCursor(0,1);
  lcd.print("Double Fan On");
  delay(500);
  }
  
  delay(500);
  }


  void motion(){
    motionRead=(digitalRead(PirSensorPIN));
   //Serial.print(motionRead);
   
    if(motionRead == LOW){  
        for (pos =180; pos >= 0; pos -= 1) { 
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(0);                       // waits 15ms for the servo to reach the position
         }
    }

     else if(motionRead == HIGH){      
        Serial.println("Motion Detected:: Bucket Door Open" );
        lcd.clear();  
        lcd.setCursor(0,0);
        lcd.print("Motion Detected:");        
  // print the number of seconds since reset: 
        lcd.setCursor(0,1);
        lcd.print("Food Bucket on");       
        delay(1000);
        for (pos = 0; pos <= 180; pos += 1) {
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(0);                       // waits 15ms for the servo to reach the position
         }
        weight();

       
    }


  }

  void weight(){

     /* get the pressure */
  value = analogRead(pressurePin);
  pkPa = ((float)value/(float)1023+0.095)/0.009;
  kg=(pkPa*0.10);
  pkPa=value;
  pAtm = kpa2atm*pkPa; 
 
  /* send pressure to serial port */
  Serial.print("Given Food weight:");
  Serial.print(kg);
  Serial.println("Kg ");
  
    if(kg<12){
       lcd.setCursor(0,0);
       lcd.print("Less food");    
       Serial.println("Less food");    
  // print the number of seconds since reset: 
        lcd.setCursor(0,1);
        lcd.print("Adding Food....");
        Serial.println("Adding Food....");  
        }else if(kg==12){
          lcd.setCursor(0,0);
          lcd.print(kg); 
          lcd.print("Enough food");      
          Serial.println("Enough Food For cattles....");    
      // print the number of seconds since reset: 
 
          }
  delay(1000);
  
    }


//  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//    myservo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(0);                       // waits 15ms for the servo to reach the position
//  }

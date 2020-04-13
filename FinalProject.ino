/*
SENZORUL RFID:
https://www.optimusdigital.ro/ro/wireless-rfid/67-modul-cititor-rfid-mfrc522.html?search_query=Rc522&results=2

SENZORUL DE CULOARE:
GND - negru - GND
S0 - alb - pin 2
S1 - galben - pin 3
S2 - portocaliu - pin 6 
S3 - verde - pin 7
VCC - mov - 5V
OUT - albastru - pin 8

DISPLAY-UL:
GND - albastru - GND 
VCC - verde - 5V
SDA - galben - A4
SCL - portocaliu - A5

SERVOMOTORUL:
GND - maro
VCC - rosu
Semnal - galben - pin 1

STEPPERUL:
step - portocaliu - pin 5
direction - albastru - pin 4*/

/*1. Orange, 2. Pink, 3. Red, 4. Blue*/

#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define SdaPin 10 
#define RstPin 9
#define StepPin 7 
#define DirPin 8 
#define S0 2 
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6
#define servoPin 1

#define noSteps 200
#define delaySteps 10
#define noTests 50

byte myCards[]={0x6B, 0x5D, 0xBD, 0x2C, //- 1. Orange
                0x1B, 0x09, 0xC8, 0x2C, //- 2. Pink
                0xFE, 0x5B, 0xBC, 0x56, //- 3. Red
                0x14, 0x96, 0xA8, 0xF9}; //- 4. Blue

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SdaPin, RstPin);  
Servo servoMotor;

void setup() {
  Serial.begin(9600);
  
  //COLOR SENSOR
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  //LDC
  lcd.init();
  lcd.backlight();

  servoMotor.attach(servoPin);
  servoMotor.write(0);

  //RFID
  SPI.begin();      
  mfrc522.PCD_Init();   
  
  //STEPPER
  pinMode(StepPin, OUTPUT); //Step
  pinMode(DirPin, OUTPUT); //Direction
}

void loop() {
  servoMotor.write(0); delay(1000);
  servoMotor.write(90); delay (100);

  int color = testColor();
  if(color!=0){
    if(color == 1){ 
      lcd.clear();lcd.print("ORANGE"); goColor(1); //i=1
    }
    else if(color == 2){
      lcd.clear();lcd.print("PINK"); goColor(2);
    }
    else if(color == 3){
      lcd.clear();lcd.print("RED"); goColor(3);
    }
    else if(color == 4){ 
      lcd.clear();lcd.print("BLUE"); goColor(4);
    }
    
    delay(5000); // ca servo-ul sa duca bomboana la gaura
    servoMotor.write(180);
  }
  
  else  {
    lcd.clear();lcd.print("No idea, honey!");
  }
  
  
}

int testColor(){
  int contorOrange = 0, contorPink = 0, contorRed = 0, contorBlue = 0, i, contorIdk = 0;
  for(i = 0; i<noTests; i++){
    // RED
    digitalWrite(S2,LOW);
    digitalWrite(S3,LOW);
    redFrequency = pulseIn(sensorOut, LOW);
    delay(20);
    
    // GREEN
    digitalWrite(S2,HIGH);
    digitalWrite(S3,HIGH);
    greenFrequency = pulseIn(sensorOut, LOW);
    delay(20);
    
    // BLUE
    digitalWrite(S2,LOW);
    digitalWrite(S3,HIGH);
    blueFrequency = pulseIn(sensorOut, LOW);
    delay(20);

    if(redFrequency>=43 & redFrequency<=59 & greenFrequency>=70 & greenFrequency<=92 & blueFrequency>=69 & blueFrequency<=84){ 
      contorOrange++;
    }
    else if(redFrequency>=45 & redFrequency<=61 & greenFrequency>=71 & greenFrequency<=88 & blueFrequency>=54 & blueFrequency<=68){
     contorPink++;
    }
    else if(redFrequency>=49 & redFrequency<=63 & greenFrequency>=83 & greenFrequency<=100 & blueFrequency>=69 & blueFrequency<=86){ 
      contorRed++;
    }
    else if(redFrequency>=43 & redFrequency<=57 & greenFrequency>=57 & greenFrequency<=73 & blueFrequency>=43 & blueFrequency<=60){ 
      contorBlue++;
    }
    else  contorIdk ++;
  }
    int result = max(max(max(contorOrange, contorPink), contorRed), contorBlue);
    if (result == contorOrange) return 1;
    else if (result == contorPink) return 2;
    else if (result == contorRed) return 3;
    else if (result == contorBlue) return 4;
    else return 0;
}


void goColor(int i){
  digitalWrite(DirPin,LOW); //high merge in sens trigonometric si low in sens orar
  int i;
  for( i = 0; i < 5000; i++)
  {
    digitalWrite(StepPin,HIGH);
    delayMicroseconds(50);
    digitalWrite(StepPin,LOW);
    delayMicroseconds(50);
    if(readingCards(i) == i)  
        return;
      
  }
}

int readingCards(int i)
{
  if ( !mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  
  if ( !mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  int i, j, count = 0;
  for(j=0; j<4; j++) //uids contain 4 numbers
    {
      if(mfrc522.uid.uidByte[j] == myCards[i*4+j])
        count++;
    }
  if(count == 4)
      return i+1;
  return 0;
}

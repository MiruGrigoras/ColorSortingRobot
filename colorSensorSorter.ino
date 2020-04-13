/* SENZORUL DE CULOARE:
GND - negru - GND
S0 - alb - pin 4
S1 - galben - pin 5
S2 - portocaliu - pin 6
S3 - verde - pin 7
VCC - mov - 5V
OUT - albastru - pin 8

DISPLAY-UL:
GND - albastru - GND 
VCC - mov - 5V
SDA - gri - A4
SCL - alb - A5

SERVOMOTORUL:
GND - maro
VCC - rosu
Semnal - galben - pin 3

STEPPERUL:
step - rosu - pin 9
direction - negru - pin 10*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>


#define S0 4 
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8
#define servoPin 3
#define stepPin 9
#define dirPin 10
#define noSteps 200
#define delaySteps 10
#define noTests 50

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;
int red = 0;
int green = 0;
int blue = 0;

/*1. Orange, 2. Pink, 3. Red, 4. Blue*/

int currentPos = 1;
int nextPos = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servoMotor;

void setup() {
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
  Serial.begin(9600);

  //STEPPER
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);

  /*servoMotor.attach(servoPin);
  servoMotor.write(0);*/
   delay(1000);
}

void oneStepBack(){
  digitalWrite(dirPin, HIGH); //high merge in sens trigonometric si low in sens orar
    for(int x = 0; x < noSteps/4; x++) {
      digitalWrite(stepPin, HIGH); 
      delayMicroseconds(500); 
      digitalWrite(stepPin, LOW); 
      delayMicroseconds(500);
      delay(delaySteps);
    }
}

void oneStepForward(){
  digitalWrite(dirPin, LOW); //high merge in sens trigonometric si low in sens orar
    for(int x = 0; x < noSteps/4; x++) {
      digitalWrite(stepPin, HIGH); 
      delayMicroseconds(500); 
      digitalWrite(stepPin, LOW); 
      delayMicroseconds(500);
      delay(delaySteps);
    }
}

void twoStepsForward(){
  digitalWrite(dirPin, LOW); //high merge in sens trigonometric si low in sens orar
    for(int x = 0; x < noSteps/2; x++) {
      digitalWrite(stepPin, HIGH); 
      delayMicroseconds(500); 
      digitalWrite(stepPin, LOW); 
      delayMicroseconds(500);
      delay(delaySteps);
    }
}

void goOrange() {
  nextPos = 1;
  if(currentPos == 2)
    oneStepBack();
  else if (currentPos == 3)
          twoStepsForward();
       else if (currentPos == 4)
              oneStepForward();
}

void goPink() {
  nextPos = 2;
  if(currentPos == 3)
    oneStepBack();
  else if (currentPos == 4)
          twoStepsForward();
       else if (currentPos == 1)
              oneStepForward();
}

void goRed() {
  nextPos = 3;
  if(currentPos == 4)
    oneStepBack();
  else if (currentPos == 1)
          twoStepsForward();
       else if (currentPos == 2)
              oneStepForward();
}

void goBlue() {
  nextPos = 4;
  if(currentPos == 1)
    oneStepBack();
  else if (currentPos == 2)
          twoStepsForward();
       else if (currentPos == 3)
              oneStepForward();
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
    int result = max(max(max(max(contorOrange, contorPink), contorRed), contorBlue), contorIdk);
    if (result == contorOrange) return 1;
    else if (result == contorPink) return 2;
    else if (result == contorRed) return 3;
    else if (result == contorBlue) return 4;
    else return 5;
}

void loop() {
  //servoMotor.write(0); delay(1000);
  //servoMotor.write(90); delay (100);

  int color = testColor();

  if(color == 1){ 
    lcd.clear();lcd.print("ORANGE"); //goOrange();
  }
  else if(color == 2){
    lcd.clear();lcd.print("PINK"); //goPink();
  }
  else if(color == 3){
    lcd.clear();lcd.print("RED"); //goRed();
  }
  else if(color == 4){ 
    lcd.clear();lcd.print("BLUE"); //goBlue();
  }
  else  {
    lcd.clear();lcd.print("No idea, honey!");
  }

  currentPos = nextPos;
  delay(1000);
  lcd.clear();lcd.print("Gata");
  
  //servoMotor.write(180);
  delay(1000);
}

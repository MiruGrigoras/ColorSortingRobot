
#define noSteps 200
#define delaySteps 2

// defines pins numbers
const int stepPin = 5; //red
const int dirPin = 4; //black
//4- white for the third

int currentPos = 1, nextPos;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
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

void loop() {
  // Traseu: 1, 4, 3, 1, 2, 2, 1, 4
  goBlue(); delay(1000);
  currentPos = nextPos;
  goRed(); delay(1000);
  currentPos = nextPos;
  goOrange(); delay(1000);
  currentPos = nextPos;
  goPink(); delay(1000);
  currentPos = nextPos;
  goPink(); delay(1000);
  currentPos = nextPos;
  goOrange(); delay(1000);
  currentPos = nextPos;
  goBlue(); delay(1000);
  currentPos = nextPos;
  
  /*digitalWrite(dirPin, LOW); 

  for(int i = 0; i<4; i++){
    for(int x = 0; x < 50; x++) {
      digitalWrite(stepPin, HIGH); 
      delayMicroseconds(500); 
      digitalWrite(stepPin, LOW); 
      delayMicroseconds(500);
      delay(500 );
    }
    delay(2000);
  }*/

  
}

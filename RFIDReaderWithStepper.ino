//https://www.optimusdigital.ro/ro/wireless-rfid/67-modul-cititor-rfid-mfrc522.html?search_query=Rc522&results=2

#include <MFRC522.h>
#include <SPI.h>
#define SdaPin 10
#define RstPin 9
#define StepPin 5 //red
#define DirPin 4 //black

byte myCards[] = {0x6B, 0x5D, 0xBD, 0x2C, //RATT tata - 1. Orange
                  0x1B, 0x09, 0xC8, 0x2C, //RATT Flav - 2. Pink
                  0xFE, 0x5B, 0xBC, 0x56, //RATT Miru - 3. Red
                  0x14, 0x96, 0xA8, 0xF9
                 }; //UPT Miru - 4. Blue

MFRC522 mfrc522(SdaPin, RstPin);

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Approximate your card to the reader...");
  pinMode(StepPin, OUTPUT); //Step
  pinMode(DirPin, OUTPUT); //Direction

}
void loop() {
  digitalWrite(DirPin, LOW);
  int i;
  for ( i = 0; i < 5000; i++)
  {
    digitalWrite(StepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(StepPin, LOW);
    delayMicroseconds(500);
    if (readingCards() == 1)
    {
      return;
    }
  }
  delay(1000);
}

int readingCards()
{
  if ( !mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( !mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  int i, j;
  for (i = 0; i < 4; i++) //number of cups
  {
    int count = 0;
    for (j = 0; j < 4; j++) //uids contain 4 numbers
    {
      if (mfrc522.uid.uidByte[j] == myCards[i * 4 + j])
        count++;
    }
    if (count == 4)
    {
      Serial.print("Card number: ");
      Serial.println(i + 1);
      delay(5000);
      return 1;
    }
  }
  return 0;
}

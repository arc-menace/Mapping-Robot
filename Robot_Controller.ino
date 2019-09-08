#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#define CSN 9
#define CE 8
#define led 10
#define button 4

RF24 myRadio(CE, CSN);

const byte address[6] = "00001";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myRadio.begin();
  myRadio.openWritingPipe(address);
  myRadio.setPALevel(RF24_PA_LOW);
  myRadio.stopListening();
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*if (Serial.available() > 0){
    int command { Serial.read() };
    Serial.write(command);
    digitalWrite(led, HIGH);
    for(int i = 0; i < 10; i++){
      myRadio.write(&command, sizeof(command));
    }
    digitalWrite(led, LOW);
    delay(10);
  } 
  */
  
  int command = 1;
  bool button_state;
  button_state = digitalRead(button);
  if(button_state == HIGH){
    digitalWrite(led, HIGH);
    myRadio.write(&command, sizeof(command));
    digitalWrite(led, LOW);
    delay(10);
  }
  
}

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//Wireless Transciever
const int CE = 3;
const int CSN = 10;

//Ultrasonic Pins
const int trigPin = 5;    // Trigger
const int echoPin = 2;    // Echo
long duration, cm;

//Motor Driver Pins
// motor one

const int in1 = 6;
int in2 = 7;
// motor two

const int in3 = 8;
const int in4 = 9;

//LED Pins
const int led = 4;

//Declare Wireless Transciever
RF24 radio(CE, CSN);
const byte address[6] = "00001";

int ping();

void setup() {
  Serial.begin(9600);
  
  //Initiallize Pins:
  
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);

  //Setup Transciever
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
  //Calibrate Motors:
  //Check for obstacle (distance = d1)
  //Rotate for approximately 15/25 degrees
  //Move forward for 1 second
  //Check distance again and note displacement 

  //Set Motor Speed using PWM
  analogWrite(led, 255);
}

void loop() {
  bool obstacle = false;
  int command = 0;
  if (radio.available()) {
    analogWrite(led, 0);
    radio.read(&command, sizeof(command));
    Serial.println(command);
    analogWrite(led, 255);
      if(command == 1) { //Forward
      while(!obstacle){
        if(ping() < 20){
          obstacle = true;
          continue;
        }
        else{
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW); 
          delay(50);
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, LOW); 
        }
      }  
    }
    else if(command == 2){
      while(!obstacle){
        if(ping() < 20){
          obstacle = true;
          continue;
        }
        else{
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
          digitalWrite(in3, LOW);
          digitalWrite(in4, HIGH); 
          delay(50);
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, LOW); 
        }
      }  
    }
    else if(command == 3){
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH); 
      delay(50);
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW); 
    }
    else if(command == 4){
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW); 
      delay(50);
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW); 
    }
    else{ Serial.println("Nothing to see here..."); }
    obstacle = false;
  }
  else{ Serial.println("No Signal"); }
}

int ping(){
  //Ping the Surroundings
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  //Serial.println(cm);
  return cm;
}

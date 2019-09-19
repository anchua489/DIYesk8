//BOARD SCRIPT FINAL Austin Chua
//Establishes the skateboard end if the electric skateboard setup. Includes an LED power indicator along with a safety to gradually deccelerate the board
//if the connection with the remote is lost

#include <Servo.h>

Servo ESC; //Inclusion of Servo library to control speed of the brushless motor

//Setup block to establish baud rate, input output designations, and the pin to control motor control
void setup() {
  Serial.begin(38400);
  pinMode(7, INPUT);
  pinMode(13, OUTPUT);
  ESC.attach(9);
}

//Initialize speed read value and connection variables
int num = 0; //default the speed value to 0 at startup
int isconnected = false;

void loop() {
  digitalWrite(13,HIGH); //Power on LED
  isconnected = digitalRead(7); //Attain connection boolean value
  if(isconnected == 1){ 
    if(Serial.available() > 0){ //If a value is written by the remote, then read the value
      num = Serial.read();
    }
    Serial.println(num); //Serial print for debugging purposes
    ESC.write(num); //Finally write the written remote value to the Electronic Speed Controller
  }
  else{ //Else block to account for connection loss
    if(num > 0){ //check if the speed value is anything greater than 0, if so, then gradually deccelerate the speed to 0
      for(num; num > 0; --num){
        Serial.println(num);
        ESC.write(num);
        delay(10);
        
      }
    }
  }
  delay(100); //Include 100 ms delay to avoid overloading the processor
}

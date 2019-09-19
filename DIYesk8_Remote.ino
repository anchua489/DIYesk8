//REMOTE SCRIPT FINAL Austin Chua
//Establishes the script for the remote side of the electric skateboard setup. Includes safety precautions 
//to avoid sudden takeoffs if the potentiometer is not set to zero when a connection is made, and includes 
//an LED to give indication of the system status to the user


class LED //Class for implementation of LED functions
{
private:
  int pinNum;
  
public:
  LED(int pin) //Constructor that establishes pin connection
  {
    pinNum = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pinNum, LOW);
  }

  void turnON() //Powers on an LED
  {
    digitalWrite(pinNum, HIGH);
  }

  void turnOFF() //Powers off LED
  {
    digitalWrite(pinNum, LOW);
  }

  void blink1s()//LED 1 second interval blink
  {
    digitalWrite(pinNum, HIGH);
    delay(1000);
    digitalWrite(pinNum, LOW);
    delay(1000);
  }

  void rapidBlink()//LED rapid succession blinking
  {
    digitalWrite(pinNum, HIGH);
    delay(200);
    digitalWrite(pinNum, LOW);
    delay(200);
  }
};

class Potentiometer //Class to establish key potentiometer functions
{
private:
  int pinNum;

public:
  Potentiometer(int pin) //Constructor to establish pin connection
  {
    pinNum = pin;
  }

  int getval() //Attain a potentiometer read over an average of 6 reads
  {
    int avg = 0;
    for(int i = 0; i < 6; ++i){
      avg += analogRead(pinNum);
    }
    return avg/6;
  }
};

void setup() {
  Serial.begin(38400);//Begin serial connection at 38400 baud rate
}

//Following block initializes instances of the Potentiometer class and LED class, along with key variables
Potentiometer p(1);
LED blue(2);
int val;
int transnum;
bool ZeroOnTurnOn = false;
bool isconnected = false;

//loop function to control key function of the remote
void loop() {
  isconnected = digitalRead(3); //Attain whether or not the remote has established connection
  if(isconnected == 1){ //Check for established connection
    val = p.getval(); //Get Potentiometer read value
    if(Serial.available() > 0){  
      if(Serial.read() == 'O'){ 
        ZeroOnTurnOn = false; //Cease remote function if connection is lost
      }
    }
    if(val <= 10){ //Enable remote function if Potentiometer is dialed to zero to avoid accidental take off on an established connection
      ZeroOnTurnOn = true;
    }
    if(ZeroOnTurnOn == true){
      blue.turnON(); //Turn on a solid LED to indicate system readiness
      transnum = map(val, 0, 1023, 90 ,130); //map the potentiometer value to a value the Electronic Speed Controller can read
      Serial.write(transnum); //Write the value through serial bluetooth connection
      //Serial.println(transnum);
    }
    else if(ZeroOnTurnOn == false){
      blue.blink1s(); //if the remote is not dialed to zero at time of connection, indicate with a 1 second LED pulse
    }
  }
  else{ //if there is no extablished connection, indicate with a rapid pulsing LED and cease function of the remote until paired
    blue.rapidBlink();
    ZeroOnTurnOn = false;
  }
  delay(100); //Addition of a 100 ms delay to avoid overloading the processor
}

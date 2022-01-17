int sensePin1 = A0; //declares A0 as our sensor1 input
int sensePin2 = A1; //declares A1 as our sensor2 input
int Flash = 6; //control for our flasher board
#include<Servo.h>//adds servo control library
int flashcount;//count of flash cycles
int pos = 90; //start position for servos
Servo myservo;//names servo control

void setup() {
  Serial.begin(9600);//enables serial monitoring
  pinMode(Flash, OUTPUT);//establishing pin 6 as an output pin
  myservo.attach(9);//sets up pin 9 as our servo control
}

enum CROSSINGSTATES
{
  ST_OFF,
  ST_FLASHING1,
  ST_FLASHING2,
  ST_FLASHING3,
  ST_FLASHING4,
  ST_ON1,
  ST_ON2,
}; //Identifies our 7 crossing states

CROSSINGSTATES crossingState = ST_OFF;// default crossing state

void loop() {
  int valA1 = analogRead(sensePin1);//reads sensor 1
  int valA2 = analogRead(sensePin2);//reads sensor 2
  Serial.println(valA1);
  Serial.println(valA2);//prints values in serial monitor
  delay(200);

  switch (crossingState)
  {
    case ST_OFF:
      crossingoff(valA1, valA2);
      break;
    case ST_FLASHING1:
      crossingflashing1(valA1, valA2);
      break;
    case ST_FLASHING2:
      crossingflashing2(valA1, valA2);
      break;
    case ST_FLASHING3:
      crossingflashing3(valA1, valA2);
      break;
    case ST_FLASHING4:
      crossingflashing4(valA1, valA2);
      break;
    case ST_ON1:
      crossingon1(valA1, valA2);
      break;
    case ST_ON2:
      crossingon2(valA1, valA2);
      break;
  }
}

void crossingoff(int valA1, int valA2) {
  digitalWrite(Flash, LOW);//keeps power down on flasher board
  delay(200);
  flashcount = 0; //resets flashcounter
  if (valA1 > 200 && valA2 < 200) {
    crossingState = ST_FLASHING1;
  }//if sensePin1 is triggered
  if (valA1 < 200 && valA2 > 200) {
    crossingState = ST_FLASHING2;
  }//if sensePin2 is triggered
}

void crossingflashing1(int valA1, int valA2) {
  digitalWrite(Flash, HIGH);//turns on flasher board
  for (pos = 90; pos <= 180; pos += 1) //the servo goes from 90 to 180 degreesin steps of 1 degree
  {
    myservo.write(pos);//controls the servo
    delay(15);
    crossingState = ST_ON1;
  }
}

void crossingflashing2(int valA1, int valA2) {
  digitalWrite(Flash, HIGH);//turns on flasher board
  for (pos = 90; pos <= 180; pos += 1) //the servo goes from 90 to 180 degreesin steps of 1 degree
  {
    myservo.write(pos);//controls the servo
    delay(15);
    crossingState = ST_ON2;
  }
}

void crossingon1(int valA1, int valA2) {
  digitalWrite(Flash, HIGH); // keeps the flasher on
  delay(1000); //1 second delay
  flashcount++;//adds 1 to the flashcount
  if (valA1 < 200 && valA2 > 200) {
    flashcount = 0;
    crossingState = ST_FLASHING3; //switches to flashing 3 if the exit sensor is tripped
  }
  else if (valA1 > 200 && valA2 > 200 && flashcount > 5) {
    crossingState = ST_FLASHING4; //if the train is shorter than the distance between the crossing sensors or is sitting in between the sensors
  }
}

void crossingon2(int valA1, int valA2) {
  digitalWrite(Flash, HIGH); // keeps the flasher on
  delay(1000); //1 second delay
  flashcount++;//adds 1 to the flashcount
  if (valA1 > 200 && valA2 < 200) {
    flashcount = 0;
    crossingState = ST_FLASHING3; //switches to flashing 3 if the exit sensor is tripped
  }
  else if (valA1 > 200 && valA2 > 200 && flashcount > 5) {
    crossingState = ST_FLASHING4; //if the train is shorter than the distance between the crossing sensors or is sitting in between the sensors
  }
}

void crossingflashing3(int valA1, int valA2) {
  digitalWrite(Flash, HIGH); // keeps the flasher on
  delay(1000); //1 second delay
  flashcount++;//adds 1 to the flashcount
  if (valA1 > 200 && valA2 > 200 && flashcount > 5) {
    {
      for (pos = 180; pos >= 90; pos -= 1) // goes from 180 to 90 in increments of 1 degree
        myservo.write(pos);
      delay(15);
    }
    delay(1000);
    crossingState = ST_OFF;
  }
}

void crossingflashing4(int valA1, int valA2) {
  digitalWrite(Flash, HIGH); // keeps the flasher on
  delay(1000); //1 second delay
  flashcount++;//adds 1 to the flashcount
  if (valA1 < 200 || valA2 < 200) {
    flashcount = 0;
    crossingState = ST_FLASHING3;
  }
  else if (flashcount > 120) {
    {
    for (pos = 180; pos >= 90; pos -= 1) // goes from 180 to 90 in increments of 1 degree
      myservo.write(pos);
    delay(15);
  }
    crossingState = ST_OFF;
  }
}

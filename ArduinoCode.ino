#include<Servo.h>

// Wheels variables

# define enR 5 // Right motors Enable

# define enL 5 // Left motors Enable

# define IN1R 24 // IN1 of Right motors
# define IN2R 25 // IN2 of Right motors

# define IN1L 22 // IN1 of Left Motors
# define IN2L 23 // IN2 of Left Motors

// motor speeds
int RSpeed = 0;
int LSpeed = 0; 

int state;
int data;
// Collecting variables

Servo servo1; // right

Servo servo2; // left

int x = 170, y = 30; // initial servo positions

int servopin = 11; // servo pin in the arduino

// positive, negative and total ball counts
int pBc = 0;
int nBc = 0;
int tBc = 0;
//
int firstFlag = 1;
int wheelSpeed = 75;
void setup() {

//==================================================
  // initialize enable pins, and assign motor speeds
  pinMode(enR, OUTPUT);
  pinMode(enL, OUTPUT);

  digitalWrite(enR, RSpeed);
  digitalWrite(enL, LSpeed);
//===================================================

  pinMode(13, OUTPUT);

  pinMode(IN1R, OUTPUT);
  pinMode(IN2R, OUTPUT);


  pinMode(IN1L, OUTPUT);
  pinMode(IN2L, OUTPUT);


  servo1.attach(11);
  servo2.attach(11);


  servo1.write(50);
  servo2.write(50);

}

void rotateLeft(){
  // move the right wheels forward
  digitalWrite(IN1R, HIGH);
  digitalWrite(IN2R, LOW); 
  // move the left wheels backward (or stop them if this doesn't work)
  digitalWrite(IN1L, LOW);
  digitalWrite(IN2L,  HIGH);
}

void rotateRight(){
  // move the left wheels forward
  digitalWrite(IN1L, HIGH);
  digitalWrite(IN2L, LOW);
  // move the right wheels backward (or stop them if this doesn't work)
  digitalWrite(IN1R, LOW);
  digitalWrite(IN2R, HIGH); 
}

void moveForward(){
  // move the right wheels forward
  digitalWrite(IN1R, HIGH);
  digitalWrite(IN2R, LOW);

  // move the left wheels forward
  digitalWrite(IN1L, HIGH);
  digitalWrite(IN2L, LOW);
}

void moveBackward(){
  // move the right wheels backward
  digitalWrite(IN1R, LOW);
  digitalWrite(IN2R, HIGH);

  // move the right wheels backward
  digitalWrite(IN1L, LOW);
  digitalWrite(IN2L, HIGH);
}

void stopMotion()
{
  // STOP the right wheels 
  digitalWrite(IN1R, LOW);
  digitalWrite(IN2R, LOW);

  // STOP the left wheels 
  digitalWrite(IN1L, LOW);
  digitalWrite(IN2L, LOW);  
}

void moveMotors(int direction, int speed = 0) {
  if (direction == 1)  // forward
  {
    moveForward();
    analogWrite(enR, speed);
    analogWrite(enL, speed);
    // delay(500);
  }
  else if (direction == 2)  // left
  {
    rotateLeft();
    analogWrite(enR, speed);
    analogWrite(enL, speed);
  }
  else if (direction == 3)  //backward
  {
    moveBackward();
    analogWrite(enR, speed);
    analogWrite(enL, speed);
  }
  else if (direction = 4) // right
  {
    rotateRight();
    analogWrite(enR, speed);
    analogWrite(enL, speed);
  }
  else{
    stopMotion();
    analogWrite(enR, 0);
    analogWrite(enL, 0);
  }
}

void moveServos(int x, int y){
  servo1.write(x);
  servo2.write(y);
}
void collect(){
    for(int i = 0; i < 60; i+=5){
    moveServos(x-i, y+i);
  }
  delay(1000);
  for(int i = 0; i < 60; i+=5){
    moveServos(x+i, y-i);
  }
  delay(1000);
}
void collect2(){
  servo1.write(137);
  servo2.write(137);
  delay(500);
  moveMotors(1, wheelSpeed);
  delay(500);
  moveMotors(5, 0);
  servo1.write(50);
  servo2.write(50);
  delay(1000);
}
void rot(){
  //RIGHT MOTORS
   digitalWrite(24 , HIGH);
    digitalWrite(25 , LOW);
    analogWrite(5 , 255);
    delay(500);

    digitalWrite(24 , LOW);
    digitalWrite(25 , LOW);
    analogWrite(5 , 0);
    delay(500);

    //LEFT MOTORS
    digitalWrite(22 , LOW);
    digitalWrite(23 , HIGH);
    analogWrite(enR , 255);
    delay(500);

    digitalWrite(22 , LOW);
    digitalWrite(23 , LOW);
    analogWrite(enL , 0);
    delay(500);
}
void loop(){
  Serial.begin(9600);
  if(firstFlag) { // 5 sec delay until python code starts running
    firstFlag = 0;
    //digitalWrite(13, LOW);
    delay(5000);
    moveMotors(1, wheelSpeed); // move the motors forward
  }
  while(Serial.available() == 0){continue;}
  state = Serial.parseInt();
  if(state){  // positive ball
  delay(100);
    moveMotors(5, 0); // stop the robot
    delay(300);
    collect2();
    pBc++;
    tBc++;
  }else if(state == 0){ // negative ball
    nBc++;
    tBc++;
    delay(500);
  }
  if(tBc == 5 || tBc == 9 || tBc == 13 || tBc == 16 || tBc == 19 || tBc == 21 || tBc == 23 || tBc == 24){
    moveMotors(5, 0);
    delay(500);
    }
  moveMotors(1, wheelSpeed); // set the motor to start moving forward again (if it stopped moving in the first place)
  Serial.end();
}
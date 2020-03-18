#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <Servo.h>        //Servo Motor library            
#include <NewPing.h>      //Ultrasonic sensor library
#include<MFRC522.h>       // MFRC522 library
#include<SPI.h>           // MFRC522 Supportuing library

#define SS_PIN 53         // RFID Pin
#define RST_PIN 5         //RFID Pin
#define TRIG_PIN A0 // Pin A0 on the Motor Drive Shield soldered to the ultrasonic sensor
#define ECHO_PIN A1 // Pin A1 on the Motor Drive Shield soldered to the ultrasonic sensor
#define MAX_DISTANCE 300 // sets maximum useable sensor measuring distance to 300cm
#define MAX_SPEED 160 // sets speed of DC traction motors to 150/250 or about 70% of full speed - to get power drain down.
#define COLL_DIST 10 // sets distance at which robot stops and reverses to 30cm
#define TURN_DIST COLL_DIST+20 // sets distance at which robot veers away from object
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // sets up sensor library to use the correct pins to measure distance.


/*
PIN DIAGRAM
RST-5
SS-53
MOSI-51
MISO-50
SCK-52
3.3V-3.3V
GND-GND

{2,3,4,6=>Motor Running},{9,10 => Servo Motor Movement }, {5,53,51,50,52,3.3,GND => RFID Pins} 
*/

void turnLeft();
void turnRight();    //Function Call definations
void turnAround();
void rfid();

const int lf = 2;
const int lb = 3;
const int rf = 4;
const int rb = 13;
                    //Pin attachments
const int lf1 = 7;
const int lb1 = 8;
const int rf1 = 9;
const int rb1 = 10;

int count = 0;

Servo pan;  // create servo object to control a servo 
MFRC522 mfrc522(SS_PIN, RST_PIN);

int leftDistance, rightDistance; //distances on either side
int curDist = 0;

//-------------------------------------------- SETUP LOOP ----------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init(); //Initialize MFRC522 library
  pan.attach(12);  // attaches the servo on pin 10 (SERVO_1 on the Motor Drive Shield to the servo object 
  pan.write(90); // tells the servo to position at 90-degrees ie. facing forward.
  delay(1000); // delay for one seconds
 }
//------------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------MAIN LOOP ------------------------------------------------------------------------------
void loop() {
  pan.write(90);  // move servo eyes forward
  delay(90);
  curDist = readPing();   // read distance
  if (curDist < COLL_DIST) {changePath();}  // if forward is blocked change direction
  moveForward();  // move forward
  delay(500);
 }
//-------------------------------------------------------------------------------------------------------------------------------------

void changePath() {
  moveStop();   // stop forward movement
  delay(2000);
  while(count<5)
  {
    rfid();
    count = count+1;
  }
  count = 0;
  pan.write(36);  // check distance to the right
    delay(500);
    rightDistance = readPing(); //set right distance
    delay(500);
    pan.write(144);  // check distace to the left
    delay(700);
    leftDistance = readPing(); //set left distance
    delay(500);
    pan.write(90); //return to center
    delay(100);
    compareDistance();
  }

  
void compareDistance()   // find the longest distance
{
  if (leftDistance>rightDistance) //if left is less obstructed 
  {
    turnLeft();
  }
  else if (rightDistance>leftDistance) //if right is less obstructed
  {
    turnRight();
  }
   else //if they are equally obstructed
  {
    turnAround();
  }
}


//-------------------------------------------------------------------------------------------------------------------------------------

int readPing() { // read the ultrasonic sensor distance
  delay(70);   
  unsigned int uS = sonar.ping();
  int cm = uS/US_ROUNDTRIP_CM;
  return cm;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void moveStop() {
analogWrite(lf, 0);
analogWrite(lb, 0);
analogWrite(rf, 0);
analogWrite(rb, 0);
analogWrite(lf1, 0);
analogWrite(lb1, 0);
analogWrite(rf1, 0);
analogWrite(rb1, 0);
}  // stop the motors.
//-------------------------------------------------------------------------------------------------------------------------------------
void moveForward() {

analogWrite(lf, MAX_SPEED);
analogWrite(lb, 0);
analogWrite(rf, MAX_SPEED);
analogWrite(rb, 0);
analogWrite(lf1, MAX_SPEED);
analogWrite(lb1, 0);
analogWrite(rf1, MAX_SPEED);
analogWrite(rb1, 0);
  }
//-------------------------------------------------------------------------------------------------------------------------------------
void moveBackward() {
  
analogWrite(lf, 0);
analogWrite(lb, MAX_SPEED);
analogWrite(rf, 0);
analogWrite(rb, MAX_SPEED);
analogWrite(lf1, 0);
analogWrite(lb1, MAX_SPEED);
analogWrite(rf1, 0);
analogWrite(rb1, MAX_SPEED);
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void turnRight() {
  
analogWrite(lf, MAX_SPEED);
analogWrite(lb, 0);
analogWrite(rf, 0);
analogWrite(rb, MAX_SPEED);
analogWrite(lf1, MAX_SPEED);
analogWrite(lb1, 0);
analogWrite(rf1, 0);
analogWrite(rb1, MAX_SPEED);
delay(1500);
moveForward();
        
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void turnLeft() {
analogWrite(lf, 0);
analogWrite(lb, MAX_SPEED);
analogWrite(rf, MAX_SPEED);
analogWrite(rb, 0);
analogWrite(lf1, 0);
analogWrite(lb1, MAX_SPEED);
analogWrite(rf1, MAX_SPEED);
analogWrite(rb1, 0);
delay(1500);
moveForward();
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void turnAround() {
analogWrite(lf, MAX_SPEED);
analogWrite(lb, 0);
analogWrite(rf, 0);
analogWrite(rb, MAX_SPEED);
analogWrite(lf1, MAX_SPEED);
analogWrite(lb1, 0);
analogWrite(rf1, 0);
analogWrite(rb1, MAX_SPEED);
delay(1700);
moveForward();
        
}

void rfid()
{
  delay(1000);
  if ( ! mfrc522.PICC_IsNewCardPresent()) //Check if new card present 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) //If new card present read the uid
  {
    return;
  }
  for (int i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i],HEX);  //Send the UID to Pi via serial communication
     count = 10;
  }
  Serial.println(); 
  while(Serial.available()==0){}  //Wait for face recognition to complete if RFID is successful
  count=Serial.parseInt();
  delay(1000);
}


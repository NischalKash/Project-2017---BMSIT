#include<Servo.h> //Arduino Nano + Pan & Tilt Assembly

Servo pan,tilt; //Basic Servo object declaration

int pan_value=0,tilt_value=0; // Read the serial character either it can be 0 or 1 , pan & tile respectively

int pan_position=90,tilt_position=60; // Initialize pan and tilt positions



void setup() {
  // put your setup code here, to run once:
  pan.attach(9); // Attach Pan Servo to pin 9
  tilt.attach(10); // Attach Tilt Servo to pin 10 
  pan.write(pan_position);  // Let the initial Pan Servo position be 90 degree
  tilt.write(tilt_position); // Let the initial Tilt Servo position be 60 degree
  Serial.begin(9600); // Begin Serial Communication with that port or baudrate 
}

void loop() {
  if(Serial.available()) //Wait for Serial input
  {
    pan_value=Serial.parseInt(); //Recieve the Pan degree information
    tilt_value=Serial.parseInt(); //Recieve the Tilt degree information
    pan.write(pan_value);         //Rotate the pan 
    tilt.write(tilt_value);       //Rotate the tilt
  }

//TODO:Check whether this code is needed and make alterations necessary
/*  
  //Find out if the Y component of the face is below the middle of the screen.
    if(midFaceY < (midScreenY - midScreenWindow)){
      if(servoTiltPosition >= 5)servoTiltPosition -= stepSize; //If it is below the middle of the screen, update the tilt position variable to lower the tilt servo.
    }
    //Find out if the Y component of the face is above the middle of the screen.
    else if(midFaceY > (midScreenY + midScreenWindow)){
      if(servoTiltPosition <= 175)servoTiltPosition +=stepSize; //Update the tilt position variable to raise the tilt servo.
    }
    //Find out if the X component of the face is to the left of the middle of the screen.
    if(midFaceX < (midScreenX - midScreenWindow)){
      if(servoPanPosition >= 5)servoPanPosition -= stepSize; //Update the pan position variable to move the servo to the left.
    }
    //Find out if the X component of the face is to the right of the middle of the screen.
    else if(midFaceX > midScreenX + midScreenWindow){
      if(servoPanPosition <= 175)servoPanPosition +=stepSize; //Update the pan position variable to move the servo to the right.
    }
*/
  delay(1000);
}

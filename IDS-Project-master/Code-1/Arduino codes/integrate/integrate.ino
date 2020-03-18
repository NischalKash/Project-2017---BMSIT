#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Servo.h>
#include<SPI.h>
#include<MFRC522.h>
#define INPUT_SIZE 30

#define SS_PIN 53
#define RST_PIN 5 

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

MFRC522 mfrc522(SS_PIN, RST_PIN);

Servo pan,tilt;

const int lf = 2;
const int lb = 3;
const int rf = 4;
const int rb = 6; //Changed from 5-6

void motors(int inputLF,int inputLB,int inputRF,int inputRB);
void servo(int pan_value, int tilt_value);
void rfid();

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pan.attach(9); 
  tilt.attach(10); 
  pan.write(90);
  tilt.write(60);
  delay(500);
  pan.detach();
  tilt.detach(); 
}

void loop()
{
  char *token;
  int count = 0;
  int inputs[4];
  char input[INPUT_SIZE + 1];
  byte size = Serial.readBytes(input, INPUT_SIZE);
  input[size] = 0;
  token = strtok(input, ",");
  rfid();
  while (token != NULL) 
  {
    inputs[count] = atoi(token);
    count++;
    token = strtok(NULL, ",");
  }
  if (count == 4)
  {
  	motors(inputs[0],inputs[1],inputs[2],inputs[3]);
  	delay(500);
  }
  else if (count == 2)
  {
    servo(inputs[0],inputs[1]);
    delay(500);
  }
  delay(500);
}

void motors(int inputLF,int inputLB,int inputRF,int inputRB)
{  
  analogWrite(lf, inputLF);
  analogWrite(lb, inputLB);
  analogWrite(rf, inputRF);
  analogWrite(rb, inputRB);
}

void servo(int pan_value, int tilt_value)
{
  pan.attach(9); 
  tilt.attach(10); 
  pan.write(90);
  tilt.write(60);
	pan.write(pan_value);         
  tilt.write(tilt_value); 
  delay(500);
  pan.detach();
  tilt.detach(); 
}
void rfid()
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  for (int i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i],HEX);
  }
  Serial.println();
  delay(500);
}


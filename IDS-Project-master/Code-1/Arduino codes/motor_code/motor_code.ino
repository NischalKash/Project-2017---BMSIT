const int lf = 5;
const int lb = 3;
const int rf = 9;
const int rb = 6;

int inputLF;
int inputLB;
int inputRF;
int inputRB;

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(10);

  pinMode(lf , OUTPUT);
  pinMode(lb , OUTPUT);
  pinMode(rf , OUTPUT);
  pinMode(rb , OUTPUT);
}

void loop()
{
  if (Serial.available()) {
    inputLF = Serial.parseInt();
    inputLB = Serial.parseInt();
    inputRF = Serial.parseInt();
    inputRB = Serial.parseInt();

    driveMotors(inputLF, inputLB, inputRF, inputRB);
  }

  delay(1000);
}

void driveMotors(int inputLF, int inputLB, int inputRF, int inputRB) {
  analogWrite(lf, inputLF);
  analogWrite(lb, inputLB);
  analogWrite(rf, inputRF);
  analogWrite(rb, inputRB);

  Serial.print("Driving motors ");
  Serial.print(inputLF);
  Serial.print(" ");
  Serial.print(inputLB);
  Serial.print(" ");
  Serial.print(inputRF);
  Serial.print(" ");
  Serial.print(inputRB);
  Serial.println("");
} 

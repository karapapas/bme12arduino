// #include <BigNumber.h>

// Constants
const int d01 = 0;
const int d02 = 2;  //LM35 button
const int d03 = 3;
const int d04 = 4;  //KY-039 button
const int d05 = 5;
const int d06 = 6;
const int d07 = 7;  //KY-039 LED
const int d08 = 8;  //LM35 LED

const int a00 = A0;
const int a01 = A1;
const int a02 = A2; //LM35
const int a03 = A3;
const int a04 = A4; //KY-039
const int a05 = A5;

// Variables
int a02Raw = 0;
int a04Raw = 0;
double temperature;  // Variable to store the temperature value in Celsius
double areaundercurve;
int lm35Button;
int ky039Button;
int tempCounter = 0;
int irCounter = 0;

void setup() {
  pinMode(d02, INPUT);
  pinMode(d04, INPUT);
  pinMode(d07, OUTPUT);
  pinMode(d08, OUTPUT);
  Serial.begin(9600);       // Initialize serial communication for debugging
}

void loop() {

  lm35Button = digitalRead(d02);
  ky039Button = digitalRead(d04);
  Serial.print("lm35Button: ");
  Serial.println(lm35Button);
  
  if(lm35Button == 1){
    digitalWrite(d08, HIGH);
    tempCounter++;
  } else {
    digitalWrite(d08, LOW);
    tempCounter = 0;
  }

  if(ky039Button == 1){
    digitalWrite(d07, HIGH);
    irCounter++;
  } else {
    digitalWrite(d07, LOW);
    irCounter = 0;
  }

  a02Raw = analogRead(a02);           //read the analog sensor and store it
  temperature = (double)a02Raw / 1024;  //find percentage of input reading
  temperature = temperature * 5;            //multiply by 5V to get voltage
  temperature = (temperature - 0.5);        //Subtract the offset
  temperature = temperature * 100;          //Convert to degrees
  Serial.print("LM35 raw value: ");
  Serial.print(a02Raw);
  Serial.print(" volt. Current Temperature: ");
  Serial.println(temperature);

  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  a04Raw = analogRead(a04);
  float s4Voltage = a04Raw * (5.0 / 1024.0);
  Serial.print("IR LED raw value: ");
  Serial.print(a04Raw);
  Serial.print(" Voltage: ");
  Serial.println(s4Voltage);

  delay(1000);
}

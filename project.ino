// Constants
const int ledPin = 13;  // Digital pin connected to the LED
const int irLedPin = 14;
const int s1pin = A1;   // BPV22NF. Phototransistor. Raman Spectroscopy.
const int s2pin = A2;   // TMP36. Temperature.
const int s3pin = A3;   // BPW34. Photodiode. Optical Coherence tomography.
const int s4pin = A4;

// Variables
int s1RawValue = 0;       // Variable to store the sensor reading
int s2RawValue = 0;
int s3RawValue = 0;
int s4RawValue = 0;
float temperature = 0.0;  // Variable to store the temperature value in Celsius

void setup() {
  pinMode(ledPin, OUTPUT);  // Set the LED pin as an output
  Serial.begin(9600);       // Initialize serial communication for debugging
  digitalWrite(irLedPin, HIGH);  // turn the LED on (HIGH is the voltage level)

}

void loop() {

  // s1RawValue = analogRead(s1pin);
  // // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  // float s1Voltage = s1RawValue * (5.0 / 1024.0);
  // Serial.print("BPV22NF raw value: ");
  // Serial.print(s1RawValue);
  // Serial.print(" Voltage: ");
  // Serial.println(s1Voltage);

  // s1RawValue = analogRead(s2pin);           //read the analog sensor and store it
  // temperature = (double)s1RawValue / 1024;  //find percentage of input reading
  // temperature = temperature * 5;            //multiply by 5V to get voltage
  // temperature = (temperature - 0.5);        //Subtract the offset
  // temperature = temperature * 100;          //Convert to degrees
  // Serial.print("TMP36 raw value: ");
  // Serial.print(s1RawValue);
  // Serial.print(" volt. Current Temperature: ");
  // Serial.println(temperature);

  // s3RawValue = analogRead(s3pin);
  // // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  // float s3Voltage = s3RawValue * (5.0 / 1024.0);
  // Serial.print("BPW34 raw value: ");
  // Serial.print(s3RawValue);
  // Serial.print(" Voltage: ");
  // Serial.println(s3Voltage);


  s4RawValue = analogRead(s4pin);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float s4Voltage = s4RawValue * (5.0 / 1024.0);
  Serial.print("IR LED raw value: ");
  Serial.print(s4RawValue);
  Serial.print(" Voltage: ");
  Serial.println(s4Voltage);

  digitalWrite(ledPin, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(ledPin, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);
}

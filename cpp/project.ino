#include "LogisticRegression.h" // Include the header file for the refactored class. The file name may be different in your case

// LCD pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

Eloquent::ML::Port::LogisticRegression predictor(rs, en, d4, d5, d6, d7);

void setup() {
  // No setup needed for this example
}

void loop() {
  // Call the readAndPredict method to read sensor data and display the prediction on the LCD
  predictor.readAndPredict();
  
  // Delay for a second before the next prediction
  delay(1000);
}

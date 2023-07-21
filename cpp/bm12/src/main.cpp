
#include "Arduino.h"
#include <Wire.h>                   // needed for the lcd display
#include <LiquidCrystal.h>          // needed for the lcd display
#include "MAX30100_PulseOximeter.h" // needed for the heartrate sensor

#define DISPLAYING_PERIOD_MS 1000
uint32_t tsLastDisplay = 0;

// init display
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// init heartrate sensor
PulseOximeter pox;

// init temperature sensor
int LM35 = A1;
float C;
float filteredTemperature = 0.0;

// init Kalman filter variables
float Q = 0.02; // Process noise covariance
float R = 0.1;  // Measurement noise covariance
float X = 0;    // Estimated value
float P = 1;    // Estimation error covariance
float K;        // Kalman gain

// compute dot product
float dotproduct(float *x, float *coeffs, uint16_t size)
{
    float product = 0.0;
    for (uint16_t i = 0; i < size; i++)
    {
        product += x[i] * coeffs[i];
    }
    return product;
}

// predict the class for the given features vector
int predict(float *x)
{
    float votes[2] = {0.591330903038};
    float coeffs[2] = {13.990523280129, 6.103957240713};
    votes[0] += dotproduct(x, coeffs, 2);
    // return argmax of votes
    uint8_t classIdx = 0;
    float maxVotes = votes[0];
    for (uint8_t i = 1; i < 2; i++)
    {
        if (votes[i] > maxVotes)
        {
            classIdx = i;
            maxVotes = votes[i];
        }
    }
    return classIdx;
}

void displayResults()
{
    if (millis() - tsLastDisplay > DISPLAYING_PERIOD_MS)
    {
        lcd.clear();

        // display heartrate
        float heartRate = pox.getHeartRate();
        lcd.setCursor(0, 0);
        lcd.print("H:");
        lcd.print(heartRate);

        // display temperature
        lcd.setCursor(8, 0);
        lcd.print("T:");
        lcd.print(filteredTemperature);

        // call predict and display the estimated glucose class
        float x[2] = {filteredTemperature, heartRate};
        int prediction = predict(x);
        lcd.setCursor(0, 2);
        lcd.print("G:");
        lcd.print((prediction == 0) ? "Under" : "Over");
        lcd.print(" 114mg/dL");

        tsLastDisplay = millis();
    }
}

void recordTemperature()
{
    // Read the temperature sensor
    int rawReading = analogRead(LM35);
    // Convert the reading to voltage
    float voltage = rawReading * (3.3 / 4096.0);
    // Convert the voltage to temperature
    float C = voltage * 100.0;

    // Kalman filter
    K = P / (P + R);
    X = X + K * (C - X);
    P = (1 - K) * P;

    // Store filtered temperature
    filteredTemperature = X;
}

void setup()
{
    // init display
    lcd.begin(16, 2);

    // init heartrate sensor
    if (!pox.begin())
    {
        Serial.println("HR sensor FAIL");
        for (;;)
            ;
    }
    else
    {
        Serial.println("SUCCESS");
    }
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}

void loop()
{
    pox.update();
    recordTemperature();
    displayResults();
}

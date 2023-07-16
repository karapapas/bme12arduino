#pragma once
#include <Wire.h>
#include <LM35.h>
#include <MAX30100_PulseOximeter.h>
#include <cstdarg>
#include <LiquidCrystal.h>

#define REPORTING_PERIOD_MS     1000

namespace Eloquent {
    namespace ML {
        namespace Port {
            class LogisticRegression {
                public:
                    LogisticRegression(int rs, int en, int d4, int d5, int d6, int d7) :
                        lcd(rs, en, d4, d5, d6, d7) {
                            lcd.begin(16, 2);
                        }

                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        float votes[2] = { 0.591330903038 };
                        votes[0] += dot(x,   13.990523280129, 6.103957240713);
                        // return argmax of votes
                        uint8_t classIdx = 0;
                        float maxVotes = votes[0];

                        for (uint8_t i = 1; i < 2; i++) {
                            if (votes[i] > maxVotes) {
                                classIdx = i;
                                maxVotes = votes[i];
                            }
                        }

                        return classIdx;
                    }

                protected:
                    /**
                    * Compute dot product
                    */
                    float dot(float *x, ...) {
                        va_list w;
                        va_start(w, 2);
                        float dot = 0.0;

                        for (uint16_t i = 0; i < 2; i++) {
                            const float wi = va_arg(w, double);
                            dot += x[i] * wi;
                        }

                        va_end(w);

                        return dot;
                    }

                public:
                    /**
                    * Read data from sensors and perform prediction
                    */
                    void readAndPredict() {
                        float temperature = readLM35();
                        float heartRate = readMAX30100();
                        float x[2] = {temperature, heartRate};

                        int prediction = predict(x);

                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print("Estimated glucose is");

                        lcd.setCursor(0, 1);
                        if (prediction == 0) {
                            lcd.print("under 114 mg/dL");
                        }
                        else {
                            lcd.print("over 114 mg/dL");
                        }
                    }

                protected:
                    /**
                    * Read temperature from LM35 sensor
                    */
                    float readLM35() {
                        LM35 temp(A0);  // Connect LM35 on Analog pin 0
                        float temperature = temp.cel();
                        return temperature;
                    }

                    /**
                    * Read heart rate from MAX30100 sensor
                    */
                    float readMAX30100() {
                        PulseOximeter pox;

                        // Initialize the PulseOximeter instance
                        if (!pox.begin()) {
                            Serial.println("FAILED");
                            return 0;
                        }

                        uint32_t tsLastReport = 0;
                        while (millis() - tsLastReport < REPORTING_PERIOD_MS) {
                            pox.update();
                        }

                        float heartRate = pox.getHeartRate();
                        return heartRate;
                    }

                private:
                    LiquidCrystal lcd;
                };
            }
        }
    }
}

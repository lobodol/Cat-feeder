/**
 * Cat-feeder V2
 *
 * @author lobodol <grobodol@gmail.com>
 */

#include <Servo.h>

#define STOP     90
#define BACKWARD 0
#define FORWARD  180

Servo motor;
int           servo_pin       = 5;
int           push_button_pin = 2;
unsigned long cycle_duration  = 4 * 60 *60 * 1000; // Cycle duration in ms (4h).
unsigned long timer           = 0;

/**
 * Setup routine
 */
void setup() {
    pinMode(push_button_pin, INPUT);
    motor.attach(servo_pin);
    attachInterrupt(digitalPinToInterrupt(push_button_pin), manualDistribution, LOW);

    // Make sure servo is stopped
    motor.write(STOP);
}

/**
 * Main loop
 */
void loop() {
    timer = millis();

    if (timer >= cycle_duration) {
        // Reset timer
        timer = 0;

        runDistribution();
    }

}

/**
 * Makes servo turn for 8.4s. To prevent  pet-food to block the screw,
 * make the servo run counterclockwise every 2s for 0.6s.
 */
void runDistribution() {
    for (int i = 0; i < 3; i++) {
        // Each loop takes 2.8s
        motor.write(FORWARD);
        delay(2000);

        // Stopping servo is necessary before changing rotation way. Otherwise it simply stops turning.
        motor.write(STOP);
        delay(100);

        motor.write(BACKWARD);
        delay(600);

        // Stopping servo is necessary before changing rotation way. Otherwise it simply stops turning.
        motor.write(STOP);
        delay(100);
    }
}

/**
 * Makes turn the servo until push button is released.
 * Used for manual distribution.
 */
void manualDistribution() {
    // If push button is released, stop the servo
    if (digitalRead(push_button_pin, HIGH)) {
        motor.write(STOP);
    } else {
        runDistribution();
    }
}
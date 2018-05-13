/**
 * Pet feeder V2. This Arduino sketch is provided "as is" without any guaranty.
 * Feel free to edit according to your needs.
 *
 * @author lobodol <grobodol@gmail.com>
 * @licence MIT
 */

#include <Servo.h>
//----------------------------------------------------------------------------------------------------------------------
#define FORWARD       180
#define STOP          90
#define BACKWARD      0
#define FEED_DURATION 7000 // Duration of feeding in ms
#define FEED_CYCLE    4    // Feed every 4 hours
//----------------------------------------------------------------------------------------------------------------------
Servo motor;
byte button_pin = 2;
byte motor_pin  = 3;
unsigned long loop_timer;
//----------------------------------------------------------------------------------------------------------------------
/**
 * Setup routine
 */
void setup() {
    Serial.begin(9600);

    // Initialize loop timer.
    loop_timer = millis();

    motor.attach(motor_pin);
    pinMode(button_pin, INPUT_PULLUP);

    // Initialize motor's speed.
    motor.write(STOP);
}

/**
 * Main loop
 */
void loop() {
    if (isButtonPressed()) {
        manualFeed();
    }

    if (isFeedTime()) {
        feed(FEED_DURATION);
    }
}

/**
 * Feed for given duration
 *
 * @param unsigned int duration : Duration in ms (max 65535ms)
 * @return void
 */
void feed(unsigned int duration)
{
    unsigned long start = millis(); // When feeding process started.
    unsigned long now;

    // Casting (millis() - start) is to handle millis()'s overflow after 50 days.
    while((unsigned long)((now = millis()) - start) < duration) {
        feedCycle(start, now);
    }

    // Just make sure to stop the motor when button is released.
    motor.write(STOP);

    // Reset loop timer.
    loop_timer = millis();
}

/**
 * Trigger manual feeding while button is pressed.
 *
 * @return void
 */
void manualFeed()
{
    unsigned long start = millis(); // When feeding process started.

    while(isButtonPressed()) {
        feedCycle(start, millis());
    }

    motor.write(STOP);
}

/**
 * Run part of a cycle depending on when it started and current time.
 *
 * A cycle is composed of 2s FORWARD + 0.1s STOP + 0.5s BACKWARD + 0.1s STOP.
 * A cycle is then 2.7s long.
 *
 * @param unsigned long start : Time when feed cycle started
 * @param unsigned long now   : Current time
 */
void feedCycle(unsigned long start, unsigned long now)
{
    int   cycle    = 2700; // Duration of a full cycle in ms
    float position = (now - start) % cycle;

    if (position < 2000) {
        motor.write(FORWARD);
    } else if (position < 2100 || position > 2600) {
        motor.write(STOP);
    } else {
        motor.write(BACKWARD);
    }
}

/**
 * Return TRUE if it's feed time, FALSE otherwise.
 *
 * @return bool
 */
bool isFeedTime()
{
    // Casting (millis() - loop_timer) is to handle millis()'s overflow after 50 days.
    return (unsigned long)(millis() - loop_timer) >= (unsigned long)(FEED_CYCLE * 60 * 60 * 1000);
}

/**
 * Return whether the push button is pressed.
 *
 * @return bool
 */
bool isButtonPressed()
{
    return digitalRead(button_pin) == LOW;
}

/* Encoder Library - TwoKnobs Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder EncLeft(7, 8);
Encoder EncRight(9, 10);
//   avoid using pins with LEDs attached

void setup() {
  Serial.begin(9600);
}

long positionLeft  = 0;
long positionRight = 0;

void loop() {
  long newLeft, newRight;
  newLeft = EncLeft.read();
  newRight = EncRight.read();
  if (newLeft != positionLeft || newRight != positionRight) {
    Serial.print("Left = ");
    Serial.print(newLeft);
    Serial.print(" (");
    Serial.print(newLeft / 8400.0);      // how to count: 64 counts/rev (PDR) * 131.25:1 (gear ratio) = 8400
    Serial.print(" revs)");
    Serial.print(", Right = ");
    Serial.print(newRight);
    Serial.print(" (");
    Serial.print(newRight / 8400.0);      // how to count: 64 counts/rev (PDR) * 131.25:1 (gear ratio) = 8400
    Serial.print(" revs)");
    Serial.println();
    positionLeft = newLeft;
    positionRight = newRight;
  }
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset both knobs to zero");
    EncLeft.write(0);
    EncRight.write(0);
  }
}

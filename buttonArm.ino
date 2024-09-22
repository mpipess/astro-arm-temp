/*
Full arm control by buttons
Currently targeting a positive logic setup
i.e. DIR-, PUL-, and ENA- all grounded
DIR+, PUL+ are active high(?)
ENA+ is active low
*/

// Outputs to Motor Controllers
#define WAIST_ENABLE 0 // ENA+ signal to waist motor controller. Connect to Pin D0.
#define WAIST_DIRECTION 1 // DIR+ signal to waist motor controller. Connect to Pin D1.
#define WAIST_PULSE 2 // PUL+ signal to waist motor controller. Connect to Pin D2.

#define SHOULDER_ENABLE 3 // ENA+ signal to shoulder motor controller. Connect to Pin D3.
#define SHOULDER_DIRECTION 4 // DIR+ signal to shoulder motor controller. Connect to Pin D4.
#define SHOULDER_PULSE 5 // PUL+ signal to shoulder motor controller. Connect to Pin D5.

#define ELBOW_ENABLE 6 // ENA+ signal to elbow motor controller. Connect to Pin D6.
#define ELBOW_DIRECTION 7 // DIR+ signal to elbow motor controller. Connect to Pin D7.
#define ELBOW_PULSE 8 // PUL+ signal to elbow motor controller. Connect to Pin D8.

// Inputs to Microcontroller
#define WAIST_LEFT 62 // Input to microcontroller to step the waist "left". Connect to Pin D62.
#define WAIST_RIGHT 63 // Input to microcontroller to step the waist "right". Connect to Pin D63.

#define SHOULDER_UP 64 // Input to microcontroller to step the shoulder "up". Connect to Pin D64.
#define SHOULDER_DOWN 65 // Input to microcontroller to step the shoulder "down". Connect to Pin D65.

#define ELBOW_UP 66 // Input to microcontroller to step the elbow "up". Connect to D66.
#define ELBOW_DOWN 67 // Input to microcontroller to step the elbow "down". Connect to D67.

#define STEP_DELAY 100 // Time in milleseconds to delay each step of the stepper motor

// Need to know how the end effector/hand control works before writing code

void setup() {

    // Set all the outputs to outputs
    pinMode(WAIST_ENABLE, OUTPUT);
    pinMode(WAIST_DIRECTION, OUTPUT);
    pinMode(WAIST_PULSE, OUTPUT);

    pinMode(SHOULDER_ENABLE, OUTPUT);
    pinMode(SHOULDER_DIRECTION, OUTPUT);
    pinMode(SHOULDER_PULSE, OUTPUT);

    pinMode(ELBOW_ENABLE, OUTPUT);
    pinMode(ELBOW_DIRECTION, OUTPUT);
    pinMode(ELBOW_PULSE, OUTPUT);

    // Set all the inputs to inputs
    pinMode(WAIST_LEFT, INPUT);
    pinMode(WAIST_RIGHT, INPUT);

    pinMode(SHOULDER_UP, INPUT);
    pinMode(SHOULDER_DOWN, INPUT);

    pinMode(ELBOW_UP, INPUT);
    pinMode(ELBOW_DOWN, INPUT);

}

/*
Concept of operations:
*/
void loop() {

// RECOMMENDATION: Try to implement PWM pins instead and an internal timer instead of delay
// Also, test if the motor direction is good or not

    /*
    * Assuming that DIR+ high makes the motor step clockwise
    * Reference point being from "behind" the motor (opposite of shaft)
    * Waist motor shaft points "up", so CW --> left, CCW --> right
    */
    if (digitalRead(WAIST_LEFT)) {
        digitalWrite(WAIST_ENABLE, LOW);
        digitalWrite(WAIST_DIRECTION, HIGH);  
        digitalWrite(WAIST_PULSE, HIGH);
        delay(STEP_DELAY);
        digitalWrite(WAIST_PULSE, LOW);
        digitalWrite(WAIST_ENABLE, HIGH);
    }
    else if (digitalRead(WAIST_RIGHT)) {
        digitalWrite(WAIST_ENABLE, LOW);
        digitalWrite(WAIST_DIRECTION, LOW);
        digitalWrite(WAIST_PULSE, HIGH);
        delay(STEP_DELAY);
        digitalWrite(WAIST_PULSE, LOW);
        digitalWrite(WAIST_ENABLE, HIGH);
    }

    /*
    * Assuming that DIR+ high makes the motor step clockwise
    * Reference point being from "behind" the motor (opposite of shaft)
    * Shoulder motor shaft faces "right", so CW --> up, CCW --> down
    */
    if (digitalRead(SHOULDER_UP)) {
        digitalWrite(SHOULDER_ENABLE, LOW);
        digitalWrite(SHOULDER_DIRECTION, HIGH);
        digitalWrite(SHOULDER_PULSE, HIGH);
        delay(STEP_DELAY);
        digitalWrite(SHOULDER_PULSE, LOW);
        digitalWrite(SHOULDER_ENABLE, HIGH);
    }
    else if (digitalRead(SHOULDER_DOWN)) {
        digitalWrite(SHOULDER_ENABLE, LOW);
        digitalWrite(SHOULDER_DIRECTION, LOW);
        digitalWrite(SHOULDER_PULSE, HIGH);
        delay(STEP_DELAY);
        digitalWrite(SHOULDER_PULSE, LOW);
        digitalWrite(SHOULDER_ENABLE, HIGH);
    }

    /*
    * Assuming that DIR+ high makes the motor step clockwise
    * Reference point being from "behind" the motor (opposite of shaft)
    * Elbow motor shaft faces "right", so CW --> up, CCW --> down
    */
    if (digitalRead(ELBOW_DOWN)) {
        digitalWrite(ELBOW_ENABLE, LOW);
        digitalWrite(ELBOW_DIRECTION, HIGH);
        digitalWrite(ELBOW_PULSE, HIGH);
        delay(STEP_DELAY);
        digitalWrite(ELBOW_PULSE, LOW);
        digitalWrite(ELBOW_ENABLE, HIGH);
    }
    else if (digitalRead(ELBOW_UP)) {
        digitalWrite(ELBOW_ENABLE, LOW);
        digitalWrite(ELBOW_DIRECTION, LOW);
        digitalWrite(ELBOW_PULSE, HIGH);
        delay(STEP_DELAY);
        digitalWrite(ELBOW_PULSE, LOW);
        digitalWrite(ELBOW_ENABLE, HIGH);
    }

}

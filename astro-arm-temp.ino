// Pins used for an Arduino Mega (ATmega2560)
// RE-EVALUATE THESE WHEN PORTED TO ANOTHER MCU
#define WAIST_POT A1 // Can be any analog pin
#define WAIST_PUL 11 // Internally connected to Timer1
#define WAIST_DIR 12 // Can be any pin
#define WAIST_ENA 13 // Can be any pin

#define SHOULDER_POT A3 // Can be any analog pin
#define SHOULDER_PUL 5 // Internally connected to Timer3
#define SHOULDER_DIR 4 // Can be any pin
#define SHOULDER_ENA 3 // Can be any pin

#define ELBOW_POT A4 // Can be any analog pin
#define ELBOW_PUL 6 // Internally connected to Timer4
#define ELBOW_DIR 7 // Can be any pin
#define ELBOW_ENA 8 // Can be any pin

// Width of dead zone to either side of center
// Depends on input mapping, number is pretty abstract
#define DEAD_WIDTH 100

// Parameters for timer configuration
// MUST DO A SANITY CHECK IF YOU CHANGE FREQS (run the numbers)
#define PSCL 1024 // Prescaler setting (SET MANUALLY IN setup())
#define PCLK F_CPU / PSCL // Prescaled clock
#define FMAX 50 // Maximum desired step frequency
#define FMIN 5 // Minimum desired step frequency
// Frequencies multiplied by 2 because one toggle is half of a pulse cycle
#define TOP_FMAX PCLK / (2 * FMAX) // Value of timer's TOP to achieve FMAX
#define TOP_FMIN PCLK / (2 * FMIN) // Value of timer's TOP to achieve FMIN

void setup() {

    // Some gross low-level stuff to make Timer 1 do PFM
    // Clear Timer 1 config registers
    TCCR1B = 0x00;
    TCCR1A = 0x00;
    // Set bits for 1/1024 prescaler
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    // Set bits for CTC mode with OCR1A TOP
    TCCR1B |= (1 << WGM12);
    TCCR1A |= (1 << COM1A0);

    // Repeat for Timer 3
    // Clear Timer 3 config registers
    TCCR3B = 0x00;
    TCCR3A = 0x00;
    // Set bits for 1/1024 prescaler
    TCCR3B |= (1 << CS30);
    TCCR3B |= (1 << CS32);
    // Set bits for CTC mode with OCR3A TOP
    TCCR3B |= (1 << WGM32);
    TCCR3A |= (1 << COM3A0);
    // End of low-level configuration

    // Repeat for Timer 4
    // Clear Timer 4 config registers
    TCCR4B = 0x00;
    TCCR4A = 0x00;
    // Set bits for 1/1024 prescaler
    TCCR4B |= (1 << CS40);
    TCCR4B |= (1 << CS42);
    // Set bits for CTC mode with OCR4A TOP
    TCCR4B |= (1 << WGM42);
    TCCR4A |= (1 << COM4A0);
    // End of low-level configuration

    pinMode(WAIST_POT, INPUT);
    pinMode(WAIST_PUL, OUTPUT);
    pinMode(WAIST_DIR, OUTPUT);
    pinMode(WAIST_ENA, OUTPUT);

    pinMode(SHOULDER_POT, INPUT);
    pinMode(SHOULDER_PUL, OUTPUT);
    pinMode(SHOULDER_DIR, OUTPUT);
    pinMode(SHOULDER_ENA, OUTPUT);

    pinMode(ELBOW_POT, INPUT);
    pinMode(ELBOW_PUL, OUTPUT);
    pinMode(ELBOW_DIR, OUTPUT);
    pinMode(ELBOW_ENA, OUTPUT);
}

int waist_input = 0;
int waist_top = 0;

int shoulder_input = 0;
int shoulder_top = 0;

int elbow_input = 0;
int elbow_top = 0;

void loop() {

    // Read an analog signal from a joystick or smth
    waist_input = analogRead(WAIST_POT);

    // Map input to a range that meets max and min step frequency needs
    // Here step frequency is prescaled clock divided by input
    // For maximum frequency fmax, top needs to be (pclk)/fmax
    // For minimum frequency fmin, top needs to be (pclk)/fmin
    // Mapping range should be +- (top(fmin) - top(fmax))
    // Oh also add in some dead zone buffer
    waist_input = map(waist_input, 0, 1023, -(TOP_FMIN - TOP_FMAX) - DEAD_WIDTH, (TOP_FMIN - TOP_FMAX) + DEAD_WIDTH);
    // Offset the input so 0 happens at joystick centered (Vin = Vdd/2)
    waist_top = TOP_FMIN + 2 * DEAD_WIDTH - abs(waist_input);

    // Repeat for the shoulder input
    shoulder_input = analogRead(SHOULDER_POT);
    shoulder_input = map(shoulder_input, 0, 1023, -(TOP_FMIN - TOP_FMAX) - DEAD_WIDTH, (TOP_FMIN - TOP_FMAX) + DEAD_WIDTH);
    shoulder_top = TOP_FMIN + 2 * DEAD_WIDTH - abs(shoulder_input);

    // Repeat for the elbow input
    elbow_input = analogRead(ELBOW_POT);
    elbow_input = map(elbow_input, 0, 1023, -(TOP_FMIN - TOP_FMAX) - DEAD_WIDTH, (TOP_FMIN - TOP_FMAX) + DEAD_WIDTH);
    elbow_top = TOP_FMIN + 2 * DEAD_WIDTH - abs(elbow_input);

    // More gross low-level stuff, but pretty self-explanatory
    // Set a new TOP to change Timer period
    OCR1A = waist_top;
    OCR3A = shoulder_top;
    OCR4A = elbow_top;

    // Avoid the timer skipping past the new TOP
    if (TCNT1 > OCR1A) {
        TCNT1 = OCR1A - 1;
    }
    if (TCNT3 > OCR3A) {
        TCNT3 = OCR3A - 1;
    }
    if (TCNT4 > OCR4A) {
        TCNT4 = OCR4A - 1;
    }
    // End of more low-level stuff

    // Set which direction the motor should step in
    if (waist_input > DEAD_WIDTH) {
        digitalWrite(WAIST_DIR, HIGH);
    }
    else {
        digitalWrite(WAIST_DIR, LOW);
    }
    if (shoulder_input > DEAD_WIDTH) {
        digitalWrite(SHOULDER_DIR, HIGH);
    }
    else {
        digitalWrite(SHOULDER_DIR, LOW);
    }
    if (elbow_input > DEAD_WIDTH) {
        digitalWrite(ELBOW_DIR, HIGH);
    }
    else {
        digitalWrite(ELBOW_DIR, LOW);
    }

    // Set ENA if input exceeds the dead zone on either side
    if (abs(waist_input) > DEAD_WIDTH) {
        digitalWrite(WAIST_ENA, HIGH);
    }
    else {
        digitalWrite(WAIST_ENA, LOW);
    }
    if (abs(shoulder_input) > DEAD_WIDTH) {
        digitalWrite(SHOULDER_ENA, HIGH);
    }
    else {
        digitalWrite(SHOULDER_ENA, LOW);
    }
    if (abs(elbow_input) > DEAD_WIDTH) {
        digitalWrite(ELBOW_ENA, HIGH);
    }
    else {
        digitalWrite(ELBOW_ENA, LOW);
    }
}


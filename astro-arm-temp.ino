// Pins used for an Arduino Mega (ATmega2560)
// RE-EVALUATE THESE WHEN PORTED TO ANOTHER MCU
#define POT_PIN A7 // Can be any analog pin
#define PUL_PIN 11 // Internally connected to Timer1
#define DIR_PIN 7 // Can be any pin
#define ENA_PIN 6 // Can be any pin

// Width of dead zone to either side of center
// Depends on input mapping, number is pretty abstract
#define DEAD_WIDTH 100

// Parameters for timer configuration
// MUST DO A SANITY CHECK IF YOU CHANGE FREQS (run the numbers)
#define PSCL 1024 // Prescaler setting (SET MANUALLY IN setup())
#define PCLK F_CPU / PSCL // Prescaled clock
#define FMAX 1000 // Maximum desired step frequency
#define FMIN 20 // Minimum desired step frequency
// Frequencies multiplied by 2 because one toggle is half of a pulse cycle
#define TOP_FMAX PCLK / (2 * FMAX) // Value of timer's TOP to achieve FMAX
#define TOP_FMIN PCLK / (2 * FMIN) // Value of timer's TOP to achieve FMIN

void setup() {

    // Some gross low-level stuff to make Timer1 do PFM
    // Clear Timer 1 config registers
    TCCR1B = 0x00;
    TCCR1A = 0x00;
    // Set bits for 1/1024 prescaler
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    // Set bits for CTC mode with OCR1A TOP
    TCCR1B |= (1 << WGM12);
    TCCR1A |= (1 << COM1A0);
    // End of low-level configuration

    pinMode(POT_PIN, INPUT);
    pinMode(PUL_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(ENA_PIN, OUTPUT);
}

int input = 0;
int top = 0;
void loop() {

    // Read an analog signal from a joystick or smth
    input = analogRead(POT_PIN);

    // Map input to a range that meets max and min step frequency needs
    // Here step frequency is prescaled clock divided by input
    // For maximum frequency fmax, top needs to be (pclk)/fmax
    // For minimum frequency fmin, top needs to be (pclk)/fmin
    // Mapping range should be +- (top(fmin) - top(fmax))
    // Oh also add in some dead zone buffer
    input = map(input, 0, 1023, -(TOP_FMIN - TOP_FMAX) - DEAD_WIDTH, (TOP_FMIN - TOP_FMAX) + DEAD_WIDTH);
    // Offset the input so 0 happens at joystick centered (Vin = Vdd/2)
    top = TOP_FMIN + 2 * DEAD_WIDTH - abs(input);

    // More gross low-level stuff, but pretty self-explanatory
    // Set a new TOP to change Timer1 period
    OCR1A = top;

    // Avoid the timer skipping past the new TOP
    if (TCNT1 > OCR1A)
    {
        TCNT1 = OCR1A - 1;
    }
    // End of more low-level stuff

    // Set which direction the motor should step in
    if (input > DEAD_WIDTH) {
        digitalWrite(DIR_PIN, HIGH);
    }
    else {
        digitalWrite(DIR_PIN, LOW);
    }

    // Set ENA if input exceeds the dead zone on either side
    // Note that our motor driver (DM556) uses negative logic
    if (abs(input) > DEAD_WIDTH) {
        digitalWrite(ENA_PIN, LOW);
    }
    else {
        digitalWrite(ENA_PIN, HIGH);
    }

    // Look at that, no delay() in sight!
}


#define POT_PIN A7

void setup() {
	// Set PB1 (OC1A) to output
	DDRB = DDRB | (1 << DDB1);

	// Clear Timer 1 config registers
	TCCR1B = 0x00;
	TCCR1A = 0x00;
	// Set bits for 1/256 prescaler
	TCCR1B = TCCR1B | (1 << CS12);
	// Set bits for CTC mode with OCR1A TOP
	TCCR1B = TCCR1B | (1 << WGM12);
	TCCR1A = TCCR1A | (1 << COM1A0);

	// Set output compare value
	OCR1A = 0xFFFF;

	pinMode(POT_PIN, INPUT);
}

uint16_t input = 0;
void loop() {

	// Being strict with types because of later bitwise ops
	input = (uint16_t) analogRead(POT_PIN);

	// Set a lower bound for sanity
	if (input <= 0) {
		input = 1;
	}

	// Set a new TOP to change Timer1 period
	// Shift the 10-bit analog value to fill 16 bits
	OCR1A = (input << 6);

	// Try to avoid the timer skipping past the new TOP
	if (TCNT1 > OCR1A)
	{
		TCNT1 = 0x0000;
	}

	// Delay for stability reasons (should be a better way)
	delay(10);
}


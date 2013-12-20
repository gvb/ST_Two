//****************************************************************************
// Called by Timer 1 Interrupt to draw next column in LED matrix
//****************************************************************************
// Only light one ROW (and one column) ie one pixel at a time. = lower
// current draw, but lower refresh rate.

void LEDupdateTWO()		// ONE ROW of selected column at a time
{
	if (ROWBITINDEX > 6) {
		Mcolumn = Mcolumn + 1;	// Prep for next column
		if (Mcolumn > 19) {
			Mcolumn = 0;
		}

		PORTB = (PORTB & B10000000);	// Clear last column
		PORTC = (PORTC & B11110000) | B00001111;

		if (Mcolumn < 16)	// Matrix column (from 0 to 15)
		{
			// Decode digit Col. 1 to 16 - Select De-Mux chip
			PORTB = (PORTB & B01111111);

			// Decode address to 74HC154
			PORTD = (PORTD & B00001111) | (Mcolumn << 4);
		} else {
			// Decode digit Col. 16 to 19 - UN-Select De-Mux chip
			PORTB = (1 << PORTB7);

			// Using PC0 to PC4 to address col. 17 to 19 directly
			PORTC = (PORTC & B11110000) | ~(1 << (Mcolumn - 16));
		}
		ROWBITINDEX = 0;
	} else {
		PORTB = (PORTB & B10000000);
		if (bitRead(LEDMAT[Mcolumn], ROWBITINDEX)) {
			bitSet(PORTB, ROWBITINDEX);
		}

		if (Mcolumn < 16)	// Matrix column (from 0 to 15)
		{
			delayMicroseconds(120);
		}
		ROWBITINDEX = ROWBITINDEX + 1;
	}
}

//****************************************************************************
// Called by Timer 1 Interrupt to draw next column in LED matrix
//****************************************************************************
// This version of LED refresh / drawing lights full column at once =
// higher current draw (but can be brighter)

void LEDupdate()		// All ROWs of selected column at the same time
{

	PORTB = (PORTB & B10000000);	// Clear last column
	PORTC = (PORTC & B11110000) | B00001111;

	if (Mcolumn < 16)	// Matrix column (from 0 to 15)
	{
		// Decode digit Col. 1 to 16 - Select De-Mux chip
		PORTB = (PORTB & B01111111);

		// Decode address to 74HC154
		PORTD = (PORTD & B00001111) | (Mcolumn << 4);
	} else {
		PORTB = (1 << PORTB7);
		// Decode digit Col. 16 to 19 - UN-Select De-Mux chip

		PORTC = (PORTC & B11110000) | ~(1 << (Mcolumn - 16));
		// Using PC0 to PC4 to address col. 17 to 19 directly
	}

	// Light LEDs - turn on ROWs
	PORTB = (PORTB & B10000000) | (LEDMAT[Mcolumn]);

	Mcolumn = Mcolumn + 1;	// Prep for next column
	if (Mcolumn > 19) {
		Mcolumn = 0;
	}
}

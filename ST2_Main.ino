//****************************************************************************
// Main Loop
//****************************************************************************
void loop()
{
	// Test for Sleep ------------------------------------------------*

	currentMillis = millis();
	OptionModeFlag = false;

	if (((currentMillis - SleepTimer) > SleepLimit) && SleepEnable) {
		STATE = SLEEP;
	}
	// Test for Mode Button Press ------------------------------------*

	bval = !digitalRead(MODEBUTTON);
	if (bval) {
		if (ALARMON) {
			CheckAlarm();
		}

		if (ALARM1FLAG) {
			ALARM1FLAG = false;
			ALARMON = false;
			EnableAlarm1(false);
			STATE = ALARM_TRIGGERED;
			JustWokeUpFlag = false;
		} else {
			if (JustWokeUpFlag) {
				JustWokeUpFlag = false;
				JustWokeUpFlag2 = true;	// Used to suppress "Time" text from showing when waking up.
			} else {
				NextStateRequest = true;
			}
//    SUBSTATE = SLEEP;

			while (bval) {
				bval = !digitalRead(SETBUTTON);
				if (bval) {
					OptionModeFlag = true;
					NextStateRequest = false;
					NextSUBStateRequest = false;
					displayString("SPEC");
					delay(300);
				}
				bval = !digitalRead(MODEBUTTON);
			}

			delay(100);
			SleepTimer = millis();
		}
	}
	// Test for SET Button Press ------------------------------------*

	bval = !digitalRead(SETBUTTON);
	if (bval && !OptionModeFlag) {
		NextSUBStateRequest = true;

		while (bval) {
			bval = !digitalRead(MODEBUTTON);
			if (bval) {
				OptionModeFlag = true;
				NextStateRequest = false;
				NextSUBStateRequest = false;
				displayString("SPEC");
				delay(300);
			}
			bval = !digitalRead(SETBUTTON);
		}
		delay(100);
		SleepTimer = millis();
	}
	// Running Blink counter ------------------------------------*
	if (blinkFlag) {
		blinkCounter = blinkCounter + 1;
		if (blinkCounter > blinkTime) {
			blinkON = !blinkON;
			blinkCounter = 0;
		}
	} else {
		blinkON = true;	// Not blinking, just leave the LEDs lit
	}

//****************************************************************************
// Main Loop - State Machine
//****************************************************************************

	switch (STATE) {
	case SETUP:
		STATE++;
		break;
#ifdef TIME
	case TIME:
		DisplayTimeSub();
		break;
#endif
#ifdef SET_TIME
	case SET_TIME:
		setTimeSub();
		break;
#endif
#ifdef CONFIG_ALARM
	case CONFIG_ALARM:
		setAlarmSub();
		break;
#endif
#ifdef STOPWATCH
	case STOPWATCH:
		StopWatch();
		break;
#endif
#ifdef SCROLLING_TEXT
	case SCROLLING_TEXT:
		DisplaySerialData();
		break;
#endif
#ifdef GRAPHIC_DEMO
	case GRAPHIC_DEMO:
		graphican();
		break;
#endif
#ifdef MUSIC
	case MUSIC:
		playMusic();
		break;
#endif
// ---------------------------------------------------------------------------

	case ALARM_TRIGGERED:		// Alarm Triggered

		blinkFlag = true;
		displayString("Beep");

		if (blinkON) {
			pinMode(SETBUTTON, OUTPUT);
			tone(SETBUTTON, 4000);
			delay(100);
			noTone(SETBUTTON);
			digitalWrite(SETBUTTON, HIGH);
		}
#if ARDUINO >= 101
		pinMode(SETBUTTON, INPUT_PULLUP);
#else
		pinMode(SETBUTTON, INPUT);
#endif
		delay(250);

		if (NextSUBStateRequest | NextStateRequest) {
			STATE = SETUP;
			SUBSTATE = 0;
			NextStateRequest = false;
			NextSUBStateRequest = false;
			blinkFlag = false;
		}
		break;

// ---------------------------------------------------------------------------

	case SLEEP:		// Sleep
		displayString("Nite");
		delay(500);
		clearmatrix();
		GoToSleep();
		SleepTimer = millis();
		STATE = SETUP;
		SUBSTATE = 0;
		break;

// ---------------------------------------------------------------------------

	}
}

//***************************************************************************
// End of Main Loop
//***************************************************************************

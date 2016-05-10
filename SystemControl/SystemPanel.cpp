//**SystemPanel*******************************//
#include "SystemPanel.h"
//#include "PanelComponents.h"
#include "Panel.h"
#include "Arduino.h"
#include "flagMessaging.h"

//Redefined here...
#define REFPIN A0
#define BATTAPIN A1
#define BATTBPIN A2
#define SYSTEMRAILPIN A3

SystemPanel::SystemPanel( void )
{
	state = PInit;
	bootState = 0;
	altFunctionHeld = 0;
}

void SystemPanel::reset( void )
{
	//Set explicit states
	//Set all LED off
	aLed.setState(LEDFLASHINGFAST);
	
	update();
	
}

//---------------------------------------------------------------------------//
//
//  To process the machine,
//    take the inputs from the system
//    process human interaction hard-codes
//    process the state machine
//    clean up and post output data
//
//---------------------------------------------------------------------------//
void SystemPanel::processMachine( void )
{
	//Do small machines

	//Do main machine
	tickStateMachine();
	
	//Set outputs for select objects
	if(serialActivityFlag.getFlag())
	{
		aLed.setState(LEDOFF);
		serialActivityFlag.clearFlag();
	}
	else
	{
		aLed.setState(ALEDSTATE);
	}
	//Update the hardware
	update();
}

void SystemPanel::tickStateMachine()
{
	//***** PROCESS THE LOGIC *****//
	//Now do the states.
	PStates nextState = state;
	switch( state )
	{
	case PInit:
		nextState = PIdle;
		ALEDSTATE = LEDFLASHING;
		break;
	case PIdle:
		//Can't be running, if button pressed move on
		if( aButton.serviceRisingEdge() )
		{
			Keyboard.println("pi");
			nextState = PBootA;
		}
		break;
	case PBootA:
		if( aButton.serviceRisingEdge() )
		{
			Keyboard.println("raspberry");
			delay(300);
			
			//Log in serial
			while( Serial.available() )
			{
				Serial.read();
			}
			Serial.print("pi\r");
			delay(100);
			while( Serial.available() )
			{
				Serial.read();
			}
			Serial.print("raspberry\r");
			delay(100);
			while( Serial.available() )
			{
				Serial.read();
			}		
			Serial.print("touch system_status.log\r");
			delay(100);
			while( Serial.available() )
			{
				Serial.read();
			}		
			nextState = PBootB;
		}
		break;
	case PBootB:
		if( aButton.serviceRisingEdge() )
		{
			Keyboard.println("startx");
			nextState = PBootC;
		}
		break;
	case PBootC:
		if( aButton.serviceRisingEdge() )
		{
			//run log
			writeFileFlag.setFlag();
			ALEDSTATE = LEDON;
			nextState = PRunning;
		}
		break;
	case PRunning:
		if( aButton.serviceRisingEdge() )
		{
			if( altFunctionHeld )
			{
				//Alt function, reset keyboard
				nextState = PInit;
			}
			else
			{
				Serial.print("sh buttonA.sh &\r");
			}
		}
		if( bButton.serviceRisingEdge() )
		{
			if( altFunctionHeld )
			{
				//Alt function, run with sudo
				Serial.print("sudo sh buttonB.sh &\r");
			}
			else
			{
				Serial.print("sh buttonB.sh &\r");
			}
		}
		if( cButton.serviceRisingEdge() )
		{
			if( altFunctionHeld )
			{
				//Alt function, run with sudo
				Serial.print("sudo sh buttonC.sh &\r");
			}
			else
			{
				Serial.print("sh buttonC.sh &\r");
			}
		}
		if( dButton.serviceRisingEdge() )
		{
			Serial.print("sudo ./buttonD.sh &\r");
		}
		if( dButton.serviceHoldRisingEdge() )
		{
			altFunctionHeld = 1;
		}
		if( dButton.serviceFallingEdge() )
		{
			altFunctionHeld = 0;
		}

		break;
	default:
		nextState = PInit;
		break;
	}
	state = nextState;

}

void SystemPanel::timersMIncrement( uint8_t inputValue )
{
	aButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
	bButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
	cButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
	dButton.buttonDebounceTimeKeeper.mIncrement(inputValue);

}

uint8_t SystemPanel::getState( void )
{
	return state;
}
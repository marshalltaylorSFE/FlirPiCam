#include <TimerOne.h>

//HOW TO OPERATE
//  Make TimerClass objects for each thing that needs periodic service
//  pass the interval of the period in ticks
//
//  Set MAXINTERVAL to rollover rate
//  Set MAXTIMER to the max foreseen interval of any timer.
//  MAXTIMER + MAXINTERVAL = max countable value.

//NOTICE:
//  The timerModule32 only works on teensy / fast processors.  It works the same
//  but keeps track of everything in us counts.


//Not used by this sketch but dependant on one 
#include "Wire.h"
#include "Panel.h"
#include "stdint.h"

#include "timerModule32.h"

#define REFPIN A0
#define BATTAPIN A1
#define BATTBPIN A2
#define SYSTEMRAILPIN A3


//Globals
uint32_t MAXTIMER = 60000000;
uint32_t MAXINTERVAL = 2000000;

uint8_t ledToggleState = 0;
uint8_t ledToggleFastState = 0;

//**Copy to make a new timer******************//  
//        TimerClass32 usTimerA( 20000 ); //20 ms
//
//Note on TimerClass-
//Change with usTimerA.setInterval( <the new interval> );
TimerClass32 panelUpdateTimer(10000);
TimerClass32 debounceTimer(5000);
TimerClass32 ledToggleTimer( 333000 );
TimerClass32 ledToggleFastTimer( 100000 );

TimerClass32 debugTimer(5000000);

//**Panel State Machine***********************//
#include "SystemPanel.h"
SystemPanel embKeys;

uint32_t usTicks = 0;

//  The lock works like this:
//
//    When the interrupt fires, the lock is removed.  Now
//    the main free-wheeling loop can update the change to
//    the timerModules.  Once complete, the lock is replaced
//    so that it can't update more than once per firing
//    of the interrupt

uint8_t usTicksLocked = 1; //start locked out

void setup()
{
	Serial.begin(115200);

  	//Init panel.h stuff
	embKeys.init();
	
	// initialize timer
	Timer1.initialize(25);
	Timer1.attachInterrupt(serviceUS); // blinkLED to run every 0.001 seconds
	
	//Go to fresh state
	embKeys.reset();
	
	//Update the panel
	embKeys.update();
  
}

void loop()
{
	if( Serial.available() )
	{
		Serial.read();
		embKeys.serialActivityFlag.setFlag();
		delay(10);
	}
	
	//Update the timers, but only once per interrupt
	if( usTicksLocked == 0 )
	{
		//**Copy to make a new timer******************//  
		//msTimerA.update(usTicks);

		//Panel timers
		ledToggleTimer.update(usTicks);
		ledToggleFastTimer.update(usTicks);
		panelUpdateTimer.update(usTicks);
		debounceTimer.update(usTicks);

		debugTimer.update(usTicks);
		
		//Done?  Lock it back up
		usTicksLocked = 1;
	}  //The ISR will unlock.

	//**Copy to make a new timer******************//  
	//if(usTimerA.flagStatus() == PENDING)
	//{
	//	//User code
	//}
	
	//**Debounce timer****************************//  
	if(debounceTimer.flagStatus() == PENDING)
	{
		embKeys.timersMIncrement(5);
	
	}
		
	//**Process the panel and state machine***********//  
	if(panelUpdateTimer.flagStatus() == PENDING)
	{
		//Provide inputs

		//Tick the machine
		embKeys.processMachine();
		
		//Deal with outputs 
		//  Form: if( myFlagName.resetTapHeadFlag.serviceRisingEdge() )...
		
	}
	//**Fast LED toggling of the panel class***********//  
	if(ledToggleFastTimer.flagStatus() == PENDING)
	{
		embKeys.toggleFastFlasherState();
		
	}

	//**LED toggling of the panel class***********//  
	if(ledToggleTimer.flagStatus() == PENDING)
	{
		embKeys.toggleFlasherState();
		
	}
	//**Debug timer*******************************//  
	if(debugTimer.flagStatus() == PENDING)
	{
		//uint32_t tempRef = analogRead(REFPIN);
		//Serial.println("**************************Debug Service**************************");
		//Serial.print("Current state: ");
		//Serial.println(embKeys.getState());
		//
		//Serial.print("Ref value: ");
		//Serial.println(tempRef, HEX);
		//
		//Serial.print("A voltage: ");
		//Serial.println(((float)analogRead(BATTAPIN) * 5)/tempRef);
		//
		//Serial.print("B voltage: ");
		//Serial.println(((float)analogRead(BATTBPIN) * 5)/tempRef);
		//
		//Serial.print("System soltage: ");
		//Serial.println(((float)analogRead(SYSTEMRAILPIN) * 5)/tempRef);
		
		if(embKeys.writeFileFlag.getFlag() )
		{
			uint32_t tempRef = analogRead(REFPIN);
			
			Serial.print("echo ");
			Serial.print("ref=");
			Serial.print(tempRef, HEX);
			Serial.print(" > system_status.log\r");
			delay(100);
			while( Serial.available() )
			{
				Serial.read();
			}		
			
			Serial.print("echo ");
			Serial.print("battA=");
			Serial.print(((float)analogRead(BATTAPIN) * 5)/tempRef);
			Serial.print(" >> system_status.log\r");
			delay(100);
			while( Serial.available() )
			{
				Serial.read();
			}		
			
			Serial.print("echo ");
			Serial.print("battB=");
			Serial.print(((float)analogRead(BATTBPIN) * 5)/tempRef);
			Serial.print(" >> system_status.log\r");
			delay(100);
			while( Serial.available() )
			{
				Serial.read();
			}		
			
			Serial.print("echo ");
			Serial.print("pwr=");
			Serial.print(((float)analogRead(SYSTEMRAILPIN) * 5)/tempRef);
			Serial.print(" >> system_status.log\r");
		}
	
	}
}

void serviceUS(void)
{
  uint32_t returnVar = 0;
  if(usTicks >= ( MAXTIMER + MAXINTERVAL ))
  {
    returnVar = usTicks - MAXTIMER;

  }
  else
  {
    returnVar = usTicks + 25;
  }
  usTicks = returnVar;
  usTicksLocked = 0;  //unlock
}

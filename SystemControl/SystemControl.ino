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

//Globals
uint32_t MAXTIMER = 60000000;
uint32_t MAXINTERVAL = 2000000;

#define LEDPIN 6
#include "timerModule32.h"
#include "stdint.h"

//**Copy to make a new timer******************//  
//TimerClass32 usTimerA( 20000 ); //20 ms
TimerClass32 secondTimer( 1000000 ); //20 ms
TimerClass32 usTimerB( 25000 );
TimerClass32 usTimerC( 27300 );
//Note on TimerClass-
//Change with usTimerA.setInterval( <the new interval> );


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
  //Serial.begin(9600);
  pinMode(LEDPIN, OUTPUT);
  Timer1.initialize(25);
  Timer1.attachInterrupt(serviceUS); // blinkLED to run every 0.15 seconds
  Serial.begin(9600);
  
}

void loop()
{
	//Update the timers, but only once per interrupt
	if( usTicksLocked == 0 )
	{
		//**Copy to make a new timer******************//  
		//msTimerA.update(usTicks);
		
		secondTimer.update(usTicks);
		usTimerB.update(usTicks);
		usTimerC.update(usTicks);
		//Done?  Lock it back up
		usTicksLocked = 1;
	}  //The ISR will unlock.

	//**Copy to make a new timer******************//  
	//if(usTimerA.flagStatus() == PENDING)
	//{
	//	//User code
	//}
	
	if(secondTimer.flagStatus() == PENDING)
	{
		//User code
		digitalWrite( LEDPIN, digitalRead( LEDPIN ) ^ 0x01 );
	}
	if(usTimerB.flagStatus() == PENDING)
	{
		//User code
		//digitalWrite( LEDPIN, digitalRead( LEDPIN ) ^ 0x01 );
	}
	if(usTimerC.flagStatus() == PENDING)
	{
		//User code
		//digitalWrite( LEDPIN, digitalRead( LEDPIN ) ^ 0x01 );
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

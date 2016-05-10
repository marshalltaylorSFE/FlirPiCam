#include "Panel.h"

//This is where PanelComponents are joined to form the custom panel
#define aButtonPin 2
#define bButtonPin 3
#define cButtonPin 4
#define dButtonPin 5
#define aLedPin 6

Panel::Panel( void )
{
	flasherState = 0;
	fastFlasherState = 0;
	aLed.outputInvert(0x01);
}

void Panel::init( void )
{
	aButton.init(aButtonPin);
	bButton.init(bButtonPin);
	cButton.init(cButtonPin);
	dButton.init(dButtonPin);
	aLed.init(aLedPin, 0, &flasherState, &fastFlasherState);

 	flasherState = 0;
	fastFlasherState = 0;
}

void Panel::update( void )
{
	aButton.update();
	bButton.update();
	cButton.update();
	dButton.update();
	aLed.update();
}

void Panel::toggleFlasherState( void )
{
	flasherState ^= 0x01;
}

void Panel::toggleFastFlasherState( void )
{
	fastFlasherState ^= 0x01;
}
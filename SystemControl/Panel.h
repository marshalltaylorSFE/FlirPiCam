#ifndef PANEL_H
#define PANEL_H

#include "PanelComponents.h"

class Panel
{
public:
	Panel( void );
	void update( void );
	void init( void );
	void toggleFlasherState( void );
	void toggleFastFlasherState( void );

	PanelButton aButton;
	PanelButton bButton;
	PanelButton cButton;
	PanelButton dButton;
	PanelLed aLed;

private:
	volatile uint8_t flasherState;
	volatile uint8_t fastFlasherState;

};

#endif // PANEL_H




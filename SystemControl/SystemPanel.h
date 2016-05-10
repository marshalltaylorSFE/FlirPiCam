//Header
#ifndef LOOPERPANEL_H_INCLUDED
#define LOOPERPANEL_H_INCLUDED


#include "stdint.h"
#include "timeKeeper.h"
#include "PanelComponents.h"
#include "Panel.h"
#include "flagMessaging.h"

enum PStates
{
	PInit,
	PIdle,
	PBootA,
	PBootB,
	PBootC,
	PRunning

};

class SystemPanel : public Panel
{
public:
	SystemPanel( void );
	void reset( void );
	//State machine stuff  
	void processMachine( void );
	void tickStateMachine( void );

	void timersMIncrement( uint8_t );
	
	uint8_t getState( void );
	//**Flags and data used by the state machine********//
	//Flags coming in from the system
	MessagingFlag writeFileFlag;
	MessagingFlag serialActivityFlag;
	
	//Internal - and going out to the system - Flags
	//		Form: MessagingFlag sendPanicFlag;
	
	//  ..and data.


private:
	//Internal Flags

	//  ..and data
	uint8_t bootState;
	uint8_t altFunctionHeld;
	ledState_t ALEDSTATE;
	
	//State machine stuff  
	PStates state;

};

#endif
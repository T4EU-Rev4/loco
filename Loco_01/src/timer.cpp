/*

  Alle Routinen für den Timerbetrieb
  
  Die Bibliothek stellt eine Reihe von Flags zur Verfügung, die in bestimmten Intervallen gesetzt werden. 
  Der Benutzer kann diese Flags in seinem Hauptprogramm prüfen und eine Aktion ausführen, wenn es gesetzt ist. 
  Der Benutzer muss das Flag wieder löschen. Auch dazu werden Funktionen bereitgestellt
  
 Created: 27.05.2011 08:27:44
 Author:  busser.michael

*/
#include <Arduino.h>
#include "timer.h"

//------------------------------------------------------------------------- 
// interne Variablen
//------------------------------------------------------------------------- 
unsigned long timerCounter;
uint8_t   cnt5MS     = 5;              //- Der Vorteiler für die 5/1000-tel Sekunden
uint8_t   cnt10MS    = 10;             //- Der Vorteiler für die 100-tel Sekunden 
uint8_t   cnt100MS   = 100;            //- Der Vorteiler für die 10-tel Sekunden 
uint16_t  cntsek     = 1000;           //- Der Vorteiler für die Sekunden 
uint16_t  cnt10sek   = 10000;		   //- Der Vorteiler für 10 Sekunden	
uint16_t  cntMin     = 60000;	       //- Der Vorteiler für 1 Minute

timer_callback_t	CB_Event_10MS = NULL;			//- Variablen für die Callback-Funktionen
timer_callback_t	CB_Event_100MS = NULL;			//- diese müssen mit den entsprechenden register-Funktionen 
timer_callback_t	CB_Event_1S = NULL;	    		//- registriert werden.
timer_callback_t  CB_Event_10S = NULL;
timer_callback_t  CB_Event_MIN = NULL;

union TTimer timerFlags;       //keep all the flags in one byte

void timer_Setup( void ) {
    timerCounter = millis();        //fetch the starting value
}


//------------------------------------------------------------------------- 
// Call this routine from the main loop to keep the timer running
//------------------------------------------------------------------------- 
void timer_Trigger( void ) {
  unsigned long count = millis() - timerCounter;        //delay in ms since last call

  if (count > 0) {
    timerFlags.flags.bMilli = 1;
  }

  if (count > cnt5MS) { cnt5MS = 0; } else { cnt5MS -= count; }   
  if (cnt5MS == 0) {
	timerFlags.flags.b5MS = 1;	  
	cnt5MS = 5;
  }
  
  if (count > cnt10MS) { cnt10MS = 0; } else { cnt10MS -= count; }   
  if (cnt10MS == 0) { 
	timerFlags.flags.b10MS = 1;
  	cnt10MS = 10; 
	if (CB_Event_10MS != NULL) { CB_Event_10MS(); } 
  }

  if (count > cnt100MS) { cnt100MS = 0; } else { cnt100MS -= count; }   
  if (cnt100MS == 0) { 
	timerFlags.flags.b100MS = 1;
  	cnt100MS = 100; 
	if (CB_Event_100MS != NULL) { CB_Event_100MS(); } 
  }

  if (count > cntsek) { cntsek = 0; } else { cntsek -= count; }   
  if (cntsek == 0) { 
	timerFlags.flags.bSek = 1;
  	cntsek = 1000; 
	if (CB_Event_1S != NULL) { CB_Event_1S(); } 
  }

  if (count > cnt10sek) { cnt10sek = 0; } else { cnt10sek -= count; }   
  if (cnt10sek == 0) { 
	timerFlags.flags.b10Sek = 1;
  	cnt10sek = 10000; 
	if (CB_Event_10S != NULL) { CB_Event_10S(); } 
  }

  if (count > cntMin) { cntMin = 0; } else { cntMin -= count; }   
  if (cntMin == 0) { 
	timerFlags.flags.bMin = 1;
  	cntMin = 60000; 
	if (CB_Event_MIN != NULL) { CB_Event_MIN(); } 
  }
  timerCounter += count;
}
//------------------------------------------------------------------------- 
void timer_ClearAll( void ) {
  timerFlags.all = 0;   // clear all flags in one step 
  cnt5MS     = 5;       // preset the counters
  cnt10MS    = 10;             
  cnt100MS   = 100;          
  cntsek     = 1000;      
  cnt10sek   = 10000;			
  cntMin     = 60000;	
}
//------------------------------------------------------------------------- 
void timer_ClearMilli( void ) {
  timerFlags.flags.bMilli = 0;
}
//-------------------------------------------------------------------------
void timer_Clear5MS( void ) {
	timerFlags.flags.b5MS = 0;
}
//------------------------------------------------------------------------- 
void timer_Clear10MS( void ) {
  timerFlags.flags.b10MS = 0;
}
//------------------------------------------------------------------------- 
void timer_Clear100MS( void ) {
  timerFlags.flags.b100MS = 0;
}
//------------------------------------------------------------------------- 
void timer_ClearSek( void ) {
  timerFlags.flags.bSek = 0;
}
//------------------------------------------------------------------------- 
void timer_Clear10Sek( void ){
  timerFlags.flags.b10Sek = 0;    
}
//------------------------------------------------------------------------- 
void timer_ClearMin( void ) {
  timerFlags.flags.bMin = 0;
}
//------------------------------------------------------------------------- 

void timer_register_Callback( enum TCBInterval cbi, timer_callback_t cbf ) {
    switch (cbi) {
        case tCB_10MS:  CB_Event_10MS = cbf;
                        break;
        case tCB_100MS: CB_Event_100MS = cbf;
                        break;
        case tCB_1SEK:  CB_Event_1S = cbf;
                        break;
        case tCB_10SEK: CB_Event_10S= cbf;
                        break;
        case tCB_1MIN:  CB_Event_MIN = cbf;
                        break;
    }
}

void timer_unregister_Callback( enum TCBInterval cbi  ) {
    switch (cbi) {
        case tCB_10MS:  CB_Event_10MS = NULL;
                        break;
        case tCB_100MS: CB_Event_100MS = NULL;
                        break;
        case tCB_1SEK:  CB_Event_1S = NULL;
                        break;
        case tCB_10SEK: CB_Event_10S= NULL;
                        break;
        case tCB_1MIN:  CB_Event_MIN = NULL;
                        break;
    }   
}


#ifndef _TIMER_
#define _TIMER_
/*

  Alle Routinen für den Timerbetrieb
  
  Die Bibliothek stellt eine Reihe von Flags zur Verfügung, die in bestimmten Intervallen gesetzt werden. 
  Der Benutzer kann diese Flags in seinem Hauptprogramm prüfen und eine Aktion ausführen, wenn es gesetzt ist. 
  Der Benutzer muss das Flag wieder löschen. Auch dazu werden Funktionen bereitgestellt
  
  Der Timerinterrupt setzt in der globalen Variablen timerFlags
  je ein Flag für das Auftreten des entsprechenden Events.
  Die Flags müssen nach der Bearbeitung des Events vom Hauptprogramm gelöscht werden.
  Dadurch kann die Eventbearbeitung auch länger dauern als die Interruptzeit erlauben würde (asynchron)

  Synchrone Verarbeitung ist über die DoXXX Prozeduren möglich
  
  Timer0 -> internes Taktraster
  
  Created: 27.05.2011 08:27:44
  Author:  busser.michael  

*/

enum TCBInterval  {
    tCB_10MS,
    tCB_100MS,
    tCB_1SEK,
    tCB_10SEK,
    tCB_1MIN
};

struct TFlagsTimer {
  unsigned bMilli:1;    //- Flag für 1/1000 Sekunde vergangen
  unsigned b2MS:1;	    //- Flag für 2/1000 Sekunde vergangen
  unsigned b5MS:1;	    //- Flag für 5/1000 Sekunde vergangen
  unsigned b10MS:1;     //- Flag für 1/100 Sekunde
  unsigned b100MS:1;    //- Flag für 1/10 Sekunde
  unsigned bSek:1;      //- Flag für 1 Sekunde
  unsigned b10Sek:1;    //- Flag für 10 Sekunden sind vergangen
  unsigned bMin:1;      //- Flag für 1 Minute ist vergangen
};


union TTimer {
  unsigned char         all;
  struct   TFlagsTimer  flags;
};


typedef void (*timer_callback_t) (void);		//prototype event callback-function passing no arguments

void timer_Setup( void );
void timer_Trigger();

void timer_ClearAll( void );
void timer_ClearMilli( void );
void timer_Clear5MS( void );
void timer_Clear10MS( void );
void timer_Clear100MS( void );
void timer_ClearSek( void );
void timer_Clear10Sek( void );
void timer_ClearMin( void );

void timer_register_Callback( enum TCBInterval cbi, timer_callback_t cbf );
void timer_unregister_Callback( enum TCBInterval cbi );


#endif	//_TIMER_
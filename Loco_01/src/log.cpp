#include <log.h>


U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
// setup the terminal (U8X8LOG) and connect to u8g2 for automatic refresh of the display

uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];
U8X8LOG u8x8log;



void log_setup(){
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  
  u8x8log.begin(u8x8, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8x8log.setRedrawMode(1);		// 0: Update screen with newline, 1: Update screen for every char  
  u8x8log.print("T4EU Rev4\n");
}

void log_print(const String &s){
  u8x8log.print( s );
  //u8x8log.print("\n");
}

void log_print(const char str[]){
  u8x8log.print( str );
  //u8x8log.print("\n");    
}

void log_line( uint8_t line, const char* s){
  u8x8.clearLine( line );
  u8x8.drawString( 0, line, s );
  //u8x8log.print("\n");
}
#ifndef LIBSK_B_OLED_H
  #define LIBSK_B_OLED_H   
/*
DEFAULT SDA PIN FOR ARDUINO UNO = PIN 8
DEFAULT SCL PIN FOR ARDUINO UNO = PIN 9 

This library is designed to work without using the Hardware i2c support. It uses the 
regular I/O Pin from PORTB to do the i2c communication. Some atTiny does not have hardware
support for the i2c. 

Another use is, when you need to use two SSD1306 OLED Modules at the same time ( hardware i2c
can support only one SSD1306 OLED i2c Module at a time, unless you have a SSD1306 OLED with 
a different i2c Address )

NOTE: This "bit bang" version may not be as reliable as the hardware i2c

This Library uses the PORTB since both atMega and atTiny have this same PORT defination.
Use: DDRB for Input/Output Mode
PORTB available for 8-pin atTiny13, atTiny25, atTiny45, atTiny85 
- PB0 to PB4 ( AVR Pin Number for atTiny )
PORTB available for atMega8, atMega168, atmega328 
- PB0 to PB4 ( AVR Pin Number for atMega, Arduino Uno Pin 8,9,10,11,12 )

The main aim is to make it as small as possible to fit the atTiny. This OLED Library only have
font for numbers 0 to 9 for _showChar() function, If you need to display other than numbers, 
use the _showBitmap() function.

1. sk_oled_selectPin(_pinSDA,_pinSCL) - This is optional. if not run SDA=PB0, SCL=PB1
If required different Pin is required, sk_oled_selectPin() must run before the sk_oled_begin()

2. sk_oled_begin() - Run this just once, before other library display functions
3. sk_oled_clearScreen() - Clears the entire OLED screen
4. sk_oled_showBitmap(_row,_col,_a_byt,_len,_len_idx) - display 8-bit bitmap (1-128 pixel)
5. sk_oled_showChar(_row,_col,_cha) - Show a ASCII Char ( 48 to 57 ), number 0 to 9
6. sk_oled_showChar_L(uint8_t _row, uint8_t _col, char _cha) 
_pinSDA - This is either PB0,PB1,PB2,PB3 or PB4 only, this AVR Pin Number
_pinSCL - This is either PB0,PB1,PB2,PB3 or PB4 only, this AVR Pin Number
_row - (0 to 8 - top to bottom ), for Char and Bitmap
_row - (0 to 7 - top to bottom ), for "Char_L" ( each "Char_L" also uses 2 rows )
_col - (0 to 127 pixel - left to right), for Bitmap ( each Byte of Bitmap is 1 pixel )
_col - (0 to 123 pixel - left to right), for Char ( each Char is 5-pixel ) 
_col - (0 to 118 pixel - left to right), for "Char_L" ( each "Char_L" is 10-pixel ) 
_cha     - ASCII Char ( 48 to 57 )
_str     - Array of Chars, terminated by NULL ( 1-pixel gap between the Chars, 2-pixel gap between the "Char_L" )
_a_byt   - Array of Byte ( 8-bits each ) - for _showBitmap()
_len     - Total elements in _a_byt to be used - for _showBitmap()
_len_idx - Starting index from _a_byt ( 0 to _len-1 ) - for _showBitmap()

---

How to install this library into Arduino IDE Software ?

1. Find [Arduino User Program Folder] from the Arduino IDE Software, 
"File | Preferences | Sketchbook Location: [Arduino User Program Folder]"

2. Use the Computer File Browser, 
Look for an existing folder named "libraries" inside the [Arduino User Program Folder]

3. Inside the "libraries" folder, 
Create a new folder named "libsk_oled"

4. Copy "libsk_b_oled.h" into the newly created "libsk_oled" folder
[Arduino User Program Folder] / libraries / libsk_oled / libsk_b_oled.h

Installation DONE!!!

---

After installation, you may want to try out these small test program 

#define F_CPU 16000000UL
#include <libsk_b_oled.h>
int main() {
  // sk_oled_selectPin(PB1,PB2); // for Arduino Uno, PB1=Pin 9, PB2=Pin 10
  sk_oled_begin();
  sk_oled_showChar(3,61,'0');
  while(1) {}
  return 0; 
}
// *Without Arduino setup() and loop()
// Sketch uses 538 bytes (1%) of program storage space. Maximum is 32256 bytes.
// Global variables use 26 bytes (1%) of dynamic memory, leaving 2022 bytes for local variables. Maximum is 2048 bytes.
//
// For very simple Program, can potentially fit atTiny13 (with just 1000bytes program memory, 64bytes dynamic memory)

... or ...

#include <libsk_b_oled.h>
void setup() {   
  // sk_oled_selectPin(PB1,PB2); // for Arduino Uno, PB1=Pin 9, PB2=Pin 10
  sk_oled_begin();
  sk_oled_showChar(3,61,'0');
}
// *With Arduino setup() and loop()
// Sketch uses 850 bytes (2%) of program storage space. Maximum is 32256 bytes.
// Global variables use 35 bytes (1%) of dynamic memory, leaving 2013 bytes for local variables. Maximum is 2048 bytes.
*/

#include <avr/io.h>

#define ADD_I2C_R   0x79 // i2c Read Address, NOT USED
#define ADD_I2C_W   0x78 // i2c Write Address(SSD1306OLED is a write only device)
#define ADD_PAGE    0x22 // select row - ( 0 to   7 ) each is 8 pixel vert
#define ADD_COL     0x21 // select col - ( 0 to 127 ) each is 1 pixel horz
#define ADD_DATA    0x40 // Address to start data transmission
#define ADD_COMM    0x00 // Address to start Command

// Since primary goal is to support the atTiny, we will just limit ourselves to
// use PB0 to PB4 as I/O Pin sinc both atMega and and atTiny have these Pins.

uint8_t _bb_pinSDA = PB0; // Default to PB0 ( Arduino Pin 8 )
uint8_t _bb_pinSCL = PB1; // Default to PB1 ( Arduino Pin 9 )

// --- start function declaration ---
void _sk_oled_i2c_start();
void _sk_oled_i2c_stop();
uint8_t _sk_oled_i2c_write(uint8_t dat);
uint8_t _sk_oled_i2c_read(uint8_t ack);
void _sk_oled_i2c_startComm();
void _sk_oled_i2c_startData();
void _sk_oled_setRowCol(uint8_t _row, uint8_t _col );
void sk_oled_selectPin(uint8_t _pinSDA, uint8_t _pinSCL);
void sk_oled_begin();
void sk_oled_clearScreen();
void sk_oled_showBitmap(uint8_t _row, uint8_t _col, uint8_t *_bitmap, uint8_t _len, uint8_t _len_idx);
void sk_oled_showChar(uint8_t _row, uint8_t _col, char _cha);
void sk_oled_showChar_L(uint8_t _row, uint8_t _col, char _cha) ;
// --- end function declration ---

const char _sk_oled_font[] = {
 0b00111110,0b01010001,0b01001001,0b01000101,0b00111110,  /* 0 (ascii 48) */
 0b00000000,0b01000010,0b01111111,0b01000000,0b00000000,  /* 1 */
 0b01000010,0b01100001,0b01010001,0b01001001,0b01000110,  /* 2 */
 0b00100001,0b01000001,0b01000101,0b01001011,0b00110001,  /* 3 */
 0b00011000,0b00010100,0b00010010,0b01111111,0b00010000,  /* 4 */
 0b00100111,0b01000101,0b01000101,0b01000101,0b00111001,  /* 5 */
 0b00111100,0b01001010,0b01001001,0b01001001,0b00110000,  /* 6 */
 0b00000001,0b01110001,0b00001001,0b00000101,0b00000011,  /* 7 */
 0b00110110,0b01001001,0b01001001,0b01001001,0b00110110,  /* 8 */
 0b00000110,0b01001001,0b01001001,0b00101001,0b00011110   /* 9 (ascii 57) */
};  

void _sk_oled_i2c_start() {
  (PORTB |= (1<< _bb_pinSDA)); 
   __asm__("NOP");
  (PORTB |= (1<< _bb_pinSCL)); 
   __asm__("NOP");
  (PORTB &= ~(1<<_bb_pinSDA));
   __asm__("NOP");
  (PORTB &= ~(1<<_bb_pinSCL));
   __asm__("NOP");
}

void _sk_oled_i2c_stop() {
  (PORTB &= ~(1<<_bb_pinSDA));
   __asm__("NOP");
  (PORTB |= (1<< _bb_pinSCL));
   __asm__("NOP");
  (PORTB |= (1<< _bb_pinSDA));
   __asm__("NOP");
}

uint8_t _sk_oled_i2c_write(uint8_t dat) {
uint8_t ack;  
  for(uint8_t i=0; i < 8; i++) {
    (dat & 0x80) ? (PORTB |= (1<< _bb_pinSDA)) : (PORTB &= ~(1<< _bb_pinSDA));
    dat<<=1;      
     __asm__("NOP");
    (PORTB |= (1<< _bb_pinSCL));
     __asm__("NOP");
    (PORTB &= ~(1<<_bb_pinSCL));
     __asm__("NOP");
  }
  (PORTB |= (1<<_bb_pinSDA));
  (PORTB |= (1<<_bb_pinSCL));
   __asm__("NOP");
  ack = !(PINB & (1<<_bb_pinSDA)); // Acknowledge bit
  (PORTB &= ~(1<<_bb_pinSCL));
  return ack;
}

uint8_t _sk_oled_i2c_read(uint8_t ack) {
uint8_t dat = 0;
  (PORTB |= (1<<_bb_pinSDA));
  for( uint8_t i=0; i < 8; i++) {
    dat <<= 1;
    do {
      (PORTB |= (1<<_bb_pinSCL));
    } while((PINB & (1<<_bb_pinSCL)) == 0);  // SCL stretching
     __asm__("NOP");
    if((PINB & (1<<_bb_pinSDA))) dat |=1;
     __asm__("NOP");
    (PORTB &= ~(1<<_bb_pinSCL));
  }
  ack ? (PORTB &= ~(1<< _bb_pinSDA)) : (PORTB |= (1<< _bb_pinSDA));
  (PORTB |=  (1<<_bb_pinSCL));
   __asm__("NOP");
  (PORTB &= ~(1<<_bb_pinSCL));
  (PORTB |=  (1<<_bb_pinSDA));
  return(dat);
}

void _sk_oled_i2c_startComm() { 
  _sk_oled_i2c_start();
  _sk_oled_i2c_write(ADD_I2C_W);
  _sk_oled_i2c_write(ADD_COMM);
}

void _sk_oled_i2c_startData() {
  _sk_oled_i2c_start();
  _sk_oled_i2c_write(ADD_I2C_W);
  _sk_oled_i2c_write(ADD_DATA); 
}

void _sk_oled_setRowCol(uint8_t _row, uint8_t _col ) { 
  _sk_oled_i2c_startComm();
  _sk_oled_i2c_write(ADD_PAGE); // Starting Row position
  _sk_oled_i2c_write(_row);_sk_oled_i2c_write(7);
  _sk_oled_i2c_write(ADD_COL);  // Starting Col position
  _sk_oled_i2c_write(_col);_sk_oled_i2c_write(127);  
  _sk_oled_i2c_stop();
}

void sk_oled_selectPin(uint8_t _pinSDA, uint8_t _pinSCL) {
// You must run this before the _sk_oled_begin() if you wish to change the SDA and 
// SCL pin from the default PB0 and PB1. _pinSDA and _pinSCL are coded in AVR Pin
// Number PB0,PB1,PB2,PB3,PB4 equals to Arduino Uno Pin 8,9,10,11,12
// Reason?: We want to support the atTiny together with atMega at the same time, both
// have these same AVR Pin Numbers. No error correction are coded here to reduce
// program size. Make sure they are PB0,PB1,PB2,PB3,PB4 Only
  _bb_pinSDA = _pinSDA;
  _bb_pinSCL = _pinSCL;  
}

void sk_oled_begin() {
uint8_t initData[26]= { 
  0xAE,       // 0xAE Display OFF
  0xD5,0x80,  // 0xD5 Display Clock Div
  0xA8,0x3F,  // 0xA8 Set Multiplex: 0x1F for 128x32, 0x3F for 128x64
  0xD3,0x00,  // 0xD3 Set Display offset 0
  0x40,       // 0x40 Start Line 0
  0x8D,0x14,  // 0x8D Set Charge pump Enabled,internal VCC
  0x20,0x00,  // 0x20 Set Memory Mode: 0x00 for H, 0x01 for V
  0xA1,       // 0xA1 Rotate 180
  0xC8,       // 0xC8 Com Scan Dec
  0xDA,0x12,  // 0xDA Set COM Pins, 0x12 for h=64 or 0x02 for h=32
  0x81,0xCF,  // 0x81 Set Contrast
  0xD9,0xF1,  // 0xD9 Set pre-charge period
  0xDB,0x40,  // 0xDB Set vcom detect
  0xA4,       // 0xA4 all pixel 0N = A4, all pixel OFF=A5
  0xA6,       // 0xA6 Normal Display
  0x2E,       // 0x2E No Scroll
  0xAF        // 0xAF Display on
  };
  
  DDRB |= ( (1<<_bb_pinSDA) | (1<<_bb_pinSCL) );  
  _sk_oled_i2c_start(); // Start the i2c communication

  // Intital Setup Values for the SSD1306 I2C OLED 64x128 pixel only
  for (uint8_t i=0; i < 26; i++) {
    _sk_oled_i2c_startComm();    
    _sk_oled_i2c_write(initData[i]);
    _sk_oled_i2c_stop();    
  } 
  sk_oled_clearScreen(); // Clear screen and ready for use
}

void sk_oled_clearScreen() {
  _sk_oled_setRowCol(0,0); 
  _sk_oled_i2c_startData(); 
  for (uint8_t r=0; r < 8; r++) {
    for (uint8_t c=0; c < 128; c++) {
      _sk_oled_i2c_write(0);
    }
  } 
  _sk_oled_i2c_stop();
  _sk_oled_setRowCol(0,0);
}

void sk_oled_showBitmap(uint8_t _row, uint8_t _col, uint8_t *_bitmap, uint8_t _len, uint8_t _len_idx) {
  _sk_oled_setRowCol(_row,_col);
  _sk_oled_i2c_startData();
  for (uint8_t i=0; i < _len; i++) {
    _sk_oled_i2c_write(_bitmap[i+_len_idx]);
  }
  _sk_oled_i2c_stop();
}  

void sk_oled_showChar(uint8_t _row, uint8_t _col, char _cha) {
  
  if (_cha < 48 || _cha > 57 ) return; // Ascii 48 to 57 only, Ignore the rest
  
  _sk_oled_setRowCol(_row,_col);  
  _sk_oled_i2c_startData();
  for (int i=0; i < 5; i++) {  // EACH FONT is 5 byte in FONT ARRAY
    _sk_oled_i2c_write(_sk_oled_font[((_cha-48)*5)+i]); 
  } 
  _sk_oled_i2c_stop();   
}

void sk_oled_showChar_L(uint8_t _row, uint8_t _col, char _cha) {
char cTmp;  
char enlargeMask[]= {
  0b00000000,0b00000011,0b00001100,0b00001111,0b00110000,0b00110011,0b00111100,0b00111111,
  0b11000000,0b11000011,0b11001100,0b11001111,0b11110000,0b11110011,0b11111100,0b11111111
  };
  
  if (_cha < 48 || _cha > 57 ) return; // Ascii 48 to 57 only, Ignore the rest

  // Double Size, Top Part
  _sk_oled_setRowCol(_row,_col); 
  _sk_oled_i2c_startData();
  for (uint8_t i=0; i < 5; i++) {   
    cTmp = _sk_oled_font[((_cha-48)*5)+i];
    cTmp = cTmp & 0x0F;       // store LOW BYTE
    cTmp = enlargeMask[cTmp]; // mapped into Enlargement Mask
    _sk_oled_i2c_write(cTmp); // write to OLED
    _sk_oled_i2c_write(cTmp); // same thing, to double size
  }
  _sk_oled_i2c_stop(); 
 
  // Double Size, Bottom Part        
  _sk_oled_setRowCol(_row+1,_col); 
  _sk_oled_i2c_startData(); 
  for (uint8_t i=0; i < 5; i++) {
    cTmp = _sk_oled_font[((_cha-48)*5)+i];
    cTmp = (cTmp&0xF0) >> 4 ; // store HIGH BYTE
    cTmp = enlargeMask[cTmp]; // mapped into Enlargement Mask
    _sk_oled_i2c_write(cTmp); // write to OLED
    _sk_oled_i2c_write(cTmp); // same thing, to double size      
  } 
  _sk_oled_i2c_stop();
}

#endif

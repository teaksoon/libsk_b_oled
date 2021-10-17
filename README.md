This library libsk_b_oled is designed to work without using the Hardware i2c support. It uses the regular I/O Pin from PORTB to do the i2c communication. Some atTiny does not have hardware support for the i2c. Another use is, when you need to use two SSD1306 OLED Modules at the same time ( hardware i2c can support only one SSD1306 OLED i2c Module at a time, unless you have a SSD1306 OLED with a different i2c Address ) 

- NOTE: This "bit bang" version may not be as reliable as the hardware i2c

DEFAULT SDA PIN FOR ARDUINO UNO = PIN 8
DEFAULT SCL PIN FOR ARDUINO UNO = PIN 9 

This Library uses the PORTB since both atMega and atTiny have this same PORT defination.
- DDRB for Input/Output Mode 
- PORTB  is available for most of the AVR micro-controllers, since atTiny only have PB0 to PB4, we will just stick those few Pins.
- PB0 to PB4 ( AVR Pin Number for atTiny )
- PB0 to PB4 ( AVR Pin Number for atMega, Arduino Uno Pin 8,9,10,11,12 )

The main aim is to make it as small as possible to fit the atTiny. This OLED Library only have font for numbers 0 to 9 for _showChar() function, If you need to display other than numbers, use the _showBitmap() function.

1. sk_oled_selectPin(_pinSDA,_pinSCL) - This is optional. if not run SDA=PB0, SCL=PB1. If required different Pin is required, sk_oled_selectPin() must run before the sk_oled_begin()
2. sk_oled_begin() - Run this just once, before other library display functions
3. sk_oled_clearScreen() - Clears the entire OLED screen
4. sk_oled_showBitmap(_row,_col,_a_byt,_len,_len_idx) - display 8-bit bitmap (1-128 pixel)
5. sk_oled_showChar(_row,_col,_cha) - Show a ASCII Char ( 32 to 126 )
6. sk_oled_showChar_L(_row,_col,_cha) - Same as "_showChar()" but doubled in size
- _row - (0 to 8 - top to bottom ), for Char and Bitmap
- _row - (0 to 7 - top to bottom ), for "Char_L" ( each "Char_L" also uses 2 rows )
- _col - (0 to 127 pixel - left to right), for Bitmap ( each Byte of Bitmap is 1 pixel )
- _col - (0 to 123 pixel - left to right), for Char ( each Char is 5-pixel )
- _col - (0 to 118 pixel - left to right), for "Char_L" ( each "Char_L" is 10-pixel )
- _cha - ASCII Char ( 48 to 57 ) Numbers only 0 to 9
- _a_byt   - Array of Byte ( 8-bits each ) - for _showBitmap()
- _len     - Total elements in _a_byt to be used - for _showBitmap()
- _len_idx - Starting index from _a_byt ( 0 to _len-1 ) - for _showBitmap()

------

How to install this library into the Arduino IDE Software ?

- Find [Arduino User Program Folder] from the Arduino IDE Software,
"File | Preferences | Sketchbook Location: [Arduino User Program Folder]"
Use the Computer File Browser,
-  Look for an existing folder named "libraries" inside the [Arduino User Program Folder]
Inside the "libraries" folder,
-  Create a new folder named "libsk_oled"
Copy "libsk_b_oled.h" into the newly created "libsk_oled" folder
-  [Arduino User Program Folder] / libraries / libsk_oled / libsk_b_oled.h


Installation DONE!!!

------

After installation, you may want to try out these small test program 

1. #define F_CPU 16000000UL
2. #include <libsk_b_oled.h>
3. int main() {
4.   // sk_oled_selectPin(PB1,PB2); // for Arduino Uno, PB1=Pin 9, PB2=Pin 10
5.   sk_oled_begin();
6.   sk_oled_showChar(3,61,'0');
7.   while(1) {}
8.   return 0; 
9. }

// *Without Arduino setup() and loop()
// Sketch uses 538 bytes (1%) of program storage space. Maximum is 32256 bytes.
// Global variables use 26 bytes (1%) of dynamic memory, leaving 2022 bytes for local variables. Maximum is 2048 bytes.
//
// For very simple Program, can potentially fit atTiny13 (with just 1000bytes program memory, 64bytes dynamic memory)

1. #include <libsk_b_oled.h>
2. void setup() {   
3. // sk_oled_selectPin(PB1,PB2); // for Arduino Uno, PB1=Pin 9, PB2=Pin 10
4.   sk_oled_begin();
5.   sk_oled_showChar(3,61,'0');
6. }
7. void loop() {}

// *With Arduino setup() and loop()
// Sketch uses 850 bytes (2%) of program storage space. Maximum is 32256 bytes.
// Global variables use 35 bytes (1%) of dynamic memory, leaving 2013 bytes for local variables. Maximum is 2048 bytes.

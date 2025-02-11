/**
 * @file    SparkFun_LED_8x7.h
 * @brief   Library for the SparkFun 8x7 Charlieplex LED Array
 * @author  Shawn Hymel, Jim Lindblom (SparkFun Electronics)
 *
 * @copyright	This code is public domain but you buy me a beer if you use
 * this and we meet someday (Beerware license).
 *
 * This library controls the 8x7 Charlieplex LED array. Note that Timer2 is used
 * in this library to control the LED refresh. You will not be able to use it
 * for other uses.
 *
 * The graphics algorithms are based on Jim Lindblom's Micro OLED library:
 * https://github.com/sparkfun/Micro_OLED_Breakout
 *
 * Relies on the Chaplex library written by Stefan Götze.
 */
 
#ifndef SparkFun_LED_8x7_H
#define SparkFun_LED_8x7_H

#include <Arduino.h>
#include <Chaplex.h>

#include "LED_Font_1.h"

/* Debug */
#define LED_8X7_DEBUG   0

/* Constants */
static const unsigned int NUM_CHAPLEX_PINS = 8; // Number of pins  
static const unsigned int DEFAULT_SHIFT_DELAY = 50; // Ticks before scrolling
static const unsigned int MAX_CHARS = 100;      // Maximum characters to scroll
static const unsigned int CHAR_OFFSET = 0x20;   // Starting place for ASCII
static const unsigned int CHAR_SPACE = 1;       // Number columns between chars
static const unsigned int END_SPACE = 8;        // Number columns after text
static const unsigned int COL_SIZE = 7;         // Number LEDs in a column
static const unsigned int ROW_SIZE = 8;         // Number LEDs in a row

/* Derived constants */
static const unsigned int NUM_LEDS = COL_SIZE * ROW_SIZE;
static const unsigned int ALL_BUT_LAST_COL = NUM_LEDS - COL_SIZE;

/* Global variables */

#if defined __AVR_ATmega168__ || \
    defined __AVR_ATmega328__ || \
    defined __AVR_ATmega328P__

ISR(TIMER2_OVF_vect);

#elif defined __AVR_ATmega32U4__

ISR(TIMER1_OVF_vect); 

#endif

/* LED Array class */
class SparkFun_LED_8x7 {
    
    /* The ISR is our friend! It can call our functions. */

#if defined __AVR_ATmega168__ || \
    defined __AVR_ATmega328__ || \
    defined __AVR_ATmega328P__

    friend void TIMER2_OVF_vect();

#elif defined __AVR_ATmega32U4__
    
    friend void TIMER1_OVF_vect();
    
#endif
    
public:
    
    /* Initialization */
    SparkFun_LED_8x7();
    ~SparkFun_LED_8x7();
    bool init(byte pins[NUM_CHAPLEX_PINS]);
    
    /* LED drawing methods */
    void display();
    void clear();
    void pixel(uint8_t x, uint8_t y, uint8_t on = 1);
    void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
    void rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
    void rectFill(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
    void circle(uint8_t x0, uint8_t y0, uint8_t radius);
    void circleFill(uint8_t x0, uint8_t y0, uint8_t radius);
    void drawBitmap(const byte bitmap[NUM_LEDS]);
    
    /* Scrolling text methods */
    void scrollText(char *in_string);
    void scrollText(char *in_string, int times, bool blocking = false);
    void stopScrolling();
    
    /* Support methods */
    uint8_t getArrayWidth();
    uint8_t getArrayHeight();

private:

    /* Helper functions */
    unsigned char getPGMFontByte(int idx, int offset = 0);
    void swap(uint8_t &a, uint8_t &b);

    /* Interrupt service routine that is called by the system's ISR */
    inline void isr();

    /* Members */
    Chaplex *chaplex_;          /// Chaplex object for controlling the LEDs
    byte frame_buffer_[56];     /// Storing the state of each LED to be written
    uint8_t timer2_count_;      /// Stores the next start point for Timer2
    byte *scroll_buf_;          /// Buffer of text graphics to scroll
    volatile byte scrolling_;   /// Boolean to indicate if we are scrolling text
    unsigned int shift_count_;  /// Count number of ticks before shifting text
    unsigned int shift_delay_;  /// Number of ticks to wait before shifting text
    unsigned int scroll_index_; /// Index of where to scroll text
    unsigned int scroll_len_;   /// Number of bytes in scroll_buf_
    unsigned int scroll_times_; /// Number of times to scroll text
    unsigned int scroll_count_; /// Counter for times text has scrolled
    static const charlieLed charlie_leds_[]; /// Relative location of the LEDs
};

/* Yup, we're going to declare a global instance of our object */
extern SparkFun_LED_8x7 Plex;

#endif // SparkFun_LED_8x7_H

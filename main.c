/*
 * Project: Ultrasonic Distance Measurement using ATmega32
 * Author: Kushal Pitaliya
 * Description: Measures distance using HC-SR04 and displays on 16x2 LCD.
 *              Uses Timer1 and External Interrupt INT0.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

// Configuration
#define F_CPU 1000000UL     // 1 MHz Clock Frequency

// LCD Pin Definitions
#define LCD_DATA_PORT   PORTB
#define LCD_CTRL_PORT   PORTD
#define LCD_ENABLE_PIN  5
#define LCD_RS_PIN      6

// Global Variables
static volatile int pulse_width = 0;
static volatile int edge_state = 0; // 0 = Rising Edge, 1 = Falling Edge

// Function Prototypes
void lcd_command(unsigned char command);
void lcd_data(unsigned char character);
void lcd_string(char *string_of_characters);
void init_hardware(void);

int main(void)
{
    char show_a[16];
    int16_t distance_cm = 0;

    init_hardware();
    sei(); // Enable Global Interrupts

    while(1)
    {
        // 1. Send Trigger Pulse to HC-SR04 (PD0)
        LCD_CTRL_PORT |= (1<<0);
        _delay_us(15);
        LCD_CTRL_PORT &= ~(1<<0);

        // 2. Calculate Distance
        // Formula: Distance in cm = (Pulse width in uS) / 58
        distance_cm = pulse_width / 58;

        // 3. Display on LCD
        lcd_command(0x01); // Clear screen
        _delay_ms(2);
        
        lcd_string("CIRCUIT DIGEST");
        
        lcd_command(0xC0); // Move cursor to 2nd line
        lcd_string("DIST: ");
        
        itoa(distance_cm, show_a, 10); // Convert integer to string
        lcd_string(show_a);
        lcd_string(" cm");

        _delay_ms(500); // Refresh rate
    }
}

ISR(INT0_vect)
{
    if (edge_state == 1) // Falling Edge detected (End of Echo)
    {
        TCCR1B = 0;          // Stop Timer
        pulse_width = TCNT1; // Save Timer value
        TCNT1 = 0;           // Reset Timer
        edge_state = 0;      // Next state: Look for Rising Edge
    }
    else if (edge_state == 0) // Rising Edge detected (Start of Echo)
    {
        TCCR1B |= (1<<CS10); // Start Timer1 (No prescaler)
        edge_state = 1;      // Next state: Look for Falling Edge
    }
}

void init_hardware(void)
{
    DDRA = 0xFF;    // Optional Port
    DDRB = 0xFF;    // LCD Data Port Output
    DDRD = 0b11111011; // PD0 (Trig) Output, PD2 (Echo/INT0) Input, PD5/6 LCD Ctrl

    // Interrupt Setup
    GICR |= (1<<INT0);   // Enable INT0
    MCUCR |= (1<<ISC00); // Trigger on Any Logical Change

    // LCD Initialization
    lcd_command(0x01); // Clear Screen
    _delay_ms(50);
    lcd_command(0x38); // 8-bit mode
    _delay_ms(50);
    lcd_command(0x0C); // Display ON, Cursor OFF
    _delay_ms(50);
}

void lcd_command(unsigned char command)
{
    LCD_DATA_PORT = command;
    LCD_CTRL_PORT &= ~(1<<LCD_RS_PIN); // RS = 0 (Command)
    LCD_CTRL_PORT |= (1<<LCD_ENABLE_PIN);
    _delay_ms(2);
    LCD_CTRL_PORT &= ~(1<<LCD_ENABLE_PIN);
    LCD_DATA_PORT = 0;
}

void lcd_data(unsigned char character)
{
    LCD_DATA_PORT = character;
    LCD_CTRL_PORT |= (1<<LCD_RS_PIN);   // RS = 1 (Data)
    LCD_CTRL_PORT |= (1<<LCD_ENABLE_PIN);
    _delay_ms(2);
    LCD_CTRL_PORT &= ~(1<<LCD_ENABLE_PIN);
    LCD_DATA_PORT = 0;
}

void lcd_string(char *string_of_characters)
{
    while(*string_of_characters > 0)
    {
        lcd_data(*string_of_characters++);
    }
}

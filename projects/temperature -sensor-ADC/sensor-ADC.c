#include <io.h>
#include <mega16.h>
#include <delay.h>
#include <alcd.h>
#include <stdio.h>
#include <stdlib.h>

int n = 0; 
int low, high;
int t;
unsigned char str[10]; 
unsigned char count[10];
int i = 0;

void main(void)
{
    // Set PORTC and PORTB as output
    DDRC |= 0xFF;
    DDRB |= 0xFF;
    
    // Initialize the LCD with 16 columns
    lcd_init(16);
    
    // Set PORTD as input
    DDRD &= 0x00; 
    
    // Configure ADC (Analog-to-Digital Converter)
    // REFS1 = 0, ADLAR = 0, MUX[4:0] = 0, REFS0 = 1
    ADMUX &= ~((1<<REFS1) | (1<<ADLAR) | (1<<MUX4) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0));
    ADMUX |= (1<<REFS0);
    
    // Enable ADC, start conversion, set prescaler to 64
    // ADEN = 1, ADSC = 1, ADPS[2:0] = 011
    ADCSRA &= ~((1<<ADATE) | (1<<ADIF) | (1<<ADIE) | (1<<ADPS0));
    ADCSRA |= ((1<<ADEN) | (1<<ADSC) | (1<<ADPS1) | (1<<ADPS2));

    while (1)
    {   
        // Wait for ADC conversion to complete
        while ((ADCSRA & (1<<ADIF)) == 0)
        {
            lcd_clear();
            
            // If button connected to PIND0 is pressed, increment counter
            if ((PIND & 0x01) == 0) {
                itoa(i, count);
                lcd_gotoxy(0, 1);
                lcd_puts(count);
                i++;
                delay_ms(500);
            } 
            
            // If button connected to PIND1 is pressed, decrement counter
            if ((PIND & 0x02) == 0) {
                itoa(i, count);
                lcd_gotoxy(0, 1);
                lcd_puts(count);
                i--; 
                delay_ms(500);
            }

            // Read ADC value
            low = (int) ADCL;
            high = (int) ADCH;
            n = (int) high * 256 + low;
            t = 0.5 * n; // Convert ADC value to temperature or other meaningful value
            itoa(t, str);
            lcd_gotoxy(0, 0);
            lcd_puts(str);
            delay_us(300);

            // Clear ADC interrupt flag and restart ADC conversion
            ADCSRA |= (1<<ADIF);
            delay_ms(400);
            ADCSRA |= ((1<<ADEN) | (1<<ADSC) | (1<<ADPS1) | (1<<ADPS2));

            // Control PORTC based on comparison of t and i
            if (t <= i) {
                PORTC = 0b11111111; // Set all PORTC pins high
            } else if (t >= i) {
                PORTC = 0b00000000; // Set all PORTC pins low
            }
        }
    }
}

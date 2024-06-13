#include <io.h>
#include <mega16.h>
#include <delay.h>

int i = 0;

interrupt[EXT_INT0] void ext_int0_isr(void) {
    delay_ms(100); 
    i++;
}


interrupt[EXT_INT1] void ext_int1_isr(void) {
    delay_ms(100); 
    i--;
}

void main(void) {
    unsigned int left, right;
    unsigned int seg[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 
                            0x6D, 0x7C, 0x27, 0x7F, 0x67};
    
    DDRC = 0xFF; // 7-segment display
    DDRB = 0xFF; // transistors
    DDRA = 0x00; //inputs
    
    #asm ("sei"); // Enable global interrupts
    GICR |= 0b11000000; // Enable INT0 and INT1
    MCUCR |= 0b00000010; 
    MCUCR &= 0b00000000; 
    while (1) {
        for (i = 0; i <= 99;) { 
            right = i % 10; // Get the right digit
            left = i / 10;  // Get the left digit
            
            // Display right digit
            PORTC = seg[right];
            PORTB = 0b11111110; 
            delay_ms(80);
            PORTB = 0xFF;
            
            // Display left digit
            PORTC = seg[left];
            PORTB = 0b11111101;
            delay_ms(80);
            PORTB = 0xFF;
            
            // Increment i if button on PINA0 is pressed
            if ((PINA & 0x01) == 0) {
                i++;
            }
            
            // Decrement i if button on PINA1 is pressed
            if ((PINA & 0x02) == 0) {
                if (i == 0) {
                    i = 99; 
                }
                i--;
            }
            
            // Countdown if button on PINA2 is pressed
            if ((PINA & 0x04) == 0) {
                for (; i > 0; i--) {
                    right = i % 10;
                    left = i / 10;
                    
                    // Display right digit
                    PORTC = seg[right];
                    PORTB = 0b11111110; 
                    delay_ms(70);
                    PORTB = 0xFF;
                    
                    // Display left digit
                    PORTC = seg[left];
                    PORTB = 0b11111101;
                    delay_ms(70);
                    PORTB = 0xFF;
                }
            }
        }
    }
}

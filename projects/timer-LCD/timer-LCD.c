#include <stdlib.h>
#include <alcd.h>
#include <io.h>
#include <mega16.h>
#include <delay.h>

// Buffer for string conversion
unsigned char str[10];
int h;

interrupt [EXT_INT0] void ext_int0_isr(void) {
    lcd_gotoxy(3, 1);
    itoa(h, str);
    lcd_puts(str);
    h++;
}

interrupt [EXT_INT1] void ext_int1_isr(void) {
    lcd_gotoxy(3, 1);
    itoa(h, str);
    lcd_puts(str);
    h--;
}

void main(void) {
    int m, s;

    // Initialize LCD with 16 columns
    lcd_init(16);

    // Set PORTC as output for LCD
    DDRC = 0xFF;

    // Display initial message on the LCD
    lcd_gotoxy(4, 0); 
    lcd_putsf("timer");
    delay_ms(50); 
    
    // Enable global interrupts
    #asm ("sei"); 

    // Enable external interrupts INT0 and INT1
    GICR |= 0b11000000;
    MCUCR |= 0b00000010;
    MCUCR &= 0b00000000; 

    while (1) {  
        for (h = 0; h < 24; h++) { 
            for (m = 0; m < 60; m++) {  
                for (s = 0; s < 60; s++) {
                    lcd_clear();

                    // Display hours
                    lcd_gotoxy(3, 1);
                    itoa(h, str);
                    lcd_puts(str);
                    lcd_gotoxy(5, 1);
                    lcd_putsf(":"); 

                    // Display minutes
                    lcd_gotoxy(6, 1);
                    itoa(m, str);
                    lcd_puts(str);
                    lcd_gotoxy(8, 1); 
                    lcd_putsf(":");

                    // Display seconds
                    lcd_gotoxy(9, 1);
                    itoa(s, str);
                    lcd_puts(str);

                    // Delay to simulate real-time clock
                    delay_ms(100);
                }
            }
        }

        lcd_clear(); 
    }
}

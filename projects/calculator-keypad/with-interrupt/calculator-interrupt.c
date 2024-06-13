#include <io.h>
#include <mega16.h>
#include <alcd.h>
#include <delay.h>
#include <stdlib.h>
#include <stdio.h>

//WITH INTERRUPT

unsigned int num1=0; 
unsigned int num2=0; 
bit flag1=0;     
bit flag2=0;     
unsigned char str[16]; 
int mode = 0;

void number(int n) {
    if (flag1 == 0) {
        num1 = (num1) * 10;
        num1 += n;
    } else if (flag1 == 1) {
        num2 = (num2) * 10;
        num2 += n;
    }          
}

void display_result(unsigned int answer){
    num1 = answer;
    num2 = 0;
    lcd_clear();
    itoa(answer, str);
    lcd_puts(str);
    flag2 = 1;
}
void sum() {
    int answer = num1 + num2;
    display_result(answer);

}

void sub() {
    int answer = num1 - num2;
    display_result(answer);
}
void mul() {
    int answer = num1 * num2; 
   display_result(answer);
}

void div() {
    int answer = num1 / num2;
    display_result(answer);
}

interrupt[EXT_INT0] void ext_int0_isr(void) {
while (1) {
        // Row 1
        PORTA = 0b00000001;
        delay_ms(60);
        if ((PINA & 0b11110000) == 0b00010000) {
            lcd_putsf("7");
            number(7);
        } else if ((PINA & 0b11110000) == 0b00100000) {
            lcd_putsf("8");
            number(8);
        } else if ((PINA & 0b11110000) == 0b01000000) {
            lcd_putsf("9");
            number(9);
        } else if ((PINA & 0b11110000) == 0b10000000) {
            if (flag1 == 0 || flag2 == 1) {
                flag1 = 1;    // enter second number
                lcd_putchar('/');
                flag2 = 0; // Start new calculation
                mode = 1;  
            }
        }

        // Row 2
        PORTA = 0b00000010;
        delay_ms(60);
        if ((PINA & 0b11110000) == 0b00010000) {
            lcd_putsf("4");
            number(4);
        } else if ((PINA & 0b11110000) == 0b00100000) {
            lcd_putsf("5");
            number(5);
        } else if ((PINA & 0b11110000) == 0b01000000) {
            lcd_putsf("6");
            number(6);
        } else if ((PINA & 0b11110000) == 0b10000000) {
            if (flag1 == 0 || flag2 == 1) {
                flag1 = 1;
                flag2 = 0;
                lcd_putchar('*');
                mode = 2;  
            }
        }

        // Row 3
        PORTA = 0b00000100;
        delay_ms(60);
        if ((PINA & 0b11110000) == 0b00010000) {
            lcd_putsf("1");
            number(1);
        } else if ((PINA & 0b11110000) == 0b00100000) {
            lcd_putsf("2");
            number(2);
        } else if ((PINA & 0b11110000) == 0b01000000) {
            lcd_putsf("3");
            number(3);
        } else if ((PINA & 0b11110000) == 0b10000000) {
            if (flag1 == 0 || flag2 == 1) {
                flag1 = 1;
                flag2 = 0;
                lcd_putchar('-');
                mode = 3; 
            }
        }

        // Row 4
        PORTA = 0b00001000;
        delay_ms(60);
        if ((PINA & 0b11110000) == 0b00010000) {
            num1 = 0;
            num2 = 0;
            flag1 = 0;
            flag2 = 0;
            lcd_clear();
        } else if ((PINA & 0b11110000) == 0b00100000) {
            lcd_putsf("0");
            number(0);
        } else if ((PINA & 0b11110000) == 0b01000000) {
            switch (mode) {
                case 1:
                    div();
                    break;
                case 2:
                    mul();
                    break;
                case 3:
                    sub();
                    break;
                case 4:
                    sum();
                    break;
            }
        } else if ((PINA & 0b11110000) == 0b10000000) {
            if (flag1 == 0 || flag2 == 1) {
                flag1 = 1;
                flag2 = 0;
                lcd_putchar('+');
                mode = 4; 
            }
        }
    }
}

void main(void) {
    DDRA = 0b00001111;
    lcd_init(16);
    #asm("sei");
    GICR |= 0b01000000; // Enable INT0
    MCUCR |= 0b00001100; //  rising 

    while (1) {
        // waiting for interrupts
    }
}

#include <io.h>
#include <mega16.h>
#include <alcd.h>
#include <delay.h>
#include <stdlib.h>
#include <stdio.h>

// Variables to store the numbers for calculation
unsigned long num1 = 0;
unsigned long num2 = 0;

// Flags for controlling input and calculation process
bit flag1 = 0;  // Flag to indicate input for the first or second number
bit flag2 = 0;  // Flag to indicate continuation after a calculation

// Buffer for displaying strings on the LCD
unsigned char str[16];

// Mode to determine the current operation
int mode = 0;   // Mode: 0 = no operation, 1 = division, 2 = multiplication, 3 = subtraction, 4 = addition

// Function to add a digit to the current number
void number(int n) {
    if (flag1 == 0) { 
        num1 = (num1 * 10) + n;  // Add digit to num1
    } else if (flag1 == 1) { 
        num2 = (num2 * 10) + n;  // Add digit to num2
    }
}

// Function to display the result on the LCD
void display_result(unsigned long answer) {
    num1 = answer;  // Set result as the first number for next calculation
    num2 = 0;       // Reset the second number
    lcd_clear();    // Clear the LCD
    sprintf(str, "%lu", answer);  // Convert result to string
    lcd_puts(str);  // Display result on the LCD
    flag2 = 1;      // Set flag to indicate continuation
}

// Function to perform addition
void sum() {
    unsigned long answer = num1 + num2;
    display_result(answer);
}

// Function to perform subtraction
void sub() {
    unsigned long answer = num1 - num2;
    display_result(answer);
}

// Function to perform multiplication
void mul() {
    unsigned long answer = num1 * num2;
    display_result(answer);
}

// Function to perform division
void div() {
    unsigned long answer = num1 / num2;
    display_result(answer);
}

void main(void) {
    // Set PORTA pins 0-3 as output
    DDRA = 0b00001111;
    
    // Initialize the LCD with 16 columns
    lcd_init(16);

    while (1) {
        // Check the first row of the keypad
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
                flag1 = 1;    // Start input for the second number
                lcd_putchar('/');
                flag2 = 0;    // Reset continuation flag
                mode = 1;     // Set mode to division
            }
        }

        // Check the second row of the keypad
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
                flag1 = 1;    // Start input for the second number
                flag2 = 0;    // Reset continuation flag
                lcd_putchar('*');
                mode = 2;     // Set mode to multiplication
            }
        }

        // Check the third row of the keypad
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
                flag1 = 1;    // Start input for the second number
                flag2 = 0;    // Reset continuation flag
                lcd_putchar('-');
                mode = 3;     // Set mode to subtraction
            }
        }

        // Check the fourth row of the keypad
        PORTA = 0b00001000;
        delay_ms(60);
        if ((PINA & 0b11110000) == 0b00010000) {
            // Clear everything and reset flags
            num1 = 0;
            num2 = 0;
            flag1 = 0;
            flag2 = 0;
            lcd_clear();
        } else if ((PINA & 0b11110000) == 0b00100000) {
            lcd_putsf("0");
            number(0);
        } else if ((PINA & 0b11110000) == 0b01000000) {
            // Perform the calculation based on the mode
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
                flag1 = 1;    // Start input for the second number
                flag2 = 0;    // Reset continuation flag
                lcd_putchar('+');
                mode = 4;     // Set mode to addition
            }
        }
    }
}

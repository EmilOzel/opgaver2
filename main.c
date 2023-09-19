#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "I2C.h"
#include "ssd1306.h"

void init() {
    DDRG &= ~(1 << PG5); // Button as input
    PORTG |= (1 << PG5); // Enable pull-up for button
    DDRK = 0x00;         // Dip-switch as input
    PORTK = 0xFF;        // Enable pull-ups for dip-switch
}

uint8_t readDipSwitch() {
    return PINK;
}

void displayMessage(uint8_t counter) {
    switch(counter) {
        case 0:
            sendStrXY("hej", 0, 0);
            break;
        case 1:
            sendStrXY("alt", 0, 0);
            break;
        case 2:
            sendStrXY("Per", 0, 0);
            break;
        default:
            sendStrXY("123", 0, 0);
            break;
    }
}

volatile uint8_t buttonPressed = 0;

void checkButtonPress() {
    if (!(PING & (1 << PG5))) { // If button is pressed
        _delay_ms(50); // Debounce delay
        if (!(PING & (1 << PG5))) { // Check button state again
            buttonPressed = 1;
        }
    }
}

int main(void) {
    init();
    
    _i2c_address = 0x78;
    
    I2C_Init();
    InitializeDisplay();
    print_fonts();
    clear_display();

    uint8_t messageCounter = 0;

    while (1) {
        checkButtonPress();
        if (buttonPressed) {
            displayMessage(messageCounter);
            messageCounter = (messageCounter + 1) % 4; // Cycle through 4 messages
            buttonPressed = 0;
        }
    }

    return 0;
}

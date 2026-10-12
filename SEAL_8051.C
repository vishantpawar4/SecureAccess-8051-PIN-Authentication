/*
 * SecureAccess: 8051 Based PIN Authentication System
 * MCU  : AT89C51
 * Tools: Keil uVision, Proteus 
 * PIN  : 1234
 */

#include <reg51.h>

/* LCD pins */
sbit RS = P1^1;
sbit RW = P1^2;
sbit EN = P1^3;

/* LED pins */
sbit GREEN = P1^0;
sbit RED = P1^4;

/* Keypad rows */
sbit r0 = P2^0;
sbit r1 = P2^1;
sbit r2 = P2^2;
sbit r3 = P2^3;

/* Keypad columns */
sbit c0 = P2^4;
sbit c1 = P2^5;
sbit c2 = P2^6;
sbit c3 = P2^7;

/* LCD data pins */
#define data_line P3

/* Function declarations */
void lcd_init(void);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_text(char *);
void delay(void);
void delay_ms(unsigned int);
unsigned char keypad(void);

void main(void)
{
    unsigned char key;
    unsigned char count;
    unsigned char attempts = 0;
    unsigned char correct;
    char pin[4];

    GREEN = 0;
    RED = 0;
    P2 = 0xFF;

    lcd_init();

    /* Startup LED test (both LEDs ON for 1 sec) */
    GREEN = 1;
    RED = 1;
    delay_ms(1000);
    GREEN = 0;
    RED = 0;

    while(1)
    {
        GREEN = 0;
        RED = 0;

        lcd_cmd(0x01);
        lcd_cmd(0x80);
        lcd_text("SECURE ACCESS");
        lcd_cmd(0xC0);
        lcd_text("ENTER PIN:");

        count = 0;

        while(count < 4)
        {
            key = keypad();

            if(key != 0)
            {
                pin[count] = key;
                lcd_data('*');
                count++;
            }
        }

        /* Check PIN: 1234 */
        correct = 1;

        if(pin[0] != '1') correct = 0;
        if(pin[1] != '2') correct = 0;
        if(pin[2] != '3') correct = 0;
        if(pin[3] != '4') correct = 0;

        lcd_cmd(0x01);

        if(correct == 1)
        {
            attempts = 0;

            GREEN = 1;
            RED = 0;

            lcd_cmd(0x80);
            lcd_text("ACCESS GRANTED");

            delay_ms(2000);

            GREEN = 0;
        }
        else
        {
            attempts++;

            GREEN = 0;
            RED = 1;

            lcd_cmd(0x80);
            lcd_text("WRONG PIN");

            if(attempts >= 3)
            {
                lcd_cmd(0xC0);
                lcd_text("LOCKED 5 SEC");

                delay_ms(5000);

                attempts = 0;
            }
            else
            {
                delay_ms(1500);
            }

            RED = 0;
        }
    }
}

/* Keypad scanning */
unsigned char keypad(void)
{
    /* Row 0: 7 8 9 */
    r0 = 0;
    r1 = 1;
    r2 = 1;
    r3 = 1;

    if(c0 == 0)
    {
        while(c0 == 0);
        return '7';
    }

    if(c1 == 0)
    {
        while(c1 == 0);
        return '8';
    }

    if(c2 == 0)
    {
        while(c2 == 0);
        return '9';
    }

    /* Row 1: 4 5 6 */
    r0 = 1;
    r1 = 0;
    r2 = 1;
    r3 = 1;

    if(c0 == 0)
    {
        while(c0 == 0);
        return '4';
    }

    if(c1 == 0)
    {
        while(c1 == 0);
        return '5';
    }

    if(c2 == 0)
    {
        while(c2 == 0);
        return '6';
    }

    /* Row 2: 1 2 3 */
    r0 = 1;
    r1 = 1;
    r2 = 0;
    r3 = 1;

    if(c0 == 0)
    {
        while(c0 == 0);
        return '1';
    }

    if(c1 == 0)
    {
        while(c1 == 0);
        return '2';
    }

    if(c2 == 0)
    {
        while(c2 == 0);
        return '3';
    }

    /* Row 3: ON/C 0 = + */
    r0 = 1;
    r1 = 1;
    r2 = 1;
    r3 = 0;

    if(c1 == 0)
    {
        while(c1 == 0);
        return '0';
    }

    return 0;
}

/* LCD initialization */
void lcd_init(void)
{
    delay_ms(20);
    lcd_cmd(0x38);
    lcd_cmd(0x0C);
    lcd_cmd(0x01);
    lcd_cmd(0x06);
    lcd_cmd(0x80);
}

/* LCD command */
void lcd_cmd(unsigned char cmd)
{
    data_line = cmd;

    RS = 0;
    RW = 0;
    EN = 1;

    delay();
    EN = 0;
    delay();

    if(cmd == 0x01 || cmd == 0x02)
        delay_ms(2);
}

/* LCD data */
void lcd_data(unsigned char dat)
{
    data_line = dat;

    RS = 1;
    RW = 0;
    EN = 1;

    delay();
    EN = 0;
    delay();
}

/* LCD text */
void lcd_text(char *text)
{
    while(*text)
    {
        lcd_data(*text);
        text++;
    }
}

/* Short delay */
void delay(void)
{
    unsigned int i;

    for(i = 0; i < 50; i++);
}

/* Approximate millisecond delay */
void delay_ms(unsigned int ms)
{
    unsigned int i, j;

    for(i = 0; i < ms; i++)
        for(j = 0; j < 120; j++);
}
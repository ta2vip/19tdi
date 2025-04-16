#include <12F675.h>

#fuses INTRC_IO, NOWDT, PUT, NOMCLR, NOBROWNOUT, PROTECT, CPD
#use delay(clock=4000000)

#define FIRST_CYLINDER_PIN PIN_A0
#define SECOND_CYLINDER_PIN PIN_A1
#define THIRD_CYLINDER_PIN PIN_A2
#define FOURTH_CYLINDER_PIN PIN_A4

#define INPUT1 PIN_A3  // GP3
#define INPUT2 PIN_A5  // GP5

#define MODE_0 900   // 900 RPM
#define MODE_1 1500  // 1500 RPM
#define MODE_2 2500  // 2500 RPM
#define MODE_3 3500  // 3500 RPM
#define ADD_RAND_RPM 1200

void delay_variable_ms(unsigned int16 ms) {
    while (ms--) {
        delay_ms(1);
    }
}

void main() {
    set_tris_a(0b00101000); // GP3 ve GP5 giriþ, diðerleri çýkýþ
    output_low(FIRST_CYLINDER_PIN);
    output_low(SECOND_CYLINDER_PIN);
    output_low(THIRD_CYLINDER_PIN);
    output_low(FOURTH_CYLINDER_PIN);
    
    unsigned int16 rpm;
    unsigned int16 strokeDuration;
    unsigned int16 maxStrokeRand;
    
    while (TRUE) {
        // Buton kombinasyonuna göre RPM belirle
        if (!input(INPUT1) && !input(INPUT2)) {
            rpm = MODE_0; // 900 RPM
        } else if (input(INPUT1) && !input(INPUT2)) {
            rpm = MODE_1; // 1500 RPM
        } else if (!input(INPUT1) && input(INPUT2)) {
            rpm = MODE_2; // 2500 RPM
        } else {
            rpm = MODE_3; // 3500 RPM
        }

        maxStrokeRand = (rpm > ADD_RAND_RPM) ? 1 : 5; // Rastgelelik sýnýrý
        strokeDuration = 1000 / (rpm / 60) / 4;

        output_low(FIRST_CYLINDER_PIN);
        delay_variable_ms(strokeDuration + (get_timer0() % maxStrokeRand));
        output_high(FIRST_CYLINDER_PIN);
        delay_variable_ms(strokeDuration);

        output_low(THIRD_CYLINDER_PIN);
        delay_variable_ms(strokeDuration + (get_timer0() % maxStrokeRand));
        output_high(THIRD_CYLINDER_PIN);
        delay_variable_ms(strokeDuration);

        output_low(FOURTH_CYLINDER_PIN);
        delay_variable_ms(strokeDuration + (get_timer0() % maxStrokeRand));
        output_high(FOURTH_CYLINDER_PIN);
        delay_variable_ms(strokeDuration);

        output_low(SECOND_CYLINDER_PIN);
        delay_variable_ms(strokeDuration + (get_timer0() % maxStrokeRand));
        output_high(SECOND_CYLINDER_PIN);
        delay_variable_ms(strokeDuration);
    }
}


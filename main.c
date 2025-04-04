#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"

// Definición de pines
#ifndef PICO_DEFAULT_LED_PIN // LED integrado del RPi Pico
    #define LED_PIN 25
#else
    #define LED_PIN PICO_DEFAULT_LED_PIN
#endif

#define RESET_PIN 0  // Conectar al pin RST de la CPU de la Xbox 360
#define POST_PIN  1  // Conectar a un pin POST_OUT (ej. POSTBIT 0-7) para sincronización

// Parámetros del glitch (ajustables)
#define GLITCH_DELAY_US 5000  // Retardo inicial en microsegundos antes del pulso (aproximado para Xenon)
#define GLITCH_PULSE_NS 100   // Duración del pulso en nanosegundos (100 ns típico para RGH)

// Inicializar GPIO para el LED
void pico_led_init(void) {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

// Encender y apagar LED
void pico_set_led(bool led_on) {
    gpio_put(LED_PIN, led_on);
}

// Configuración inicial
void glitch_init() {
    gpio_init(RESET_PIN);
    gpio_set_dir(RESET_PIN, GPIO_OUT);
    gpio_put(RESET_PIN, 0); // Estado inicial bajo

    gpio_init(POST_PIN);
    gpio_set_dir(POST_PIN, GPIO_IN);
}

// Generar un pulso de reset
void send_glitch_pulse() {
    gpio_put(RESET_PIN, 1); // Activar reset
    pico_set_led(true);

    // Calcular los ciclos de espera para 100 ns
    uint32_t delay_cycles = (clock_get_hz(clk_sys) / 10000000) * GLITCH_PULSE_NS / 100;
    busy_wait_at_least_cycles(delay_cycles);

    gpio_put(RESET_PIN, 0); // Desactivar reset
    pico_set_led(false);
}

// Monitorear POST y enviar el glitch
void perform_glitch() {
    while (!gpio_get(POST_PIN)) {
        sleep_us(1); // Polling simple
    }

    sleep_us(GLITCH_DELAY_US);
    send_glitch_pulse();
}

int main() {
    stdio_init_all();
    glitch_init();
    pico_led_init();  // Inicializar LED

    while (true) {
        perform_glitch();
        sleep_ms(1000); // 1 segundo entre intentos
    }
}

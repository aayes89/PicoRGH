#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"

// Definición de pines

#ifndef PICO_DEFAULT_LED_PIN
#endif

#define RESET_PIN 0  // Conectar al pin RST de la CPU de la Xbox 360
#define POST_PIN  1  // Conectar a un pin POST_OUT (ej. POSTBIT 0-7) para sincronización

// Parámetros del glitch (ajustables)
#define GLITCH_DELAY_US 5000  // Retardo inicial en microsegundos antes del pulso (aproximado para Xenon)
#define GLITCH_PULSE_NS 100   // Duración del pulso en nanosegundos (100 ns típico para RGH)

// Inicializar GPIO para el LED
void pico_led_init(void) {
#ifdef PICO_DEFAULT_LED_PIN
    // utiliza la funcionalidad de GPIO para encender o apagar el LED
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#endif
}

// Encender y apagar LED
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    // Establece el estado a ON/OFF
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#endif
}

// Configuración inicial
void glitch_init() {
    // Configurar RESET como salida
    gpio_init(RESET_PIN);
    gpio_set_dir(RESET_PIN, GPIO_OUT);
    gpio_put(RESET_PIN, 0); // Estado inicial bajo

    // Configurar POST como entrada
    gpio_init(POST_PIN);
    gpio_set_dir(POST_PIN, GPIO_IN);
}

// Generar un pulso de reset
void send_glitch_pulse() {
    gpio_put(RESET_PIN, 1); // Activar reset
    pico_set_led(true);
    //sleep_ns(GLITCH_PULSE_NS); // Duración del pulso (100 ns)
    busy_wait_at_least_cycles(GLITCH_PULSE_NS * (clock_get_hz(clk_sys) / 1000000000));
    gpio_put(RESET_PIN, 0); // Desactivar reset
    pico_set_led(false);
}

// Monitorear POST y enviar el glitch
void perform_glitch() {
    // Esperar a que POST cambie (indica que la consola está arrancando)
    while (!gpio_get(POST_PIN)) {
        sleep_us(1); // Polling simple, ajustar según necesidad
    }

    // Retardo antes del glitch (ajustar según timing de Xenon)
    sleep_us(GLITCH_DELAY_US);

    // Enviar el pulso de glitch
    send_glitch_pulse();
}

int main() {
    stdio_init_all();
    glitch_init();

    while (true) {
        // Intentar el glitch en cada ciclo de arranque
        perform_glitch();

        // Esperar un tiempo antes de intentar de nuevo (evitar bucles rápidos)
        sleep_ms(1000); // 1 segundo entre intentos
    }
}

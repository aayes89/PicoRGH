#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include "glitch.pio.h"  // Archivo generado desde el código PIO

// Definición de pines
#ifndef PICO_DEFAULT_LED_PIN // LED integrado del RPi Pico
    #define LED_PIN 25
#else
    #define LED_PIN PICO_DEFAULT_LED_PIN
#endif

// Configuración de pines
#define CPU_RESET_PIN    0 // RST_CPU - [R8C2 | J8C1 Pin2 ](TOP) || [C7R112] (BOTTOM)
#define GLITCH_OUT_PIN   1 // POST - FT6U1 (TOP|BOTTOM) || FT6U7 (BOTTOM) 

// Parámetros del glitch
#define GLITCH_DELAY_CYCLES  20
#define GLITCH_WIDTH_CYCLES  3

volatile bool trigger_glitch = false;
PIO glitch_pio = pio0;
uint glitch_sm;

// Interrupción para detectar el reset
void gpio_irq_handler(uint gpio, uint32_t events) {
    if (gpio == CPU_RESET_PIN && (events & GPIO_IRQ_EDGE_FALL)) {
        trigger_glitch = true;
    }
}

// Inicializar PIO
void init_glitch_pio() {
    uint offset = pio_add_program(glitch_pio, &glitch_program);
    pio_sm_config cfg = glitch_program_get_default_config(offset);
    
    sm_config_set_out_pins(&cfg, GLITCH_OUT_PIN, 1);
    sm_config_set_clkdiv(&cfg, 1.0);  // 125 MHz → 8 ns/ciclo
    
    glitch_sm = pio_claim_unused_sm(glitch_pio, true);
	//pio_claim_sm_mask(glitch_pio, 1u << glitch_sm);
	pio_sm_init(glitch_pio, glitch_sm, offset, &cfg);
    pio_sm_set_enabled(glitch_pio, glitch_sm, true);
}

int main() {
	// Inicializar hardware básico
	stdio_init_all();
	
    // Inicializar hardware (sin stdio)
    gpio_init(CPU_RESET_PIN);
    gpio_init(GLITCH_OUT_PIN);
    gpio_init(LED_PIN);    
    
    gpio_set_dir(CPU_RESET_PIN, GPIO_IN);    
    gpio_set_dir(LED_PIN, GPIO_OUT);    
    gpio_set_function(GLITCH_OUT_PIN, GPIO_FUNC_PIO0);
	
    // Configurar interrupción
    gpio_set_irq_enabled_with_callback(CPU_RESET_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    
    // Iniciar PIO
    init_glitch_pio();
    
    // Bucle principal
    while(1) {
        if(trigger_glitch) {
            // Parpadear LED verde (glitch activo)
            gpio_put(LED_PIN, 1);  // Enciende el LED            
            pio_sm_put_blocking(glitch_pio, glitch_sm, GLITCH_DELAY_CYCLES);
            pio_sm_put_blocking(glitch_pio, glitch_sm, GLITCH_WIDTH_CYCLES);
            gpio_put(LED_PIN, 0);  // Apaga el LED
            trigger_glitch = false;
        }         
        __wfi();  // Pone el microcontrolador en modo de espera activa
    }
}

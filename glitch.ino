#define CPU_RESET_PIN    0      // RST_CPU de Xenon
#define GLITCH_OUT_PIN   1      // Pin que genera el glitch
#define POST_PIN         2      // Lectura de latch de POST (por ahora solo 1 bit)
#define LED_PIN          LED_BUILTIN     // LED_BUILTIN en Raspberry Pi Pico

#define GLITCH_DELAY_US  5      // Ajuste fino tras detectar POST deseado
#define GLITCH_WIDTH_US  1      // Ancho del glitch

volatile bool ready_to_monitor_post = false;
bool glitch_fired = false;

void cpu_reset_handler() {       // IRAM_ATTR - para ESP
  if (digitalRead(CPU_RESET_PIN) == LOW) {
    ready_to_monitor_post = true;
    glitch_fired = false;
  }
}

void setup() {
  pinMode(CPU_RESET_PIN, INPUT_PULLUP);
  pinMode(GLITCH_OUT_PIN, OUTPUT);
  pinMode(POST_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(GLITCH_OUT_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  attachInterrupt(digitalPinToInterrupt(CPU_RESET_PIN), cpu_reset_handler, FALLING);

  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("Glitcher sincronizado con POST listo");
}

void generate_glitch() {
  digitalWrite(LED_PIN, HIGH);
  delayMicroseconds(GLITCH_DELAY_US);
  digitalWrite(GLITCH_OUT_PIN, HIGH);
  delayMicroseconds(GLITCH_WIDTH_US);
  digitalWrite(GLITCH_OUT_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  static uint8_t last_post = 0x00;
  uint8_t current_post;
  delay(250);
  generate_glitch();

  if (ready_to_monitor_post && !glitch_fired) {
    current_post = read_post_code();  // leer valor de POST_OUT

    if (current_post != last_post) {
      Serial.print("POST Code: 0x");
      Serial.println(current_post, HEX);
      last_post = current_post;
    }

    // Si detectamos 0x51, por ejemplo, disparamos glitch
    if (current_post == 0x51 && !glitch_fired) {
      generate_glitch();
      glitch_fired = true;
      ready_to_monitor_post = false;
      Serial.println("Glitch lanzado en POST 0x51");
    }
  }
}

// Suponiendo que POST_PIN tiene los 8 bits en un latch (mejor usar 8 GPIOs)
uint8_t read_post_code() {
  // Aquí deberías leer todos los pines conectados a POST_OUT[7:0]
  // Versión de prueba si solo estás usando 1 pin (para lógica más simple)
  return digitalRead(POST_PIN);  // ¡Esto debe mejorarse a múltiples bits!
}

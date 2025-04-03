# PicoRGH
<p>
  Conversión del timing en formato xsvf creado por @Octal450 para RGH en la Xbox 360 placa Xenon utilizando una Raspberry Pi Pico. (RPI-Pico) <br>
  Sólo necesita de 4 cables. (hasta el momento)
</p>

# Conexiones
1. <h3>RESET (GPIO 0 ↔ RST de la CPU)</h3>

    Conecta el GPIO 0 de la Pico al pin RST (Reset de la CPU) en la Xbox 360.

    Este cable es el que envía el pulso de glitch para desestabilizar el arranque.

2. <h3>POST_OUT (GPIO 1 ↔ POST de la Xbox 360)</h3>

    Conecta el GPIO 1 de la Pico a un punto POST_OUT de la placa de la Xbox 360.

    POST_OUT proporciona señales de sincronización sobre el estado de arranque de la consola.

3. <h3>GND (GND de la Pico ↔ GND de la Xbox 360)</h3>

    Conecta GND de la Raspberry Pi Pico a GND de la Xbox 360.

    Esto es crucial para que ambas placas compartan referencia de voltaje.
4. <h3> VCC (VBUS o 3V3 de la Pico ↔ 3.3v de la Xbox 360)</h3>
    Puede utilizar si lo desea una fuente de alimentación externa (vía powerbank) e incluso un cable USB. 
    
    Eliminando la necesidad de soldar cables VCC y GND.

    Puede utilizar 5V (no recomandado) si conoce la tolerancia de su RPI-Pico.
<h3>Imágenes (para referencia)</h3>
<h3> RST </h3>

![RST](https://github.com/user-attachments/assets/fc7839ae-bb3b-43c3-8eb0-e6dc817f1cb5)

<h3> POST </h3>

![POST](https://github.com/user-attachments/assets/8466ae24-9950-4b7c-9dbd-655dcbf801ee)

<h3> VCC y GND </h3>

![J2B1](https://github.com/user-attachments/assets/a979f091-94c1-4bc4-b15f-74748f81dbe6)

Pueden encontrar más puntos de conexión en la siguiente dirección: <a href="https://consolemods.org/wiki/Xbox_360:Alternate_Solder_Points">Console Mods</a>

# Descarga de responsabilidad
<p>
El objetivo de este proyecto es meramente educativo, sin fines de lucro y no tiene ningún respaldo comprobado hasta este instante. En la medida que pueda ser probado y mejorado, podrá dar cuenta de ello según vaya obteniendo resultados, los cuales, serán reflejados en las próximas versiones. En caso de que consiga un resultado satisfactorio, no tengo intención de continuar mejorándolo para otras consolas.<br>
Si ud. desea continuar el proyecto y/o probarlo en su equipo,<br> 
<b>NO me hago responsable del daño que pueda ocasionar o mal uso que pueda darle, por acción directa o indirecta.</b>
</p>

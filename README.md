# BOBBY
Bobby es un robot construido para competir en los juegos bonaerenses, en la categoría de Triatlón( Quita obstaculos - Sumo - Seguidor de linea ), por los estudiantes, Iago Botto, Dante Abel Canale, Lautaro Lammers y Lucas Chavez

[img_20240703_164955.md](https://github.com/user-attachments/files/17923314/img_20240703_164955.md)

## FUNCIONAMIENTO

### Seguidor de linea:
  Tenemos un sistema de sensores reflectivos QRE1113GR, en configuracion de array, que permite detectar con precisión, una linea blanca que representa el camino, y debemos recorrerlo en el menor tiempo posible
Utilizamos un algorimo de PD
~~~
  uint8_t PID(){
    int error = setpoint - lecturaQRE();
    int P = error * Kp;
    int D = (error - ultimo_error) * Kd;
    ultimo_error = error;
    float output = P + D;
    output = abs(output);
    output = map(output, 0, 25000, 0, limite_output);
    output = constrain(output, 0, limite_output);
    return (output);
  }
~~~
que nos permite poder variar los valores PWM enviados al motor, para seguir la linea correctamente

### Quita Obstáculos
  Nos encontramos con 5 sensores GP2Y0A02YK0F de distancia, que con un algoritmo y filtro digital, verificamos si hay un objeto o no. Agregado a esto, se utilizan 3 sensores de piso QRE1113GR, para evitar caernos de la pista
cuando atacamos a cada obstáculo.

### Sumo
  Aqui tenemos a nuestra categoría estrella, en esta disciplina, el robot es controlado por el usuario a través de una conexión Bluetooth entre un joystick de Ps3, y un modulo ESP32. Se compite contra otro robot en las mismas condiciones en un tatami.

## INFORME DE COSTES
#### Sensores
  -Sharp GP2Y0A02YK0F: 10.509$ c/u (starware)  (x5)
  -Sensores de piso QRE1113GR: 969.37$ c/u (mouser)  (x8)
  -impresion 3D: 8.000$ (300 gr)
  -Motores AP&S SP6-260: 26.000$ c/u (AP&S)  (x2)
  -Controladores BTS7960: 20.0000$ c/u (MercadoLibre)  (x2)
  -ESP32: 7.329$ c/u (Starware)   (x1)
  -Conexionado y cables: 20.000$ (it&t)

## FALLAS Y COSAS A MEJORAR
  -Problemas en el cálculo de tiempos
  -incumplimientos de los plazos acordados
  -falta de compromiso con el proyecto
  -
  
  

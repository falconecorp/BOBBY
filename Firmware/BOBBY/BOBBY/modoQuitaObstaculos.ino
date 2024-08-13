#define PIN_SENSOR_SHARP_DERECHA 26
#define PIN_SENSOR_SHARP_IZQUIERDA 25
#define PIN_SENSOR_SHARP_MEDIO 33
#define PIN_SENSOR_SHARP_IZQUIERDA_MEDIO 27
#define PIN_SENSOR_SHARP_DERECHA_MEDIO 14
#define DOBLAR 255
#define DOBLAR_RAPIDO 200
#define VEL_MAX 255
#define N 100
#define FREC 1000
#define RES 8
#define CHANNEL1 1
#define CHANNEL2 2
#define CHANNEL3 3
#define CHANNEL4 4
#define PIN_MOTOR_A1 5
#define PIN_MOTOR_B1 16
#define PIN_MOTOR_B2 17
#define PIN_MOTOR_A2 18 
#define PIN_BOTON_ROTAR 22
#define PIN_BOTON_SELECCIONAR 23
#define OBJETO_DETECTADO 85
#define SI 1
#define NO 0
bool boton_presionado = false;
int valor_sensor;
int motorA = 0;
int motorB = 0;
enum estado_giro{
  IZQUIERDA_RAPIDO,
  IZQUIERDA,
  DERECHA,
  DERECHA_RAPIDO,
  NINGUNA
};
estado_giro direccion_giro;

enum tipos_de_modo{
  REPOSO,
  SEGURIDAD,
  BUSCAR,
  ATACAR,
  REUBICAR
};
tipos_de_modo modos_quita;

/*bool deteccionBorde(){
  int piso_derecho = analogRead(PIN_SENSOR_PISO_DERECHA);
  int piso_izquierdo = analogRead(PIN_SENSOR_PISO_IZQUIERDA);
  if(piso_derecho <= BLANCO)
  {
    borde = SI; 
  }
  else
  {
    borde = NO;
  }
  return(borde);
}
*/
int deteccionObjeto(int pin_sensor) {
  long suma = 0;
  for(int i = 0; i < N; i++) {
    suma += analogRead(pin_sensor);
  }
  valor_sensor = suma / N;
  valor_sensor = constrain(valor_sensor, 1000, 3000);
  valor_sensor = map(valor_sensor, 1000, 3000, 20, 150); 
  return (constrain(valor_sensor, 20, 150));
}

void direcciones(){
  switch(direccion_giro){
    case IZQUIERDA:{
      motorA = DOBLAR;
      motorB = DOBLAR;
      movimientoQuitaIzquierda();
      break;
    }
    case DERECHA:{
      motorA = DOBLAR;
      motorB = DOBLAR;
      movimientoQuitaDerecha();
      break;
    }
    case IZQUIERDA_RAPIDO:{
      motorA = DOBLAR;
      motorB = DOBLAR;
      movimientoQuitaIzquierda();
      break;
    }
    case DERECHA_RAPIDO:{
      motorA = DOBLAR;
      motorB = DOBLAR;
      movimientoQuitaDerecha();
      break;
    }
    case NINGUNA: {
      motorA = 0;
      motorB = 0;
      movimientoQuitaDerecha();
      break;
    }
  }
}
void reposo(){                                  //  _
  while(!boton_presionado){                     //   |
    if(digitalRead(PIN_BOTON_ROTAR) == LOW){    //   |
      boton_presionado = true;                  //   |
      delay(200);                               //   |      codigo para
    }                                           //   |      evitar debounce
  }                                             //   |
  while(digitalRead(PIN_BOTON_ROTAR) == LOW){   //   |
  }                                             //  _|
  modos_quita = BUSCAR;
}

void buscar(){
  if(deteccionObjeto(PIN_SENSOR_SHARP_MEDIO) >= OBJETO_DETECTADO){
    modos_quita = ATACAR;
  }
  else if(deteccionObjeto(PIN_SENSOR_SHARP_IZQUIERDA_MEDIO) >= OBJETO_DETECTADO){
    direccion_giro = IZQUIERDA;
  }
  else if(deteccionObjeto(PIN_SENSOR_SHARP_DERECHA_MEDIO) >= OBJETO_DETECTADO){
    direccion_giro = DERECHA;
  }
  else if(deteccionObjeto(PIN_SENSOR_SHARP_IZQUIERDA) >= OBJETO_DETECTADO){
    direccion_giro = IZQUIERDA_RAPIDO;
  }
  else if(deteccionObjeto(PIN_SENSOR_SHARP_DERECHA) >= OBJETO_DETECTADO){
    direccion_giro = DERECHA_RAPIDO;
  }
  else
  direccion_giro = NINGUNA;
}
void atacar()
{
  motorA = VEL_MAX;
  motorB = VEL_MAX;
  movimientoQuitaAdelante();
  if(/*deteccionBorde() == SI || */deteccionObjeto(PIN_SENSOR_SHARP_MEDIO) <= OBJETO_DETECTADO){
    modos_quita = BUSCAR;
  }
}
void reubicar()
{
  motorA = VEL_MAX;
  motorB = VEL_MAX;
  movimientoQuitaAtras();
  delay(1000);
  modos_quita = BUSCAR;
}
void movimientoQuitaAdelante(){
  ledcWrite(PIN_MOTOR_A1, LOW);
  ledcWrite(PIN_MOTOR_A2, motorA);
  ledcWrite(PIN_MOTOR_B1, LOW);
  ledcWrite(PIN_MOTOR_B2, motorB);
}
void movimientoQuitaAtras(){
  ledcWrite(PIN_MOTOR_A1, motorA);
  ledcWrite(PIN_MOTOR_A2, LOW);
  ledcWrite(PIN_MOTOR_B1, motorB);
  ledcWrite(PIN_MOTOR_B2, LOW);
}
void movimientoQuitaIzquierda(){
  ledcWrite(PIN_MOTOR_A1, LOW);
  ledcWrite(PIN_MOTOR_A2, motorA);
  ledcWrite(PIN_MOTOR_B1, motorB);
  ledcWrite(PIN_MOTOR_B2, LOW);
}
void movimientoQuitaDerecha(){
  ledcWrite(PIN_MOTOR_A1, motorA);
  ledcWrite(PIN_MOTOR_A2, LOW);
  ledcWrite(PIN_MOTOR_B1, LOW);
  ledcWrite(PIN_MOTOR_B2, motorB);
}

void quita() {      
  switch(modos_quita){
    case REPOSO:{
      reposo();
      break;
    }
    case BUSCAR:{
      buscar();
      direcciones();
      break;
    }
    case ATACAR:{
      atacar();
      break;
    }
    case REUBICAR:{
      reubicar();
      break;
    }
  }
}

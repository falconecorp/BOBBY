#include <Ps3Controller.h>
#include <BluetoothSerial.h>

BluetoothSerial BT;
//MOTOR
#define N 100
#define FREC 1000
#define RES 8
#define CHANNEL1 1
#define CHANNEL2 2
#define CHANNEL3 3
#define CHANNEL4 4
#define PIN_MOTOR_A1 19
#define PIN_MOTOR_A2 18
#define PIN_MOTOR_B1 4
#define PIN_MOTOR_B2 16
#define ENABLE 2

//modoSeleccion
#define PIN_BOTON_ROTAR 22
#define PIN_BOTON_SELECCIONAR 23

//modoQuitaObstaculos
#define OBJETO_IZQUIERDA 25
#define OBJETO_IZQUIERDA_MEDIO 13
#define OBJETO_MEDIO 35
#define OBJETO_DERECHA_MEDIO 26
#define OBJETO_DERECHA 36
#define BLANCO 2000
#define DOBLAR 150
#define VEL_MAX 255
#define OBJETO_DETECTADO 85
#define SI 1
#define NO 0
enum tipos_de_modo{
  REPOSO,
  SEGURIDAD,
  BUSCAR,
  ATACAR,
  REUBICAR
};
tipos_de_modo modos_quita;

//seleccion
#define DELAY_200 200
#define TIEMPO_SEGURIDAD 5000
#define TIEMPO_IMPRESION 50
#define NUMERO_MODOS 3

long tiempo=millis(); //tiempo antes de iniciar la lectura
bool DEBUG = 0;

bool boton_presionado = false;
int modo_actual = 0;
bool boton_confirmar_presionado = false;
bool modo_de_seleccion = false;
bool modo_seleccionado = false;

void botonInicio(){
  while(!boton_presionado){
    if(digitalRead(PIN_BOTON_ROTAR) == LOW){
    boton_presionado = true;
    delay(DELAY_200);
    }
  }
  while(digitalRead(PIN_BOTON_ROTAR) == LOW){
  }
}

void configurarEntradas(){
  pinMode(OBJETO_IZQUIERDA, INPUT);
  pinMode(OBJETO_IZQUIERDA_MEDIO, INPUT);
  pinMode(OBJETO_MEDIO, INPUT);
  pinMode(OBJETO_DERECHA_MEDIO, INPUT);
  pinMode(OBJETO_DERECHA, INPUT);
  pinMode(PIN_BOTON_ROTAR, INPUT_PULLUP);
  pinMode(PIN_BOTON_SELECCIONAR, INPUT_PULLUP);
}

void configurarMotor(){
  pinMode(ENABLE, OUTPUT);
  digitalWrite (ENABLE, HIGH);
  ledcAttachChannel(PIN_MOTOR_A1, FREC, RES, CHANNEL1); //l_pwm-A
  ledcAttachChannel(PIN_MOTOR_A2, FREC, RES, CHANNEL2); //r_pwm-A
  ledcAttachChannel(PIN_MOTOR_B1, FREC, RES, CHANNEL3); //l_pwm-B
  ledcAttachChannel(PIN_MOTOR_B2, FREC, RES, CHANNEL4); //r_pwm-B
}
void setup(){
  Serial.begin(115200);
  //BT.begin("esp32");
  configurarEntradas();
  configurarMotor();
  //modos_quita = REPOSO;
}
void loop()
{
  sumo();
}
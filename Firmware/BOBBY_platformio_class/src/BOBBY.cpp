#include <stdint.h>
#include <iostream>
#include <Ps3Controller.h>
#include <BluetoothSerial.h>
#define VALUE_0_01 0.01
#define VALUE_0_1 0.1
#define N 100
#define FREC 800
#define RES 8
#define CHANNEL1 1
#define CHANNEL2 2
#define CHANNEL3 3
#define CHANNEL4 4
#define PIN_MOTOR_A1 5
#define PIN_MOTOR_B1 16
#define PIN_MOTOR_B2 17
#define PIN_MOTOR_A2 18
#define PIN_BOTON 22
int tolerancia = 35;
float COMPENSACION_POS = 1.2;
float COMPENSACION_NEG = 1.3;
bool DEBUG = 0;
bool boton_presionado = false;
BluetoothSerial BT;
void setup() {
  Serial.begin(256000);
  Ps3.begin("00:00:00:00:00:00");
  delay(2000);
  ledcSetup(CHANNEL1, FREC, RES);
  ledcSetup(CHANNEL2, FREC, RES);
  ledcSetup(CHANNEL3, FREC, RES);
  ledcSetup(CHANNEL4, FREC, RES);
  ledcAttachPin(PIN_MOTOR_A1, CHANNEL1);
  ledcAttachPin(PIN_MOTOR_A2, CHANNEL2);
  ledcAttachPin(PIN_MOTOR_B1, CHANNEL3); 
  ledcAttachPin(PIN_MOTOR_B2, CHANNEL4);
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(25, INPUT);
  pinMode(26, INPUT);
  pinMode(27, INPUT);
  pinMode(14, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////  LINE FOLLOWER   ////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

enum class direccion_enumerada{
  FRENAR,
  ADELANTE,
  DERECHA,
  IZQUIERDA
};
struct retorno_linea{
  direccion_enumerada direccion;
  uint8_t velocidad;
};  
class Line_follower{
private:
  uint8_t vel_base = 90;
  uint8_t vel_pos;
  uint8_t vel_neg;
  uint8_t vel_max = 90;
  uint16_t setpoint = 21000;
  uint8_t Kp=1.3, Kd=0.6;
  uint8_t limite_output = 70;
  uint16_t error;
  uint16_t ultimo_error;
  uint8_t tolerancia = 35;
  retorno_linea estado_motor;
public:
  uint16_t lectura_qre();
  uint8_t PID();
  void control_linea();
  void movimiento_linea();
  void valor_motor();
  void menuBT();
  //Line_follower(uint8_t _kp, uint8_t _kd);
};
/*
Line_follower::Line_follower(uint8_t _kp, uint8_t _kd):Kp(_kp),Kd(_kd)
{
  
}
*/
uint16_t Line_follower::lectura_qre() {
  uint16_t qre1, qre2, qre3, qre4, qre5, qre6, qre7, qre8;
  qre1 = analogRead(32);
  qre2 = analogRead(33);
  qre3 = analogRead(25);
  qre4 = analogRead(26);
  qre5 = analogRead(27);
  qre6 = analogRead(14);
  qre7 = analogRead(12);
  qre8 = analogRead(13);
  uint16_t valor_qre = (qre1 + qre2 + qre3 + qre4 + qre5 + qre6 + qre7 + qre8);
  if(DEBUG){
  BT.print("qre:");
  BT.println(valor_qre);
  }
  return valor_qre;
}
uint8_t Line_follower::PID(){
  error = setpoint - lectura_qre();
  int P = error * Kp;
  int D = (error - ultimo_error) * Kd;
  ultimo_error = error;
  if(DEBUG){
    BT.print("error: ");
    BT.println(error);
  }
  float output = P + D;
  output = abs(output);
  output = map(output, 0, 28000, 0, limite_output);
  output = constrain(output, 0, limite_output);
  return output;
}
void Line_follower::valor_motor(){
  uint8_t vel_motor = estado_motor.velocidad;
  vel_pos = vel_base + vel_motor;
  vel_neg = vel_base - vel_motor;
  vel_pos = vel_pos * COMPENSACION_POS;
  vel_neg = vel_neg / COMPENSACION_NEG;
    if(vel_neg < tolerancia){
    vel_neg = 0;
  }
  vel_pos = constrain(vel_pos, 0, 255);
  vel_neg = constrain(vel_neg, 0, 255);
}
void Line_follower::control_linea() {
  PID();
  int lim = 2800;
  int lim_neg = -2800;
  if (error < lim_neg) {
    estado_motor.direccion = direccion_enumerada::DERECHA;
    estado_motor.velocidad = PID();
  } else if (error > lim) {
    estado_motor.direccion = direccion_enumerada::IZQUIERDA;
    estado_motor.velocidad = PID();
  } else {
    estado_motor.direccion = direccion_enumerada::ADELANTE;
    estado_motor.velocidad = PID();
  }
}
void Line_follower::movimiento_linea() {
  control_linea();
  switch (estado_motor.direccion)
  {
    case direccion_enumerada::DERECHA:
    {  
      valor_motor();
      ledcWrite(CHANNEL1, LOW);
      ledcWrite(CHANNEL2, vel_pos);
      ledcWrite(CHANNEL3, LOW);
      ledcWrite(CHANNEL4, vel_neg);
      if(DEBUG){
      BT.println("derecha");
      BT.print("vel_pos: ");      
      BT.println(vel_pos);
      BT.print("vel_neg: ");  
      BT.println(vel_neg);
      }  
      break;
    }
    case direccion_enumerada::IZQUIERDA:
    {
      valor_motor();
      ledcWrite(CHANNEL1, LOW);
      ledcWrite(CHANNEL2, vel_neg);
      ledcWrite(CHANNEL3, LOW);
      ledcWrite(CHANNEL4, vel_pos);      
      if(DEBUG){
      BT.println("izquierda");
      BT.print("vel_pos: ");      
      BT.println(vel_pos);
      BT.print("vel_neg: ");  
      BT.println(vel_neg);
      }
    }
    case direccion_enumerada::ADELANTE:
    {
      ledcWrite(CHANNEL1, LOW);
      ledcWrite(CHANNEL2, vel_max);
      ledcWrite(CHANNEL3, LOW);
      ledcWrite(CHANNEL4, vel_max);  
      if(DEBUG){
        BT.println("Adelante");
      }
      break;
    }
  }
}

void Line_follower::menuBT(){
  if (BT.available())
  { 
    char Message = BT.read();
    switch (Message)
    {
      case 'Q':
      {
        Kp += VALUE_0_1;
        BT.print("Kp: ");
        BT.println(Kp);
        break;
      }
      case 'W':
      {
        Kp -= VALUE_0_1;
        BT.print("Kp: ");
        BT.println(Kp);
        break;
      }
      case 'E':
      {
        Kp += VALUE_0_01;
        BT.print("Kp: ");
        BT.println(Kp);
        break;
      }
      case 'R':
      {
        Kp -= VALUE_0_01;
        BT.print("Kp: ");
        BT.println(Kp);
        break;
      }
      case 'T':
      {
        Kd += VALUE_0_1;
        BT.print("Kd: ");
        BT.println(Kd);
        break;
      }
      case 'Y':
      {
        Kd -= VALUE_0_1;
        BT.print("Kd: ");
        BT.println(Kd);
        break;
      }
      case 'U':
      {
        Kd += VALUE_0_01;
        BT.print("Kd: ");
        BT.println(Kd);
        break;
      }
      case 'I':
      {
        Kd -= VALUE_0_01;
        BT.print("Kd: ");
        BT.println(Kd);
        break;
      }
      case 'O':
      {
        vel_base += 10;
        BT.print("Velocidad base: ");
        BT.println(vel_base);
        break;
      }
      case 'P':
      {
        vel_base -= 10;
        BT.print("Velocidad base: ");
        BT.println(vel_base);
        break;
      }
      case 'A':
      {
        setpoint += 1000;
        BT.print("setpoint: ");
        BT.println(setpoint);
        break;
      }
      case 'S':
      {
        setpoint -= 1000;
        BT.print("setpoint: ");
        BT.println(setpoint);
        break;
      }
      case 'D':
      {
        tolerancia += 5;
        BT.print("Tolerancia: ");
        BT.println(tolerancia);
        break;
      }
      case 'F':
      {
      tolerancia -=5;
        BT.print("Tolerancia: ");
        BT.println(tolerancia);
        break;
      }
      case 'G':
      {
        limite_output += 5;
        BT.print("limite output: ");
        BT.println(limite_output);
        break;
      }
      case 'H':
      {
      limite_output -= 5;
        BT.print("limite output: ");
        BT.println(limite_output);
        break;
      }            
      case 'J':
      {
        DEBUG = 1;
        BT.print("DEBUG: ");
        BT.println(DEBUG);
        break;
      }
      case 'K':
      {
        DEBUG = 0;
        BT.print("DEBUG: ");
        BT.println(DEBUG);
        break;
      }
    }
  }
}
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////   SUMO   /////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
enum class retorno_direccion{
  ADELANTE,
  ATRAS,
  DERECHA,
  IZQUIERDA,
  FRENAR
};
enum class retorno_motor{
  A,
  B
};
struct retorno_sumo{
  retorno_direccion direccion;
  retorno_motor motor; 
};
class sumo{
private:
  int8_t lim = 10;
  int8_t lim_neg = -10;
  uint16_t eje_y;
  char16_t direccion;
  char16_t motor;
  retorno_sumo estado_motor;
public:
  uint8_t lectura_stickA();
  uint8_t lectura_stickB();
  void control_sumo();
  void movimiento_sumo();
  void conexion_bt();
  void inicio();
};
uint8_t sumo::lectura_stickA(){
  eje_y = Ps3.data.analog.stick.ly;
  eje_y = abs(eje_y);
  if(Ps3.data.button.r2)
  {
    eje_y = eje_y * 2;
    return(eje_y);
  }
  else if (Ps3.data.button.l2)
  {
    eje_y = eje_y / 2;
    return(eje_y);
  }
  return(eje_y);    
}
uint8_t sumo::lectura_stickB(){
  eje_y = Ps3.data.analog.stick.ry;
  eje_y = abs(eje_y);
  if(Ps3.data.button.r2)
  {
    eje_y = eje_y * 2;
    return(eje_y);
  }
  else if (Ps3.data.button.l2)
  {
    eje_y = eje_y / 2;
    return(eje_y);
  }
  return(eje_y);         
}
void sumo::control_sumo(){
  int lim_neg = -20;
  int lim = 20;
  int eje_ly_comparacion = Ps3.data.analog.stick.ly * -1;
  if(eje_ly_comparacion < lim_neg)
  {  
    BT.println("ATRAS");
    estado_motor.direccion = retorno_direccion::ATRAS;
    estado_motor.motor = retorno_motor::A;
  }
  else if(eje_ly_comparacion > lim)
  {
    BT.println("ADELANTE");
    estado_motor.direccion = retorno_direccion::ADELANTE;
    estado_motor.motor = retorno_motor::A;
  }
  else
  {
    BT.println("FRENAR");
    estado_motor.direccion = retorno_direccion::FRENAR;
    estado_motor.motor = retorno_motor::A;
  }
  int eje_ry_comparacion = Ps3.data.analog.stick.ry * -1;
  if(eje_ry_comparacion < lim_neg)
  {
    BT.println("ATRAS");
    estado_motor.direccion = retorno_direccion::ATRAS;
    estado_motor.motor = retorno_motor::B;
  }
  else if(eje_ry_comparacion > lim)
  {
    BT.println("ADELANTE");
    estado_motor.direccion = retorno_direccion::ADELANTE;
    estado_motor.motor = retorno_motor::B;
  }
  else
  {
    BT.println("FRENAR");
    estado_motor.direccion = retorno_direccion::FRENAR;
    estado_motor.motor = retorno_motor::B;
  }

}
void sumo::movimiento_sumo(){
  control_sumo();
  switch(estado_motor.motor){
    case retorno_motor::A:
    {
      switch(estado_motor.direccion) 
      {
        case retorno_direccion::ADELANTE:
        {
          ledcWrite(CHANNEL1, LOW);
          ledcWrite(CHANNEL2, lectura_stickA());
          break;    
        }
        case retorno_direccion::ATRAS:
        {
          ledcWrite(CHANNEL1, lectura_stickA());
          ledcWrite(CHANNEL2, LOW);
          break;
        }
        case retorno_direccion::FRENAR:
        {
          ledcWrite(CHANNEL1, LOW);
          ledcWrite(CHANNEL2, LOW);
          break;
        }
      }
      break;
    }
    case retorno_motor::B:
    {
      switch(estado_motor.direccion) 
      {
        case retorno_direccion::ADELANTE:
        {
          ledcWrite(CHANNEL1, LOW);
          ledcWrite(CHANNEL2, lectura_stickB());   
          break;      
        }
        case retorno_direccion::ATRAS:
        {
          ledcWrite(CHANNEL1, lectura_stickB());
          ledcWrite(CHANNEL2, LOW);
          break;
        }
        case retorno_direccion::FRENAR:
        {
          ledcWrite(CHANNEL1, LOW);
          ledcWrite(CHANNEL2, LOW);
          break;
        }
      }
      break;
    }
  }
}
void sumo::conexion_bt(){
  Ps3.begin("00:00:00:00:00:00");
  if(!Ps3.isConnected())
  {
    Ps3.begin("00:00:00:00:00:00");
    delay(200);
  }
}
void sumo::inicio(){
  conexion_bt();
  movimiento_sumo();
}

sumo Luchador;
Line_follower Linea; // Crear un objeto de la clase Line_follower
void loop(){
  while(!boton_presionado){
    if(digitalRead(PIN_BOTON) == LOW){
      boton_presionado = true;
      delay(200);
    }
  }
  while(digitalRead(PIN_BOTON) == LOW){
  }
  Luchador.inicio();
}
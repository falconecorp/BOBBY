/*
#include <stdint.h>
#include <Arduino.h>
#define FREC 800
#define RES 8
#define CHANNEL1 1
#define CHANNEL2 2
#define CHANNEL3 3
#define CHANNEL4 4
enum class direccion_enumerada{
  FRENAR,
  ADELANTE,
  DERECHA,
  IZQUIERDA,
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
  uint8_t vel_max = 130;
  uint16_t setpoint = 21000;
  uint8_t Kp=1.3, Kd=0.6;
  uint8_t limite_output = 70;
  uint16_t error;
  uint16_t ultimo_error;
  retorno_linea estado_motor;
public:
  uint16_t lectura_qre();
  uint8_t PID();
  void control_linea();
  void movimiento_linea();
  void menuBT();
  Line_follower(uint8_t _kp, uint8_t _kd);
};

Line_follower::Line_follower(uint8_t _kp, uint8_t _kd):Kp(_kp),Kd(_kd)
{
  
}

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
  return valor_qre;
}
uint8_t Line_follower::PID(){
  error = setpoint - lectura_qre();
  int P = error * Kp;
  int D = (error - ultimo_error) * Kd;
  ultimo_error = error;
  float output = P + D;
  output = abs(output);
  output = map(output, 0, 28000, 0, limite_output);
  output = constrain(output, 0, limite_output);
  return output;
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
  uint8_t velocidad_giro = estado_motor.velocidad;
  switch (estado_motor.direccion)
  {
    case direccion_enumerada::DERECHA:
    {  
      vel_pos = vel_base + velocidad_giro;
      vel_neg = vel_base - velocidad_giro;
      ledcWrite(CHANNEL1, LOW);
      ledcWrite(CHANNEL2, vel_pos);
      ledcWrite(CHANNEL3, LOW);
      ledcWrite(CHANNEL4, vel_neg);  
      break;
    }
    case direccion_enumerada::IZQUIERDA:
    {
      vel_pos = vel_base + velocidad_giro;
      vel_neg = vel_base - velocidad_giro;
      ledcWrite(CHANNEL1, LOW);
      ledcWrite(CHANNEL2, vel_neg);
      ledcWrite(CHANNEL3, LOW);
      ledcWrite(CHANNEL4, vel_pos);      
    }
    case direccion_enumerada::ADELANTE:
    {
      ledcWrite(CHANNEL1, LOW);
      ledcWrite(CHANNEL2, vel_max);
      ledcWrite(CHANNEL3, LOW);
      ledcWrite(CHANNEL4, vel_max);  
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
    }
  }
}*/
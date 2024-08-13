#include <stdint.h>
#include <Arduino.h>
#include <Ps3Controller.h>
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
    estado_motor.direccion = retorno_direccion::ATRAS;
    estado_motor.motor = retorno_motor::A;
  }
  else if(eje_ly_comparacion > lim)
  {
    estado_motor.direccion = retorno_direccion::ADELANTE;
    estado_motor.motor = retorno_motor::A;
  }
  else
  {
    estado_motor.direccion = retorno_direccion::FRENAR;
    estado_motor.motor = retorno_motor::A;
  }
  int eje_ry_comparacion = Ps3.data.analog.stick.ry * -1;
  if(eje_ry_comparacion < lim_neg)
  {
    estado_motor.direccion = retorno_direccion::ATRAS;
    estado_motor.motor = retorno_motor::B;
  }
  else if(eje_ry_comparacion > lim)
  {
    estado_motor.direccion = retorno_direccion::ADELANTE;
    estado_motor.motor = retorno_motor::B;
  }
  else
  {
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
  }
}
void sumo::conexion_bt(){
  while (!Ps3.isConnected)
  {
    Ps3.begin(00:00:00:00:00:00);
    delay(1000);
  }
}
void sumo::inicio(){
  conexion_bt();
  movimiento_sumo();
}
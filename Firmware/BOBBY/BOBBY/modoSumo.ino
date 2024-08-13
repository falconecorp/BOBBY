void sumo(){
  if(!Ps3.isConnected())
  {
    Serial.println("no estaria conectando");
    Ps3.begin("00:00:00:00:00:00");
    delay(1000);
  }
  comparacion();
}
int controlIzq(){
  int eje_ly = Ps3.data.analog.stick.ly;
  eje_ly = abs(eje_ly);
  if(Ps3.data.button.r2){
    eje_ly = eje_ly * 2;
  }
  if(DEBUG){
    Serial.print("velocidad motor: ");
    Serial.println(eje_ly);
    delay(100);
  }
  return(eje_ly);
} 
int controlDer(){
  int eje_ry = Ps3.data.analog.stick.ry;
  eje_ry = abs(eje_ry);
  if(Ps3.data.button.r2){
    eje_ry = eje_ry * 2;
  }
  if(DEBUG){
    Serial.print("velocidad motor: ");
    Serial.println(eje_ry);
    delay(100);
  }
  return(eje_ry);
}

void movimientoSumoFrenarIzq(){
 //motor A
  ledcWrite(PIN_MOTOR_B1, LOW);
  ledcWrite(PIN_MOTOR_B2, LOW);
}
void movimientoSumoAdelanteIzq(){
  //motor A
  ledcWrite(PIN_MOTOR_B1, LOW);
  ledcWrite(PIN_MOTOR_B2, controlIzq());
}
void movimientoSumoAtrasIzq(){
  //motor A
  ledcWrite(PIN_MOTOR_B1, controlIzq());
  ledcWrite(PIN_MOTOR_B2, LOW);
}
void movimientoSumoFrenarDer(){
  ledcWrite(PIN_MOTOR_A1, LOW);
  ledcWrite(PIN_MOTOR_A2, LOW);
}
void movimientoSumoAdelanteDer(){
  ledcWrite(PIN_MOTOR_A1, LOW);
  ledcWrite(PIN_MOTOR_A2, controlDer());
}
void movimientoSumoAtrasDer(){
  ledcWrite(PIN_MOTOR_A1, controlDer());
  ledcWrite(PIN_MOTOR_A2, LOW);
}

void comparacion(){
  int lim_neg = -20;
  int lim = 20;
  int eje_ly_comparacion = Ps3.data.analog.stick.ly * -1;
  if(eje_ly_comparacion < lim_neg)
  {  
    movimientoSumoAtrasIzq();
    if(DEBUG){
      Serial.println("atras izquierda");
      delay(DELAY_200);
    }
  }
  else if(eje_ly_comparacion > lim)
  {
    movimientoSumoAdelanteIzq();
    if(DEBUG){
      Serial.println("adelante izquierda");
      delay(DELAY_200);
    }
  }
  else
  {
    movimientoSumoFrenarIzq();
  }
  int eje_ry_comparacion = Ps3.data.analog.stick.ry * -1;
  if(eje_ry_comparacion < lim_neg)
  {
    movimientoSumoAtrasDer();
    if(DEBUG){
      Serial.println("atras derecha");
      delay(DELAY_200);
    }
  }
  else if(eje_ry_comparacion > lim)
  {
    movimientoSumoAdelanteDer();
    if(DEBUG){
      Serial.println("adelante derecha");
      delay(DELAY_200);
    }
  }
  else
  {
    movimientoSumoFrenarDer();
  }
}
  //funciona
  
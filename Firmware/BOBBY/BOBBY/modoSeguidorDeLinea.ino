/*
int vel_motor = 0;
int vel_pos = 0;
int vel_neg = 0;
int vel_base = 110; 
#define VALUE_0_1 0.1
#define VALUE_0_01 0.01
#define COMPENSACION_POS 1
#define COMPENSACION_NEG 1
#define COMPENSACION_ACTIVA 0
int tolerancia = 75;
uint8_t limite_output = 70;
double setpoint = 21000;
double Kp=1.25, Kd=0.25;
double ultimo_error = 0;
int valor_qre;

void seguidorLinea(){
  menuBT();
  control();
}
uint16_t lecturaQRE(){
  int qre1, qre2, qre3, qre4, qre5, qre6, qre7, qre8;
  qre1 = analogRead(32);
  qre2 = analogRead(33);
  qre3 = analogRead(25);
  qre4 = analogRead(26);
  qre5 = analogRead(27);
  qre6 = analogRead(14);
  qre7 = analogRead(12);
  qre8 = analogRead(13);
  valor_qre = (qre1 + qre2 + qre3 + qre4 + qre5 + qre6 + qre7 + qre8); 
  if(DEBUG){
  BT.print("qre:");
  BT.println(valor_qre);
  }
  return(valor_qre);
}
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
void valorMotor(){
  vel_motor = PID();
  vel_pos = vel_base + vel_motor;
  vel_neg = vel_base - vel_motor;
  if(COMPENSACION_ACTIVA){
  vel_pos = vel_pos * COMPENSACION_POS;
  vel_neg = vel_neg / COMPENSACION_NEG;
  }
    if(vel_neg < tolerancia){
    vel_neg = 0;
  }
  vel_pos = constrain(vel_pos, 0, 255);
  vel_neg = constrain(vel_neg, 0, 255);
}
void derecha(){
  valorMotor();
  if(DEBUG){
    BT.println("derecha");
    BT.print("vel_pos: ");      
    BT.println(vel_pos);
    BT.print("vel_neg: ");  
    BT.println(vel_neg);
  }
  //motor A
  ledcWrite(PIN_MOTOR_A1, LOW);
  ledcWrite(PIN_MOTOR_A2, vel_neg);
  //motor B
  ledcWrite(PIN_MOTOR_B1, LOW);
  ledcWrite(PIN_MOTOR_B2, vel_pos);
}
void izquierda(){
  valorMotor();
  if(DEBUG){
    BT.println("izquierda");
    BT.print("vel_pos: ");      
    BT.println(vel_pos);
    BT.print("vel_neg: ");  
    BT.println(vel_neg);
  }
  //motor A
  ledcWrite(PIN_MOTOR_A1, LOW);
  ledcWrite(PIN_MOTOR_A2, vel_pos);
  //motor B
  ledcWrite(PIN_MOTOR_B1, LOW);
  ledcWrite(PIN_MOTOR_B2, vel_neg);
}
void adelante(){
  if(DEBUG){
    BT.println("adelante");
  }
  ledcWrite(PIN_MOTOR_A1, LOW);
  ledcWrite(PIN_MOTOR_A2, vel_base);
  ledcWrite(PIN_MOTOR_B1, LOW);
  ledcWrite(PIN_MOTOR_B2, vel_base);  
}

void menuBT(){
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
        vel_base += 5;
        BT.print("Velocidad base: ");
        BT.println(vel_base);
        break;
      }
      case 'P':
      {
        vel_base -= 5;
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
        DEBUG = 0;
        BT.print("DEBUG 0");
        break;
      }
      case 'K':
      {
      DEBUG = 1;
      BT.print("DEBUG 1");
      }            
    }
  }
}
void control(){
  lecturaQRE();
  int lim = 24000;
  int lim_neg = 18000;
 // tiene que ser 0, si error < 0 => va a la izquierda, si error > 0 => va a la derecha
  if(valor_qre < lim_neg){
    derecha();
  }
  else if(valor_qre > lim){
    izquierda();
  }
  else{
    adelante();
  }
}
*/
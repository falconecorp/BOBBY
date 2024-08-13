/*
enum Modos 
{
  MODO_Seleccion,
  MODO_Sumo,
  MODO_QuitaObstaculos,
  MODO_SeguidorDeLinea
};

void ejecutarModo(int modo) 
{
  switch (modo) {
    case MODO_Sumo:
      Serial.println("Modo Sumo seleccionado");
      sumo();
      break;
    case MODO_QuitaObstaculos:
      Serial.println("Modo Quita Obstaculos seleccionado");
      delay(TIEMPO_SEGURIDAD);  //esta mal, cada vez que completa el ciclo, vuelve a hacer el tiempo de seguridad
      quita();
      break;
    case MODO_SeguidorDeLinea:
      Serial.println("Modo Seguidor de Linea seleccionado");
      delay(TIEMPO_SEGURIDAD);
      seguidorLinea();
      break;
    default:
      Serial.println("Ningún modo seleccionado");
      break;
  }
}

void seleccionarModo() 
{
  while(!boton_confirmar_presionado)
  {
    if(digitalRead(PIN_BOTON_SELECCIONAR) == LOW)
    {
      delay(TIEMPO_IMPRESION);
      if(digitalRead(PIN_BOTON_SELECCIONAR) == LOW)
      {
        modo_de_seleccion = true;  
        while(digitalRead(PIN_BOTON_SELECCIONAR) == LOW);

        Serial.println("modo de seleccion activado");
      }
    }
    
    while(modo_de_seleccion)
    {
      Serial.println("entro al modo de seleccion");
      if(digitalRead(PIN_BOTON_ROTAR) == LOW) 
      {
        delay(TIEMPO_IMPRESION); // Debounce
        if(digitalRead(PIN_BOTON_ROTAR) == LOW) 
        {
          modo_de_seleccion = false;
          while(digitalRead(PIN_BOTON_ROTAR) == LOW);

          modo_actual++;
          if(modo_actual > NUMERO_MODOS) 
          {
            modo_actual = 1;
          }
          Serial.print("Modo: ");
          Serial.println(modo_actual);
        }
      }
      if(digitalRead(PIN_BOTON_SELECCIONAR) == LOW)
      {
        delay(TIEMPO_IMPRESION);
        if(digitalRead(PIN_BOTON_SELECCIONAR) == LOW)
        {
          Serial.println("modo confirmacion");
          while(digitalRead(PIN_BOTON_SELECCIONAR) == LOW);

          modo_de_seleccion = false;
          boton_confirmar_presionado = true;
        }
      }
    }
  }
  ejecutarModo(modo_actual);
}    
*/
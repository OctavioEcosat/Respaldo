#include "header.h"

/**
   Funcion para inicializar wifi.
*/
void inicializarWifi()
{
  /**
    Parametros de wifi
  */  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Conectando a:\t");
  Serial.println(ssid);

  /**
     Esperamos a que haga conexion al wifi.
  */
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print('.');
  }
  server.begin(); 
  Serial.println();
  Serial.print("Conectado a:\t");
  Serial.println(WiFi.SSID());
  Serial.print("IP address;\t");
  Serial.println(WiFi.localIP());
  // Serial.print("MAC address;\t");
  // Serial.println(WiFi.macAddress());
}

void inicializarSensores()
{
  for (int i = 0; i < 2; i++)
  {
    pinMode(estadoInputPin[i], INPUT);    
    pinMode(ledOutputPin[i], OUTPUT);
  }
}

void setup()
{
  Serial.begin(115200);

  inicializarWifi();
  inicializarSensores();
}

void SendRoomInfo(String ip, String puerto, int id, int data, String namefile)
{
  HTTPClient http;
  String dataline;
  // dataline = "https://" + ip + "/aplicaciones/temperatura/" + namefile + ".php?&id=" + String(id, DEC) + "&data=" + data;
  dataline = "https://" + ip + "/test/" + namefile + ".php?&id=" + String(id, DEC) + "&data=" + String(data,DEC);
  // Serial.println(dataline);

  //http://ecosat.com.mx/aplicaciones/temperatura/insert-sensor.php?id=1&data=0
  //http://localhost/test/insert-sensor.php?id=1&data=0

  if (!http.begin(dataline))
  {
    Serial.println("Error when sending to server: ");
    Serial.println(dataline);
  }
  else
  {
    int httpCode = http.GET();
    if (httpCode > 0)
    { // Request has been made
      Serial.printf("HTTP status: %d Message: ", httpCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else
    { // Request could not be made
      Serial.printf("HTTP request failedOutputPin[.] Error: %s\r\n", http.errorToString(httpCode).c_str());
    }
  }
  http.end();
}

/*Get the Thermistor Values*/
void sensortermistor()
{
  for (int i = 0; i < 2; i++)
  {
    float raw = analogRead(sensorInputPin[i]);
    float V = raw / 4095 * VCC;
 
    float R = (RC * V) / (VCC - V);

    float logR = log(R);
    float R_th = 1.0 / (CONST_A + CONST_B * logR + CONST_C * logR * logR * logR);

    float kelvin = R_th - V * V / (CONST_K * R); //Remove self-heating error
    sensorsValues[i] = kelvin - 273.15;    //Convert into C

    Serial.print("Temperatura: ");
    Serial.print(sensorsValues[i]);
    Serial.println(" C");
    //SendRoomInfo(ipServer, puertoServer, i + 1, sensorsValues[i], "update-temperature"); //Send data to the Server
  }
}

void sensorEstado()
{
  //    TRUE == HIGH
  //    FALSE == LOW
  bool estadoActual;
  String status = "";
  for (int i = 0; i < 2; i++)
  {
    if (digitalRead(estadoInputPin[i]) == HIGH)
    {
      estadoActual = true;
    }
    else
    {
      estadoActual = false;
    }

    if (estadoActual != estadoAnterior[i])
    {
      switch (estadoActual)
      {
      case 0:
        digitalWrite(ledOutputPin[i], HIGH); //Apaga ledOutputPin[i].
        status = "ENCENDIDO";
        break;
      case 1:
        digitalWrite(ledOutputPin[i], LOW); //Enciende ledOutputPin[i]
        status = "APAGADO";
        break;
      }
      estadoAnterior[i] = estadoActual;
      Serial.println(status + (i + 1));
      // SendRoomInfo(ipServer, puertoServer, i + 1, estadoActual, "insert-sensor");
    }
  }
}

void controlRelays(int pin){
  int value = digitalRead(relayInPin[pin]);
  if(value == HIGH){
    digitalWrite(relayOutPin[pin], HIGH);
  }
  else{
    digitalWrite(relayOutPin[pin], LOW);
  }

}

void loop()
{
  // Wait a few seconds between measurements.

  //While the Wifi is connected
  if (WiFi.status() == WL_CONNECTED)
  {
    if (tiempoSensor == tiempoSensorCompletado) //Sensor Time is equal than the Total time.
    {
      tiempoSensor = 0;
      sensortermistor();
      sensorEstado();
    }
    else if (tiempoSensor == tiempoEnvioCompletado)
    {
      Serial.println("Aun no se enviara la informacion por el tiempoTranscurridoSensor: " + String(tiempoSensor, DEC) + " del tiempo sensor completado: " + String(tiempoSensorCompletado, DEC));
      Serial.println("Sin conexion a internet intentaremos hacer reconexion");
    }
  }

  tiempoSensor += DELAY_TIME;
  delay(DELAY_TIME);
}

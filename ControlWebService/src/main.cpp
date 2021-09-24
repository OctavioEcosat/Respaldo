#include <Arduino.h>
#include <WiFi.h>

// WiFiServer server(80);

const char* ssid = "Ecosat";           
const char* password = "ECOSAT2021"; 

WiFiServer server(80);
WiFiClient client;

unsigned long lastConnectionTime = 0;               // last time you connected to the server, in milliseconds

const unsigned long postingInterval = 10L * 1000L;  // delay between updates, in milliseconds


String header; // Variable para guardar el HTTP request

// String estadoSalida = "off";

const int relay[3] = {25, 26, 27}; // gpio 
bool stateRelay[3] = {false, false, false};

char NAcounts = 0;
bool brnlaststateonline = false;

//------------------------CODIGO HTML------------------------------
String pagina = "<!DOCTYPE html>"
"<html>"
"<head>"
"<meta charset='utf-8' />"
"<title> ESP32 GPIO web</title>"
"</head>"
"<body>"
"<center>"
"<h1>control web GPIO ESP32</h1>"
"<p><a href='/0'><button style='height:50px;width:100px'>1</button></a></p>"
"<p><a href='/1'><button style='height:50px;width:100px'>2</button></a></p>"
"<p><a href='/2'><button style='height:50px;width:100px'>3</button></a></p>"
"</center>"
"</body>"
"</html>";
//---------------------------SETUP--------------------------------

void WiFiConnection(){

  // Conexión WIFI
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && NAcounts < 50) 
  { 
    delay(500);
    Serial.print(".");
    NAcounts ++;
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("reseteando");
    ESP.restart();
  }
  else
  {
    Serial.println("WiFi conectado");
    Serial.print("Conectado a: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("MAC address;\t");
    Serial.println(WiFi.macAddress());
    server.begin();
  } 
  
}
void setup(){
  Serial.begin(115200);
  WiFiConnection();
  pinMode(relay[0], OUTPUT); 
  pinMode(relay[1], OUTPUT); 
  pinMode(relay[2], OUTPUT); 
  digitalWrite(relay[0], LOW);
  digitalWrite(relay[1], LOW);
  digitalWrite(relay[2], LOW);
}

void httpRequest(){
  WiFiClient client = server.available();   // Escucha a los clientes entrantes

  if (client) {                             // Si se conecta un nuevo cliente
    Serial.println("New Client.");          // 
    String currentLine = "";                //
    while (client.connected()) {            // loop mientras el cliente está conectado
      if (client.available()) {             // si hay bytes para leer desde el cliente
        char c = client.read();             // lee un byte
        Serial.write(c);                    // imprime ese byte en el monitor serial
        header += c;
        if (c == '\n') {                    // si el byte es un caracter de salto de linea
          // si la nueva linea está en blanco significa que es el fin del 
          // HTTP request del cliente, entonces respondemos:
          
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // enciende y apaga el GPIO
            // 0 0 0
            if (header.indexOf("GET /0") >= 0 ) {
              if(stateRelay[0] == false){
                digitalWrite(relay[0], HIGH);
              }else
              {
                digitalWrite(relay[0], LOW);
              }
                stateRelay[0] = !stateRelay[0];
            }
            if (header.indexOf("GET /1") >= 0 ) {
              if(stateRelay[1] == false){
                digitalWrite(relay[1], HIGH);
              }else
              {
                digitalWrite(relay[1], LOW);
              }
                stateRelay[1] = !stateRelay[1];
            }
            if (header.indexOf("GET /2") >= 0 ) {
              if(stateRelay[2] == false){
                digitalWrite(relay[2], HIGH);
              }else
              {
                digitalWrite(relay[2], LOW);
              }
                stateRelay[2] = !stateRelay[2];
            }
            // Muestra la página web
            client.println(pagina);
            
            // la respuesta HTTP temina con una linea en blanco
            client.println();
            break;
          } else { // si tenemos una nueva linea limpiamos currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si C es distinto al caracter de retorno de carro
          currentLine += c;      // lo agrega al final de currentLine
        }
        
      }
    }
    // Limpiamos la variable header
    header = "";
    // Cerramos la conexión
  
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");

  }

}

void loop(){

  httpRequest();
  delay(250);
}


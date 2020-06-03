/*
 CÓDIGO MODIFICADO POR SAMUEL AQUINO RÍOS
 saquinorios@gmail.com  
 Crea un servidor Web en modo Access Point (AP) que permite
 encender y apagar 4 relevadores conectados a las salidas
 D0, D1, D2 y D3 del módulo NodeMCU con ESP-12F.
  
 Este código está Basado en el trabajo siguiente:
 NodeMCU Access Point - Servidor Web por Dani No www.esploradores.com
 Crea un servidor Web en modo Access Point que permite encender y apagar un LED conectado a la salida D4 (GPIO02) del módulo NodeMCU.
 Este código de ejemplo es de público dominio.
 */

#include <ESP8266WiFi.h>            //Incluye la librería ESP8266WiFi

const char ssid[] = "ESP_Test";     //Definimos la SSDI de nuestro servidor WiFi -nombre de red- 
const char password[] = "12345678"; //Definimos la contraseña de nuestro servidor 
WiFiServer server(80);              //Definimos el puerto de comunicaciones

int Relay1 = D0;  //Definimos el pin de salida - D1 para el relevador 1
int Relay2 = D1;  //Definimos el pin de salida - D1 para el relevador 2
int Relay3 = D2;  //Definimos el pin de salida - D1 para el relevador 3
int Relay4 = D3;  //Definimos el pin de salida - D1 para el relevador 4

int estadoRly1 = HIGH;  //Definimos la variable que va a recoger el estado del relevador 1
int estadoRly2 = HIGH;  //Definimos la variable que va a recoger el estado del relevador 2
int estadoRly3 = HIGH;  //Definimos la variable que va a recoger el estado del relevador 3
int estadoRly4 = HIGH;  //Definimos la variable que va a recoger el estado del relevador 4

void setup() {
  Serial.begin(115200);

  pinMode(Relay1, OUTPUT);      //Inicializamos el D0 como salida
  pinMode(Relay2, OUTPUT);      //Inicializamos el D1 como salida
  pinMode(Relay3, OUTPUT);      //Inicializamos el D2 como salida
  pinMode(Relay4, OUTPUT);      //Inicializamos el D3 como salida
  digitalWrite(Relay1, HIGH);   //Dejamos inicialmente el relevador 1 apagado
  digitalWrite(Relay2, HIGH);   //Dejamos inicialmente el relevador 2 apagado
  digitalWrite(Relay3, HIGH);   //Dejamos inicialmente el relevador 3 apagado
  digitalWrite(Relay4, HIGH);   //Dejamos inicialmente el relevador 4 apagado
  
  server.begin();                     //inicializamos el servidor
  WiFi.mode(WIFI_AP);                 //WiFi del NodeMCU en modo AP
  //WiFi.softAP(ssid, password);      //Red con clave, en el canal 1 y visible
  //WiFi.softAP(ssid, password,3,1);  //Red con clave, en el canal 3 y visible 
  WiFi.softAP(ssid);                  //Red abierta

  Serial.println();

  Serial.print("Direccion IP Access Point - por defecto: ");
  Serial.println(WiFi.softAPIP());                //Imprime la dirección IP
  Serial.print("Direccion MAC Access Point: ");                
  Serial.println(WiFi.softAPmacAddress());        //Imprime la dirección MAC

  //IPAddress local_ip(192, 168, 1, 1);           //Modifica la dirección IP 
  //IPAddress gateway(192, 168, 1, 1);   
  //IPAddress subnet(255, 255, 255, 0);
  //WiFi.softAPConfig(local_ip, gateway, subnet);
  //Serial.println();
  //Serial.print("Access Point - Nueva direccion IP: ");
  //Serial.println(WiFi.softAPIP());
}

void loop() 
{
  // Comprueba si el cliente ha conectado
  WiFiClient client = server.available();  
  if (!client) {
    return;
  }

  // Espera hasta que el cliente envía alguna petición
  Serial.println("Nuevo Cliente");
  while(!client.available()){
    delay(1);
  }

  // Imprime el número de clientes conectados
  Serial.printf("Clientes conectados al Access Point: %d", WiFi.softAPgetStationNum()); 
  Serial.println("");

  // Lee la petición
  String peticion = client.readStringUntil('\r');
  Serial.println(peticion);
  client.flush();

  // Comprueba la petición
  if (peticion.indexOf("/RLY1=ON") != -1) {
    estadoRly1 = HIGH;
  } 
  if (peticion.indexOf("/RLY1=OFF") != -1){
    estadoRly1 = LOW;
  }
  if (peticion.indexOf("/RLY2=ON") != -1) {
    estadoRly2 = HIGH;
  } 
  if (peticion.indexOf("/RLY2=OFF") != -1){
    estadoRly2 = LOW;
  }
  if (peticion.indexOf("/RLY3=ON") != -1) {
    estadoRly3 = HIGH;
  } 
  if (peticion.indexOf("/RLY3=OFF") != -1){
    estadoRly3 = LOW;
  }
  if (peticion.indexOf("/RLY4=ON") != -1) {
    estadoRly4 = HIGH;
  } 
  if (peticion.indexOf("/RLY4=OFF") != -1){
    estadoRly4 = LOW;
  }

  //Enciende o apaga el LED en función de la petición
  digitalWrite(Relay1, estadoRly1);
  digitalWrite(Relay2, estadoRly2);
  digitalWrite(Relay3, estadoRly3);
  digitalWrite(Relay4, estadoRly4);

  // Envía la página HTML de respuesta al cliente
  client.println("HTTP/1.1 200 OK");
  client.println("");                                     //No olvidar esta línea de separación
  client.println("<!DOCTYPE HTML>");
  client.println("<meta charset='UTF-8'>");
  client.println("<html>");

  //Imprime el estado de los Relevadores
  client.print("<h2>El Relevador 1 está ahora: ");                 
  if(estadoRly1 == HIGH) {
    client.print("ENCENDIDO</h2>");  
  } else {
    client.print("APAGADO</h2>");
  }
  client.print("<h2>El Relevador 2 está ahora: ");                 
  if(estadoRly2 == HIGH) {
    client.print("ENCENDIDO</h2>");  
  } else {
    client.print("APAGADO</h2>");
  }
  client.print("<h2>El Relevador 3 está ahora: ");                 
  if(estadoRly3 == HIGH) {
    client.print("ENCENDIDO</h2>");  
  } else {
    client.print("APAGADO</h2>");
  }
  client.print("<h2>El Relevador 4 está ahora: ");                 
  if(estadoRly4 == HIGH) {
    client.print("ENCENDIDO</h2>");  
  } else {
    client.print("APAGADO</h2>");
  }

  Se crean enlaces para modificar el estado de los Relevadores
  client.println("Presiona <a href='/RLY1=ON'>AQUÍ</a> para encender el Relevador 1<br>");
  client.println("Presiona <a href='/RLY1=OFF'>AQUÍ</a> para apagar el Relevador 1<br><br>");
  client.println("Presiona <a href='/RLY2=ON'>AQUÍ</a> para encender el Relevador 2<br>");
  client.println("Presiona <a href='/RLY2=OFF'>AQUÍ</a> para apagar el Relevador 2<br><br>");
  client.println("Presiona <a href='/RLY3=ON'>AQUÍ</a> para encender el Relevador 3<br>");
  client.println("Presiona <a href='/RLY3=OFF'>AQUÍ</a> para apagar el Relevador 3<br><br>");
  client.println("Presiona <a href='/RLY4=ON'>AQUÍ</a> para encender el Relevador 4<br>");
  client.println("Presiona <a href='/RLY4=OFF'>AQUÍ</a> para apagar el Relevador 4<br><br>");

  //Se crean cajas de comprobación (checkbox) para modificar el estado de los Relevadores
  client.println("<input type='checkbox' onClick=location.href='/RELY1=ON'> ENCENDER Relevador 1 </input><br>");
  client.println("<input type='checkbox' onClick=location.href='/RELY1=OFF'> APAGAR Relevador 1</input><br><br>");
  client.println("<input type='checkbox' onClick=location.href='/RELY2=ON'> ENCENDER Relevador 2 </input><br>");
  client.println("<input type='checkbox' onClick=location.href='/RELY2=OFF'> APAGAR Relevador 2</input><br><br>");
  client.println("<input type='checkbox' onClick=location.href='/RELY3=ON'> ENCENDER Relevador 3 </input><br>");
  client.println("<input type='checkbox' onClick=location.href='/RELY3=OFF'> APAGAR Relevador 3</input><br><br>");
  client.println("<input type='checkbox' onClick=location.href='/RELY4=ON'> ENCENDER Relevador 4 </input><br>");
  client.println("<input type='checkbox' onClick=location.href='/RELY4=OFF'> APAGAR Relevador 4</input><br><br>");

  //Se crean botones para modificar el estado del LED
  client.println("<button type='button' onClick=location.href='/RLY1=ON'> ENCENDER Relevador 1</button>");
  client.println("<button type='button' onClick=location.href='/RLY1=OFF'> APAGAR Relevador 1</button><br><br>");
  client.println("<button type='button' onClick=location.href='/RLY2=ON'> ENCENDER Relevador 2</button>");
  client.println("<button type='button' onClick=location.href='/RLY2=OFF'> APAGAR Relevador 2</button><br><br>");
  client.println("<button type='button' onClick=location.href='/RLY3=ON'> ENCENDER Relevador 3</button>");
  client.println("<button type='button' onClick=location.href='/RLY3=OFF'> APAGAR Relevador 3</button><br><br>");
  client.println("<button type='button' onClick=location.href='/RLY4=ON'> ENCENDER Relevador 4</button>");
  client.println("<button type='button' onClick=location.href='/RLY4=OFF'> APAGAR Relevador 4</button><br><br>");

  //Se crean botones con estilos para modificar el estado de los Relevadores
  client.println("<button type='button' onClick=location.href='/RLY1=ON' style='margin:auto; background-color:green; color:#A9F5A9; padding:5px; border:2px solid black; width:200;'><h2> ENCENDER RL 1</h2> </button>");
  client.println("<button type='button' onClick=location.href='/RLY1=OFF' style='margin:auto; background-color:red; color:#F6D8CE; padding:5px; border:2px solid black; width:200;'><h2> APAGAR  RL 1</h2> </button><br><br>");
  client.println("<button type='button' onClick=location.href='/RLY2=ON' style='margin:auto; background-color:green; color:#A9F5A9; padding:5px; border:2px solid black; width:200;'><h2> ENCENDER RL 2</h2> </button>");
  client.println("<button type='button' onClick=location.href='/RLY2=OFF' style='margin:auto; background-color:red; color:#F6D8CE; padding:5px; border:2px solid black; width:200;'><h2> APAGAR  RL 2</h2> </button><br><br>");
  client.println("<button type='button' onClick=location.href='/RLY3=ON' style='margin:auto; background-color:green; color:#A9F5A9; padding:5px; border:2px solid black; width:200;'><h2> ENCENDER RL 3</h2> </button>");
  client.println("<button type='button' onClick=location.href='/RLY3=OFF' style='margin:auto; background-color:red; color:#F6D8CE; padding:5px; border:2px solid black; width:200;'><h2> APAGAR  RL 3</h2> </button><br><br>");
  client.println("<button type='button' onClick=location.href='/RLY4=ON' style='margin:auto; background-color:green; color:#A9F5A9; padding:5px; border:2px solid black; width:200;'><h2> ENCENDER RL 4</h2> </button>");
  client.println("<button type='button' onClick=location.href='/RLY4=OFF' style='margin:auto; background-color:red; color:#F6D8CE; padding:5px; border:2px solid black; width:200;'><h2> APAGAR  RL 4</h2> </button><br><br>");

  //Se crea un único botón para modificar el estado de los Relevadores
  if(estadoRly1 == HIGH) {
    client.print("<button type='button' onClick=location.href='/RLY1=OFF'> APAGAR R1</button><br><br>");  
  } else {
    client.print("<button type='button' onClick=location.href='/RLY1=ON'> ENCENDER R1</button><br><br>");
  }
  if(estadoRly2 == HIGH) {
    client.print("<button type='button' onClick=location.href='/RLY2=OFF'> APAGAR R2</button><br><br>");  
  } else {
    client.print("<button type='button' onClick=location.href='/RLY2=ON'> ENCENDER R2</button><br><br>");
  }
  if(estadoRly3 == HIGH) {
    client.print("<button type='button' onClick=location.href='/RLY3=OFF'> APAGAR R3</button><br><br>");  
  } else {
    client.print("<button type='button' onClick=location.href='/RLY3=ON'> ENCENDER R3</button><br><br>");
  }
  if(estadoRly4 == HIGH) {
    client.print("<button type='button' onClick=location.href='/RLY4=OFF'> APAGAR R4</button><br><br>");  
  } else {
    client.print("<button type='button' onClick=location.href='/RLY4=ON'> ENCENDER R4</button><br><br>");
  }
  client.println("</html>"); 
  delay(1);
  Serial.println("Petición finalizada");          // Se finaliza la petición al cliente. Se inicaliza la espera de una nueva petición.

  //Desconexión de los clientes
  //WiFi.softAPdisconnect();
}

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

String ssid     = "nome della rete wifi";
String password = "password";

ESP8266WebServer server(80);

String head = "<!DOCTYPE html><html><head><title>ESP8266 LED</title></head>";
String body  = "<body><h1>Controllo LED</h1><a href=\"/ledon\"><button class=\"button\">ON</button></a>  <a href=\"/ledoff\"><button class=\"button\">OFF</button></a>";
String footer= "</body></html>";
String html;



void pagina()
{
  html = head + body + footer;
  server.send(200, "text/html", html);   
}

void paginaon()
{
 digitalWrite(D0,HIGH); 
 String stato = "<br>LED acceso";
 html = head + body + stato + footer;
 server.send(200, "text/html", html );
}

void paginaoff()
{
 digitalWrite(D0,LOW); 
 String stato = "<br>LED spento";
 html = head + body + stato + footer;
 server.send(200, "text/html", html );
}

void setup() {
  Serial.begin(115200);
  
 /* IPAddress ip(192, 168, 1, 108); 
  IPAddress gateway(192, 168, 1, 1); 
  IPAddress subnet(255, 255, 255, 0); 
  WiFi.config(ip, gateway, subnet);
*/
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Sei collegato alla rete LAN");
  Serial.println(WiFi.localIP());
  
  pinMode(D0,OUTPUT); 
  digitalWrite(D0,LOW); 
  
  server.begin();  
  Serial.println("Server online");

  //gestiamo le azione passate col GET nell'url
  server.on("/",    pagina);    //pagina base
  server.on("/ledon",  paginaon);  //pagina cliccato acceso
  server.on("/ledoff", paginaoff); //pagina cliccato spento
}

void loop() {
  server.handleClient(); 
  Serial.println("attendo la chiamata client");
}

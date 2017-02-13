#include <ESP8266WiFi.h>
#include <PubSubClient.h>


IPAddress server(192, 168, 0, 56); // IP de la raspberry Pi

const char* ssid     = "SSID"; // Your ssid
const char* pass = "Password"; // Your Password

int alarmON=1;
int alarmOFF=0;
int pir = 2;
int relay = 0;
boolean estado_pir=0;

#define BUFFER_SIZE 100
WiFiClient wclient;
PubSubClient client(wclient, server);

void callback(const MQTT::Publish& pub) {
    if(pub.payload_string() == "on")
    {
      digitalWrite(relay, LOW); // en caso de que el modulo rele funcione al reves, cambiarl LOW por HIGH
    }
    else
    {
      digitalWrite(relay, HIGH); // en caso de que el modulo rele funcione al reves, cambiarl HIGH por LOW
    }  
}
void setup() 
{
  pinMode(pir,INPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  client.set_callback(callback);
  if (WiFi.status() != WL_CONNECTED) 
  {
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println("...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  return;
  Serial.println("WiFi connected");
  }
  
}

void loop() 
{
if (WiFi.status() == WL_CONNECTED)
{
estado_pir=digitalRead(pir);
if (estado_pir==HIGH)
{
if (client.connected())
  {
    Serial.println(String(alarmON))  ;
    client.publish("Movimiento1",String(alarmON));
    delay(5000);
    Serial.println(String(alarmOFF))  ;
    client.publish("Movimiento1",String(alarmOFF));
  }  
}

{
  if (!client.connected()) 
  {
    if (client.connect("ESP8266: LuzPIR1"))  
    {   
      client.subscribe("LuzPIR1");
    }
  }

    client.loop();
  }
  delay(1);
} 
}


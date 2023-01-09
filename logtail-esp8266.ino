#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid     = "";
const char* password = "";

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  pinMode(LED_BUILTIN, OUTPUT);
  
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
  }  
}
 
void loop() {
  WiFiClientSecure client;
  client.setInsecure();

  String PostData = "{\"message\": \"From ESP with love!\"}";
  const char* host = "in.logtail.com"; // IP address of Logstash server

  if (client.connect(host, 443)) {
    Serial.println("Connected to logtail ..");
    
    digitalWrite(LED_BUILTIN, LOW); // This mixed with the lower HIGH will just flash the built in LED while uploading
    
    client.println("POST / HTTP/1.1");
    client.println("Host: in.logtail.com");
    client.println("Cache-Control: no-cache");
    client.println("Content-Type: application/json");
    client.println("Authorization: Bearer <TOKEN HERE>");
    client.print("Content-Length: ");
    client.println(PostData.length());
    client.println();
    client.println(PostData);
    client.stop();
    Serial.println("Sent to logtail ..");
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    Serial.println("connection failed");
  }

  delay(5000); // Delay before taking next readin in milliseconds
}

#include <Arduino.h>
#include <SoftwareSerial.h>

#define st true
String content, post, com;

SoftwareSerial esp(10, 11);
// function dec
int getWaterLevel();
String sendData(String command, const int timeout, boolean debug);

void setup()
{
  Serial.begin(115200);
  Serial.begin(115200);
  esp.begin(115200);
  sendData("AT+RST\r\n", 10000, st);
  sendData("AT+CWMODE=3\r\n", 1000, st);
  sendData("AT+CWJAP=\"MonaConnect\",\"\"\r\n", 5000, st);
}

void loop()
{
  sendData("AT+CIPSTART=\"TCP\",\"10.22.12.97\",5000\r\n", 3000, st);
  content = "";
  content = "{	\"tank_id\": \"5\"" + String(", \"water_level\": ");
  content += String(getWaterLevel());
  content += String(" }");
  post = "";
  post = String("POST /tank HTTP/1.1\r\nHost: 10.22.12.97:5000\r\nContent-Type: application/json\r\nContent-Length: ");
  post += String(content.length());
  post += String("\r\n\r\n");
  post += content;
  post += String("\r\n\r\n");
  com = "";
  com = String("AT+CIPSEND=");
  com += post.length();
  com += String("\r\n");
  sendData(com, 2000, st);
  sendData(post, 5000, st);
  delay(2000);
}

int getWaterLevel()
{
  return random(10, 201);
}

String sendData(String command, const int timeout, boolean debug)
{
  String response = "";

  esp.print(command); // send the read character to the esp8266

  unsigned long int time = millis();

  while ((time + timeout) > millis())
  {
    while (esp.available())
    {

      // The esp has data so display its output to the serial window
      char c = esp.read(); // read the next character.
      response += c;
    }
  }

  if (debug)
  {
    Serial.print(response);
  }

  return response;
}
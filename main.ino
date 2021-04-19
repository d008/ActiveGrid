// sudo chmod a+rw /dev/ttyUSB0
// :D 

#include <ESP8266WiFi.h>
#define dirPin 2
#define stepPin 0
#define stepsPerRevolution 1000
#define hallPin 4
#define enablePin 5

const char* ssid     = "TP-LINK_6BCD";
const char* password = "51360306";
String dirString = "0";
String delayString = "2000";
String stepdegString = "1";
String timesString = "1"; 
int dir = 0;
int delayMS = 500;
int stepDeg = 1;
int steps = 5; 
int times = 1; 

WiFiClient client;
WiFiServer server(80);  // Set web server port number to 80

// NETWORK: Static IP details
IPAddress ip(192,168,0,183);
IPAddress gateway(192,168,0,0);
IPAddress subnet(255,255,255,0);


void setup() {
  // Sets the two pins as outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(hallPin, INPUT);
  pinMode(enablePin, OUTPUT);

  digitalWrite(enablePin,HIGH); // high disables
  
  Serial.begin(115200);
  delay(10);

  // Connect WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Static IP setup info 
  WiFi.config(ip,gateway,subnet);

  // Start server
  WiFi.hostname("Name");
  WiFi.begin(ssid, password);
  
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Print the IP address
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
}





void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  client.setNoDelay(1);
  if(client)
    {
      Serial.println("New Client");
      String req = ""; 
      while (client.connected())
        {
          while(client.available())
            {
              
              Serial.println("client available");
              String req = client.readStringUntil('\r');
              Serial.println(req);


              if (req.indexOf("X") != -1) 
                {
                  digitalWrite(enablePin,LOW);
                  
                }

        
              if (req.indexOf("H") != -1) 
                {
                  digitalWrite(enablePin,LOW);
                  digitalWrite(dirPin,HIGH);
                  while(digitalRead(hallPin))
                    {
                      digitalWrite(stepPin,HIGH);
                      delayMicroseconds(1000);
                      digitalWrite(stepPin,LOW);
                      delayMicroseconds(1000);
                      yield();
                    }
                   digitalWrite(enablePin,HIGH);
                   digitalWrite(enablePin,HIGH);
                }
                

                if (req.indexOf("R") != -1)
                  {
                    digitalWrite(enablePin,LOW);
                    dirString = req[req.indexOf("R")+1];
                    delayString = req.substring(req.indexOf("D")+1,req.indexOf("S"));
                    stepdegString = req.substring(req.indexOf("S")+1,req.indexOf("T"));
                    timesString = req.substring(req.indexOf("T")+1);
                    long dir = dirString.toInt();
                    long delayMS = delayString.toInt();
                    long stepDeg = stepdegString.toInt();
                    steps = stepsPerRevolution*stepDeg/360L;
                    int times = timesString.toInt();
                    Serial.println(dir); 
                    Serial.println(delayMS); 
                    Serial.println(stepDeg); 
                    Serial.println(times); 
                    
                    digitalWrite(dirPin,dir);
                    for (int j = 1; j<times+1; j++){
                      for (int i = 0; i < steps; i++) {
                        // These four lines result in 1 step:
                        digitalWrite(stepPin, HIGH);
                        delayMicroseconds(delayMS);
                        digitalWrite(stepPin, LOW);
                        delayMicroseconds(delayMS);
                        yield();
                      }
                    }
                    digitalWrite(enablePin,HIGH);
                  }

//                if (req.indexOf("S") != -1)
//                  {
//                    digitalWrite(enablePin,LOW);
//                    dirString = req[req.indexOf("S")+1];
//                    stepdegString = req.substring(req.indexOf("S")+2);
//                    long dir = dirString.toInt();
//                    long stepDeg = stepdegString.toInt();
//                    digitalWrite(dirPin,dir);
//                    steps = stepsPerRevolution*stepDeg/360L;
//                    Serial.println(steps);
//                    
//                    for (int i = 0; i < steps; i++) {
//                      // These four lines result in 1 step:
//                      digitalWrite(stepPin, HIGH);
//                      delayMicroseconds(1000);
//                      digitalWrite(stepPin, LOW);
//                      delayMicroseconds(1000);
//                      yield();
//                    }
//                    digitalWrite(enablePin,HIGH);
//                  }
                
              client.flush();
              break;
              
            }
        }
      client.stop();
      Serial.println("Client disconnected.");
      Serial.println("");  
    }
  
  
}

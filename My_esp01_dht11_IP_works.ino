/*
* Charlie's Weather Station
* using a esp8266 and dht11
* Add static IP address 
* 8-16-2018
*/ 
 
#include "ESP8266WiFi.h"                                
#include "Adafruit_Sensor.h"                            
#include "DHT.h"                                        
 
const char* ssid = "REPLACE_WITH_YOUR_SSID";                           
const char* password = "REPLACE_WITH_YOUR_PASSWORD";                     

 IPAddress staticIP(192,168,1,250);
 IPAddress gateway(192,168,1,1);
 IPAddress subnet(255,255,255,0);

#define DHTPIN 2                                         
#define DHTTYPE DHT11                                   
  // #define DHTTYPE DHT21
  // #define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);                               
WiFiServer server(80);                                  

       float tF;
       float dP;
       float dPF;
 
 
void setup() 
{
  Serial.begin(115200);                                  
  delay(10);                                            
  dht.begin();                                          
                             
  Serial.println("");                                    
  Serial.print("Connecting to ");                       
  Serial.println(ssid);                                 

//  WiFi.config(staticIP, gateway, subnet);
  delay(100);
  WiFi.mode(WIFI_STA);
  WiFi.config(staticIP, gateway, subnet);
  WiFi.begin(ssid, password);                           
   
  while (WiFi.status() != WL_CONNECTED)                 
  {
   delay(500);                                          
   Serial.print(".");                                   
  }
   Serial.println("");                                                                            
   Serial.println("WiFi connected");                    
   server.begin();                                      
   Serial.println("Web server running.");               
   delay(10000);                                        
   Serial.println(WiFi.localIP());                      
}
void loop() 
{
 WiFiClient client = server.available();                 
  
 if (client){
  Serial.println("New client");                         
  boolean blank_line = true;                             
  while (client.connected()){                            
    if (client.available()){                             
     char c = client.read();                            
     if (c == '\n' && blank_line){                       
       float t = dht.readTemperature();                  
       float h = dht.readHumidity();
       float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

 // Compute heat index in Fahrenheit (the default)
   float hif = dht.computeHeatIndex(f, h);
  
 
//       t = t* 9/5 + 32;
       dP=(dewPointFast(t,h));
       dPF=((dP*9)/5)+32; 
       
       
       
       client.println("HTTP/1.1 200 OK");                
       client.println("Content-Type: text/html"); 
       client.println("Connection: close");             
       client.println("Refresh: 10");                    
       client.println();
       client.println("<!DOCTYPE HTML>");               
       client.println("<html>");                         
       client.println("<head>");
       client.print("<title>ESP8266 TEMP</title>");     
       client.println("</head>");
       client.println("<body>");
       client.println("<h1>Charlie's Weather Station</h1>");
       client.println("<h1>ESP8266 - Temperature & Humidity</h1>"); 
       client.println("<h3>Temmperature = ");
       client.println(f);
       client.println("F</h3>");
       client.println("</head>");
       client.println("<body>");
       client.println("<h3>Humidity = ");
       client.println(h);                               
       client.println("%</h3>");
       client.println("</body>");
       client.println("<body>");
       client.println("<h3>Heat Index = ");
       client.println(hif);                               
       client.println("F</h3>");
       client.println("</body>");
       client.println("<body>");
       client.println("<h3>Dew Point = ");
       client.println(dPF);                               
       client.println("F</h3>");
       client.println("</body>");
       client.println("</html>");                        
       break;                                           
       }
        if (c == '\n'){                                                                              
         blank_line = true;                             
        }                                          
         else if (c != '\r'){                                                                  
          blank_line = false;                            
         }                                        
    }
  }  
    delay(1);
    client.stop();                                      
    Serial.println("Client disconnected.");             
  }
}

// delta max = 0.6544 wrt dewPoint()
// 6.9 x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
double a = 17.271;
double b = 237.7;
double temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
double Td = (b * temp) / (a - temp);
return Td;
}

double heatIndex(double tempF, double humidity)
{
double c1 = -42.38, c2 = 2.049, c3 = 10.14, c4 = -0.2248, c5= -6.838e-3, c6=-5.482e-2, c7=1.228e-3, c8=8.528e-4, c9=-1.99e-6 ;
double T = tempF;
double R = humidity;

double A = (( c5 * T) + c2) * T + c1;
double B = ((c7 * T) + c4) * T + c3;
double C = ((c9 * T) + c8) * T + c6;

double rv = (C * R + B) * R + A;
return rv;
}

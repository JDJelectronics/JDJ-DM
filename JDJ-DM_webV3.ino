/*
 SPECIAL FOR ESP8266 Version
 Name:    JDJ-DM v2.5
 Created: 3/11/2019
 Author:  JDJ Electronics
*/

//Lib
#include <Arduino.h>
#include <U8g2lib.h>
#include <dht11.h>  
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#define RELAY 16
#define dht 13
dht11 DHT;

// Variable
bool schakel;
bool r_on;
float currentDewpoint;

int BAT= A0;              //Analog channel A0 as used to measure battery voltage
float RatioFactor=4.320;  //Resistors Ration Factor 


// Oled 0.96
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 5, /* data=*/ 4);   // ESP32 Thing, HW I2C with pin remapping


// Create WebServer object on port 80
ESP8266WebServer server(80);  


 void handle_OnConnect() {
  DHT.read(dht);  
  float temperature = DHT.temperature;
  float humidity = DHT.humidity;
  float currentDewpoint = dewPoint(temperature, humidity);
 // pressure = bme.readPressure() / 100.0F;
 // altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  server.send(200, "text/html", SendHTML(temperature,humidity,currentDewpoint)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float temperature,float humidity,float currentDewpoint){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta http-equiv=refresh content=15 name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<link rel=\"icon\" href=\"https://www.jdjelectronics.nl/wp-content/uploads/2019/10/cropped-Favicon-1.0-3-270x270.png\" type= \"image/x-icon>\n";
  ptr +="<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  ptr +="<title>JDJ electronics | dauwpuntmeter</title>\n";
  ptr +="<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #303030;}\n";
  ptr +="body{margin-top: 50px;}\n";
  ptr +="h1 {margin: 50px auto 30px;}\n";
  ptr +=".side-by-side{display: inline-block;vertical-align: middle;position: relative;}\n";
  ptr +=".humidity-icon{background-color: #3498db;width: 30px;height: 30px;border-radius: 50%;line-height: 36px;}\n";
  ptr +=".humidity-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}\n";
  ptr +=".humidity{font-weight: 300;font-size: 60px;color: #White;}\n";
  ptr +=".temperature-icon{background-color: #f39c12;width: 30px;height: 30px;border-radius: 50%;line-height: 40px;}\n";
  ptr +=".temperature-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}\n";
  ptr +=".temperature{font-weight: 300;font-size: 60px;color: #White;}\n";
  ptr +=".superscript{font-size: 17px;font-weight: 600;position: absolute;right: -20px;top: 15px;}\n";
  ptr +=".data{padding: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  
   ptr +="<div id=\"webpage\">\n";
   
   ptr +="<h1>JDJ ELECTRONICS</h1>\n";
   ptr +="<div class=\"data\">\n";
   ptr +="<div class=\"side-by-side temperature-icon\">\n";
   ptr +="<svg version=\"1.1\" id=\"Layer_1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"\n";
   ptr +="width=\"9.915px\" height=\"22px\" viewBox=\"0 0 9.915 22\" enable-background=\"new 0 0 9.915 22\" xml:space=\"preserve\">\n";
   ptr +="<path fill=\"#303030\" d=\"M3.498,0.53c0.377-0.331,0.877-0.501,1.374-0.527C5.697-0.04,6.522,0.421,6.924,1.142\n";
   ptr +="c0.237,0.399,0.315,0.871,0.311,1.33C7.229,5.856,7.245,9.24,7.227,12.625c1.019,0.539,1.855,1.424,2.301,2.491\n";
   ptr +="c0.491,1.163,0.518,2.514,0.062,3.693c-0.414,1.102-1.24,2.038-2.276,2.594c-1.056,0.583-2.331,0.743-3.501,0.463\n";
   ptr +="c-1.417-0.323-2.659-1.314-3.3-2.617C0.014,18.26-0.115,17.104,0.1,16.022c0.296-1.443,1.274-2.717,2.58-3.394\n";
   ptr +="c0.013-3.44,0-6.881,0.007-10.322C2.674,1.634,2.974,0.955,3.498,0.53z\"/>\n";
   ptr +="</svg>\n";
   ptr +="</div>\n";
   ptr +="<div class=\"side-by-side temperature-text\">Temperature</div>\n";
   ptr +="<div class=\"side-by-side temperature\">";
   ptr +=(int)temperature;
   ptr +="<span class=\"superscript\">°C</span></div>\n";
   ptr +="</div>\n";
   ptr +="<div class=\"data\">\n";
   ptr +="<div class=\"side-by-side humidity-icon\">\n";
   ptr +="<svg version=\"1.1\" id=\"Layer_2\" xmlns=\"\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"\n\"; width=\"12px\" height=\"17.955px\" viewBox=\"0 0 13 17.955\" enable-background=\"new 0 0 13 17.955\" xml:space=\"preserve\">\n";
   ptr +="<path fill=\"#FFFFFF\" d=\"M1.819,6.217C3.139,4.064,6.5,0,6.5,0s3.363,4.064,4.681,6.217c1.793,2.926,2.133,5.05,1.571,7.057\n";
   ptr +="c-0.438,1.574-2.264,4.681-6.252,4.681c-3.988,0-5.813-3.107-6.252-4.681C-0.313,11.267,0.026,9.143,1.819,6.217\"></path>\n";
   ptr +="</svg>\n";
   ptr +="</div>\n";
   ptr +="<div class=\"side-by-side humidity-text\">Humidity</div>\n";
   ptr +="<div class=\"side-by-side humidity\">";
   ptr +=(int)humidity;
   ptr +="<span class=\"superscript\">%</span></div>\n";
   ptr +="</div>\n";
   ptr +=(float)currentDewpoint;
   ptr +="<span class=\"superscript\">%</span></div>\n";
   ptr +="</div>\n";
   
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}  


void setup(void) {
  Serial.begin(115200);
  Wire.begin(5,4); // change hardware I2C pins to (5,4) (D1,D2)
  int chk = DHT.read(dht);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, 1);// Turns Relay On
  u8g2.begin(); // init display
//-------------------------------------------  
//Versie firmware 
 u8g2.clearBuffer();         // clear the internal memory  
 u8g2.setFont(u8g2_font_helvB08_tr);
 u8g2.drawStr( 0, 10, "JDJ electronics");
 u8g2.drawStr( 0, 30, "Model: DM");
 u8g2.drawStr( 0, 40, "Firmware: v2.5"); 
 u8g2.drawStr( 0, 50, "251019"); //firmware date  
 u8g2.drawStr( 10, 60, "2019");
 u8g2.setCursor(0, 60);
 u8g2.setFont(u8g2_font_helvB08_tf);
 u8g2.print(char(169)); // copyright karakter
 u8g2.sendBuffer();          // transfer internal memory to the display 
 delay(3000);
   
//Wifi setup + oled
 u8g2.clearBuffer();         // clear the internal memory  
 u8g2.setCursor(15, 36);
 u8g2.setFont(u8g2_font_open_iconic_www_2x_t);
 u8g2.print(char(81)); // WIFI icon
 u8g2.setFont(u8g2_font_helvB08_tr);
 u8g2.drawStr( 40, 32, "Connect to");
 u8g2.drawStr( 40, 45, "JDJ-DM");
 u8g2.sendBuffer();          // transfer internal memory to the display  
 
  WiFiManager wifiManager;
  wifiManager.autoConnect("JDJ-DM");
  wifiManager.setConfigPortalTimeout(180);
    if(!wifiManager.autoConnect("JDJ-DM")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }
  
  Serial.println("Connected.");
  server.begin();

 server.on("/", handle_OnConnect);
 server.onNotFound(handle_NotFound);

 server.begin();
 Serial.println("HTTP server started");
 Serial.println(WiFi.localIP());
 
//Connected Succes 
 u8g2.clearBuffer();         // clear the internal memory  
 u8g2.setCursor(15, 36);
 u8g2.setFont(u8g2_font_open_iconic_www_2x_t);
 u8g2.print(char(81)); // WIFI icon
 u8g2.setFont(u8g2_font_helvB08_tr);
 u8g2.drawStr( 40, 32, "Connected");
 u8g2.setCursor( 40, 45);
 u8g2.print(WiFi.localIP());
 u8g2.sendBuffer();          // transfer internal memory to the display 
 delay(1000); 
 
//Check up
 u8g2.clearBuffer();         // clear the internal memory  
 u8g2.setCursor(15, 36);
 u8g2.setFont(u8g2_font_open_iconic_embedded_2x_t);
 u8g2.print(char(66)); // tandwiel
 u8g2.setFont(u8g2_font_helvB08_tr);
 u8g2.drawStr( 40, 32, "DM-CHECK");
 u8g2.drawStr( 40, 45, "WAIT...");
 u8g2.sendBuffer();          // transfer internal memory to the display  
 delay(1000);

 //Volt check up
 u8g2.clearBuffer();         // clear the internal memory  
 u8g2.setCursor(15, 36);
 u8g2.setFont(u8g2_font_open_iconic_embedded_2x_t);
 u8g2.print(char(73)); // tandwiel
 u8g2.setFont(u8g2_font_helvB08_tr);
 u8g2.drawStr( 40, 32, "VOLTAGE");
 u8g2.drawStr( 40, 45, "CHECK...");
 u8g2.sendBuffer();          // transfer internal memory to the display  
 delay(1000);

 //Volt check compleet
 u8g2.clearBuffer();         // clear the internal memory  
 u8g2.setCursor(15, 36);
 u8g2.setFont(u8g2_font_open_iconic_embedded_2x_t);
 u8g2.print(char(73)); // tandwiel
 u8g2.setFont(u8g2_font_helvB08_tr);
 u8g2.drawStr( 40, 32, "VOLTAGE");
 u8g2.setCursor( 40, 45);
 //u8g2.println(Tvoltage);
 u8g2.sendBuffer();          // transfer internal memory to the display  
 delay(1000);
 
//Check compleet
 u8g2.clearBuffer();         // clear the internal memory 
 u8g2.setCursor(45, 45);
 u8g2.setFont(u8g2_font_open_iconic_check_4x_t);
 u8g2.print(char(64)); // vinkje iccon
 u8g2.sendBuffer();
 delay(1000);
 
 
//-------------------------------------------
}

void loop(void) {
  server.handleClient();
  
  float Tvoltage=0.0;
  float Vvalue=0.0,Rvalue=0.0;

    /////////////////////////////////////Battery Voltage//////////////////////////////////  
  for(unsigned int i=0;i<10;i++){
  Vvalue=Vvalue+analogRead(BAT);         //Read analog Voltage
  delay(5);                              //ADC stable
  }
  Vvalue=(float)Vvalue/10.0;            //Find average of 10 bits values
  Rvalue=(float)(Vvalue/1024.0)*5;      //Convert Voltage in 5v factor
  Tvoltage=Rvalue*RatioFactor;          //Find original voltage by multiplying with factor
    /////////////////////////////////////Battery Voltage//////////////////////////////////
  Serial.println(Tvoltage);
        
  schakel = !schakel;
  DHT.read(dht);  // Lees pin analog op DHT11
  float h = DHT.humidity;
  // Read temperature as Celsius (the default)
  float t = DHT.temperature;

  currentDewpoint = dewPoint(t, h);
  int Dewpointalarm = (currentDewpoint + 2) * 100;
  int t100 = t * 100;

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("Dew Point: ");

    // Relay configuratie
    if (Dewpointalarm >= t100) {
    digitalWrite(RELAY, 1); // Relay AAN
    //Serial.print("Relay On");
    r_on = true;
    Serial.print(" Dewpointalarm: ");
    Serial.print(Dewpointalarm);
    Serial.print(" > ");
    Serial.println(t100);
  }
  else {
    r_on = false;
    digitalWrite(RELAY, 0);// Relay UIT
    Serial.println("Relay Off");   
    }
//-------------------------------------------    
  // Oled dauwpunt AAN   
    if (Dewpointalarm >= t100) {
      u8g.firstPage();
      do
      {
        drawON();
      } while (u8g.nextPage());
      
    }
    
    else {
      u8g.firstPage();
      do
      {
        drawOFF();
      } while (u8g.nextPage());
    }
    delay(2000);
  } 
  

----------------------------------------------

  void drawON(void) }
  
  u8g2.clearBuffer();         // clear the internal memory 
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(25, 10, "JDJ Electronics");
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(0, 30, "Temp: ");   // put string of display at position X, Y
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(0, 43, "Humi : ");
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setCursor(35, 30);        // set position
  u8g2.print(DHT.temperature, 1);  // display temperature from DHT11 in Celsius
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.println(char(176)); // Celcius karakter
  u8g2.setFont(u8g2_font_helvB08_tr);   // select font
  u8g2.drawStr(50, 30, "C");
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.setCursor(35, 43);        // set position
  u8g2.print(DHT.humidity, 1);     // display humidity from DHT11
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.setCursor(50, 43);        // set position
  u8g2.println("%");
  u8g2.setFont(u8g2_font_helvB08_tr);   // select font
  u8g2.drawStr(70, 30, "DM STATE");
  u8g2.setCursor(100, 53);
  u8g2.setFont(u8g2_font_open_iconic_embedded_2x_t);
  u8g2.print(char(78)); // uit icoon
  u8g2.setFont(u8g2_font_helvB08_tr);   // select font
  u8g2.drawStr(70, 50, "ON");
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(0, 56, "DewP:");
  u8g2.setCursor(35, 56);
  u8g2.print(currentDewpoint, 0);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.println(char(176)); // Celcius karakter
  u8g2.setFont(u8g2_font_helvB08_tr);   // select font
  u8g2.drawStr(50, 56, "C");
  u8g2.sendBuffer(); 
  }


  void drawOFF(void) ]
  
  u8g2.clearBuffer();         // clear the internal memory 
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(25, 10, "JDJ Electronics");
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(0, 30, "Temp: ");   // put string of display at position X, Y
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(0, 43, "Humi : ");
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setCursor(35, 30);        // set position
  u8g2.print(DHT.temperature, 1);  // display temperature from DHT11 in Celsius
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.println(char(176)); // Celcius karakter
  u8g2.setFont(u8g2_font_helvB08_tr);   // select font
  u8g2.drawStr(50, 30, "C");
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.setCursor(35, 43);        // set position
  u8g2.print(DHT.humidity, 1);     // display humidity from DHT11
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.setCursor(50, 43);        // set position
  u8g2.println("%");
  u8g2.setFont(u8g2_font_helvB08_tr);   // select font
  u8g2.drawStr(70, 30, "DM STATE");
  u8g2.setCursor(100, 53);
  u8g2.setFont(u8g2_font_open_iconic_embedded_2x_t);
  u8g2.print(char(78)); // uit icoon
  u8g2.setFont(u8g2_font_helvB08_tr);   // select font
  u8g2.drawStr(70, 50, "OFF");
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(0, 56, "DewP:");
  u8g2.setCursor(35, 56);
  u8g2.print(currentDewpoint, 0);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.println(char(176)); // Celcius karakter
  u8g2.setFont(u8g2_font_helvB08_tr);   // select font
  u8g2.drawStr(50, 56, "C");
  u8g2.sendBuffer();
  }

  void drawMANUEL(void) {
  
  u8g2.clearBuffer();         // clear the internal memory   
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(25, 10, "JDJ Electronics");
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(0, 30, "Temp: ");   // put string of display at position X, Y
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(0, 43, "Humi : ");
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setCursor(35, 30);        // set position
  u8g2.print(DHT.temperature, 1);  // display temperature from DHT11 in Celsius
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.println(char(176)); // Celcius karakter
  u8g2.setFont(u8g2_font_helvB08_tr);   // select font
  u8g2.drawStr(50, 30, "C");
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.setCursor(35, 43);        // set position
  u8g2.print(DHT.humidity, 1);     // display humidity from DHT11
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.setCursor(50, 43);        // set position
  u8g2.println("%");
  u8g2.setFont(u8g2_font_helvB08_tr);   // select font
  u8g2.drawStr(70, 30, "MANUAL");
  u8g2.setCursor(85, 53);
  u8g2.setFont(u8g2_font_open_iconic_embedded_2x_t);
  u8g2.print(char(65)); // Bel icoon
  u8g2.setFont(u8g2_font_helvB08_tr);   // select font
  u8g2.drawStr(70, 50, "ON");
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(0, 56, "DewP:");
  u8g2.setCursor(35, 56);
  u8g2.print(currentDewpoint, 0);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.println(char(176)); // Celcius karakter
  u8g2.setFont(u8g2_font_helvB08_tr);   // select font
  u8g2.drawStr(50, 56, "C");
  u8g2.sendBuffer();
}

 

// Douwpunt calculaties
double dewPoint(double celsius, double humidity)
{
  // (1) Saturation Vapor Pressure = ESGG(T)
  double RATIO = 373.15 / (273.15 + celsius);
  double RHS = -7.90298 * (RATIO - 1);
  RHS += 5.02808 * log10(RATIO);
  RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / RATIO))) - 1);
  RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1);
  RHS += log10(1013.246);

  // factor -3 is to adjust units - Vapor Pressure SVP * humidity
  double VP = pow(10, RHS - 3) * humidity;

  // (2) DEWPOINT = F(Vapor Pressure)
  double T = log(VP / 0.61078); // temp var
  return (241.88 * T) / (17.558 - T);
}

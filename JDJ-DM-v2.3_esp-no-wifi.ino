/*
 Name:    JDJ-DM v2.3
 Created: 05/11/2019
 Author:  JDJ Electronics
*/

//Lib
#include <U8g2lib.h>  // U8glib library
#include <dht11.h>     // DHT library
#include <Wire.h>
//-------------------------------------------------------------------

#define RELAY 16
#define dht 13  // Analog pin to which the sensor is connected
dht11 DHT;
//-------------------------------------------------------------------
// Variable
bool schakel;
bool r_on;
float currentDewpoint;
 
//-------------------------------------------------------------------
//OlED Define
//0.96 INCH
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 5, /* data=*/ 4);   // ESP32 Thing, HW I2C with pin remapping
//U8G2_SSD1306_128X64_NONAME_2_SW_I2C OLED_1 (U8G2_R0, A5, A4);
//1.3 INCH
//U8G2_SH1106_128X64_NONAME_1_HW_I2C OLED_1(U8G2_R0);
//-------------------------------------------------------------------
void setup(void)
{
  Serial.begin(9600);
  int chk = DHT.read(dht);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, 1);// Turns Relay On
  u8g2.begin(); // init display
  
//-------------------------------------------  
//Versie firmware 
 u8g2.clearBuffer();         // clear the internal memory  
 u8g2.setFont(u8g2_font_helvB08_tr);
 u8g2.drawStr( 0, 10, "JDJ electronics");
 u8g2.drawStr( 0, 30, "Model: DM");
 u8g2.drawStr( 0, 40, "Firmware: v2.3"); 
 u8g2.drawStr( 0, 50, "051119"); //firmware date  
 u8g2.drawStr( 10, 60, "2019");
 u8g2.setCursor(0, 60);
 u8g2.setFont(u8g2_font_helvB08_tf);
 u8g2.print(char(169)); // copyright karakter
 u8g2.sendBuffer();          // transfer internal memory to the display 
 delay(3000);

 //Check up
 u8g2.clearBuffer();         // clear the internal memory  
 u8g2.setCursor(15, 36);
 u8g2.setFont(u8g2_font_open_iconic_embedded_2x_t);
 u8g2.print(char(66)); // tandwiel
 u8g2.setFont(u8g2_font_helvB08_tr);
 u8g2.drawStr( 40, 32, "DM-CHECK");
 u8g2.drawStr( 40, 45, "WAIT...");
 u8g2.sendBuffer();          // transfer internal memory to the display  
 delay(2000);

 //Check compleet
 u8g2.clearBuffer();         // clear the internal memory 
 u8g2.setCursor(45, 45);
 u8g2.setFont(u8g2_font_open_iconic_check_4x_t);
 u8g2.print(char(64)); // vinkje iccon
 u8g2.sendBuffer();
 delay(1000);
  
}
//-------------------------------------------------------------------
//END SETUP 

void loop(void) {
  delay(2000);    
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
  u8g2.print(char(67)); // aan icoon
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
  
  // Oled dauwpunt UIT
  else{  
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
  
 //-------------------------------------------  
  }  
}


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

#include <ESP8266WiFi.h> // Including library for ESP8266
#include <FirebaseArduino.h> // Including library for firebase
#include <DHT.h>  // Including library for dht


// Set these to run
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define DHTPIN D6          //pin where the dht11 is connected

DHT dht(DHTPIN, DHT11);

int bulbPin = D2;
int pumpPin = D3;

int moisturePin = A0; 
int moisturesensorValue;  
int output_value ;

void setup() {
  pinMode(bulbPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);

  Serial.begin(9600);
  delay(10);
  dht.begin();


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  


}


void loop() {
  
  dhtAndmoisturesensor();
  bulb();
  pump();
 }
 



void dhtAndmoisturesensor() {
  
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      
              if (isnan(h) || isnan(t))   //Here, we have a new function, isnan(), that returns true when there is not a number in its argument. So, we are checking whether DHT is responding or not.
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }

                  // set value
                  Firebase.set("/DHT/one/Humidity",h);
                  Firebase.set("/DHT/one/Temperature",t);
                 
                  Serial.print("Temperature: ");
                  Serial.print(t);
                  Serial.print(" degrees Celcius, Humidity: ");
                  Serial.print(h);
                  Serial.println("%. Send to firebase.");
                  Serial.println("Waiting...");



                  //moisture sensor
                  moisturesensorValue = analogRead(moisturePin);
                  output_value = map(moisturesensorValue,900, 10,0,100); 
                  Serial.println("Analog Value : ");
                  Serial.println(moisturesensorValue);
                  
                  // set value
                  Firebase.set("/MoistureSensor/area/moisturesensor1",output_value);
                  
                  delay(1000);
  
                  }


void bulb(){
    
              int bulb1 = Firebase.getInt("/Bulbs/room1/light1");
              Serial.println(bulb1);

              if(bulb1 == 1){
            
              Serial.println("bulb1 on");
              digitalWrite(bulbPin, HIGH);
              
              }else{

              Serial.println("bulb1 off");
              digitalWrite(bulbPin, LOW);

              }
    }


  void pump(){
    
          int pump1 = Firebase.getInt("/Pumps/area/pump1");
          Serial.println(pump1);

          if(pump1 == 1){
              Serial.println("pump1 on");
              digitalWrite(pumpPin, HIGH);

          }else{
              Serial.println("pump1 off");
              digitalWrite(pumpPin, LOW);

            }
    }


    

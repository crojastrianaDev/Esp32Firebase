#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>
FirebaseData firebaseData;
void init();
void wifiConection();
void updateSensor();
void updateMQ135();
void updateMQ7();
char ssid[] = "SSID DE TU RED";
char password[] = "CONTRASEÑA";
#ifndef led
#define led 13
#endif
#define analog135 12
#define analog7 4
int digval;
unsigned long tiempo_actual = 0;

#define DHTPIN 25
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  wifiConection();
  
  pinMode(led,OUTPUT);
  pinMode(analog135,INPUT);
  pinMode(analog7,INPUT);


  Firebase.begin("URL TU FIREBASE", "LLAVE FIREBASE");
  dht.begin(); // Sensor humedad-temperatura.
  tiempo_actual = millis();
  Firebase.setString(firebaseData,"/Ubicación/Pais","Colombia");
  tiempo_actual = millis();
  Firebase.setString(firebaseData,"/Ubicación/Departamento","Huila");
  tiempo_actual = millis();
  Firebase.setString(firebaseData,"/Ubicación/Ciudad","Pitalito");
}
void wifiConection()
{
  WiFi.begin(ssid, password);
  Serial.print("Conectando wifi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
   Serial.println();
  Serial.print("Connectado, su IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void loop() {
  
  
  updateDHT();
  updateMQ135();
  delayMicroseconds(10);
  updateMQ7();

}
void updateDHT(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float s =dht.computeHeatIndex(t,h,false);
  tiempo_actual = millis();
  Firebase.setFloat(firebaseData,"/temp/Humedad",h);
  tiempo_actual = millis();
  Firebase.setFloat(firebaseData,"/temp/Temperatura",t);
  tiempo_actual = millis();
  Firebase.setFloat(firebaseData,"/temp/SensaciónTermica",s);
}
void updateMQ135()
{
  int mq135 = analogRead(analog135); //Leemos la salida analógica del MQ
  double voltaje = mq135 * (5.0 / 1023.0); //Convertimos la lectura en un valor de voltaje *haberiguar el valor de 1023.0
  double rs = 1000 * ((5 - voltaje) / voltaje); //Calculamos  
  double co2 = 245.0 * pow(rs / 5463, -2.26); //valores de tabla co2 *averiguar a el valor de 5463
  double nox = 132.6 * pow(rs / 5463, -2.74); //valores de tabla nox
  double nh3 = 161.7 * pow(rs / 5463, -2.74); //valores de tabla nh3 
  double maximo1;
  double maximo2;
  double final;
  maximo1 = max(rs,co2);
  maximo2 = max(nox,nh3);
  final = max(maximo1,maximo2);
  tiempo_actual = millis();
  Firebase.setDouble(firebaseData,"/Calidad/MQ-135",final);
}
void updateMQ7(){
  int mq7 = analogRead(analog7);
  float vol = mq7 *(5.0/1023.0);
  float res = 1000*((5-vol)/vol);
  double co = 233.9*pow(res/5463, -1.40);
  tiempo_actual = millis();
  Firebase.setDouble(firebaseData,"/Calidad/MQ-7",co);
}
#include <SoftwareSerial.h> 
#include <TinyGPS.h> 
const int pingPin = 13; // Trigger Pin of Ultrasonic Sensor
const int up = 7; // Echo Pin of Ultrasonic Sensor
const int down = 8; // Echo Pin of Ultrasonic Sensor
const int water = A4;
const int panic = 12;
const int buzz = 10;
long duration,cm,cm1,duration1;
String way;
TinyGPS gps; // create gps object 
float lat,lon; 
String msg;
SoftwareSerial sim(2, 3);

void setup() 
{
   Serial.begin(9600); // Starting Serial Terminal
   sim.begin(9600);
   pinMode(up, INPUT);
   pinMode(down, INPUT);
   pinMode(water, INPUT);
   pinMode(panic, INPUT);
   pinMode(panic, INPUT);
   pinMode(buzz, OUTPUT);

}

void loop() 
{


if (get_distance(down)=="obstacle"){buzzer(150);}
if (digitalRead(water)){buzzer(1000);}
if (get_distance(up)=="obstacle"){buzzer(150);}
if (digitalRead(panic)){send_location();}
delay(200);

}

void buzzer(int t)
{

    digitalWrite(buzz,HIGH);
    delay(t);
    digitalWrite(buzz,LOW);
    delay(t);
    digitalWrite(buzz,HIGH);
    delay(t);
    digitalWrite(buzz,LOW);
    delay(t);
    digitalWrite(buzz,HIGH);
    delay(t);
    digitalWrite(buzz,LOW);
    delay(t);
    digitalWrite(buzz,HIGH);
    delay(t);
    digitalWrite(buzz,LOW);
    delay(t);
    
 
}

void updateSerial()
{
  delay(500);
  while (sim.available()) 
  {
    Serial.write(sim.read());//Forward what Serial received to Software Serial Port
  }
  while(sim.available()) 
  {
    Serial.write(sim.read());//Forward what Software Serial received to Serial Port
  }
}

void update_gps()
{
   while(Serial.available())
  { 
    if(gps.encode(Serial.read()))// encode gps data 
    {  
    gps.f_get_position(&lon,&lat); // get latitude and longitude 
    }
  } 
}

void send_location()
{
  
  update_gps();
  update_gps();
  if (lon != 0.00 || lat !=0.00) 
 {
  sim.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial(); 
  sim.println("AT+CMGS=\"+234---------\""); //replace with your number 
  updateSerial();
  msg = "PANIC_LOCATION: GPS = " + String(lon) + "," + String(lat) + " Copy to google map to see location!";
  msg = "PANIC_LOCATION: GPS = " + String(lon) + "," + String(lat) + " Copy to google map to see location!";
  sim.print(msg);
  updateSerial();
  sim.write(26);
  updateSerial();
  }
}


String get_distance(int sensor)
{
     
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   duration = pulseIn(sensor, HIGH);
   cm = microsecondsToCentimeters(duration);
   if (cm == 0 || cm >15){way="free";}
   else if (cm <= 15) {way="obstacle";}
   return way;
}


long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

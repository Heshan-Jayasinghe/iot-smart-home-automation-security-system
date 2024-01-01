#define BLYNK_TEMPLATE_ID "TMPLhkrWhkES"
#define BLYNK_TEMPLATE_NAME "IOT Home Automation  Security  System"
#define BLYNK_AUTH_TOKEN "HP7fcYUvGehuDycWT5Na2xOp3S1Op12a"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Sahan's iPhone";
char pass[] = "aaaaaaaa";


#define pir A0
#define light 5   //D1
#define fan 4     //D2
#define plug 0    //D3
#define servo 14  //D5
#define buzzer 12 //D6
#define gas 13    //D7
//#define pir1 4
//#define pir2 5


BlynkTimer timer;
bool pirButton = 0;
bool gasButton = 0;
Servo door;

void setup() {
  Serial.begin(9600);

  pinMode(gas, INPUT);
  pinMode(pir, INPUT);
  pinMode(light, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(plug, OUTPUT);
  pinMode(buzzer, OUTPUT);
  door.attach(servo);

  digitalWrite(light, HIGH);
  digitalWrite(fan, HIGH);
  digitalWrite(plug, HIGH);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  timer.setInterval(100L, gassensor);
  timer.setInterval(100L, pirsensor);
}

void loop() {
  Blynk.run();
  timer.run();
}


BLYNK_WRITE(V1) {
  bool lightStatus = param.asInt();
  if (lightStatus == 1) {
    digitalWrite(light, LOW);
    //    Serial.println("Light ON");
  } else {
    digitalWrite(light, HIGH);
    Serial.println("Light OFF");
  }
}

BLYNK_WRITE(V2) {
  bool fanStatus = param.asInt();
  if (fanStatus == 1) {
    digitalWrite(fan, LOW);
    Serial.println("Fan ON");
  } else {
    digitalWrite(fan, HIGH);
    Serial.println("Fan OFF");
  }
}

BLYNK_WRITE(V3) {
  bool switchStatus = param.asInt();
  if (switchStatus == 1) {
    digitalWrite(plug, LOW);
    Serial.println("Switch ON");
  } else {
    digitalWrite(plug, HIGH);
    Serial.println("Switch OFF");
  }
}

BLYNK_WRITE(V4) {
  bool doorStatus = param.asInt();
  if (doorStatus == 1) {
    door.write(150);
    Serial.println("Door Open");
  } else {
    door.write(10);
    Serial.println("Door Close");
  }
}

BLYNK_WRITE(V5) {
  pirButton = param.asInt();
}

void pirsensor() {
  int pirStatus = analogRead(pir);

  if (pirButton == 1) {

    if (pirStatus > 50) {
      Serial.println("PIR ON");
      Blynk.virtualWrite(V6, 1);
      Blynk.logEvent("Motion");
      digitalWrite(buzzer, HIGH);
      delay(2000);
    }
    Blynk.virtualWrite(V6, 0);
    digitalWrite(buzzer, LOW);
  }
}

BLYNK_WRITE(V8) {
  gasButton = param.asInt();
}
void gassensor() {
  int gasStatus = digitalRead(gas);
  if (gasButton == 1) {
    if (gasStatus == 0) {
      Blynk.logEvent("Gas_Leak");
      Blynk.virtualWrite(V7, 1);
      digitalWrite(buzzer, HIGH);
      Serial.print( "Gas leak detected - ");
    }
    Blynk.virtualWrite(V7, 0);
    digitalWrite(buzzer, LOW);
    Serial.print("Gas leak Not detected - ");

  }
}

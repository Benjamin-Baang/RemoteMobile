#define BLYNK_TEMPLATE_ID "TMPLJ5KVg2lv"
#define BLYNK_DEVICE_NAME "CarProject"
#define BLYNK_AUTH_TOKEN "Pgovx6JjFjUogL4lCX7mD8u3uoVLxmYw"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "BSWIFI";
char pass[] = "4F3227,s";

BlynkTimer timer;

int vcc_left = 5; 
int gnd_left = 4; 
int vcc_right = 0; 
int gnd_right = 2; 
int buzzer = 14;
int left_led = 12;
int right_led = 10;
int vel = 255; // Motor Velocity (0-255)
int left_blink = 0;
int right_blink = 0;
int yield_blink = 0;

void myTimerEvent()
{
//  Blynk.virtualWrite(V2, millis() / 1000);
}

BLYNK_CONNECTED()
{
  Blynk.syncAll();
}

BLYNK_WRITE(V0)
{
  vel = param.asInt();
}

BLYNK_WRITE(V1)
{
  int value = param.asInt();
  Serial.println(value);
  if (value) forward();
  else brake();
}

BLYNK_WRITE(V2)
{
  int value = param.asInt();
  if (value) reverse();
  else brake();
}

BLYNK_WRITE(V3)
{
  int value = param.asInt();
  if (value) left_turn();
  else brake();
}

BLYNK_WRITE(V4)
{
  int value = param.asInt();
  if (value) right_turn();
  else brake();
}

BLYNK_WRITE(V5)
{
  int value = param.asInt();
  if (value > 0) analogWrite(buzzer, 127);
  else analogWrite(buzzer, 0);
}

BLYNK_WRITE(V6)
{
  left_blink = param.asInt();
}

BLYNK_WRITE(V7)
{
  right_blink = param.asInt();
}

BLYNK_WRITE(V8)
{
  yield_blink = param.asInt();
}

void forward() {
  analogWrite(gnd_right, 0); 
  analogWrite(gnd_left, 0); 
  analogWrite(vcc_right, vel); 
  analogWrite(vcc_left, vel); 
  digitalWrite(left_led, LOW);
  digitalWrite(right_led, LOW);
}

void reverse() {
  analogWrite(vcc_right, 0); 
  analogWrite(vcc_left, 0);
  analogWrite(gnd_right, vel); 
  analogWrite(gnd_left, vel); 
  digitalWrite(left_led, HIGH);
  digitalWrite(right_led, HIGH);
}

void left_turn() {
  analogWrite(gnd_right, 0); 
  analogWrite(gnd_left, vel);
  analogWrite(vcc_left, 0);
  analogWrite(vcc_right, vel); 
  digitalWrite(left_led, HIGH);
  digitalWrite(right_led, LOW);
}

void right_turn() {
  analogWrite(gnd_right, vel); 
  analogWrite(gnd_left, 0); 
  analogWrite(vcc_right, 0); 
  analogWrite(vcc_left, vel); 
  digitalWrite(left_led, LOW);
  digitalWrite(right_led, HIGH);
}

void brake() {
  analogWrite(gnd_right, 0); 
  analogWrite(gnd_left, 0); 
  analogWrite(vcc_right, 0); 
  analogWrite(vcc_left, 0); 
  digitalWrite(left_led, HIGH);
  digitalWrite(right_led, HIGH);
}

void blink_led() {
  if (left_blink) {
    digitalWrite(left_led, HIGH);
    delay(500);
    digitalWrite(left_led, LOW);
    delay(500);
  }
  else if (right_blink) {
    digitalWrite(right_led, HIGH);
    delay(500);
    digitalWrite(right_led, LOW);
    delay(500);
  }
  else if (yield_blink) {
    digitalWrite(left_led, HIGH);
    digitalWrite(right_led, HIGH);
    delay(500);
    digitalWrite(left_led, LOW);
    digitalWrite(right_led, LOW);
    delay(500);
  }
  else {
    digitalWrite(left_led, HIGH);
    digitalWrite(right_led, HIGH);
  }
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, blink_led);
  pinMode(vcc_right, OUTPUT);
  pinMode(gnd_right, OUTPUT);
  pinMode(vcc_left, OUTPUT);
  pinMode(gnd_left, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(left_led, OUTPUT);
  pinMode(right_led, OUTPUT);
}

void loop()
{
  Blynk.run();
  timer.run();
}

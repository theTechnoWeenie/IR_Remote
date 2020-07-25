#include <ir_Lego_PF_BitStreamEncoder.h>
#include <boarddefs.h>
#include <IRremoteInt.h>
#include <IRremote.h>


long MUTE = 0x57E304FB;
long volDown = 0x57E308F7;
long volUp = 0x57E3F00F;
long diy2 = 0xFFB04F;
long diy3 = 0xFF708F;
long diy1 = 0xFF30CF;

int MOSFET = 5;
int RECV_PIN = 6;
IRrecv irrecv(RECV_PIN);
decode_results results;

bool isOff = true;
int currentState = 0;
int brightnii[8];
void setup() {
  for (int i = 0; i < 8; i++) {
    brightnii[i] = 32 * i;
  }
  brightnii[7] -= 1;
  currentState = 3;
  pinMode(MOSFET, OUTPUT);
  irrecv.enableIRIn();
  Serial.begin(9600);
}

void loop() {
  if (irrecv.decode(&results)) {
    if (results.value == MUTE || results.value == diy1) {
      toggleLed();
    } else if (results.value == volUp || results.value == diy2) {
      Serial.println("Turning up lights");
      increaseBrightness();
    } else if (results.value == volDown || results.value == diy3) {
      Serial.println("Turning up down");
      decreaseBrightness();
    } else {
      Serial.print("Got bad value: ");
      Serial.println(results.value, HEX);
    }
    irrecv.resume(); // Receive the next value
  }
}

void toggleLed() {
  if (isOff) {
    analogWrite(MOSFET, brightnii[currentState]);
    Serial.println("Turning on light.");
  } else {
    digitalWrite(MOSFET, LOW);
    Serial.println("Turning light off.");
  }
  isOff = !isOff;
}

void increaseBrightness() {
  if(isOff){
    return;
  }
  currentState += 1;
  if(currentState > 7){
    currentState = 7;
    digitalWrite(MOSFET, LOW);
    delay(100);
    digitalWrite(MOSFET, HIGH);
    delay(100);
    digitalWrite(MOSFET, LOW);
    delay(100);
  }
  analogWrite(MOSFET, brightnii[currentState]);
}

void decreaseBrightness(){
  if(isOff){
    return;
  }
  currentState -= 1;
  if(currentState < 1) {
    currentState = 1;
  }
  analogWrite(MOSFET, brightnii[currentState]);
}

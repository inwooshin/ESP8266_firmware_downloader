#include<Ticker.h>
#define DHT11PIN D2
#define DHT11PIN2 D3

Ticker tickerLed;

void ICACHE_RAM_ATTR ledISR();

int i =0, count = 0, cnt = 0, readHumid = 0, readTemp = 0, dt[82] = {0,}, before = micros();

/*
int readDHT11(){
  int dt[82] = {0,};

  //phase 1
  digitalWrite(DHT11PIN, 1);
  pinMode(DHT11PIN, OUTPUT);
  delay(1);
  digitalWrite(DHT11PIN, 0);
  delay(20);
  pinMode(DHT11PIN, INPUT_PULLUP);

  delay(1000);

  return 1;
}
*/
int rise = 0;

void ICACHE_RAM_ATTR ledISR(){
  if(i == 0){
    i++;
    digitalWrite(DHT11PIN, 1);
    pinMode(DHT11PIN, OUTPUT);
    tickerLed.attach_ms(1,ledISR);
    attachInterrupt(digitalPinToInterrupt(DHT11PIN), rising, CHANGE);
  }
  else if(i == 1){
    i++;
    digitalWrite(DHT11PIN, 0);
  }
  else if(i > 1 && i < 20) i++;
  else if(i == 20){
    i++;
    noInterrupts();
    pinMode(DHT11PIN, INPUT_PULLUP);
  }
}

void setup() {
  Serial.begin(74880);
  delay(1000);

  tickerLed.attach_ms(1,ledISR);
}

void loop() {
}

#include<Ticker.h>
#define DHT11PIN D2
#define DHT11PIN2 D3

Ticker tickerLed;

void ICACHE_RAM_ATTR ledISR();

int i =0, count = 0, cnt = 0, readHumid = 0, readTemp = 0, dt[82] = {0,}, before = micros();

void ICACHE_RAM_ATTR change(){
  //Serial.println(count);
  if(i > 20 && i < 25){
    i++;
    dt[cnt * 2] = micros();
  }
  else if(i == 25){
     dt[cnt * 2] = micros() - dt[cnt * 2];
     dt[cnt * 2 + 1] = micros();
     i = 26;
  }
  else if(i == 26){
     dt[cnt * 2 + 1] = micros() - dt[cnt * 2 + 1];  
     i = 25;
     count++;
     cnt++;
     dt[cnt * 2] = micros();
     if(count == 40){
        for(cnt = 0 ; cnt < 8 ; cnt++){
          readHumid = readHumid << 1;
          if(dt[cnt * 2 + 1] > 49){
            readHumid = readHumid + 1;  
          } 
          else readHumid = readHumid + 0;
        }
    
        for(cnt = 16 ; cnt < 24 ; cnt++){
          readTemp = readTemp << 1;
          if(dt[cnt * 2 + 1] > 49){
            readTemp = readTemp + 1;  
          } 
          else readTemp = readTemp + 0;
        }
        char buf[80];
        sprintf(buf, " Temp:%d, Humid:%d\r\n", readTemp, readHumid);
        Serial.println(buf);

        noInterrupts();
        tickerLed.attach_ms(6000,ledISR);

        
        i = 0;
        count = 0;
        cnt = 0; 
        readTemp = 0;
        readHumid = 0;
     }
  }
}

void ICACHE_RAM_ATTR ledISR(){
  if(i == 0){
    i++;
    digitalWrite(DHT11PIN, 1);
    pinMode(DHT11PIN, OUTPUT);
    tickerLed.attach_ms(1,ledISR);
    attachInterrupt(digitalPinToInterrupt(DHT11PIN), change, CHANGE);
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
  Serial.begin(57500);
  delay(1000);

  tickerLed.attach_ms(1,ledISR);
}

void loop() {
}

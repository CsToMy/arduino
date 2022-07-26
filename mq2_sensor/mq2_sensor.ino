#include <MQ2.h>

MQ2 mq2(A0);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mq2.begin();
  delay(20000);
  Serial.println("Started");
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println(mq2.readLPG());
  Serial.println(mq2.readCO());
  Serial.println(mq2.readSmoke());
  
  Serial.println("############");
  delay(4000);
}

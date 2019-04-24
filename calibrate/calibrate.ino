#include <math.h>

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

void calibrate() {
  float tem, dist, humi;

  //Star Value
  tem = analogRead();
  dist = analogRead();
  humi = analogRead();

  //Calibrate
  for (int i=0; i<60; i++) {
    tem = analogRead();
    dist = analogRead();
    humi = analogRead();
    
  }
}

float calculateSD(float data[]) {
  float sum = 0.0, mean, standardDeviation = 0.0;
  for(int i=0; i<10; ++i) {
    sum += data[i];
  }

  mean = sum/10;

  for (i=0; i<10; ++i)
    standardDeviation += pow(data[i] - mean, 2);
    
  return sqrt(standardDeviation/10);
}

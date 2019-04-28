#include <Time.h>
#include <TimeLib.h>

#include <math.h>

float temSD;
float distSD;
float humiSD;

void setup() {
  int sd = calibrate();
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
  for (int i=0; i<10; i++) {
    tem = analogRead();
    dist = analogRead();
    humi = analogRead();
  }

  temSD = calculateSD(tem);
  distSD = calculateSD(dist);
  humiSD = calculateSD(humi);

}

float calculateSD(float var) {
  float sum = 0.0, mean, standardDeviation = 0.0;
  int i;
  
  for(i=0; i<10; ++i) {
        sum += data[i];
  }

  mean = sum/10;

  for(i=0; i<10; ++i)
   standardDeviation += pow(data[i] - mean, 2);
  
  return sqrt(standardDeviation/10);
}

float calculateCToF(float celsius){
  float fahrenheit;
  fahrenheit = 1.80*celsius + 32;
  return fahrenheit;
}
void extension() {
  //for tempulature
  if (tem >= 30) {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ");
  } else if (tem >= 25) {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ");
  } else if (tem >= 20) {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ");
  } else {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ");
  }

  //for humidity
  if (humi >= 60) {
    printf("ขณะนี้ความชื้นสัมพัทธ์ในอากาศิภายในห้อง %d%%");
    printf("** ความชื้นในอากาศมีค่ามากเกินไปซึ่งป็นความชื้นที่เหมาะสมต่อการเจริญเติบโตของเชื้อรา เป็นความชื้นที่เหมาะสมต่อการเจริญเติบโตของเชื้อรา ซึ่งเชื้อราจะเป็นอันตรายต่อบุคคลที่ป่วยเป็นโรคหอบหืดซึ่งเชื้อราจะเป็นอันตรายต่อบุคคลที่ป่วยเป็นโรคหอบหืดได้ **");
  } else if (humi >= 30) {
    printf("ขณะนี้ความชื้นสัมพัทธ์ในอากาศิภายในห้อง %d%%");
  } else (humi < 30) {
    printf("ขณะนี้ความชื้นสัมพัทธ์ในอากาศิภายในห้อง %d%%");
    printf("** ความชื้นในอากาศมีค่าต่ำกว่าระดับที่เหมาะสม ควรใช้ครีมบำรุงผิวเพื่อให้ผิวชุ่มชื้นอยู่ตลอดเวลา **");
  }

  //for check celsius to fahrenheit
  printf("%f", calculateCToF(temp))
}

#include <math.h>

struct Data_for_cal {
  int count;
  float sum;
  float standardDeviation;
  float data_arr[];
} data;

void setup() {
  data.count = 0;
  data.mean = 0.0;
  data.standardDeviation = 0.0;
  int sd = calibrate;
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

float calculateSDFirstTime(float var) {
  data.data_arr[data.count] = var;
  data.count += 1;
  data.sum += var;

  float mean = data.sum/data.count;

  for (int i=0; i<data.count; ++i)
    standardDeviation += pow(data.data_arr[i] - mean, 2);
    
  return sqrt(standardDeviation/10);
}

char check(float var) {
  const percen = 2;
  float 
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
  } else if (tem >= 25) {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ");
  } else if (tem >= 20) {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ");
  } else {
    printf("ขณะนี้อุณหภูมิภายในห้อง %d องศาเซลเซียส %d ฟาเรนไฮ");
  }
}

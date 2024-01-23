#include <math.h>



  // put your setup code here, to run once:
  double y_1 = 1.00;
  double y_2 = 1.00;
  double y_3 = 1.00;
  double q = 1.00;
  double V = 1.00;
  double V_hat = 1.00;
  double fishy = 1.00;
  double beta = 1.00;
  double alpha_fishy = 1.00;
  double alpha_beta = 1.00;
  char printBuffer[200];
  unsigned long startTime;  // Variable to store the start time

  void setup(){
}

void loop() {
  //sprintf(printBuffer,"Start");
  //Serial.println(printBuffer);
  startTime = millis();  // Record the start time
  fishy = alpha_fishy * (y_2/y_1);
  beta = alpha_beta * (y_3/1);
  V = sqrt((2*y_1)/q);
  V_hat = ((V*cos(beta))/cos(fishy)) + (V*sin(beta)) + (V*cos(beta)*tan(fishy));
  unsigned long endTime = millis();
  Serial.print(" Time: ");
  Serial.println(endTime - startTime);
  //sprintf(printBuffer,"end %f",V_hat);
  //Serial.println(printBuffer);
  y_1 = y_1 + 1.00;
  y_2 = y_2 + 1.00;
  y_3 = y_3 + 1.00;
  delay(1000);

}


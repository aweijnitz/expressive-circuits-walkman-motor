/**
   Walkman tape speed controller.
   Reads control voltage (CV) on CV_PIN and maps the value to PWM duty cycle.
*/
#define CV_MAX 1023
#define CV_MIN 3
#define PWM_DUTY_MIN 200
#define PWM_DUTY_MAX 255
#define LED_DUTY_MIN 0
#define LED_DUTY_MAX 32
#define MINIMUM_DUTY_CYCLE 200
#define CV_SCALE_FACTOR 1.0
#define LED_BRIGHTNESS_FACTOR 1.0

#define CV_PIN A5
#define PWM_MOTOR_PIN 5
#define PWM_LED_PIN 6

int cvIn = 0;
int pwmDuty;
int ledDuty;


void setup() {
  Serial.begin(115200);
  analogWrite(PWM_MOTOR_PIN, 0); // Start state: stopped
  analogWrite(PWM_LED_PIN, 0);
}


// Map input to output
//
int mapValue(int val) {
  double dVal = (double) val * CV_SCALE_FACTOR;
  if (dVal <= MINIMUM_DUTY_CYCLE) // The DC motor needs a minimum duty cycle to not stall. Empirical value
    return 0;
  else
    return (int) min(dVal, 255);
}


int updateMotorSpeed(int motorPWM, int ledPWM) {
  int mapped = mapValue(motorPWM);
  //Serial.println(mapped);
  analogWrite(PWM_MOTOR_PIN, mapped);
  analogWrite(PWM_LED_PIN, (int)(ledPWM * LED_BRIGHTNESS_FACTOR));
  return mapped;
}


void loop() {
  cvIn = analogRead(CV_PIN);
  pwmDuty = (int) map(cvIn, CV_MIN, CV_MAX, PWM_DUTY_MIN, PWM_DUTY_MAX);
  ledDuty = (int) map(cvIn, CV_MIN, CV_MAX, LED_DUTY_MIN, LED_DUTY_MAX);
  updateMotorSpeed(pwmDuty, ledDuty);
}

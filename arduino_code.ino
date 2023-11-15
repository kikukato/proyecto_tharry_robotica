// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__SOFTSERIAL
#include <SoftwareSerial.h>

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL_RX 2
#define REMOTEXY_SERIAL_TX 3
#define REMOTEXY_SERIAL_SPEED 9600

// Define pins for ultrasonic sensor
#define TRIG_PIN 4
#define ECHO_PIN 5

// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 57 bytes
  { 255,3,0,0,0,50,0,16,21,0,2,1,16,29,22,11,12,28,31,31,
  79,78,0,79,70,70,0,5,16,56,21,30,30,12,20,31,130,1,13,6,
  75,8,20,129,0,37,7,29,7,12,84,72,65,82,82,89,0 };
  
// this structure defines all the variables of your control interface 
struct {

    // input variables
  uint8_t switch_1; // =1 if switch ON and =0 if OFF 
  int8_t joystick_1_x; // from -100 to 100  
  int8_t joystick_1_y; // from -100 to 100 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

#define PIN_SWITCH_1 13

//define right motor control pins
#define right_motor_AIN1 8
#define right_motor_AIN2 7
#define right_motor_speed 6 //PWM A

//define left motor control pins
#define left_motor_BIN1 9
#define left_motor_BIN2 10
#define left_motor_speed 11 //PWM B

//define two arrays with a list of pins for each motor
uint8_t RightMotor[3] = {right_motor_AIN1, right_motor_AIN2, right_motor_speed};
uint8_t LeftMotor[3] = {left_motor_BIN1, left_motor_BIN2, left_motor_speed};

// Define variables for ultrasonic sensor
long duration;
int distance;


//speed control of motors
int RightMotorSpeed;
int LeftMotorSpeed;

void Wheel (uint8_t * motor, int v) // v = motor speed, motor = pointer to an array of pins 
{
  // Check distance using ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;
  //Serial.println(distance);

  if (v > 50 && distance > 10) v=50;
  if (v < -50) v=-50;
  if (v > 0 && distance > 10){
    digitalWrite (motor [0], HIGH);
    digitalWrite (motor [1], LOW);
    analogWrite (motor [2], v * 2.55);
  }
  else if ( v<0 ){
    digitalWrite (motor [0], LOW);
    digitalWrite (motor [1], HIGH);
    analogWrite (motor [2], (-v) * 2.55);
  }
  else{
    digitalWrite (motor [0], LOW);
    digitalWrite (motor [1], LOW);
    analogWrite (motor [2], 0);
  }

  // Logica para enviar comandos por el puerto serie hacia la Raspberry Pi
  if (v > 0 && distance > 10) {
    Serial.println("w"); // Ambos motores hacia adelante
  }
  else if (v < 0 ) {
    Serial.println("x"); // Ambos motores hacia atrás
  }
  /*
  else if (v > 0) {
    Serial.println("d"); // Solo motor derecho hacia adelante
  }
  else if (v < 0) {
    Serial.println("a"); // Solo motor izquierdo hacia adelante
  }
  */
  else {
    Serial.println("s"); // Ambos motores detenidos
  }

  delay(0.1);
}

void setup() 
{
  RemoteXY_Init ();
  Serial.begin(9600);
  
  pinMode (PIN_SWITCH_1, OUTPUT);
  
  //initialization pins
  pinMode (right_motor_AIN1, OUTPUT);
  pinMode (right_motor_AIN2, OUTPUT);
  pinMode (left_motor_BIN1, OUTPUT);
  pinMode (left_motor_BIN2, OUTPUT);
  pinMode (right_motor_speed, OUTPUT);
  pinMode (left_motor_speed, OUTPUT);

  // Setup pins for ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() 
{ 
  RemoteXY_Handler ();

  digitalWrite(PIN_SWITCH_1, (RemoteXY.switch_1 == 0) ? LOW : HIGH);
    // Manage the right motor
    RightMotorSpeed = (RemoteXY.joystick_1_y - RemoteXY.joystick_1_x)/2;
    Wheel(RightMotor,RightMotorSpeed );
    // Manage the left motor
    LeftMotorSpeed = (RemoteXY.joystick_1_y + RemoteXY.joystick_1_x)/2;
    Wheel(LeftMotor, LeftMotorSpeed );
  
}

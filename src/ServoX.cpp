#include "ServoX.h"
#include <Arduino.h>
using namespace std;

extern void NL();
extern void DEBUG(String label, int64_t value, bool newline);
extern void DEBUG(int64_t value, bool newline);
extern void DEBUG(String label, bool newline);
extern void tempo_empty(unsigned long reftime);

#define MAX_SERVO 46
#define MIN_SERVO 0
#define MOVE_STEP 12


// if val = 255 the read current postion
void ServoX::save_pos(uint8_t val)
{
  // memorize to flash the actual position of servo 
  uint8_t _input  = (byte) _servo.read();
  if (val<255)
  {
    if (val<10) {_input=30;}
    if (val>50) {_input=30;}
  }
  else
  {
    _input=30;
  }
  servo_mem.begin(name.c_str(),false);

  servo_mem.putUInt("position",_input);  
  servo_mem.end();
}

void ServoX::load_pos()
{
  servo_mem.begin(name.c_str(),false);
  uint16_t test=0;
  test=(uint8_t)servo_mem.getUInt("max");  
  Serial.print("Max: ");Serial.println(test,DEC);
  init_value = (uint8_t)servo_mem.getUInt("position",20);  
  
  servo_mem.end();
  
     
  if (init_value<=min_value) {init_value=30;}
  if (init_value>=max_value) {init_value=30;}
  

  startMove(init_value);

}


int ServoX::read_pos()
{
unsigned long reftime=1000;

unsigned long ref = millis();
  while ((millis() - ref) < reftime)
  {
    if (this->moving==0) {
      break;}
  }
  int anglex=_servo.read();
  return anglex;
}

// read current stepper position

void ServoX::init(int _pin, String _name,int _min, int _max)    
{
 
 name = _name;
 PIN=_pin;
 //pinMode(PIN, OUTPUT);
 //this->_servo.attach(_pin);
 this->_servo.attach(_pin, 544, 2550);
 delayMs=5;
 // initial angle = 0 
 _servo.write(0);
 moving=0;
 min_value=_min;
 max_value=_max;
 
 timer_const=MOVE_STEP;

 
 return;   
}



 void ServoX::startMove(int targetAngle) {
        // Creează structura pentru a transmite parametrii către task
        int max = max_value;
        int min = min_value;
        if (targetAngle<min) {targetAngle=min;}
        if (targetAngle>max) {targetAngle=max;}

        ServoTaskParams* params = new ServoTaskParams{this, targetAngle};
        xTaskCreatePinnedToCore(taskWrapper, "Servo Move Task", 1000, params, 1, NULL, 1);
    }


void ServoX::task(int targetAngle) {
      int currentAngle = _servo.read(); // Citește unghiul curent

      // Afișează unghiul curent la consola serială
      
      //Serial.println();
      //Serial.print("Unghi curent: ");
      //Serial.println(currentAngle);

      // Mișcă servomotorul la unghiul țintă
      moveServo(currentAngle, targetAngle);
      // Task-ul se încheie după ce mișcarea este completă
      vTaskDelete(NULL);
  }


// Wrapper static pentru a putea folosi metoda ca task FreeRTOS
void ServoX::taskWrapper(void* parameter) {
    // Castăm parametrul la structura noastră
    ServoTaskParams* params = static_cast<ServoTaskParams*>(parameter);
    ServoX* _this = params->servoMotor;
    int targetAngle = params->targetAngle;

    // Apelarea metodei task cu unghiul specificat
    _this->task(targetAngle);

    // Eliberarea memoriei pentru structura de parametri
    delete params;
}


// Funcție de ajutor pentru a mișca servomotorul între două unghiuri
void ServoX::moveServo(int startAngle, int endAngle) {
    //used for identify when servo is still moving
    this->moving=1;
    if (startAngle < endAngle) {
        for (int angle = startAngle; angle <= endAngle; angle++) {
            _servo.write(angle);
            vTaskDelay(delayMs / portTICK_PERIOD_MS);
        }
    } else if (startAngle > endAngle) {
        for (int angle = startAngle; angle >= endAngle; angle--) {
            _servo.write(angle);
            vTaskDelay(delayMs / portTICK_PERIOD_MS);
        }
    }
    this->moving=0;
}

   
    





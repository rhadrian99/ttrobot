
#ifndef ServoX_h
#define ServoX_h

#include <stdlib.h>

#include <ESP32Servo.h>
#include <Preferences.h>

#include <Arduino.h>
#include <string.h>

#include <Brush.h>
#include <StepperX.h>

extern Brush motor_up;
extern Brush motor_down;
extern StepperX feeder;



class ServoX
{


public:

struct ServoTaskParams {
    ServoX* servoMotor; // Pointer la instanța clasei ServoMotor
    int targetAngle;        // Unghiul țintă
};

Preferences servo_mem;
int PIN;
Servo _servo;
uint8_t timer_const;

// _pin = alocated pin, _name = identifier, _min = servo minim position, _max = servo max position
void init(int _pin, String _name,int _min, int _max);

uint8_t EEPROM_address;
uint8_t EEPROM_address_min; // default 2
uint8_t EEPROM_address_max; // default 3
uint8_t init_value;
uint8_t min_value;
uint8_t max_value;
String name;
int moving;

int read_pos();

void load_pos();
void save_pos(uint8_t val);

//void save_min(uint8_t val);
//void load_min();

//void save_max(uint8_t val);
//void load_max();


void startMove(int targetAngle);


private:
    
    int delayMs;

    void task(int targetAngle); 
       
    static void taskWrapper(void* parameter);
    void moveServo(int startAngle, int endAngle);
    

};
#endif 

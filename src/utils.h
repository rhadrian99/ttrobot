#ifndef utils_h
#define utils_h
# include "Arduino.h"




void tempo_empty(unsigned long reftime) {
 unsigned long ref = millis();
  while ((millis() - ref) < reftime)
  {
    //delay(1);
    yield();
  }
}

void NL()
{
    Serial.println();
}

void PRINT()
{

}

void DEBUG(String label, int64_t value, bool newline)
{
    if (newline) {NL();}
    Serial.print(label); Serial.print(":"); Serial.print(" ["); Serial.print(value,DEC); Serial.print("]");
    if (newline) {NL();}
}

void DEBUG(int64_t value, bool newline)
{
    if (newline) {NL();}
    Serial.print(" ["); Serial.print(value,DEC); Serial.print("]");
    if (newline) {NL();}
}

void DEBUG(String label, bool newline)
{
    if (newline) {NL();}
    Serial.print(label); 
    if (newline) {NL();}
}



#endif

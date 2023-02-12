/*
 * This code is to charge battery
 * using the nicla and the battery 
 * included for the challenge
 * is needed the version 3.4.5
 */

#include "Nicla_System.h"

void setup(){
  nicla::begin();
  nicla::enableCharge(100);
}
void loop(){
// Your code here.
}

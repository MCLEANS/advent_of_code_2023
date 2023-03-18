#include <iostream>
#include "Motor.h"

int main(int argc, char* argv[]){

    Motor::get_instance()->init();
    
    while(1){

        Motor::get_instance()->run();
    }

}
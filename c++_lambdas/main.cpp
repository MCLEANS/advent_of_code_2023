#include <iostream>

/**
 * Typically works the same as function poiters.
 * Suitable for functions that are only called once hence can be defined inline
 * [Syntax]
 * [capture list](parameter_list) specifier { * Code goes here *};
 * {Capture list} > '=' : passes all variables in scope by value
 *                   '&' : passes all variables in scope by reference
 *                   'variable' : passes named 'variable' by value (to modify variabe passed by value 'mutable' specifier is required)
 *                   '&variable' : passes named variable by reference
*/

int main(int argc, char* argv[]){

    /* Capture by value*/
    int year_of_birth = 1990;
    int age = 34;

    auto current_year= [year_of_birth,age](){
        std::cout<<"The current year is :"<< year_of_birth + age <<std::endl;
    };
    current_year();

    /* capture by value*/
    auto validate_birth_year = [year_of_birth,&age](int year) {
        if((year_of_birth + age) > year){
            age = year - year_of_birth;
            std::cout<<"Invalid age provided, corrected to : "<< age << std::endl;
        }
    };

    validate_birth_year(2023);
    std::cout<<"new age : "<< age <<std::endl;

    return 0;
}
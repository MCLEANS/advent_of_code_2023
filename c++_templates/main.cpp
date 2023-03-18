#include <iostream>

/* Exercise 1 : Create a template class that can accept different data types and print out their value */
/* Exercise 2 : C++ Program to find Largest among two numbers using function template */

template<typename T>
class DataType{
private:
    T _data;
public:
    T get_data()const{return _data;}
    void set_data(T data){this->_data = data;}

};

template<typename N>
N find_largest(N first, N second){
    return (first > second)? first : second;
}

int main(int argc, char* argv[]){

    std::cout<<"\n**Exercise 1**\n"<<std::endl;
    
    DataType<float> floating_point;
    floating_point.set_data(23.54);
    std::cout<<"_data value is : "<<floating_point.get_data()<<std::endl;

    DataType<std::string> string_type;
    string_type.set_data("McLeans");
    std::cout<<"_data value is : "<<string_type.get_data()<<std::endl;

    std::cout<<"\n**Exercise 2**\n"<<std::endl;
    std::cout<<"Sample test 1"<<find_largest<float>(34.5,12.6)<<std::endl;
    std::cout<<"Sample test 2"<<find_largest<int>(56,976)<<std::endl;

    return 0;
}
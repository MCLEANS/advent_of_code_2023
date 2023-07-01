#include <iostream>
#include <map>

typedef struct student_details{

    std::string name;
    uint8_t age;
    std::string course;

}student_details_t;

int main (int argc, char** argv){

    std::map<std::string, student_details_t> student_list;

    student_details_t student1 = {
        .name = "John",
        .age = 23,
        .course = "Marketing"
    };

    student_list.insert(std::make_pair("ECI_234", student1));

    student_details_t student2 = {
        .name = "Jane",
        .age = 21,
        .course = "Engineering"
    };

    student_list.insert(std::make_pair("MFE_542", student2));


    std::cout << "Student 1 name is "<< student_list.at("ECI_234").name<<std::endl;
    std::cout << "Student 2 name is "<< student_list.at("MFE_542").name<<std::endl;
    


    return 0;
}
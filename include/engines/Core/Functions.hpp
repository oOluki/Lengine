#pragma once
#include "../../../dependencies/STD_dependencies.hpp"
#include "Objects.hpp"

namespace Le{


void print(){
    std::cout << std::endl;
}

template<typename T> void print(T arg){
    std::cout << arg << std::endl;
}

template<typename T, typename... Args> void print(T first, Args... args){
    std::cout << first;
    print(args...);
}

double power(double base, double exponent)
{
if(exponent == 0)  return base/base;
double value = base; bool dummy = false; if(exponent < 0) {exponent *= -1; dummy = true;}
for(int i = 1; i < exponent; i++) {value *= base;}
if(dummy) return 1/value;
return value;
}

int number_of_digits(int number)
{
if(number == 0) return 1;
if(number < 0) number *= -1;
int dummy = 0;
while (number > 0.9)
{
number /= 10;
dummy++;
}
return dummy;
}

int get_digit(int input, int index)
{
input = input - int(input/power(10, index+1))*power(10, index + 1);
input = int(input/power(10, index));
return input;
}

int get_int_from_char(const char& character){
    return int(character - '0');
}

//returns the integer from a string, if the string contains values unnumeric it fails and returns nonsense.
int get_int_from_string(const char* string){
    int dummy = 0;
    unsigned int counter = 0;
    while(string[counter] != '\0'){
        dummy += get_int_from_char(string[counter]) * pow(10, counter);
        counter += 1;
    } int output = 0;
    for(int i = counter; i > 0; i--){
        int j = get_digit(dummy, i - 1);
        output += j * pow(10, counter - i);
    }
    return output;
}

void get_line_parameters(Line line, int*&& out_a, int*&& out_b, int*&& out_c)
{
int a, b, c;
a = line.y2 - line.y1; b = line.x1 - line.x2; c = -b*line.y1 - a*line.x1;
*out_a = a; *out_b = b; *out_c = c;
}

// checks if two lines iterssect, even if outside the range given by the points that define them.
bool collideline(Line line1, Line line2){
    int v1[3], v2[3];
    get_line_parameters(line1, v1, v1 + 1, v1 + 2);
    get_line_parameters(line2, v2, v2 + 1, v2 + 2);
    int det = v1[0] * v2[1] - v1[1] * v2[0];
    return det? true : !(v1[0] * line2.x1 + v1[1] * line2.y1 + v1[2]);
}

// checks if two lines interssect in the range defined by the points that defined the lines,
// if so, and only if so, the two last given values are set to the x and y coordinates of the iterssection.
bool collideline(Line line1, Line line2, int*&& outx, int*&& outy)
{
    int v1[3], v2[3];
    get_line_parameters(line1, v1, v1 + 1, v1 + 2);
    get_line_parameters(line2, v2, v2 + 1, v2 + 2);
    int det = v1[0] * v2[1] - v1[1] * v2[0];
    if(!det) return false;
    int inverse[4] = {
        v2[1], -v1[1],
        -v2[0], v1[0]
    };
    int x = (-v2[1] * v1[2] + v1[1] * v2[2]) / det;

    if(abs(x - line1.x1) <= abs(line1.x2 - line1.x1)
    && abs(x - line2.x1) <= abs(line2.x2 - line2.x1))
    {
        *outy = (v2[0] * v1[2] - v1[0] * v2[2]) / det;
        *outx = x;
        return true;
    }
    return false;
}

// checks if two lines interssect, even if in a point outside the range given by the points that defined it.
// if so, and only if so, the two last given values are set to the x and y coordinates of the iterssection.
bool collidelineEXT(Line line1, Line line2, int*&& outx, int*&& outy)
{
    int v1[3], v2[3];
    get_line_parameters(line1, v1, v1 + 1, v1 + 2);
    get_line_parameters(line2, v2, v2 + 1, v2 + 2);
    int det = v1[0] * v2[1] - v1[1] * v2[0];
    if(!det){
        if(!(v1[0] * line2.x1 + v1[1] * line2.y1 + v1[2])){
            *outx = line1.x1;
            *outy = line1.y1;
            return true;
        }
        return false;
    }
    *outx = (-v2[1] * v1[2] + v1[1] * v2[2]) / det;
    *outy = *outy = (v2[0] * v1[2] - v1[0] * v2[2]) / det;

    return true;
}




}




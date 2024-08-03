#pragma once
#include "../engines/Core/Report.hpp"

#ifndef LE_UTILITIES
#define LE_UTILITIES
#endif
#ifdef NDEBUG
#define LE_DEBUG_ARRAY_BOUNDARY_CHECK(adress, index, lowerbound, upperbound)\
if(index < lowerbound || index >= upperbound){\
LE_DEBUG_REPORT_ERROR("Index " <<\
index << " Out Of Bounds (" << lowerbound <<", " << upperbound << "), & " << adress);\
exit(EXIT_FAILURE);\
}
#else
#define LE_DEBUG_ARRAY_BOUNDARY_CHECK(adress, index, lowerbound, upperbound)
#endif

namespace Le{

template<typename T>
struct Tuple
{
    T x, y;
};

template<typename T>
struct Triple
{
    T x, y, z;
};


template<typename T, size_t __size>
class Array{
public:
Array(){}

Array(const std::initializer_list<T>& list){
    for(int i = __size < list.size()? __size - 1 : list.size() - 1; i > -1; i--){
        _Array[i] = *(list.begin() + i);
    }
}

Array(void* start, void* end, size_t startingPoint = 0){
    allocate((T*)start, (T*)end, startingPoint);
}

Array(const T array[__size]){
    for(int i = 0; i < __size; i++){
        _Array[i] = array[i];
    }
}

void allocate(T* start, T* end, size_t startingPoint = 0){
    for(; startingPoint < __size && start < end; startingPoint+=1){
        _Array[startingPoint] = *(start++);
    }
}

T& operator[](size_t index){
    LE_DEBUG_ARRAY_BOUNDARY_CHECK(this, index, 0, __size);
    return _Array[index];
}

T get_copy(size_t index){
    LE_DEBUG_ARRAY_BOUNDARY_CHECK(this, index, 0, __size);
    return _Array[index];
}

size_t size() const{
    return __size;
}


private:
T _Array[__size];
};



template<typename T, size_t a_size, size_t num_of_arrays>
class MultiArray{
public:

MultiArray(){}

MultiArray(const std::initializer_list<T>& list){
    for(int i = list.size() < a_size * num_of_arrays? list.size() - 1 : a_size * num_of_arrays - 1;
    i > -1; i-=1){
        _Array[i] = *(list.begin() + i);
    }
}

MultiArray(const std::initializer_list<const std::initializer_list<T>&>& list){
    size_t d = num_of_arrays < list.size()? num_of_arrays - 1: list.size() - 1;
    bool ascheck = a_size < list.begin()->size();
    for(; d > -1; d-=1){
        for(size_t j = ascheck? a_size - 1 : list.begin()->size() - 1; j > -1; j -= 1){
            _Array[j + d * a_size] = *((list.begin() + d)->begin() + j);
        }
    }
}

MultiArray(void* start, void* end, size_t startingPoint = 0){
    allocate((T*)start, (T*)end, startingPoint);
}

void allocate(T* start, T* end, size_t startingPoint = 0){
    for(; startingPoint < a_size * num_of_arrays && start < end; startingPoint += 1){
        _Array[startingPoint] = *(start++);
    }
}

void allocate(T* start, T* end, size_t startingLine, size_t startingColumn){
    for(size_t startingPoint = startingLine * a_size + startingColumn;
    startingPoint < a_size * num_of_arrays && start < end; startingPoint += 1){
        _Array[startingPoint] = *(start++);
    }
}

constexpr size_t get_number_of_arrays() const{
    return num_of_arrays;
}

constexpr size_t get_size_of_arrays() const{
    return a_size;
}

constexpr size_t size() const{
    return a_size * num_of_arrays;
}

constexpr T& operator[](size_t index) {
    LE_DEBUG_ARRAY_BOUNDARY_CHECK(this, index, 0, num_of_arrays * a_size);
    return _Array[index];
}

T& get(size_t y, size_t x) {
    LE_DEBUG_ARRAY_BOUNDARY_CHECK(this, x + y * a_size, 0, num_of_arrays * a_size);
    return _Array[x + a_size * y];
}

T get_copy(size_t index) const{
    LE_DEBUG_ARRAY_BOUNDARY_CHECK(this, index, 0, num_of_arrays * a_size);
    return _Array[index];
}

T get_copy(size_t y, size_t x) const{
    LE_DEBUG_ARRAY_BOUNDARY_CHECK(this, x + y * a_size, 0, num_of_arrays * a_size);
    return _Array[x + a_size * y];
}


private:
T _Array[a_size * num_of_arrays];


};




}





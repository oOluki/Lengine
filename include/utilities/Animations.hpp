#pragma once
#include "../../dependencies/SDL_dependencies.hpp"

namespace Le{

template<typename T, size_t size>
class Animation{
public:
Animation(){}
Animation(T* _animation, unsigned int* _repetitions){
    for(int i = 0; i < size; i++){
        animation[i] = _animation[i];
        repetitions[i] = _repetitions[i];
    }
}

~Animation(){}

bool InBounds(size_t index) const{
    return (index < size);
}

T& operator[](size_t index) const{
    return animation[index];
}

T& IterLinear(unsigned int* current, float* counter, unsigned int* loops, float step = 1){
    if(*counter < 0){
        *current += 1;
        if(*current == size){
            *current = 0;
            *counter = repetitions[0];
            *loops += 1;
            return animation[0];
        } *counter = repetitions[*current];
        return animation[*current];
    }
    *counter -= step;
    return animation[*current];
}

T& IterPingPong(unsigned int* current, float* counter, unsigned int* loops, float step = 1){
    if(*counter < 0){
        if(*loops & 1){
            *current -= 1;
            if(*current < 0){
                *current = 1;
                *counter = repetitions[1];
                *loops += 1;
                return animation[1];
            } *counter = repetitions[*current];
            return animation[*current];
        }
        *current += 1;
        if(*current == size){
            *current = size - 2;
            *counter = repetitions[size - 2];
            *loops += 1;
            return animation[size - 2];
        } *counter = repetitions[*current];
        return animation[*current];

    }
    *counter -= step;
    return animation[*current];
}

private:
T animation[size];
unsigned int repetitions[size];

};

template<typename T>
class ScalableAnimation{
public:
ScalableAnimation(){}
ScalableAnimation(const std::vector<T>& avec, const std::vector<T>& rvec)
: animation(avec), repetitions(rvec){}
ScalableAnimation(const std::initializer_list<T>& alist, const std::initializer_list<T>& rlist)
: animation(alist), repetitions(rlist){}

T& operator[](size_t index) const{
    return animation[index];
}

unsigned int& GetRepetitions(size_t index) {
    return repetitions[index];
}

size_t Size() const{
    return animation.size();
}

T& IterLinear(unsigned int* current, float* counter, unsigned int* loops, float step = 1){
    if(*counter < 0){
        *current += 1;
        if(*current == animation.size()){
            *current = 0;
            *counter = repetitions[0];
            *loops += 1;
            return animation[0];
        } *counter = repetitions[*current];
        return animation[*current];
    }
    *counter -= step;
    return animation[*current];
}

T& IterPingPong(unsigned int* current, float* counter, unsigned int* loops, float step = 1){
    if(*counter < 0){
        if(*loops & 1){
            *current -= 1;
            if(*current < 0){
                *current = 1;
                *counter = repetitions[1];
                *loops += 1;
                return animation[1];
            } *counter = repetitions[*current];
            return animation[*current];
        }
        *current += 1;
        if(*current == animation.size()){
            *current = animation.size() - 2;
            *loops += 1;
        } *counter = repetitions[*current];
        return animation[*current];

    }
    *counter -= step;
    return animation[*current];
}

void Push(const T& item, unsigned int repetition){
    animation.push_back(item);
    repetitions.push_back(repetition);
}

void Delete(size_t index){
    animation.erase(animation.begin() + index);
    repetitions.erase(repetitions.begin() + index);
}

private:
std::vector<T> animation;
std::vector<unsigned int> repetitions;

};


bool IterLinearAnimation(unsigned int* current, unsigned int* repetitions, float* counter, unsigned int* loops, unsigned int size, float step = 1){

    if(*counter < 0){
        *current += 1;
        if(*current == size){
            *current = 0;
            *counter = repetitions[0];
            *loops += 1;
            return true;
        } *counter = repetitions[*current];
        return false;

    }
    *counter -= step;
    return false;
}

bool IterPingPongAnimation(unsigned int* current, unsigned int* repetitions, float* counter, unsigned int* loops, unsigned int size, float step = 1){

    if(*counter < 0){
        if(*loops & 1){
            *current -= 1;
            if(*current < 0){
                *current = 1;
                *counter = repetitions[1];
                *loops += 1;
                return true;
            } *counter = repetitions[*current];
            return false;
        }
        *current += 1;
        if(*current == size){
            *current = size - 2;
            *counter = repetitions[size - 2];
            *loops += 1;
            return true;
        } *counter = repetitions[*current];
        return false;

    }
    *counter -= step;
    return false;
}

bool IterVecAnimation(float* current, float* velocity, float* goal, bool* state, unsigned int size, float dt = 1){
    bool done = true;
    for(int i = 0; i < size; i++){
        if(state[i]) continue;
        done = false;
        float next = current[i] + velocity[i] * dt;
        if(goal[i] == current[i]){
            state[i] = true;
            continue;
        }
        if(abs(goal[i] - current[i]) < abs(next - current[i])){
            state[i] = true;
            current[i] = goal[i];
            continue;
        } current[i] = next;
    } return done;
}


}



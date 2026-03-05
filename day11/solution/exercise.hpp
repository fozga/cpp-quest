#pragma once

#include <iostream>
#include <string>

template <typename Derived>
class AnimalBase {
public:
    void speak() const {
        static_cast<const Derived*>(this)->speak_impl();
    }

    std::string sound() const {
        return static_cast<const Derived*>(this)->sound_impl();
    }
};

class Dog : public AnimalBase<Dog> {
public:
    void speak_impl() const { std::cout << "Woof!" << std::endl; }
    std::string sound_impl() const { return "Woof!"; }
};

class Cat : public AnimalBase<Cat> {
public:
    void speak_impl() const { std::cout << "Meow!" << std::endl; }
    std::string sound_impl() const { return "Meow!"; }
};

template <typename Animal>
void make_it_speak(const Animal& a) {
    a.speak();
}

template <typename Animal>
std::string get_sound(const Animal& a) {
    return a.sound();
}

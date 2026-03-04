#include "exercise.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::cout << "[Day 11] Static Polymorphism — running tests\n";

    Dog dog;
    Cat cat;

    // Exercise CRTP dispatch through the free function template
    std::cout << "  make_it_speak(dog): ";
    make_it_speak(dog);
    std::cout << "  make_it_speak(cat): ";
    make_it_speak(cat);

    // Verify sound strings
    assert(get_sound(dog) == "Woof!");
    assert(get_sound(cat) == "Meow!");
    std::cout << "  [PASS] get_sound returns correct strings\n";

    // Homogeneous container of Dogs
    std::vector<Dog> dogs(3);
    std::cout << "  vector<Dog> speak: ";
    for (const auto& d : dogs) {
        d.speak();
    }

    // Homogeneous container of Cats
    std::vector<Cat> cats(2);
    std::cout << "  vector<Cat> speak: ";
    for (const auto& c : cats) {
        c.speak();
    }

    // No virtual functions → no vtable pointer → sizeof should be 1
    // (minimum size for a class with no data members and no virtuals).
    static_assert(sizeof(Dog) == 1, "Dog should have no vtable pointer (size 1)");
    static_assert(sizeof(Cat) == 1, "Cat should have no vtable pointer (size 1)");
    std::cout << "  [PASS] sizeof(Dog) == 1, sizeof(Cat) == 1 (no vtable)\n";

    std::cout << "[Day 11] All tests passed.\n";
    return 0;
}

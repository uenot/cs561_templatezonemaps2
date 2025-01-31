#include "zonemaps.h"

template<typename T>
zonemap<T>::zonemap(std::vector<T> _elements, uint _num_elements_per_zone)
{
    // constructor 
    // Your code starts here ...
    elements = _elements;
    num_elements_per_zone = _num_elements_per_zone;
    build(); // sets zones and num_zones
}

template<typename T>
void zonemap<T>::build() {
    num_zones = 0;

    for (size_t i = 0; i < elements.size(); i++) {
        zone<T> z;
        for (size_t j = 0; j < num_elements_per_zone; j++) {
            T x = elements[i];
            z.elements.push_back(x);
            if (x > z.max) {
                z.max = x;
            } else if (x < z.min) {
                z.min = x;
            }
            z.size++;
        }
        zones.push_back(z);
        num_zones++;
    }
}

template<typename T>
size_t zonemap<T>::query(T key)
{
    size_t result = 0;
    for (size_t i = 0; i < num_zones; i++) {
        zone<T> z = zones[i];
        if (key > z.min && key < z.max) {
            for (size_t j = 0; j < z.elements.size(); j++) {
                if (key == z.elements[j]) {
                    result++;
                }
            }
        }
    }
    return result;
}

template<typename T>
std::vector<T> zonemap<T>::query(T low, T high)
{
    std::vector<T> result;
    if (low > high) {
        return result;
    }
    for (size_t i = 0; i < num_zones; i++) {
        zone<T> z = zones[i];
        if (high > z.min && low < z.max) {
            for (size_t j = 0; j < z.elements.size(); j++) {
                if (z.elements[j] > low && z.elements[j] < high) {
                    result.push_back(z.elements[j]);
                }
            }
        }
    }
    return result;
}


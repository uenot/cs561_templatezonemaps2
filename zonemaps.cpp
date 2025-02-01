#include "zonemaps.h"

template<typename T>
zonemap<T>::zonemap(std::vector<T> _elements, uint _num_elements_per_zone)
{
    // constructor 
    elements = _elements;
    num_elements_per_zone = _num_elements_per_zone;
    build(); // sets other fields: zones and num_zones
}

template<typename T>
void zonemap<T>::build() {
    num_zones = 0;
    for (size_t i = 0; i < elements.size(); i++) {
        zone<T> z;
        // one inner loop per zone
        for (size_t j = 0; j < num_elements_per_zone; j++) {
            T x = elements[i];
            // add to current zone
            z.elements.push_back(x);
            // set max/min if applicable
            if (x > z.max) {
                z.max = x;
            } else if (x < z.min) {
                z.min = x;
            }
            z.size++;
            i++;  // advance to next element (out of all elements)
            if (i >= elements.size()) {
                // prevent overflow with uneven zones
                break;
            }
        }
        zones.push_back(z);
        num_zones++;
    }
}

template<typename T>
size_t zonemap<T>::query(T key)
{
    size_t result = 0;
    // check all zones for possible keys
    for (size_t i = 0; i < num_zones; i++) {
        zone<T> z = zones[i];
        // check if zone can possibly contain key
        if (key > z.min && key < z.max) {
            // if so, check all elements in zone
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
    // check all zones for possible keys
    for (size_t i = 0; i < num_zones; i++) {
        zone<T> z = zones[i];
        // check if zone can possibly overlap with range
        if (high > z.min && low < z.max) {
            // if so, check all elements in zone
            for (size_t j = 0; j < z.elements.size(); j++) {
                // add element if it falls in given range
                if (z.elements[j] > low && z.elements[j] < high) {
                    result.push_back(z.elements[j]);
                }
            }
        }
    }
    return result;
}


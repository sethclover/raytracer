#pragma once

#include <cmath>

#include "raytracer/utils.hpp"

class interval {
    public:
        double min;
        double max;
        
        interval() : min(+infinity), max(-infinity) {}
        interval(double min, double max) : min(min), max(max) {}
        interval(const interval& a, const interval& b)
         : min(std::fmin(a.min, b.min)), max(std::fmax(a.max, b.max)) {}
        double size() const { return max - min; }
        bool contains(double x) const { return min <= x && x <= max; }
        bool surrounds(double x) const { return min < x && x < max; }
        double clamp(double x) const { return x < min ? min : (x > max ? max : x); }
        interval expand(double delta) const { return interval(min - delta / 2, max + delta / 2); }
};

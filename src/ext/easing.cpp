#include <cmath>
#include "easing.h"

#ifndef PI
#define PI 3.1415926545
#endif

double easeInSine(double t) {
    return sin(1.5707963 * t);
}

double easeOutSine(double t) {
    t -= 1.0;
    return 1 + sin(1.5707963 * t);
}

double easeInOutSine(double t) {
    return 0.5 * (1 + sin(3.1415926 * (t - 0.5)));
}

double easeInQuad(double t) {
    return t * t;
}

double easeOutQuad(double t) {
    return t * (2 - t);
}

double easeInOutQuad(double t) {
    return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
}

double easeInCubic(double t) {
    return t * t * t;
}

double easeOutCubic(double t) {
    t -= 1.0;
    return 1 + t * t * t;
}

double easeInOutCubic(double t) {
    if (t < 0.5)
        return 4 * t * t * t;
    t -= 1.0;
    return 1 + 4 * t * t * t;
}

double easeInQuart(double t) {
    double t2 = t * t;
    return t2 * t2;
}

double easeOutQuart(double t) {
    t -= 1.0;
    double t2 = t * t;
    return 1 - t2 * t2;
}

double easeInOutQuart(double t) {
    if (t < 0.5) {
        double t2 = t * t;
        return 8 * t2 * t2;
    }
    t -= 1.0;
    double t2 = t * t;
    return 1 - 8 * t2 * t2;
}

double easeInQuint(double t) {
    double t2 = t * t;
    return t * t2 * t2;
}

double easeOutQuint(double t) {
    t -= 1.0;
    double t2 = t * t;
    return 1 + t * t2 * t2;
}

double easeInOutQuint(double t) {
    if (t < 0.5) {
        double t2 = t * t;
        return 16 * t * t2 * t2;
    }
    t -= 1.0;
    double t2 = t * t;
    return 1 + 16 * t * t2 * t2;
}

double easeInExpo(double t) {
    return (pow(2, 8 * t) - 1) / 255;
}

double easeOutExpo(double t) {
    return 1 - pow(2, -8 * t);
}

double easeInOutExpo(double t) {
    if (t < 0.5) {
        return (pow(2, 16 * t) - 1) / 510;
    }
    return 1 - 0.5 * pow(2, -16 * (t - 0.5));
}

double easeInCirc(double t) {
    return 1 - sqrt(1 - t);
}

double easeOutCirc(double t) {
    return sqrt(t);
}

double easeInOutCirc(double t) {
    if (t < 0.5) {
        return (1 - sqrt(1 - 2 * t)) * 0.5;
    }
    return (1 + sqrt(2 * t - 1)) * 0.5;
}

double easeInBack(double t) {
    return t * t * (2.70158 * t - 1.70158);
}

double easeOutBack(double t) {
    t -= 1.0;
    return 1 + t * t * (2.70158 * t + 1.70158);
}

double easeInOutBack(double t) {
    if (t < 0.5) {
        return t * t * (7 * t - 2.5) * 2;
    }
    t -= 1.0;
    return 1 + t * t * 2 * (7 * t + 2.5);
}

double easeInElastic(double t) {
    double t2 = t * t;
    return t2 * t2 * sin(t * PI * 4.5);
}

double easeOutElastic(double t) {
    double t2 = (t - 1) * (t - 1);
    return 1 - t2 * t2 * cos(t * PI * 4.5);
}

double easeInOutElastic(double t) {
    if (t < 0.45) {
        double t2 = t * t;
        return 8 * t2 * t2 * sin(t * PI * 9);
    }
    if (t < 0.55) {
        return 0.5 + 0.75 * sin(t * PI * 4);
    }
    double t2 = (t - 1) * (t - 1);
    return 1 - 8 * t2 * t2 * sin(t * PI * 9);
}

double easeInBounce(double t) {
    return pow(2, 6 * (t - 1)) * abs(sin(t * PI * 3.5));
}

double easeOutBounce(double t) {
    return 1 - pow(2, -6 * t) * abs(cos(t * PI * 3.5));
}

double easeInOutBounce(double t) {
    if (t < 0.5) {
        return 8 * pow(2, 8 * (t - 1)) * abs(sin(t * PI * 7));
    }
    return 1 - 8 * pow(2, -8 * t) * abs(sin(t * PI * 7));
}

double linear(double t) {
    return t;
}

// Simple array lookup — enum values are sequential 0..30
static const easingFunction easingTable[] = {
    easeInSine, easeOutSine, easeInOutSine,
    easeInQuad, easeOutQuad, easeInOutQuad,
    easeInCubic, easeOutCubic, easeInOutCubic,
    easeInQuart, easeOutQuart, easeInOutQuart,
    easeInQuint, easeOutQuint, easeInOutQuint,
    easeInExpo, easeOutExpo, easeInOutExpo,
    easeInCirc, easeOutCirc, easeInOutCirc,
    easeInBack, easeOutBack, easeInOutBack,
    easeInElastic, easeOutElastic, easeInOutElastic,
    easeInBounce, easeOutBounce, easeInOutBounce,
    linear
};

easingFunction getEasingFunction(easing_functions function)
{
    if (function < 0 || function > Linear)
        return linear;
    return easingTable[function];
}

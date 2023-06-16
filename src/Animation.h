#pragma once
#include <algorithm>
#include <chrono>
#include "ext/easing.h"

class Animation
{
public:
    Animation() : animationDuration(1.0f), EaseIn(EaseInQuad), EaseOut(EaseOutQuad) {};
    Animation(float AnimDuration) : animationDuration(AnimDuration) {};
    Animation(float AnimDuration, easing_functions In, easing_functions out) : animationDuration(AnimDuration), EaseIn(In), EaseOut(out) {};

    // A helper function to get the Delta-Time, which is the time elapsed since the last frame in seconds
    inline float GetDeltaTime()
    {
        // To shorten this very long class name with just "clock"
        using clock = std::chrono::high_resolution_clock;

        static clock::time_point prevTime = clock::now();
        clock::time_point currentTime = clock::now();

        // Calculate the duration between the previous time and the current time
        std::chrono::duration<float> duration = currentTime - prevTime;

        // Convert the duration to seconds and update the previous time
        float deltaTime = duration.count();
        prevTime = currentTime;

        // Done! just return the value now
        return deltaTime;
    }

    // Update the animations
    inline void Update()
    {
        // Reset the elapsed time if the bool switches
        static bool lastSwitch = bSwitch;
        if (bSwitch != lastSwitch)
            elapsedTime = 0;

        // Calculate the progress of the animation based on elapsed time
        elapsedTime = max(0.0f, min(elapsedTime, animationDuration));
        float t = elapsedTime / animationDuration;

        // Determine the initial value based on the current state
        float initialValue = bSwitch ? 0.0f : Value;

        // Determine the target value based on the current state (1.0f is max value)
        float targetValue = bSwitch ? 1.0f : 0.0f;

        // Select the appropriate easing function based on the current state
        easing_functions EaseInOrOut = bSwitch ? EaseIn : EaseOut;

        // Apply the appropriate easing function based on fade-in or fade-out (with lerping, which is basically what's the math were doing)
        Value = initialValue + (targetValue - initialValue) * getEasingFunction(EaseInOrOut)(t);

        // Increment the elapsed time
        elapsedTime += GetDeltaTime();
        lastSwitch = bSwitch; // Update last switch's value
    }

    float   getValue() { return Value; } // Get the current value of animation
    float   getValue(float baseValue) { return Value * baseValue; } // Get the current value multiplied by a base value
    int     getValue(int baseValue) { return (int)(Value * baseValue); } // Get the current value multiplied by a base value

    bool& getSwitch() { return bSwitch; }
    bool Switch() { return bSwitch = !bSwitch; } // Switch between easeIn or Out ( Switch(bool value) to set it to a wanted value )
    void Switch(bool value) { bSwitch = value; } // Set the switch bool to a value
private:
    // Set to true for ease-in animation, false for ease-out
    bool bSwitch = false;

    // Duration of the animation in seconds
    float animationDuration = 1.0f;
    float elapsedTime = 0.f;

    // Current value of the animation
    float Value = 0.f;

    // Ease in and out functions Declaration
    easing_functions EaseIn = EaseInQuad;
    easing_functions EaseOut = EaseOutQuad;
};


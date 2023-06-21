#pragma once
#include <algorithm>
#include <chrono>
#include "ext/easing.h"

class Animation
{
public:
    Animation() : m_flAnimationDuration(1.0f), m_eEaseIn(EaseInQuad), m_eEaseOut(EaseOutQuad) {};
    Animation(float AnimDuration) : m_flAnimationDuration(AnimDuration) {};
    Animation(float AnimDuration, easing_functions In, easing_functions out) : m_flAnimationDuration(AnimDuration), m_eEaseIn(In), m_eEaseOut(out) {};

    // Has to be called every tick
    void Update()
    {
        // Reset the elapsed time if the bool switches
        if (m_bSwitch != m_bLastSwitch)
            m_flElapsedTime = 0;

        m_flElapsedTime = max(0.0f, min(m_flElapsedTime, m_flAnimationDuration));
        float t = m_flElapsedTime / m_flAnimationDuration;

        // Determine the initial and target value based on the current state
        float initialValue = m_bSwitch ? 0.0f : m_flValue;
        float targetValue = m_bSwitch ? 1.0f : 0.0f; /*(1.0f is max value)*/

        // Select the appropriate easing function based on the current state
        easing_functions EaseInOrOut = m_bSwitch ? m_eEaseIn : m_eEaseOut;

        // Apply the appropriate easing function based on fade-in or fade-out (with lerping, which is basically what's the math were doing)
        m_flValue = initialValue + (targetValue - initialValue) * getEasingFunction(EaseInOrOut)(t);

        m_flElapsedTime += getDeltaTime();
        m_bLastSwitch = m_bSwitch;
    }

    float   getValue() { return m_flValue; }
    float   getValue(float scale) { return m_flValue * scale; } // Get the current value multiplied by a scale
    int     getValue(int baseValue) { return (int)(m_flValue * baseValue); } // Get the current value multiplied by a scale

    bool& getSwitch() { return m_bSwitch; }
    bool Switch() { return m_bSwitch = !m_bSwitch; } // Switch between easeIn or Out
    void Switch(bool value) { m_bSwitch = value; } // Set the switch bool to a value
protected:
    // Set to true for ease-in animation, false for ease-out
    bool m_bSwitch = 0;
    bool m_bLastSwitch = m_bSwitch;

    // Duration of the animation in seconds
    float m_flAnimationDuration = 1.0f;
    float m_flElapsedTime = 0.f;

    // Current value of the animation
    float m_flValue = 0.f;

    // Ease in and out functions Declaration
    easing_functions m_eEaseIn = EaseInQuad;
    easing_functions m_eEaseOut = EaseOutQuad;

    // To shorten this very long class name with just "clock"
    using clock = std::chrono::high_resolution_clock;

    // Variables related to the deltaTime function
    unsigned int m_iCurrTick = 0;
    unsigned int m_iLastTick = m_iCurrTick;
    clock::time_point m_tCurrTime = clock::now();
    clock::time_point m_tLastTickTime = clock::now();

    float getDeltaTime()
    {
        m_iCurrTick++;
        m_tCurrTime = clock::now();

        float deltaTime = 0;
        if (m_iLastTick != m_iCurrTick)
        {
            std::chrono::duration<float> duration = m_tCurrTime - m_tLastTickTime;
            deltaTime = duration.count();
        }

        m_iLastTick = m_iCurrTick;
        m_tLastTickTime = clock::now();

        return deltaTime;
    }
};

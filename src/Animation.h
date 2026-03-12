// Src: https://github.com/0xKNUCKS/cpp-animation-lib
#pragma once
#include <algorithm>
#include <chrono>
#include <vector>
#include <string>
#include "ext/easing.h"

enum AnimationFlags : uint16_t
{
    AnimationFlags_nil = 0,
    INVERSE = 1 << 0, // Inverses the results, e.g. 0 will have the result of 1, etc. (1-t)
    STATIC  = 1 << 1, // Pure function output — no lerping between start/target values
};

enum AnimationManagerFlags : uint16_t
{
    AnimationManagerFlags_nil = 0,
    OBJECT_REPLACE = 1 << 0, // Insert() replaces existing object with same key
};


class Animation
{
public:
    Animation() : m_flAnimationDuration(1.0f), m_eEaseIn(EaseInQuad), m_eEaseOut(EaseOutQuad) {};
    Animation(float AnimDuration, AnimationFlags flags = AnimationFlags_nil) : m_flAnimationDuration(AnimDuration), m_iFlags(flags) {};
    Animation(float AnimDuration, easing_functions In, easing_functions out, AnimationFlags flags = AnimationFlags_nil) : m_flAnimationDuration(AnimDuration), m_eEaseIn(In), m_eEaseOut(out), m_iFlags(flags) {};

    /**
     * @brief Update function, has to be called every tick.
     */
    void Update()
    {
        // Reset the elapsed time if the bool switches, store current value as starting point
        if (m_bSwitch != m_bLastSwitch) {
            m_flElapsedTime = 0;
            m_flStartValue = m_flValue;
        }

        m_flElapsedTime = std::clamp(m_flElapsedTime, 0.0f, m_flAnimationDuration);
        m_flTime = m_flElapsedTime / m_flAnimationDuration;

        // Select the appropriate easing function based on the current state
        easing_functions EaseInOrOut = m_bSwitch ? m_eEaseIn : m_eEaseOut;

        // Get the eased time value (optionally inverted)
        float functionValue = static_cast<float>(
            getEasingFunction(EaseInOrOut)((m_iFlags & INVERSE) ? 1.0 - m_flTime : m_flTime));

        if (!(m_iFlags & STATIC)) {
            // Lerp from start value to target — preserves smooth mid-switch transitions
            float targetValue = m_bSwitch ? 1.0f : 0.0f;
            m_flValue = m_flStartValue + (targetValue - m_flStartValue) * functionValue;
        }
        else {
            // Pure function output (no lerping)
            m_flValue = functionValue;
        }

        m_flElapsedTime += getDeltaTime();
        m_bLastSwitch = m_bSwitch;
    }

    float   getValue() { return m_flValue; }
    float   getValue(float scale) { return m_flValue * scale; }
    int     getValue(int baseValue) { return static_cast<int>(m_flValue * baseValue); }

    float   getTime() { return m_flTime; }

    bool& getSwitch() { return m_bSwitch; }
    bool Switch() { return m_bSwitch = !m_bSwitch; }
    void Switch(bool value) { m_bSwitch = value; }

protected:
    uint16_t m_iFlags = 0;

    bool m_bSwitch = false;
    bool m_bLastSwitch = m_bSwitch;

    float m_flAnimationDuration = 1.0f;
    float m_flElapsedTime = 0.f;
    float m_flTime = 0.f;

    float m_flValue = 0.f;
    float m_flStartValue = 0.f;

    easing_functions m_eEaseIn = EaseInQuad;
    easing_functions m_eEaseOut = EaseOutQuad;

    using clock = std::chrono::high_resolution_clock;
    clock::time_point m_tCurrTime = clock::now();
    clock::time_point m_tLastTickTime = clock::now();
    float m_fDeltaTime = 0;

    float getDeltaTime()
    {
        m_tCurrTime = clock::now();
        std::chrono::duration<float> duration = m_tCurrTime - m_tLastTickTime;
        m_fDeltaTime = duration.count();
        m_tLastTickTime = clock::now();
        return m_fDeltaTime;
    }
};


struct AnimationObjectDataQuery {
    std::string m_strName;
    Animation* m_animObject;
    int m_iIndex;
};

/**
 * @brief Manages multiple Animation objects by string key.
 * Call Update() once per frame to tick all registered animations.
 */
class AnimationManager
{
public:
    /**
     * @brief Query whether an animation with the given key exists.
     * @param out Optional — filled with name, pointer, and index if found.
     * @return true if the object exists.
     */
    bool Query(const std::string& keyName, AnimationObjectDataQuery* out = nullptr)
    {
        auto object = std::find_if(m_animObjects.begin(), m_animObjects.end(),
            [&keyName](const std::pair<std::string, Animation*>& element) {
                return element.first == keyName;
            });

        if (object != m_animObjects.end()) {
            if (out) {
                out->m_strName = object->first;
                out->m_animObject = object->second;
                out->m_iIndex = (int)std::distance(m_animObjects.begin(), object);
            }
            return true;
        }
        return false;
    }

    void Insert(const std::string& keyName, Animation* animationObject, AnimationManagerFlags flags = AnimationManagerFlags_nil)
    {
        AnimationObjectDataQuery Data;
        if (Query(keyName, &Data) && (flags & OBJECT_REPLACE)) {
            m_animObjects[Data.m_iIndex].second = animationObject;
        }
        else if (!Query(keyName)) {
            m_animObjects.push_back(std::make_pair(keyName, animationObject));
        }
    }

    bool Remove(const std::string& keyName)
    {
        AnimationObjectDataQuery Data;
        if (Query(keyName, &Data)) {
            m_animObjects.erase(m_animObjects.begin() + Data.m_iIndex);
            return true;
        }
        return false;
    }

    Animation* Inquire(const std::string& keyName)
    {
        AnimationObjectDataQuery Data;
        if (Query(keyName, &Data))
            return Data.m_animObject;
        return nullptr;
    }

    /**
     * @brief Update all registered animations. Call once per frame.
     */
    void Update()
    {
        for (auto& animObject : m_animObjects) {
            if (animObject.second)
                animObject.second->Update();
        }
    }

    ~AnimationManager() {
        for (auto& animObject : m_animObjects)
            delete animObject.second;
    }

private:
    std::vector<std::pair<std::string, Animation*>> m_animObjects;
};

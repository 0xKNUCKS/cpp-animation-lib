#pragma once
#include <algorithm>
#include <chrono>
#include <vector>
#include <string>
#include "ext/easing.h"

enum AnimationFlags : uint16_t
{
    AnimationFlags_nil,
    INVERSE, // Inverses the results, e.g. 0 will have the result of 1, etc. (1-t)
    STATIC, // Used for just a static function usage, meaning no need for switching between two functions with lerping etc.
};

enum AnimationManagerFlags : uint16_t
{
    AnimationManagerFlags_nil,
    //OBJECT_AVALIABLE, // used for the "Inquire" function, where only allow inquires for already avaliable objects. // maybe for later @todo
    OBJECT_REPLACE, // used for the "Insert" function, where if an object already exists, it would replace it.
};


class Animation
{
public:
    Animation() : m_flAnimationDuration(1.0f), m_eEaseIn(EaseInQuad), m_eEaseOut(EaseOutQuad) {};
    Animation(float AnimDuration, AnimationFlags flags = AnimationFlags_nil) : m_flAnimationDuration(AnimDuration), m_iFlags(flags) {};
    Animation(float AnimDuration, easing_functions In, easing_functions out, AnimationFlags flags = AnimationFlags_nil) : m_flAnimationDuration(AnimDuration), m_eEaseIn(In), m_eEaseOut(out), m_iFlags(flags) {};

    /*
    * @brief Update function, has to be called every tick.
    */
    void Update()
    {
        // Reset the elapsed time if the bool switches
        if (m_bSwitch != m_bLastSwitch)
            m_flElapsedTime = 0;

        m_flElapsedTime = max(0.0f, min(m_flElapsedTime, m_flAnimationDuration));
        t = m_flElapsedTime / m_flAnimationDuration;

        // Select the appropriate easing function based on the current state
        easing_functions EaseInOrOut = m_bSwitch ? m_eEaseIn : m_eEaseOut;

        // Get the current function Value
        float functionValue = getEasingFunction(EaseInOrOut)((m_iFlags & AnimationFlags::INVERSE) ? 1.f - t : t);

        // if the STATIC flag isnt applied, do the corresponding code
        if (!(m_iFlags & AnimationFlags::STATIC)) {

            // Determine the initial and target value based on the current state
            float initialValue = m_bSwitch ? 0.0f : m_flValue;
            float targetValue = m_bSwitch ? 1.0f : 0.0f;

            // Apply the appropriate easing function based on fade-in or fade-out (with lerping, which is basically what's the math were doing)
            m_flValue = initialValue + (targetValue - initialValue) * functionValue;
        }
        else {
            // just the pure function value
            m_flValue = functionValue;
        }

        m_flElapsedTime += getDeltaTime();
        m_bLastSwitch = m_bSwitch;
    }

    float   getValue() { return m_flValue; }
    float   getValue(float scale) { return m_flValue * scale; } // Get the current value multiplied by a scale
    int     getValue(int scale) { return (int)(m_flValue * scale); } // int wrapper

    float getTime() { return t; }

    bool& getSwitch() { return m_bSwitch; }
    bool Switch() { return m_bSwitch = !m_bSwitch; }
    void Switch(bool value) { m_bSwitch = value; }
protected:
    uint16_t m_iFlags = 0;

    // Set to true for ease-in animation, false for ease-out
    bool m_bSwitch = 0;
    bool m_bLastSwitch = m_bSwitch;

    // Duration of the animation in seconds
    float m_flAnimationDuration = 1.0f;
    float m_flElapsedTime = 0.f;

    // Time elapsed
    float t = 0.f;

    // Current value of the animation
    float m_flValue = 0.f;

    // Ease in and out functions Declaration
    easing_functions m_eEaseIn = EaseInQuad;
    easing_functions m_eEaseOut = EaseOutQuad;

    // To shorten this very long class name with just "clock"
    using clock = std::chrono::high_resolution_clock;

    // Variables related to the deltaTime function
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

class AnimationManager
{
public:
    /*
    * @brief Query about a specific object with a key name.
    * @return Whether the object exists or not.
    */
    bool Query(std::string keyName, AnimationObjectDataQuery* out = nullptr)
    {
        //std::vector<std::pair<std::string, Animation*>>::iterator
        auto object =
            std::find_if(m_animObjects.begin(), m_animObjects.end(),
                [keyName](const std::pair<std::string, Animation*>& element) {
                    return element.first == keyName;
                });

        if (object != m_animObjects.end() && out != nullptr) {
            out->m_strName = object->first;
            out->m_animObject = object->second;
            out->m_iIndex = (int)std::distance(m_animObjects.begin(), object);
            return true;
        }

        return false;
    }
    void Insert(std::string keyName, Animation* animationObject, AnimationManagerFlags flags = AnimationManagerFlags_nil)
    {
        AnimationObjectDataQuery Data;
        if (Query(keyName, &Data)
            && (flags & AnimationManagerFlags::OBJECT_REPLACE)) {
            m_animObjects[Data.m_iIndex].second = animationObject;
        }
        else {
            m_animObjects.push_back(std::make_pair(keyName, animationObject));
        }
    }
    bool Remove(std::string keyName)
    {
        AnimationObjectDataQuery Data;
        if (Query(keyName, &Data)) {
            m_animObjects.erase(m_animObjects.begin() + Data.m_iIndex);
            return true;
        }

        return false;
    }
    Animation* Inquire(std::string keyName)
    {
        AnimationObjectDataQuery Data;
        if (Query(keyName, &Data)) {
            return Data.m_animObject;
        }
        
        return nullptr;
    }
    /*
    * @brief Update function, has to be called every tick.
    */
    void Update()
    {
        printf("Update() got called!\n");
        for (auto& animObject : m_animObjects) {
            if (animObject.second != nullptr) {
                printf("Calling [%s]'s Update Function!\n", animObject.first.c_str());
                animObject.second->Update(); // as easy as that
            }
        }
    }

    ~AnimationManager() {
        for (auto& animObject : m_animObjects) {
            delete animObject.second; // free da memoryy ()~~~()
        }
    }
private:
    std::vector<std::pair<std::string, Animation*>> m_animObjects;
};

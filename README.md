# cpp-animation-lib
A ready to use animations utility library made for C++

# Example
with this example we are going to use [ImGui](https://github.com/ocornut/imgui) to do a fading animation for the back-ground <br> <br>
Were gonna start with initilizing the <strong>_Animation_</strong> class
```cpp
Animation anmFade;
```
you can aslo choose the properties you want for the animation like in this example
```cpp
// Set the animation duration you want in seconds
Animation anmFade(2.5f); // args: float AnimDuration

// or also choose the easing functions you want to use
Animation anmFade(2.5f, EaseInCirc, EaseOutCirc);
// args: float AnimDuration, easing_functions In, easing_functions out
// Note: EaseInQuad, EaseOutQuad will be the default functions used if not specfied
```
Then now, we have to constantly update it by calling the <strong>_Update_</strong> function inside your main loop
```cpp
anmFade.Update();
```
Then now, to retrieve the value of the current animation, use the function <strong>_getValue_</strong>
```cpp
// Will return a value from 0.0 -> 1.0
float AlphaValue = anmFade.getValue()

// or do this to apply the animation to a certian base value
// here for example if the fade value is 0.65, and we just want to animate it creating fade-in and out affects, add a paramater inside of getValue like this
float AlphaValue = anmFade.getValue(0.65f); // This will just multiply the value by the value given
// args: float | int baseValue
```
Thats it for getting the value of the animation and updating it!<br><br>
but what about switching from fade-in or out, to do that we'll use the <strong>_Switch_</strong> function, or <strong>_getSwitch_</strong> based on what we're doing, and like said we'll be using [ImGui](https://github.com/ocornut/imgui) for our example here
```cpp
// All of the steps that we've already done
static Animation anmFade(1.5f, EaseInCubic, EaseOutSine);
anmFade.Update();

float AlphaValue = anmFade.getValue(0.65f);
ImColor col = ImVec4(0.f, 0.f, 0.f, AlphaValue);
ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImGui::GetIO()DisplaySize, col);
    
// How to use the switch for a button
if (ImGui::Button("Button Switch!"))
    anmFade.Switch(); // Will switch between ease/fade-in and out

// Or for checkboxes, were gonna have to use the "getSwtich" function
ImGui::Checkbox("Button Switch!", &anmFade.getSwitch());
```
Violla!<br>
the code above will result in a working fade-in and out back-ground easily!<br>
you can use this for basically everything in your code that you want to animate or transition any value smoothly really

# Notes
You can explore the easing functions and see their visual effects and behaviours [here](https://easings.net/) at [easings.net](https://easings.net/)!

# Credits
[Easings Functions](https://github.com/nicolausYes/easing-functions) | [Nikolay](https://github.com/nicolausYes) for the C++ easing functions library!
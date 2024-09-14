
# cpp-animation-lib

A ready to use animations utility library made for C++

# Example

in this example we are going to use [ImGui](https://github.com/ocornut/imgui) to do a fading animation for the back-ground  using our "Animation Manager"<br><br>

Were gonna start with initilizing the <strong>_AnimationManager_</strong> class

```cpp

AnimationManager animManager;

```

You can create an animation and insert it into the `AnimationManager` with a key name.

```cpp

// Set the animation duration you want in seconds

Animation  animFade(2.5f);
// args: float AnimDuration

// or also choose the easing functions you want to use

Animation  animFade(2.5f, EaseInCirc, EaseOutCirc);
// args: float AnimDuration, easing_functions In, easing_functions out

// Note: EaseInQuad, EaseOutQuad will be the default functions used if not specfied

```

Now to link it to our Manager use the `Insert` function:

```cpp

animManager.Insert("FADE", &animFade);
// args: std::string keyName, Animation* animationObject, AnimationManagerFlags flags = ?

// OR: (cleaner, more straightforward)

animManager.Insert("FADE", new Animation(...));
// Animation properties goes in ...

```

Then now, we have to constantly update it by calling the `Update` function inside your main loop

```cpp

animManager.Update();

```

now, get a hold of your animation object using `Inquire` to access it and play with it, like so:

```cpp

Animation* managerFade = animManager.Inquire("FADE");

```

now, after getting our object using <strong>_Inquire_</strong>, use the function `getValue` to retrieve your on-going animation value.

```cpp

// Will return a value from 0.0 -> 1.0
float  AlphaValue = managerFade->getValue()

// or do this to scale the animation with a certain value, add a parameters to the getValue
float  AlphaValue = managerFade->getValue(0.65f); // This will just multiply the value by the value given
// args: float | int scale

```

Thats about it for getting the value of the animation and updating it!<br><br>

but what about switching from fade-in or out, to do that we'll use the `Switch` function, or `getSwitch` based on what we're doing, and like said we'll be using [ImGui](https://github.com/ocornut/imgui) for our example here

```cpp

// How to use the switch with a button
if (ImGui::Button("Switch!")) {
 managerFade->Switch(); // Will switch between ease/fade-in and out
}

// Or for checkboxes, were gonna have to use the "getSwtich" function
ImGui::Checkbox("Button Switch!", &managerFade->getSwitch());

```

Our final code:

```cpp
// outside your loop:
AnimationManager animManager;

animManager.Insert("FADE", new Animation(0.75f, EaseInCirc, EaseOutCirc));

// inside your loop:
animManager->Update();
Animation* managerFade = animManager.Inquire("FADE");

float AlphaValue = managerFade->getValue(0.65f);
ImColor color = ImVec4(0.f, 0.f, 0.f, AlphaValue);

ImGui::Begin("Fade testing!");

if (ImGui::Button("Click me!")) {
 managerFade->Switch();
}

ImGui::End();

ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImGui::GetIO().DisplaySize, col);

```

Voilà!<br>

the code above will result in a working fade-in and out back-ground easily!<br>

you can use this for basically everything in your code that you want to animate or transition any value smoothly really

***

Another great example is also adding a <strong>_pop-up window_</strong>! To demonstrate this, I will also be using [ImGui](https://github.com/ocornut/imgui). Although all the steps are the same, I will also provide the Class Properties and the necessary math calculations specifically for the proof of concept. This will allow you to try it out, understand the code better, and become more familiar with using the library and taking advantage of our Manager.

```cpp

// OUTSIDE YOUR LOOP:

AnimationManager animManager;

// the Properties for a proper pop-up animation
animManager.Insert("POPUP", new Animation(0.5f, EaseOutBack, EaseOutSine));
animManager.Insert("FADE", new  Animation(0.75f));


// INSIDE YOUR LOOP:

animManager.Update();

// retrieve your animation objects
Animation* managerPopup = animManager.Inquire("POPUP");
Animation* managerFade = animManager.Inquire("FADE");

// the math calculations for the size, and position
int  width = 400, height = 200;

auto  Size = ImVec2(managerFade->getValue(width), managerFade->getValue(height));
auto  Pos = ImVec2((ImGui::GetIO().DisplaySize - Size) / 2);

ImGui::Begin("Animation testing!");

if (ImGui::Button("Click me!")) {
 managerFade->Switch();
 managerPopup->Switch();
}

ImGui::End();

// Background color fade!
float AlphaValue = managerFade->getValue(0.65f);
ImColor color = ImVec4(0.f, 0.f, 0.f, AlphaValue);
ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImGui::GetIO().DisplaySize, color);

// This is only because ImGui limits window sizes, and wont go to 0
if (managerPopup->getValue() > 0.1f)
{

 ImGui::SetNextWindowPos(Pos);

 ImGui::SetNextWindowSize(Size);

 ImGui::Begin("Popup Window");
 // demo window...
 ImGui::End();

}

```

Try this out for yourself and see the results!

you can play around with this as much as you want and just go crazy with it.<br>

<strong>Have fun!</strong>

# Notes

You can explore the easing functions and see their visual effects and behaviours at [easings.net](https://easings.net/)!

# Credits

[Easings Functions](https://github.com/nicolausYes/easing-functions) | [Nikolay](https://github.com/nicolausYes) for the C++ easing functions library!

<br>

[Cazz](https://github.com/cazzwastaken)'s [Discord Server](https://discord.com/invite/xcZgJbAARc) | @Patoke for the great feedback provided!

# SmartwatchMockup
A mockup of my DIY smartwatch, in which to easily test code.

## Why DIY?

I'm aiming to build an Arduino-based smartwatch, with a far reduced featureset compared to say an Apple Watch or Android Wear.

Generally, as long as it lasts 3 months on a charge/battery, I'm happy.

## Mockup?

To test code before running it on real hardware, I have stubbed libraries that will be utilised in the final real product such that they run on macOS. As a result, I can write code for the smartwatch on top of an "emulated" Arduino, greatly increasing the pace of development.

The mockup is currently interactable, with 3 buttons defined for use, mapped to number keys:
- 3 is the "action" button. 
  - Press once to select an item, or move to the menu from the watchface. 
  - Hold for a short duration to return to the watchface.
- 5 is mapped to move up.
- 6 is mapped to move down.

## Underlying principles

This project is wholly written in <code>C++</code>.

I use the concept of "pseudo-applications" to encapsulate functionality into discrete units:
- Each application inherits from <code>Application</code>, and implements their own logic. 
- <code>viewDidLoad()</code> is implemented in each application to initially set up its UI.
- <code>update()</code> is also implemented to undertake changes to the UI over time as needed.
- <code>setApplicationState()</code> is utilised to switch the application into different states of "execution".

All applications are managed by a <code>SuperApplication</code>, which handles switching between applications, managing their root views, forwarding button events, and so forth. For those familiar with iOS, think of it as an iOS 1 era <code>SpringBoard</code>.

I have also built a basic UI library that resembles <code>UIKit</code> in its structure.

All code for the smartwatch is found in <code>/Sketch</code>, with stubbed libraries in <code>/Library</code>.

## Notes

Whilst this code is public, it is very much a work in progress, and is slow progress at that.

Completed components:
- Switching between applications and setting application state
- Basic UI and animation library
- First watchface

TODO:
- Cubic bezier curves on animations
- Implement a "scrollbar-esque" UI to indicate scroll progress on <code>ListView</code>
- Implement highlighting the currently selected item in a <code>ListView</code>
- Add more applications
- ...?

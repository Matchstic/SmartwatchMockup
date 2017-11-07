# SmartwatchMockup
A mockup of my DIY smartwatch, in which to easily test code.

## Why DIY?

I'm aiming to build an Arduino-based smartwatch, with a far reduced featureset compared to say an Apple Watch or Android Wear.

Generally, as long as it lasts 3 months on a charge/battery, I'm happy.

## Mockup?

To test code before running it on real hardware, I have stubbed libraries that will be utilised in the final real product such that they run on macOS. As a result, I can write code for the smartwatch on top of an "emulated" Arduino, greatly increasing the pace of development.

## Underlying principles

This project is wholly written in <code>C++</code>.

I use the concept of "pseudo-applications" to encapsulate functionality into discrete units:
- Each application inherits from <code>Application</code>, and implements their own logic. 
- <code>viewDidLoad()</code> is implemented in each application to initially set up its UI.
- <code>update()</code> is also implemented to undertake changes to the UI over time as needed.
- <code>setApplicationState()</code> is utilised to switch the application into different states of "execution".

All applications are managed by a <code>SuperApplication</code>, which handles switching between applications, managing their root views, forwarding button events, and so forth. For those familiar with iOS, think of it as an iOS 1 era <code>SpringBoard</code>.

I have also built a basic UI library that resembles <code>UIKit</code> in its structure.

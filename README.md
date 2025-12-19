# VCV-LempModules
Rack modules for VCV Rack

## Modules

### TOGGLE
#### Output a constant voltage between 0-10v with a latch button.

Use the trim pot to set a constant voltage output between 0-10v.
When the toggle button is disengaged the output is 0v, when engaged it is set to the specified voltage output.

The CV input for the button accepts a trigger and will toggle the state of the button.

The CV input for the trim pot will allow you to modulate the voltage output. This is clamped to a range of 0-10v.

|![Toggle](/images/toggle.png)|![Toggle-Dark](/images/toggle-dark.png)|
|-|-|

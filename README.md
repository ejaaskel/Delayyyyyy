# Delayyyyyy
## That's six y's

Delayyyyyy is a simple program made with JUCE framework (v6.0.4). It's basically a delay or a beat repeater, not 100% sure which one exactly.

What it does is that it takes an input signal and copies it to a varying amounts of buffers with varying amounts of delay, causing... the delay effect. User can set the maximum delay, and the rest of the delay buffers have a increasingly small delay within that time period. Also decay time (called feedback in application), ping pong, and wetness parameters are present. There is also a BPM synced delay if the VST is used in a DAW (64-bit VST3 tested with Ableton 10)

I'm still working on finishing few small features and optimizing & bugfixing, but the VST3 in builds-folder already does what it promises. Following features are still on the to-do list:
- Different delay modes (instead of having just a "slowing" delay, have an equal and quickening delays)
- Manual BPM entry if playhead is not available
- Refactoring & optimizations
- Update JUCE version

Known issues in 0.1.0:
- This bad boy eats CPU like there's no tomorrow.
- Popping occurs when changing delay duration or echo amount

#### Thanks to:

[The Audio Programmer Youtube-channel](https://www.youtube.com/channel/UCpKb02FsH4WH4X_2xhIoJ1A)

Aalto University JUCE Delay tutorial (You may have to google this one)

![Delayyyyyy logo](https://ejaaskel.dev/wp-content/uploads/2021/10/delayyyyyy.jpg "Who names their stuff like this?")


# Delayyyyyy
## That's six y's

Delayyyyyy is a simple program made with [JUCE framework](https://juce.com/) (v6.0.4). It's basically a delay or a beat repeater, not 100% sure which one exactly.

What it does is that it takes an input signal and copies it to a varying amounts of buffers with varying amounts of delay, causing... the delay effect. User can set the maximum delay, and the rest of the delay buffers have a increasingly small delay within that time period. Also decay time (called feedback in application), ping pong, and wetness parameters are present. There is also a BPM synced delay if the VST is used in a DAW (64-bit VST3 tested with Ableton 10, Windows 10)

I'm still working on finishing few small features and optimizing & bugfixing, but the VST3 in builds-folder already does what it promises. You can find this project's [Trello-board here](https://trello.com/b/uUka0QdN/delayyyyyy).

Known issues in 0.1.0:
- This bad boy eats CPU like there's no tomorrow.
- Popping occurs when changing delay duration or echo amount

## Set-up and build instructions
1. Download and install [JUCE](https://juce.com/get-juce/download). I have been using [6.0.4 version](https://github.com/juce-framework/JUCE/releases/tag/6.0.4).
1. Open Projucer, the project management tool of JUCE
1. Download Delayyyyyy source code
1. In Projucer, select File->Open, and open Delayyyyyy.jucer file to import the project
1. Check the Exporters-section in the left-hand side menu of Projucer and ensure that your IDE is in the list of exporters. I have been using Visual Studio 2019, but you can set your own tool here. If you face issues later on with the build, I'd recommend clearing this section once even if you use Visual Studio 2019.
1. Check the Modules-section in the left-hand side menu of Projucer and ensure that the modules are loaded correctly and from the correct paths. You can either check "Use global path" box to load the module from the path Projucer thinks is correct, or set the "Path for <exporter>" option manually if you want.
1. After setting the Exporters and Modules, select the correct exporter from the top of the Projucer and click on it's icon to export the project and load it in selected IDE.
    - I have seen a situation where if you have multiple versions of Visual Studio, Projucer opens the default one, not the one you have set as an exporter. This will cause build issues. In this situation you can manually open the generated solution-file with the version of Visual Studio it was actually generated for (i.e. for me it automatically opens VS2017, despite me exporting the project for VS2019, and I had to manually load it in VS2019).
1. I can't guarantee how the project looks for other IDEs, but at least for Visual Studio there are three build targets: Delayyyyyy_SharedCode, Delayyyyyy_StandalonePlugin and Delayyyyyy_VST3
    - Delayyyyyy_SharedCode contains, yep you guessed it, shared code. Not much of use by itself.
    - Delayyyyyy_StandalonePlugin is the standalone application that can be used for testing and development.
    - Delayyyyyy_VST3 is the target for VST3 plugin that can be used in DAWs.
1. Build the Delayyyyyy_StandalonePlugin. Assuming the build successfully finishes, the application should open automatically. The audio input is disabled by default to avoid feedback loop (actually a big concern with this program!), so enable the input carefully and test the program out.

#### Thanks to:

[The Audio Programmer Youtube-channel](https://www.youtube.com/channel/UCpKb02FsH4WH4X_2xhIoJ1A)

Aalto University JUCE Delay tutorial (You may have to google this one)

![Delayyyyyy logo](https://ejaaskel.dev/wp-content/uploads/2021/10/delayyyyyy.jpg "Who names their stuff like this?")


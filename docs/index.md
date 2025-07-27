# About SpectrumAnalyzer

SpectrumAnalyzer was created because, since childhood, I liked watching the "jumping bars" moving in sync with music on my Technics EC-EH550 stereo system. Unfortunately, modern audio equipment with built-in spectrum analyzers is no longer available. There are many visualizer apps online, but they usually have limitations: they offer only a few adjustable parameters, which quickly become boring, or they rely on microphone input — which is not ideal because it picks up ambient noise, room echo, and the microphone changes the frequency response. SpectrumAnalyzer in its basic version works well with a microphone, but it is also easy to run on a Raspberry Pi, where you can capture audio samples from an optical input. I use a HiFiBerry DAC + DSP board for this purpose. One of the strengths of the application is its flexibility — users can freely modify colors and frequency bands. The app may not look spectacular yet, but it has great potential for experimentation and educational purposes. It helps new users easily get familiar with digital signal processing (DSP) and graphics programming.

---

# SpectrumAnalyzer architecture
![SpectrumAnalyzer architecture](https://github.com/user-attachments/assets/7df13dbb-58de-47f7-8432-3631ef989950)

The most important class in the program is **AudioSpectrumAnalyzer**. It manages the whole process using several threads that run in parallel to efficiently process data. These threads are:

- **samplesUpdater** – this thread is responsible for collecting data. It repeatedly calls the `getData()` function from the `audioConfig.py` file. The data doesn’t have to come directly from hardware — it can be any kind of signal, depending on what the user implements in `audioConfig.py`. The collected samples are then put into a queue.

- **fftCalculator** – this thread takes samples from the queue, applies a signal window (from `config.py`, using `getSignalWindow()`), and calculates the FFT using the FFTW library. It also uses Welch’s method, which increases the number of frames per second (FPS) by analyzing overlapping segments of the signal.

- **processing** – this thread processes the frequency-domain data. It calculates amplitude correction and signal power, then filters the data through max hold (to keep peak values), data averaging, and data smoothing. The processed data is then placed into another queue.

- **drafter** – this thread converts the dBFS levels into offsets that are sent to the graphics card to generate new vertex positions for the bars on the screen.

- **flowController** – this thread monitors the FPS and adjusts the overlapping used in Welch’s method. This adaptive algorithm automatically increases or decreases the number of frames depending on the hardware performance, making the program run smoothly on both strong and weak devices.


---

# Overview of audioConfig.py

This file handles input configuration and data acquisition for the SpectrumAnalyzer program.
While it is primarily designed for capturing audio input from hardware devices, it can be adapted for other types of input signals as well. The module initializes the input stream with parameters such as data length and sampling rate (usually provided from config.py), and provides a function to read and unpack raw data from the input device, typically stereo audio channels (left and right). Importantly, this file can be modified or replaced to simulate input data instead of reading from actual hardware. This makes it a valuable tool for testing, debugging, and experimenting with digital signal processing (DSP) algorithms without requiring real hardware input. Users can generate synthetic signals (e.g., sine waves, test patterns) or feed in data from alternative sources such as wave files or sensor devices like accelerometers. This flexibility allows developers and hobbyists to easily prototype and validate DSP techniques, explore different input scenarios, and ensure the program behaves as expected under various conditions — all without the need for a physical audio input device.

---

## Audio Input Device

### Initialization

- **Function:** `initialize(dataLength, samplingRate)`  
- **Description:** This function prepares the hardware or data source to capture chunks of data of size dataLength at the specified samplingRate. It opens an input stream using PyAudio configured for 16-bit stereo input and sets up the system to collect data in real-time. Although designed for audio input, this function is flexible — users can replace or extend it to work with other data sources, such as reading from a WAV file, accelerometer data, or any other stream. 
- **Default value:** True on success

### Collecting data from hardware

- **Function:** `getData()`  
- **Description:** This function reads a block of data from the input device. The data contains samples for both left and right audio channels (stereo). It converts the raw bytes into a list of 16-bit signed numbers, alternating between left and right channel samples. This way, you get the latest stereo audio data ready to use.
- **Default value:** *(Gets the latest data from hardware for left and right channels.)*

---

# Overview of config.py

The file config.py contains the core configuration functions for the SpectrumAnalyzer program.  
It defines window sizes, sampling parameters, colors, and shader code for visualization.

---

## Window Settings

### Fullscreen Default State

- **Function:** `getDefaultFullscreenState()`  
- **Description:** Determines whether the SpectrumAnalyzer application starts in fullscreen mode by default. Users can choose this to enable fullscreen startup automatically. This is especially useful on devices like Raspberry Pi, where fullscreen improves user experience and system integration.
- **Default value:** `False` (starts in windowed mode)

### Maximized Window Size

- **Functions:** `getMaximizedWindowHorizontalSize() and getMaximizedWindowVerticalSize`  
- **Description:** The user can select the fullscreen resolution. This is particularly useful on devices like the Raspberry Pi, where the maximum resolution may cause performance issues. Allowing users to lower the resolution can improve application responsiveness and ensure smoother performance.
- **Default value:** 1920×1080

### Normal Window Size

- **Function:** `getNormalWindowHorizontalSize() and getNormalWindowVerticalSize`  
- **Description:** The initial window size when the application starts in windowed mode. This setting ensures that the strips always look the way the user wants, providing a consistent visual layout without the need for manual resizing after launch.
- **Default value:** 1280×512

---

## Sampling and Signal Parameters

### Number of Samples
- **Function:** `getNumberOfSamples()` 
- **Description:** Users can adjust the number of audio samples processed each time to find the best setup for them. Using fewer than 4096 samples makes the display smoother and more responsive but lowers the detail at low frequencies. For example, with fewer samples, it becomes harder to distinguish between very close frequencies like 20 Hz and 30 Hz. Increasing the number of samples improves frequency detail but can make the program less responsive.

- **Default value:** 4096

### Sampling Rate
- **Function:** `getSamplingRate()` 
- **Description:**  This parameter is rarely changed but can be useful when analyzing audio at non-standard sample rates. It also allows experimenting with oversampling or downsampling (which PyAudio supports). Additionally, you might need to adjust it when connecting external devices with different sampling frequencies.
- **Default value:** 44100 Hz

### Desired Frame Rate
- **Function:** `getDesiredFrameRate()` 
- **Description:**  Sets the target number of frames per second (FPS) displayed on the screen. Since the number of samples and sampling rate limit how frequently new data can be processed, the program increases the frame rate by using overlapping. For example, with 4096 samples at a 44100 Hz sampling rate, the default frame rate is about 11 FPS. To reach higher rates like 60 FPS, the program applies Welch’s method, processing overlapping segments of the signal to effectively analyze more data per second. This parameter also allows adjusting the application to run smoothly on hardware with limited performance, such as a Raspberry Pi, which may struggle with higher frame rates, especially at higher resolutions.
- **Default value:** 60 FPS  


### Number of Signals for Averaging:
- **Function:** `getNumberOfSignalsForAveraging()` 
- **Description:** This parameter controls how many signal samples are averaged together to produce the displayed output. Increasing the number of signals for averaging results in smoother, more stable visualizations by reducing short-term fluctuations. However, it also introduces a slight delay in responsiveness. Lower values make the output more immediate and reactive but can appear more “jumpy.”
- **Default value:** 

### Number of Signals for Max Hold:
- **Function:** `getNumberOfSignalsForMaxHold()` 
- **Description:** Defines how many of the recent signal frames are considered when calculating the maximum hold value. This affects how quickly the displayed peak values update and start to drop, making the peak indicators appear either more stable or more dynamic.- **Default value:** 5

### Alpha Factor for Smoothing:
- **Function:** `getAlphaFactorForSmoothing()` 
- **Description:** Controls the amount of smoothing applied to the signal visualization. A lower alpha factor results in a smoother and more stable display, but with slower response to rapid changes. A higher alpha makes the visualization more responsive and sensitive to signal variations, although it may result in a more flickery or unstable display.
- **Default value:** 0.2


### Signal Window:
- **Function:** `getSignalWindow()` 
- **Description:** Users can apply a custom window function to the signal before frequency analysis, which allows for experimentation with different windowing techniques and their impact on the result. This offers flexibility in shaping the signal to reduce artifacts like spectral leakage. For example, you can use window functions from the scipy.signal module—such as Hamming, Blackman, or Kaiser—to tailor the analysis to your specific needs.
- **Default value:** Hanning window

### Scaling factor:
- **Function:** `getScalingFactor()` 
- **Description:** Adjusts the amplitude of the signal after windowing to ensure the signal level remains accurate, since different window functions can alter the overall amplitude.
- **Default value:** Calculates and returns the amplitude correction based on the average value of the window.


### Offset factor:
- **Function:** `getOffsetFactor()` 
- **Description:** Allows precise adjustment of the signal amplitude, useful when working with external devices or simply to raise the signal level if the displayed bars on the screen appear too low.
- **Default value:** 0



---

## Visual Settings
 
### Selecting frequencies / Number of bars:
- **Function:** `getFrequencies()` 
- **Description:** This function returns a list of target frequencies (in Hz) that the application will attempt to visualize using vertical bars on the screen. However, it’s important to understand that the actual signal analysis is carried out using an FFT (Fast Fourier Transform), whose frequency resolution is determined by two key parameters: the number of samples (getNumberOfSamples()) and the sampling rate (getSamplingRate()). For example, if you use 4096 samples with a sampling rate of 44100 Hz, the frequency resolution of the FFT is approximately 10.77 Hz (calculated by dividing the sampling rate by the number of samples, i.e., 44100 / 4096). This means that the FFT output only includes energy values at specific, evenly spaced frequency steps — also known as FFT bins — such as 0 Hz, 10.77 Hz, 21.55 Hz, 32.32 Hz, 43.09 Hz, and so on. When you define a list of frequencies using getFrequencies(), the application maps each requested frequency to the nearest available FFT bin. As a result, if multiple frequencies are too close together (for example, 40 Hz, 41 Hz, and 43 Hz), they may all get mapped to the same bin — such as 43.09 Hz — and thus produce identical or nearly identical visual bars on the screen. The number of bars shown in the visualization is equal to the number of frequencies returned by this function, but this does not improve the frequency resolution of the signal analysis. The FFT is always computed over the full spectrum based on the sample size and sampling rate. Increasing the number of requested frequencies only determines how many bars are drawn and which FFT bins are sampled — not how precisely the signal is analyzed. 
- **Default value:** (20 40 60 ... When analyzing sound, the function usually returns a list of frequencies where low frequencies are represented by closely spaced values to better distinguish subtle differences, while higher frequencies are spaced farther apart since fine details are harder to perceive. However, this approach is common but not mandatory — the frequency spacing can be adjusted based on specific needs or preferences.)

### Static lines color:
- **Function:** `getColorOfStaticLines()` 
- **Description:** Allows you to customize the color (RGB) and transparency of static lines that remain constant on the display.
- **Default value:** (0.15,0.15,0.15,0.15)

### Horizontal Line Positions in dBFS:
- **Function:** `getHorizontalLinePositions()` 
- **Description:** Lets the user define how many horizontal lines are shown on the display and at which signal power levels (in dBFS) they appear. This allows for precise customization of the scale and visual reference points in the signal visualization.
- **Default value:** –6.02, –12.04, –18.06, ..., –90.30 dBFS

### Rectangle colors for vertices:
- **Function:** `getColorsOfRectangle()` 
- **Description:** Each bar (rectangle) on the screen is made up of 4 corners (vertices). You can set a separate color for each vertex using RGBA values — where R is red, G is green, B is blue, and A stands for transparency. The GPU then smoothly blends the colors between the corners, allowing you to easily control the appearance of each bar, for example by creating smooth color gradients from top to bottom.
- **Default value:** RGBA color (Red, Green, Blue, Transparency) for each vertex

### Small rectangle colors for vertices:
- **Function:** `getColorsOfDynamicMaxHoldRectangle()` 
- **Description:** Uses the same method of assigning RGBA colors to each of the 4 rectangle vertices as for the main bars. This time, however, it applies to the small visual elements that hold and display the peak (max hold) values over time. By setting the vertex colors, you can control the appearance of these peak indicators, including gradients and transparency.
- **Default value:** RGBA color (Red, Green, Blue, Transparency) for each vertex

### Dynamic Max Hold visibility state
- **Function:** `getDynamicMaxHoldVisibilityState()` 
- **Description:** Toggles the visibility of on-screen elements that display the peak (max hold) values over time. When enabled, small markers show the highest recent signal levels.
- **Default value:** True

### Dynamic Max Hold rectangle height
- **Function:** `getDynamicMaxHoldRectangleHeightInPercentOfScreenSize()` 
- **Description:** Allows the user to control the height of the rectangles that represent dynamic max hold values. The value is given as a percentage of the screen height — for example, 1 means the rectangle will be 1% of the total screen height.
- **Default value:** 0.8

### Dynamic Max Hold speed of falling
- **Function:** `getDynamicMaxHoldSpeedOfFalling()` 
- **Description:** Sets how fast the max hold markers decrease in value, affecting the speed at which the peak indicators move downward.
- **Default value:** 900

### Dynamic Max Hold falling acceleration
- **Function:** `getDynamicMaxHoldAccelerationStateOfFalling()` 
- **Description:** Determines whether the max hold elements fall with acceleration or at a constant speed. This gives the user control over the visual behavior and final effect of the peak indicators.
- **Default value:** True


---
### Fragment Shader Code (GLSL)
- **Function:** `getAdvancedColorSettings()` 
- **Description:** The application first determines how many frequency bins are defined by the user, based on the number of frequencies provided by the getFrequencies() function. Then, it divides the screen into the same number of vertical bars — one for each frequency. Each bar is built using two triangles (6 vertices), which form a rectangle defined by 4 unique corner points in 2D space (x, y). These bars are then sent to the GPU with their initial positions. At this stage, all bars are evenly spaced and aligned. As the application analyzes the audio (or other signal) in real time, it calculates the power of each frequency. Based on this power, the height or vertical position of each bar is updated dynamically to reflect the current signal strength. The vertex shader updates the positions of the bar's corners depending on the signal’s power for that frequency. The GPU then fills in all the pixels between those corners by generating fragments — and each of these fragments is processed by the fragment shader. One of the values the fragment shader receives is calculatedPosition, which (in simplified terms) tells it how high up the current pixel is within its rectangle. This vertical position is used to control how the pixel will be colored. In addition, the shader receives vertColor values — these are colors assigned to each corner of the rectangle using application-side functions like getColorsOfRectangle() and getColorsOfDynamicMaxHoldRectangle(). The GPU automatically blends these colors across the surface of the rectangle, so each pixel gets its own color depending on where it lies. Inside the fragment shader, this blended color is combined with a vertical color gradient that transitions from blue → cyan → green → yellow → red, depending on the pixel’s height within the bar. This results in vibrant and informative color transitions that follow the signal in real time. Users can adjust the rectangle corner colors directly in the application to experiment with color effects. And because the shader computes colors for each pixel independently, there’s a lot of flexibility for customization. More advanced users can even edit the shader code to fully control how colors behave. All of this runs on the GPU, which executes the fragment shader for every pixel of every bar in parallel. This makes the visualization extremely fast and smooth — even when rendering many bars at once. This method not only results in dynamic and responsive visualizations, but also allows users to experiment and learn how shaders and GPU parallelism work in real-time graphics.

---
# Playing with graphics

### Example 1

- **Changed functions:** none
- **Description:** This is the default view in version 1.9. It will serve as the basis for further changes.
- **Picture:**  <img width="1280" height="512" alt="Drawing1" src="https://github.com/user-attachments/assets/a0a98d55-c578-4493-80b8-421b9f5fcd9f" />

- **Config:** [config1.txt](https://github.com/user-attachments/files/21334024/config1.txt) 


### Example 2

- **Changed functions:**
```python
def getDynamicMaxHoldVisibilityState():
    return False
```
- **Description:** This is one of the easiest settings to modify. By returning False instead of True, the dynamic max hold rectangles (which store peak values) will no longer be displayed.
- **Picture:**  <img width="1280" height="512" alt="Drawing2" src="https://github.com/user-attachments/assets/9d8eee9b-c61d-4ab5-9be1-9f8432b2af56" />
- **Config:** [config2.txt](https://github.com/user-attachments/files/21334082/config2.txt)



### Example 3

- **Changed functions:** 
```python
def getFrequencies():
    return tuple(range(20,16001,20))
```

- **Description:** We can easily change the number of bars. Instead of manually specifying a list of frequencies, we can simply use code to generate a large number of frequency values, which directly translates to the number of bars on the screen.
In this case, we get 800 values ranging from 20 Hz to 16,000 Hz in steps of 20.
- **Picture:**  <img width="1280" height="512" alt="Drawing3" src="https://github.com/user-attachments/assets/637cbfea-b076-4ff8-967a-002871b59f0a" />

- **Config:** [config3.txt](https://github.com/user-attachments/files/21334083/config3.txt)



### Example 4

- **Changed functions:** 
```python
def getFrequencies():
    return tuple(range(50,4000,20))
```
- **Description:** A similar example to the previous one, but this time we generate a much smaller number of bars (198), using the same 20 Hz step but starting from a different value — 50 Hz.
- **Picture:**  <img width="1280" height="512" alt="Drawing4" src="https://github.com/user-attachments/assets/2d335d9f-bd62-4553-8a23-9c8a3101c48f" />
- **Config:** [config4.txt](https://github.com/user-attachments/files/21334085/config4.txt)




### Example 5

- **Changed functions:** 

```python
#if you get errors after modification please find following log with error msg: "VS log:"
def getAdvancedColorSettings():
    return '''#version 330 core

        in vec4 calculatedPosition;
        in vec4 vertColor;
        out vec4 Color;

        void main()
        {
            Color = vertColor;

        }
        '''
```

- **Description:** This time, we modify the shader code (code running on the graphics card) so that it doesn’t change the colors but simply passes them through as received from the function below. Each bar consists of two triangles because OpenGL renders shapes using triangles; nearly everything you draw is built from them. Case 0 corresponds to the color for the bottom-left corner, Case 1 for the bottom-right corner, Case 2 for the top-right corner, and Case 3 for the top-left corner. Colors between the vertices are interpolated by the graphics card, which means you only need to set the vertex colors, and the card will automatically calculate the colors in between.

```python

# Each rectangle consists of 2 triangles

# 3_____ 2   2
# |     /   /|
# |    /   / |
# |   /   /  |
# |  /   /   |
# | /   /    |
# |/   /_____|
# 0   0      1


def getColorsOfRectangle(vertex):
    match vertex:
        case 0:
            return (0.61, 0.61, 0.61,1)
        case 1:
            return (0,0,0,1)
        case 2:
            return (0,0,0,1)
        case 3:
            return (0.61, 0.61, 0.61,1)    
```

- **Picture:** <img width="1280" height="512" alt="Drawing5" src="https://github.com/user-attachments/assets/f4b13022-1ce3-43c7-a0c1-5b8022aea58c" />
- **Config:** [config5.txt](https://github.com/user-attachments/files/21334089/config5.txt)




### Example 6


- **Changed functions:** 

```python

def getFrequencies():
    return (20,40,60,80,100,120,150,180,220,250,300,330,360,400,440, 480,520,560,600,720)


# Each rectangle consists of 2 triangles

# 3_____ 2   2
# |     /   /|
# |    /   / |
# |   /   /  |
# |  /   /   |
# | /   /    |
# |/   /_____|
# 0   0      1

def getColorsOfRectangle(vertex):
    match vertex:
        case 0:
            return (1,0,0,1) #red
        case 1:
            return (0,0,1,1) #blue
        case 2:
            return (0,0,1,1) #blue
        case 3:
            return (1,0,0,1) #red
```

- **Description:** Similar to example 5, we have an edited getAdvancedColorSettings() function. Additionally, I reduced the number of frequencies/bars to make them larger on the screen using getFrequencies(). The getColorsOfRectangle() function now contains a different set of colors. The notation (1, 0, 0, 1) represents (Red, Green, Blue, Alpha), with values ranging from 0 to 1. You can see that both Case 0 (bottom-left corner) and Case 3 (top-left corner) are set to red. Since both vertices are red, no interpolation occurs between them — the color between those points remains solid red. Vertices for Case 1 (bottom-right) and Case 2 (top-right) are set to blue. Similarly, no color interpolation happens between them. This means the left side of the bar is red, and the right side is blue, so we can expect a gradual color transition between the left and right sides, as shown in the attached image.
- **Picture:** <img width="1280" height="512" alt="Drawing" src="https://github.com/user-attachments/assets/66bbea29-bb68-4a5a-8b5c-c62f1b59ec6f" />


- **Config:** 
[config6.txt](https://github.com/user-attachments/files/21334091/config6.txt)
  


### Example 7

- **Changed functions:** 

```python

# Each rectangle consists of 2 triangles

# 3_____ 2   2
# |     /   /|
# |    /   / |
# |   /   /  |
# |  /   /   |
# | /   /    |
# |/   /_____|
# 0   0      1

def getColorsOfRectangle(vertex):
    match vertex:
        case 0:
            return (0, 0,1,1) # blue
        case 1:
            return (1,0,0,1)  # red
        case 2:
            return (1,0,0,1)  # red
        case 3:
            return (0,0,1,1)  # blue

```

- **Description:** Similar to the previous example, we modify the getColorsOfRectangle() function, but this time the colors are reversed: the vertices on the left side are blue, while the vertices on the right side are red. This way, the bars are colored in the opposite way.
- **Picture:** <img width="1280" height="512" alt="Drawing7" src="https://github.com/user-attachments/assets/2e214e1c-819d-40e3-989c-540d3adf0ffe" />

- **Config:** [config7.txt](https://github.com/user-attachments/files/21334093/config7.txt)
  
  
### Example 8

- **Changed functions:** 

```python

# Each rectangle consists of 2 triangles

# 3_____ 2   2
# |     /   /|
# |    /   / |
# |   /   /  |
# |  /   /   |
# | /   /    |
# |/   /_____|
# 0   0      1

def getColorsOfRectangle(vertex):
    match vertex:
        case 0:
            return (1,0,0,1) # red
        case 1:
            return (1,0,0,1) # red
        case 2:
            return (0,0,1,1) # blue
        case 3:
            return (0,0,1,1) # blue

```

- **Description:** This time, the bottom vertices are red while the top vertices are blue. The color transition occurs vertically.
- **Picture:** <img width="1280" height="512" alt="Drawing8" src="https://github.com/user-attachments/assets/1eda3a4c-d294-4e38-a600-6f1be60b10ca" />
- **Config:** [config8.txt](https://github.com/user-attachments/files/21334095/config8.txt)

### Example 9

- **Changed functions:** 

```python

# Each rectangle consists of 2 triangles

# 3_____ 2   2
# |     /   /|
# |    /   / |
# |   /   /  |
# |  /   /   |
# | /   /    |
# |/   /_____|
# 0   0      1

def getColorsOfRectangle(vertex):
    match vertex:
        case 0:
            return (0,0,1,1) # blue
        case 1:
            return (0,0,1,1) # blue
        case 2:
            return (1,0,0,1) # red
        case 3:
            return (1,0,0,1) # red

```

- **Description:** Similar to the previous case, but reversed: the bottom vertices are blue and the top vertices are red. Again, you can see a vertical transition between the colors.
- **Picture:** <img width="1280" height="512" alt="Drawing9" src="https://github.com/user-attachments/assets/53904a7e-c56d-415f-9025-e837c662fe0d" />
- **Config:** [config9.txt](https://github.com/user-attachments/files/21334096/config9.txt)

### Example 10

- **Changed functions:** 

```python

# Each rectangle consists of 2 triangles

# 3_____ 2   2
# |     /   /|
# |    /   / |
# |   /   /  |
# |  /   /   |
# | /   /    |
# |/   /_____|
# 0   0      1

def getColorsOfRectangle(vertex):
    match vertex:
        case 0:
            return (0, 0,1,1) # blue
        case 1:
            return (0,0,0.2,1) # dark blue
        case 2:
            return (0,0,0.2,1) # dark blue
        case 3:
            return (0,0,1,1)  #  blue
```

- **Description:** The right side of the bar features a deep blue, while the left side is a lighter blue. By adjusting the color shades, we can simulate a sense of three-dimensional depth, despite the bars being 2D.
- **Picture:** <img width="1280" height="512" alt="Drawing10" src="https://github.com/user-attachments/assets/658b2872-8322-48d8-8670-0618fc5708af" />
- **Config:** [config10.txt](https://github.com/user-attachments/files/21334097/config10.txt)




### Example 11

- **Changed functions:** 

```python
#if you get errors after modification please find following log with error msg: "VS log:"
def getAdvancedColorSettings():
    return '''#version 330 core

        in vec4 calculatedPosition;
        in vec4 vertColor;
        out vec4 Color;

        void main()
        {
            vec4 tmpColor;

            float t = clamp(calculatedPosition.y, -1.0, 1.0);

            //Move position from [-1,1] to [0,1]
            float y = (t + 1.0) * 0.5;

            vec4 red    = vec4(1.0, 0.0, 0.0,1);
            vec4 yellow = vec4(1.0, 1.0, 0.0,1);
            vec4 green  = vec4(0.0, 1.0, 0.0,1);
            vec4 cyan   = vec4(0.0, 1.0, 1.0,1);
            vec4 blue   = vec4(0.0, 0.0, 1.0,1);

            //Use 4 segments: [0,0.25), [0.25,0.5), [0.5,0.75), [0.75,1.0]

            tmpColor = mix(blue, cyan, smoothstep(0.0, 0.25, y));
            tmpColor = mix(tmpColor ,green, smoothstep(0.25,0.5, y));
            tmpColor = mix(tmpColor ,yellow, smoothstep(0.5,0.75, y));
            tmpColor = mix(tmpColor ,red, smoothstep(0.75,1, y));

            

            // removing:  Color = mix(tmpColor, vertColor, 0.4);
            // adding:
            Color = tmpColor;
        }
        '''
```

- **Description:** In this case, the line of code responsible for mixing the colors returned by the getColorsOfRectangle() function has been removed, so we only have the colors defined by this code. The operation of this code might seem complex, especially since it runs on the graphics card. In simple terms, this code is executed for every pixel inside the bar, painting each pixel individually. calculatedPosition has two dimensions: x and y. We get the pixel’s position within the bar, and based on this position, we select the pixel’s color. First, we transform the position from the range [-1, 1] to [0, 1] because it is more convenient for further calculations. Next, if the pixel’s position in the bar is between 0 and 25% of the screen height, we choose a color between blue and cyan based on this. The function smoothstep(0.0, 0.25, y) checks if y is between 0 and 0.25; if it is, it returns values between 0 and 1, which are then passed to the mix(blue, cyan, 0-1) function. This way, mix knows how much blue and how much cyan to blend. If the value is greater than 0.25, mix returns cyan. Similarly, with the step mix(tmpColor, green, smoothstep(0.25, 0.5, y)), the smoothstep(0.25, 0.5, y) checks if y is between 0.25 and 0.5; if it is below, it returns 0 (so mix returns tmpColor), if above, it returns green, and so on. Thanks to this, the screen height is divided into 4 color segments.
- **Picture:** <img width="1280" height="512" alt="Drawing11" src="https://github.com/user-attachments/assets/88b65a2b-1f25-41e5-8a69-4ad573b9eaf9" />
- **Config:** [config11.txt](https://github.com/user-attachments/files/21334919/config11.txt)


### Example 12

- **Changed functions:** 


```python

def getAdvancedColorSettings():
    return '''#version 330 core

        in vec4 calculatedPosition;
        in vec4 vertColor;
        out vec4 Color;

        void main()
        {
            vec4 tmpColor;

            float t = clamp(calculatedPosition.x, -1.0, 1.0); //changing y to x

            //Move position from [-1,1] to [0,1]
            float y = (t + 1.0) * 0.5;

            vec4 red    = vec4(1.0, 0.0, 0.0,1);
            vec4 yellow = vec4(1.0, 1.0, 0.0,1);
            vec4 green  = vec4(0.0, 1.0, 0.0,1);
            vec4 cyan   = vec4(0.0, 1.0, 1.0,1);
            vec4 blue   = vec4(0.0, 0.0, 1.0,1);

            //Use 4 segments: [0,0.25), [0.25,0.5), [0.5,0.75), [0.75,1.0]

            tmpColor = mix(blue, cyan, smoothstep(0.0, 0.25, y));
            tmpColor = mix(tmpColor ,green, smoothstep(0.25,0.5, y));
            tmpColor = mix(tmpColor ,yellow, smoothstep(0.5,0.75, y));
            tmpColor = mix(tmpColor ,red, smoothstep(0.75,1, y));

            Color = tmpColor;

        }
        '''

```

- **Description:** Similar to before, but this time we check the x-position of the pixel on the screen. If the position is near 100% of the screen width, the pixel is colored red; if it’s near 0%, it’s colored blue.
- **Picture:** <img width="1280" height="512" alt="Drawing12" src="https://github.com/user-attachments/assets/d95c7e8b-ab8e-4e97-bc8c-0fe739dc295b" />
- **Config:** [config12.txt](https://github.com/user-attachments/files/21334918/config12.txt)

---
# This is where the real fun starts

You can simply copy and paste the contents of the getAdvancedColorSettings() function into ChatGPT and ask it to do something fancy. You don’t even have to understand the function — just ask and experiment with it. The animation changes over time, so you can see the effect evolve as it runs. Here’s what I got:

### Example 13
- **Picture:** <img width="1280" height="512" alt="Drawing13" src="https://github.com/user-attachments/assets/d0592877-32b5-414a-8963-7fd0da590757" />
- **Config:** [config13.txt](https://github.com/user-attachments/files/21335020/config13.txt)

### Example 14
- **Picture:** <img width="1280" height="512" alt="Drawing14" src="https://github.com/user-attachments/assets/78be2ed1-ef67-4b6d-b01d-6ce9482d43c2" />
- **Config:** [config14.txt](https://github.com/user-attachments/files/21335021/config14.txt)

### Example 15
- **Picture:** <img width="1280" height="512" alt="Drawing15" src="https://github.com/user-attachments/assets/3d1422b1-ce85-4334-ba17-655c9b6e149e" />
- **Config:** [config15.txt](https://github.com/user-attachments/files/21335026/config15.txt)

### Example 16
- **Picture:** <img width="1280" height="512" alt="Drawing16" src="https://github.com/user-attachments/assets/3032b880-b481-41ac-8692-ac4809d5c088" />
- **Config:** [config16.txt](https://github.com/user-attachments/files/21335027/config16.txt)

### Example 17
- **Picture:** <img width="1280" height="512" alt="Drawing17" src="https://github.com/user-attachments/assets/d4b41be7-b6b1-4d63-aa22-050c72a04e58" />
- **Config:** [config17.txt](https://github.com/user-attachments/files/21335030/config17.txt)











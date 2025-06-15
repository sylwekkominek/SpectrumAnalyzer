# Copyright (C) 2024-2025, Sylwester Kominek
# This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
# see file LICENSE in this source tree.

import math

def getHorizontalSize():
    return 1280
    
def getVerticalSize():
    return 512
    
def getNumberOfSamples():
    return 4096
    
def getSamplingRate():
    return 44100

def getDesiredFrameRate():
    return 60

def getGapWidthInRelationToRectangleWidth():
    return 0.0

def getNumberOfSignalsForAveraging():
    return 1

def getNumberOfSignalsForMaxHold():
    return 5

def getAlphaFactorForSmoothing():
    return 0.2
    
def getMaxQueueSize():
    return 20

def getFrequencies():
    return (20,40,60,80,100,120,150,180,220,250,300,330,360,400,440, 480,520,560,600,720,840,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900, 2000,2100,2200,2300,2400,2500,2600,2700,2800,2900,3000,3100,3200,3300,3400,3500,3600,3700,3800,3900,4000,4100,4200,4300,4400,4500,4600,4700,4800,4900,5000,5100,5200,5300,5400,5500,5600,5700,5800,5900,6000,6100,6200,6300,6400,6500,6600,6700,6800,6900,7000,7100,7200,7300,7400,7500,7600,7700,7800,7900,8000)

def getSignalWindow():
    #Hanning Window
    N = getNumberOfSamples()
    return tuple(0.5 * (1 - math.cos(2 * math.pi * n / (N - 1))) for n in range(N))

def getScalingFactor():
    return 1.2
    
def getOffsetFactor():
    return 0
    
def getDynamicMaxHoldVisibilityState():
    return True

def getDynamicMaxHoldRectangleHeightInPercentOfScreenSize():
    return 1.0

def getDynamicMaxHoldSpeedOfFalling():
    return 900

def getDynamicMaxHoldAccelerationStateOfFalling():
    return True

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


def getColorsOfDynamicMaxHoldRectangle(vertex):
        match vertex:
            case 0:
                return (0.61, 0.61, 0.61,1)
            case 1:
                return (0,0,0,1)
            case 2:
                return (0,0,0,1)
            case 3:
                return (0.61, 0.61, 0.61,1)


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

            //Use 5 segments: [0,0.25), [0.25,0.5), [0.5,0.75), [0.75,1.0]

            tmpColor = mix(blue, cyan, smoothstep(0.0, 0.25, y));
            tmpColor = mix(tmpColor ,green, smoothstep(0.25,0.5, y));
            tmpColor = mix(tmpColor ,yellow, smoothstep(0.5,0.75, y));
            tmpColor = mix(tmpColor ,red, smoothstep(0.75,1, y));

            // mix tmpColor with those returned by getColorsOfRectangle / getColorsOfDynamicMaxHoldRectangle
            Color = mix(tmpColor, vertColor, 0.4);

        }
        '''


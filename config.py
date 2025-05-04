# Copyright (C) 2024-2025, Sylwester Kominek
# This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
# see file LICENSE in this source tree.

def getHorizontalSize():
    return 1920
    
def getVerticalSize():
    return 1080
    
def getNumberOfSamples():
    return 4096
    
def getSamplingRate():
    return 44100

def getOverlapping():
    return 0.8

def getGapWidthInRelationToRectangleWidth():
    return 0.2

def getNumberOfSignalsForAveraging():
    return 1

def getNumberOfSignalsForMaxHold():
    return 5

def getAlphaFactorForSmoothing():
    return 0.2
    
def getMaxQueueSize():
    return 100

def getFrequencies():
    return (30,60,90,120,150,180,240,300,360, 480,600,720,840,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900, 2000,2200,2400,2600,2800,3000,3200,3400,3600,3800,4000,4200,4400,4600,4800,5000,5500,6000,6500,7000,7500,8000)

def getScalingFactor():
    return 1.2
    
def getOffsetFactor():
    return 0
    
def getSmallRectanglesVisibilityState():
    return True

def getSmallRectangleHeightInPercentOfScreenSize():
    return 1.0

def getSpeedOfFalling():
    return 900

def getAccelerationStateOfFalling():
    return True

#when enabled getAdvancedColorSettings function is used instead of getColorsOfRectangle/getColorsOfSmallRectangle
def getAdvancedColorSettingEnabled():
    return True

def getColorsOfRectangle(vertex):
    match vertex:
        case 0:
            return (0.0, 0.0, 1.0)
        case 1:
            return (0.0, 0.0, 0.2)
        case 2:
            return (0.0, 0.0, 0.2)
        case 3:
            return (0.0, 0.0, 1.0)
 
def getColorsOfSmallRectangle(vertex):
    match vertex:
        case 0:
            return (0.0, 0.0, 1.0)
        case 1:
            return (0.0, 0.0, 0.2)
        case 2:
            return (0.0, 0.0, 0.2)
        case 3:
            return (0.0, 0.0, 1.0)  
    

def getAdvancedColorSettings():
    return '''#version 330 core

in vec4 calculatedPosition;
out vec4 Color;

void main()
{
    if(calculatedPosition.y >= 1)
    {
        Color = vec4(1, 0 , 0 , 1.f);
    }
    else if((calculatedPosition.y<= 1) && (calculatedPosition.y > 0.5))
    {
        Color = vec4(1, -2 * calculatedPosition.y + 2 , 0 , 1.f);
    }
    else if((calculatedPosition.y<= 0.5) && (calculatedPosition.y > 0))
    {
        Color = vec4(2 * calculatedPosition.y, 1 , 0 , 1.f);
    }
    else if((calculatedPosition.y<= 0) && (calculatedPosition.y > -0.5))
    {
        Color = vec4(0, 1 , -2 * calculatedPosition.y , 1.f);
    }
    else if((calculatedPosition.y >= -1) && (calculatedPosition.y < -0.5))
    {
        Color = vec4(0, 2 * calculatedPosition.y + 2 , 1 , 1.f);
    }
    else
    {
        Color = vec4(0, 0, 1, 1.f);
    }

}'''

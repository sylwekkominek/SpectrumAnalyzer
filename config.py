# Copyright (C) 2024, Sylwester Kominek
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
    
def getNumberOfSignalsForAveraging():
    return 4

def getNumberOfSignalsForMaxHold():
    return 7
    
def getMaxQueueSize():
    return 100

def getFrequencies():
    return (30,60,90,120,150,180,240,300,480,600,720,840,1000,1200,1400,1600,1800,2000,2200,2400,2600,2800,3000,3200,3400,3600,3800,4000,4200,4400,4600,4800,5000,5500,6000,6500,7000,7500,8000)

    
def getScalingFactor():
    return 1.2
    
def getOffsetFactor():
    return 0
    
def getSmallRectanglesVisibilityState():
    return True

def getSpeedOfFalling():
    return 900

def getAccelerationStateOfFalling():
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
    
    

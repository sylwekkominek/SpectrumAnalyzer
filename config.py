# Copyright (C) 2024, Sylwester Kominek
# This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
# see file LICENSE in this source tree.

def getHorizontalSize():
    return 1920
    
def getVerticalSize():
    return 1080
    
def getNumberOfSamples():
    return 2048
    
def getSamplingRate():
    return 44100

def getOverlapping():
    return 0.6
    
def getNumberOfSignalsForAveraging():
    return 2

def getNumberOfSignalsForMaxHold():
    return 6
    
def getMaxQueueSize():
    return 50

def getFrequencies():
    return (20,60,120,180,240,300,360,460,600,720,860,1000,1500,2000,2500,3000,3500,4000,4500,5000,5500,6000,6500,7000,7500,8000,8500,9000,9500,10000,10500,11000,11500,12000,12500,13000,13500,14000,14500,15000)  

    
def getScalingFactor():
    return 1
    
def getOffsetFactor():
    return 0
    
def getSmallRectanglesVisibilityState():
    return True

def getSpeedOfFalling():
    return 600

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
    
    

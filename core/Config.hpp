/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "CommonTypes.hpp"


#include "config/AdvancedColorSettings.hpp"
#include "config/AlphaFactor.hpp"
#include "config/BackgroundColorSettings.hpp"
#include "config/ColorOfDynamicMaxHoldLine.hpp"
#include "config/ColorOfDynamicMaxHoldSecondaryLine.hpp"
#include "config/ColorOfLine.hpp"
#include "config/ColorOfStaticLines.hpp"
#include "config/ColorOfStaticText.hpp"
#include "config/ColorsOfDynamicMaxHoldRectangle.hpp"
#include "config/ColorsOfDynamicMaxHoldSecondaryRectangle.hpp"
#include "config/ColorsOfRectangle.hpp"
#include "config/DefaultFullscreenState.hpp"
#include "config/DesiredFrameRate.hpp"
#include "config/DynamicMaxHoldAccelerationStateOfFalling.hpp"
#include "config/DynamicMaxHoldRectangleHeightInPercentOfScreenSize.hpp"
#include "config/DynamicMaxHoldSecondarySpeedOfFalling.hpp"
#include "config/DynamicMaxHoldSecondaryVisibilityState.hpp"
#include "config/DynamicMaxHoldSpeedOfFalling.hpp"
#include "config/DynamicMaxHoldVisibilityState.hpp"
#include "config/Frequencies.hpp"
#include "config/FrequencyTextPositions.hpp"
#include "config/GapWidthInRelationToRectangleWidth.hpp"
#include "config/HorizontalLinePositions.hpp"
#include "config/LinesVisibilityState.hpp"
#include "config/MaximizedWindowSize.hpp"
#include "config/MaxQueueSize.hpp"
#include "config/NormalWindowSize.hpp"
#include "config/NumberOfRectangles.hpp"
#include "config/NumberOfSamples.hpp"
#include "config/NumberOfSignalsForAveraging.hpp"
#include "config/NumberOfSignalsForMaxHold.hpp"
#include "config/OffsetFactor.hpp"
#include "config/PythonDataSourceEnabled.hpp"
#include "config/RectanglesVisibilityState.hpp"
#include "config/SamplingRate.hpp"
#include "config/ScalingFactor.hpp"
#include "config/SignalWindow.hpp"
#include "config/VerticalDbfsRange.hpp"
#include "config/VerticalLinePositions.hpp"
#include "config/WindowTitle.hpp"
#include "config/LoopbackEnabled.hpp"
#include "config/SingleScaleMode.hpp"
#include "config/HorizontalDrawingArea.hpp"

#include <vector>
#include <cstdint>
#include <string>


struct Configuration
{
    template<typename T>
    const auto& get() const
    {
        return data.get<T>().value;
    }

    AnyData data;
};

std::ostream& operator<<(std::ostream& os, const Configuration &config);


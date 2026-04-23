/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "Config.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Configuration &config)
{
    os<<config.data.get<WindowTitle>();
    os<<config.data.get<PythonDataSourceEnabled>();
    os<<config.data.get<LoopbackEnabled>();
    os<<config.data.get<DefaultFullscreenState>();
    os<<config.data.get<MaximizedWindowSize>();
    os<<config.data.get<NormalWindowSize>();
    os<<config.data.get<NumberOfRectangles>();
    os<<config.data.get<GapWidthInRelationToRectangleWidth>();
    os<<config.data.get<NumberOfSamples>();
    os<<config.data.get<SamplingRate>();
    os<<config.data.get<DesiredFrameRate>();
    os<<config.data.get<NumberOfSignalsForAveraging>();
    os<<config.data.get<NumberOfSignalsForMaxHold>();
    os<<config.data.get<AlphaFactor>();
    os<<config.data.get<MaxQueueSize>();
    os<<config.data.get<ScalingFactor>();
    os<<config.data.get<OffsetFactor>();
    os<<config.data.get<RectanglesVisibilityState>();
    os<<config.data.get<LinesVisibilityState>();
    os<<config.data.get<DynamicMaxHoldVisibilityState>();
    os<<config.data.get<DynamicMaxHoldSecondaryVisibilityState>();
    os<<config.data.get<DynamicMaxHoldRectangleHeightInPercentOfScreenSize>();
    os<<config.data.get<DynamicMaxHoldAccelerationStateOfFalling>();
    os<<config.data.get<DynamicMaxHoldSpeedOfFalling>();
    os<<config.data.get<DynamicMaxHoldSecondarySpeedOfFalling>();
    os<<config.data.get<Freqs>();
    os<<config.data.get<SignalWindow>();
    os<<config.data.get<HorizontalLinePositions>();
    os<<config.data.get<VerticalLinePositions>();
    os<<config.data.get<FrequencyTextPositions>();
    os<<config.data.get<HorizontalDrawingArea>();
    os<<config.data.get<SingleScaleMode>();
    os<<config.data.get<VerticalDbfsRange>();
    os<<config.data.get<ColorOfLine>();
    os<<config.data.get<ColorOfStaticText>();
    os<<config.data.get<ColorOfStaticLines>();
    os<<config.data.get<ColorOfDynamicMaxHoldLine>();
    os<<config.data.get<ColorOfDynamicMaxHoldSecondaryLine>();
    os<<config.data.get<ColorsOfRectangle>();
    os<<config.data.get<ColorsOfDynamicMaxHoldRectangle>();

    return os;
}



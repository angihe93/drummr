#pragma once

#include <JuceHeader.h>

namespace DrumLane
{
    inline constexpr int kNumLanes = 8;

    inline int noteToLane (int n)
    {
        switch (n)
        {
            case 42: case 44: case 46:          return 0; // Hi-Hat (closed / pedal / open)
            case 49: case 52: case 55: case 57: return 1; // Crash / China / Splash
            case 37: case 38: case 39: case 40: return 2; // Snare family + side-stick / clap
            case 50:                            return 3; // High Tom
            case 35: case 36:                   return 4; // Kick
            case 47: case 48:                   return 5; // Mid Tom
            case 41: case 43: case 45:          return 6; // Floor / Low Tom
            case 51: case 53: case 56: case 59: return 7; // Ride / Ride Bell / Cowbell
            default:                            return -1;
        }
    }

    inline const char* laneName (int lane)
    {
        switch (lane)
        {
            case 0: return "Hi-Hat";
            case 1: return "Crash";
            case 2: return "Snare";
            case 3: return "Tom 1";
            case 4: return "Kick";
            case 5: return "Tom 2";
            case 6: return "Floor";
            case 7: return "Ride";
            default: return "";
        }
    }

    inline juce::Colour laneColour (int lane)
    {
        switch (lane)
        {
            case 0: return juce::Colour (0xffffd84d); // Hi-Hat
            case 1: return juce::Colour (0xffff6b6b); // Crash
            case 2: return juce::Colour (0xfff06292); // Snare
            case 3: return juce::Colour (0xff66bb6a); // Tom 1
            case 4: return juce::Colour (0xffffa726); // Kick
            case 5: return juce::Colour (0xff26c6da); // Tom 2
            case 6: return juce::Colour (0xff42a5f5); // Floor
            case 7: return juce::Colour (0xffba68c8); // Ride
            default: return juce::Colours::white;
        }
    }

    inline float gainUnit (int lane)
    {
        return lane == 1 ? 0.82f : 1.0f;
    }

    inline float sliderToActualGain (int lane, float sliderGain)
    {
        return sliderGain * gainUnit (lane);
    }

    inline float defaultSliderGain (int)
    {
        return 1.0f;
    }
}

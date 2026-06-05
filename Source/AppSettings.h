#pragma once

#include <JuceHeader.h>

juce::PropertiesFile* getAppSettings();
bool                  saveAppSettings();

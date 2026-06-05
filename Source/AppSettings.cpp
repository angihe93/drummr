#include "AppSettings.h"

namespace
{
juce::ApplicationProperties& getAppProperties()
{
    static juce::ApplicationProperties properties;
    static const bool configured = []()
    {
        juce::PropertiesFile::Options options;
        options.applicationName = "Drummr";
        options.filenameSuffix = "settings";
        options.folderName = "Drummr";
        options.osxLibrarySubFolder = "Application Support";
        properties.setStorageParameters (options);
        return true;
    }();

    juce::ignoreUnused (configured);
    return properties;
}
} // namespace

juce::PropertiesFile* getAppSettings()
{
    return getAppProperties().getUserSettings();
}

bool saveAppSettings()
{
    if (auto* settings = getAppSettings())
        return settings->saveIfNeeded();

    return false;
}

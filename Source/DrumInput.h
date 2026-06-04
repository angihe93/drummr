#pragma once

#include <JuceHeader.h>

#include "DrumSynth.h"

// Routes note-ons from the computer keyboard and any connected MIDI input
// device into a dedicated DrumSynth. Uses a MidiMessageCollector so events
// posted from the MIDI thread / message thread reach the audio thread
// without locking.
class DrumInput : public juce::MidiInputCallback
{
public:
    DrumInput() = default;

    void prepare (double sampleRate);
    void reset();

    // Audio thread. Drains queued events, fires DrumSynth voices, and
    // additively mixes the input drum output into `output`.
    void processBlock (juce::AudioBuffer<float>& output,
                       int startSample, int numSamples);

    void setMasterGain (float g) { synth.setMasterGain (g); }
    void setLaneGain   (int lane, float g) { synth.setLaneGain (lane, g); }

    // Thread-safe. Called from the UI thread when a computer-keyboard drum
    // key is pressed.
    void postNoteOn (int midiNote, float velocity);

    // MidiInputCallback — runs on the MIDI device's thread.
    void handleIncomingMidiMessage (juce::MidiInput*,
                                    const juce::MidiMessage&) override;

private:
    juce::MidiMessageCollector collector;
    DrumSynth                  synth;
};

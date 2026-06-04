#include "DrumNotesView.h"

DrumNotesView::DrumNotesView (MidiPlayer& p)
    : player (p)
{
    setOpaque (true);
    frameEvents.reserve (64);
    startTimerHz (60);
}

DrumNotesView::~DrumNotesView()
{
    stopTimer();
}

void DrumNotesView::setLookAheadSeconds (double s)
{
    lookAheadSeconds = juce::jlimit (0.5, 8.0, s);
}

void DrumNotesView::resized() {}

void DrumNotesView::timerCallback()
{
    repaint();
}

int DrumNotesView::noteToLane (int n)
{
    return DrumLane::noteToLane (n);
}

const char* DrumNotesView::laneName (int lane)
{
    return DrumLane::laneName (lane);
}

juce::Colour DrumNotesView::laneColour (int lane)
{
    return DrumLane::laneColour (lane);
}

void DrumNotesView::paint (juce::Graphics& g)
{
    const float width  = (float) getWidth();
    const float height = (float) getHeight();

    g.fillAll (juce::Colour (0xff14141a));

    const float labelH    = 26.0f;
    const float hitH      = 4.0f;
    const float laneTop   = labelH;
    const float hitLineY  = height - labelH - hitH;
    const float laneH     = hitLineY - laneTop;
    const float laneWidth = width / (float) kNumLanes;

    // Lane backgrounds + separators
    for (int i = 0; i < kNumLanes; ++i)
    {
        const float x = i * laneWidth;
        g.setColour ((i % 2 == 0) ? juce::Colour (0xff1c1c26)
                                  : juce::Colour (0xff181820));
        g.fillRect (x, laneTop, laneWidth, laneH);

        if (i > 0)
        {
            g.setColour (juce::Colour (0xff2a2a38));
            g.drawLine (x, laneTop, x, hitLineY, 1.0f);
        }
    }

    // Falling notes
    const double now    = player.getPositionSeconds();
    const double startT = now - 0.15;              // brief tail so the hit registers visually
    const double endT   = now + lookAheadSeconds;

    frameEvents.clear();
    player.getDrumEventsInRange (startT, endT, frameEvents);

    for (const auto& e : frameEvents)
    {
        const int lane = noteToLane (e.note);
        if (lane < 0) continue;

        const double dt = e.timeSec - now;                            // future > 0
        const float  y  = (float) (hitLineY - (dt / lookAheadSeconds) * laneH);

        const float noteH = 16.0f;
        const float noteW = juce::jmax (24.0f, laneWidth - 16.0f);
        const float x     = lane * laneWidth + (laneWidth - noteW) * 0.5f;

        const juce::Rectangle<float> rect (x, y - noteH * 0.5f, noteW, noteH);

        const float velScale = 0.5f + 0.5f * juce::jlimit (0.0f, 1.0f, e.velocity);
        float       alpha    = 1.0f;
        if (dt < 0.0)
            alpha = juce::jmax (0.0f, 1.0f + (float) dt * 6.0f);      // fade out past-hit

        auto col = laneColour (lane).withMultipliedAlpha (velScale * alpha);

        const bool openHiHat = (lane == 0 && e.note == 46);
        if (openHiHat)
        {
            g.setColour (col);
            g.drawRoundedRectangle (rect.reduced (1.0f), 5.0f, 2.0f);
        }
        else
        {
            g.setColour (col);
            g.fillRoundedRectangle (rect, 5.0f);

            g.setColour (juce::Colours::white.withAlpha (0.25f * alpha));
            g.drawRoundedRectangle (rect, 5.0f, 1.0f);
        }
    }

    // Hit line
    g.setColour (juce::Colours::white.withAlpha (0.85f));
    g.fillRect (0.0f, hitLineY, width, hitH);

    // Lane labels (bottom)
    g.setFont (juce::Font (13.0f, juce::Font::bold));
    for (int i = 0; i < kNumLanes; ++i)
    {
        const float x = i * laneWidth;
        g.setColour (laneColour (i));
        g.drawText (laneName (i),
                    juce::Rectangle<float> (x, hitLineY + hitH, laneWidth, labelH),
                    juce::Justification::centred);
    }
}

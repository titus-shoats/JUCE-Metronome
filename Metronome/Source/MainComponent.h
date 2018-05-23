/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
	void prepareToPlay(int samplesPerBlockExpected,
		double sampleRate);

	void releaseResources();

	void setClock(double tempo, double phase);

	void getNextAudioBlock(AudioSourceChannelInfo const& bufferToFill);

	void advanceClock(int numSamples);

	void setupBar(int startingBeat, int beatsPerBar);

	void setMetroRunning(bool runMetro);

	void setMetroPlaying(bool playMetro);

	void setBPM(float beatsPerMinute);

	//String getBeatCount();

	//bool isCountedIn();


private:
	AudioSampleBuffer m_tick;
	double m_sampleRate;
	double bpm;
	double m_clockPhase;
	bool m_active;
	int m_lastSampleIndex;
	void createTick();
	bool shouldRun;
	bool shouldPlay;
	Synthesiser samplerSynth;                                                     //the synth to play the sound
	WavAudioFormat*	wavAudioFormat;											//we will add a waveAudioFormat to our audioFormatManager
	AudioFormatManager audioFormatManager;									//manages a list of formats we want to read (currently only .wav files)
	ScopedPointer<AudioFormatReader> metroHighReader;						//reader to load metronome .wav #1
	ScopedPointer<AudioFormatReader> metroLowReader;						//reader to load metronome .wav #2
	MidiMessageCollector midiCollector;
	int m_currentBeat, m_beatsPerBar;
	//bool* countIn;
    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
	String MainComponent::getBeatCount();

private:
    //==============================================================================
    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

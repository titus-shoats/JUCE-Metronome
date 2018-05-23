/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() :
m_clockPhase(0),
m_currentBeat(0),
m_beatsPerBar(4),
bpm(120),
shouldRun(true),
shouldPlay(true)
{


    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

	

    // specify the number of input and output channels that we want to open
    setAudioChannels (2, 2);
	
	audioFormatManager.registerFormat(wavAudioFormat = new WavAudioFormat(), true);

	//First we must load out metronome .wav files into a reader
	metroHighReader = audioFormatManager.createReaderFor(File("C:/Users/ace/Documents/Juce Projects/Metronome/Builds/VisualStudio2015/MetronomeHigh.wav"));
	metroLowReader = audioFormatManager.createReaderFor(File("C:/Users/ace/Documents/Juce Projects/Metronome/Builds/VisualStudio2015/MetronomeLow.wav"));

	//Define the midi note ranges each sound will be assigned to...
	//
	BigInteger highNotes;
	highNotes.setRange(0, 1, true);
	BigInteger lowNotes;
	lowNotes.setRange(2, 3, true);

	//then we must create SamplerSounds from the readers, and add them to our synth
	samplerSynth.addVoice(new SamplerVoice()); //this voice will be used to play the sound-- only one sound will play at a time so we only add one voice (monophonic)
	samplerSynth.addSound(new SamplerSound("metroHigh", *metroHighReader, highNotes, 0, 0, 0, 1.0));
	samplerSynth.addSound(new SamplerSound("metroLow", *metroLowReader, lowNotes, 2, 0, 0, 1.0));
	samplerSynth.setNoteStealingEnabled(false); //must turn note stealing off

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
	samplerSynth.clearSounds();
	samplerSynth.clearVoices();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()

	midiCollector.reset(sampleRate);
	samplerSynth.setCurrentPlaybackSampleRate(sampleRate);
	m_sampleRate = sampleRate;
	

	
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)

	int const numSamples = bufferToFill.numSamples;

	// the synth always adds its output so we must first clear (otherwise anything coming in will also output)
	bufferToFill.clearActiveBufferRegion();

	MidiBuffer midi;

	if (shouldRun)
	{
		double const samplesPerBeat = m_sampleRate * 60 / bpm;

		// Adjust phase so the beat is on or after the beginning of the output
		double beat = 0;
		if (m_clockPhase > 0)
			beat = 1 - m_clockPhase;
		else
			beat = 0 - m_clockPhase;

		// Set notes in midi buffer
		for (;; beat += 1)
		{
			// Calc beat pos
			int pos = static_cast <int> (beat * samplesPerBeat);
			if (pos < numSamples) {
				if (shouldPlay) {
					if (m_currentBeat == 0) {
						samplerSynth.noteOn(1, 60, 1.0);
						midi.addEvent(MidiMessage::noteOn(1, 0, 1.f), pos);
					}
					else {
						midi.addEvent(MidiMessage::noteOn(1, 2, 1.f), pos);
					}
				}

				//sendChangeMessage();

				//if we aren't yet counted in, check to see if enough beats have past to set counted in


				m_currentBeat++;
				m_currentBeat = m_currentBeat % m_beatsPerBar;
			}

			else
			{
				break;
			}
		}
		advanceClock(numSamples);
	}

	samplerSynth.renderNextBlock(*bufferToFill.buffer, midi, 0, bufferToFill.numSamples);
}

void MainComponent::setupBar(int startingBeat, int beatsPerBar) {

	m_clockPhase = 0;
	m_currentBeat = startingBeat;
	m_beatsPerBar = beatsPerBar;


}

void MainComponent::advanceClock(int numSamples) {
	double const samplesPerBeat = 44100 * 60 / bpm;

	jassert(m_clockPhase >= -.5 && m_clockPhase < .5);

	if (m_clockPhase < 0)
		m_clockPhase = m_clockPhase + 1;

	m_clockPhase = m_clockPhase + numSamples / samplesPerBeat;

	if (m_clockPhase >= .5)
		m_clockPhase -= 1;

	jassert(m_clockPhase >= -.5 && m_clockPhase < .5);
}



void MainComponent::setMetroRunning(bool runMetro) {
	shouldRun = runMetro;
	if (shouldRun)
	{
		setupBar(0, 4);
	}
}

void MainComponent::setMetroPlaying(bool playMetro) {
	shouldPlay = playMetro;
}



void MainComponent::setBPM(float beatsPerMinute) {
	bpm = beatsPerMinute;
}

String MainComponent::getBeatCount() {
	return String(m_currentBeat);
}


void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

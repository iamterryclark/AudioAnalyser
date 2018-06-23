//
//  AudioFeatureExtractor.cpp
//  AudioFeatureExtractor
//
//  Created by Terry Clark on 16/06/2018.
//

#include "AudioFeatureExtractor.hpp"

AudioFeatureExtractor::AudioFeatureExtractor(){
   
    
    //OSC
    sender.setup(HOST, PORT);
    
    //SoundStream Settings
    sampleRate = 44100; /* Sampling Rate */
    bufferSize = 256; /* Buffer Size. you have to fill this buffer with sound */
    fftSize = 512;
    

    
    lAudioIn            = new float[bufferSize];/* inputs */
    rAudioIn            = new float[bufferSize];
    
    memset(lAudioIn, 0, bufferSize * sizeof(float));
    memset(rAudioIn, 0, bufferSize * sizeof(float));
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    
    //Gui
    soundAnalysisParams.setName("Audio Analysis Params");
    soundAnalysisParams.add(nSpectrum.set("Bass Threshold", 0, 0, 1));
    soundAnalysisParams.add(nAverages.set("Mid Threshold", 0, 0, 1));
    soundAnalysisParams.add(nAverages.set("Treb Threshold", 0, 0, 1));
    
    fft.setup(fftSize, fftSize/2, fftSize/4);
    fftOct.setup(sampleRate, fftSize/2, 1);
    //fftOct.setup(float samplingRate, int nBandsInTheFFT, int nAveragesPerOctave);

    gui.setup(); // most of the time you don't need a name
    gui.add(soundAnalysisParams);
}

AudioFeatureExtractor::~AudioFeatureExtractor(){
    
}

void AudioFeatureExtractor::update(){
    
    
    
    if (fftOn) {
        ofxOscMessage m;
        m.setAddress("/fftBands");
        for (int i = 0; i < fftOct.nAverages; i++) {
            m.addFloatArg(fftOct.averages[i]);
        }
        sender.sendMessage(m);
    }
}

void AudioFeatureExtractor::draw(){
    
    ofSetColor(255, 255, 255);
    
    ofDrawBitmapString("FFT Octaves, Number of bands: " + ofToString(fftOct.nAverages), 50, 350);
    
    //FFT Octaves
    for(int i = 0; i < fftOct.nAverages; i++){
        ofDrawRectangle(10 + (i * 10), 400, 10, -(fftOct.averages
                                                  [i]) * 3);
        ofDrawBitmapString(i, 10 + (i * 10), 420);
    }
    
    gui.draw();
}

void AudioFeatureExtractor::audioRequested(float * output, int bufferSize, int nChannels){
    for (int i = 0; i < bufferSize; i++){
        wave = lAudioIn[i];
        
        //FFT
        if(fft.process(wave)){
            fftOct.calculate(fft.magnitudes);
        }

    }
}

void AudioFeatureExtractor::audioReceived(float * input, int bufferSize, int nChannels){

    //Go through audio buffer
    for (int i = 0; i < bufferSize; i++){
        lAudioIn[i] = input[ i * nChannels ];
        rAudioIn[i] = input[ i * nChannels + 1];
    }
}

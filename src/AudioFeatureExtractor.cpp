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
   
    fft.setup(fftSize, fftSize/2, fftSize/4);
    fftOct.setup(sampleRate, fftSize/2, 1);
    
    //Gui
    soundAnalysisParams.setName("Audio Analysis Params");
    
    for (int i = 0; i < fftOct.nAverages; i++){
        ofParameter<float> threshParam;
        thresBand.push_back(threshParam);
        activateBandTrig.push_back(0);
        soundAnalysisParams.add(thresBand[i].set("Threshold " + ofToString(i), 1, 0, 30));
    }
    
    gui.setup(); // most of the time you don't need a name
    gui.add(soundAnalysisParams);
}

AudioFeatureExtractor::~AudioFeatureExtractor(){
    
}

void AudioFeatureExtractor::update(){
    ofxOscMessage m;
    m.setAddress("/BandOctaves");
    
    for (int i = 0; i < fftOct.nAverages; i++) {
        m.addFloatArg(fftOct.averages[i]);
    }
    
    sender.sendMessage(m);
    
    for (int i = 0; i < fftOct.nAverages; i++){
        if (fftOct.averages[i] > thresBand[i]){
            activateBandTrig[i] = true;
        } else {
            activateBandTrig[i] = false;
        }
    }
}

void AudioFeatureExtractor::draw(){
    
    ofSetColor(255);
    
    ofDrawBitmapString("FFT Octaves, Num bands: " + ofToString(fftOct.nAverages), 50, 50);
    
    //FFT Octaves
    for(int i = 0; i < fftOct.nAverages; i++){
        float octAvg = ofMap(fftOct.averages[i], 0, 40, 0, 100);
        ofSetColor(255);
        ofDrawRectangle(50 + (i * 30), 100, 30, -octAvg);
        
        ofSetColor(255,0,0);
        ofDrawLine(50 + (i * 30), 100 - thresBand[i], 80 + (i * 30), 100 - thresBand[i]);
        
        if (activateBandTrig[i]){
            ofDrawCircle(75 + (i * 30), 150, 10, 10);
        }
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

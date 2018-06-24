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
    saParams.setName("Audio Analysis Params");
    oscValParams.setName("OSC Send Vals");
    oscTrigParams.setName("OSC Send Triggers");
    
    for (int i = 0; i < fftOct.nAverages; i++){
        activateBandTrig.push_back(0);
        
        ofParameter<float> floatParam;
        ofParameter<bool> boolParam1, boolParam2;
        
        thresBand.push_back(floatParam);
        oscSendValue.push_back(boolParam1);
        oscSendTrigger.push_back(boolParam2);
        
        saParams.add(thresBand[i].set("Threshold " + ofToString(i), 1, 0, 30));
        oscValParams.add(oscSendValue[i].set("Send Band Vals " + ofToString(i), false));
        oscTrigParams.add(oscSendTrigger[i].set("Send Band Trigger " + ofToString(i), false));
    }
    
    soundAnalysis.setup();
    oscVals.setup();
    oscTrigs.setup();
    
    soundAnalysis.setPosition(0, 200);
    oscVals.setPosition(soundAnalysis.getWidth(), 200);
    oscTrigs.setPosition(soundAnalysis.getWidth() + oscVals.getWidth(), 200);
    
    soundAnalysis.add(saParams);
    oscVals.add(oscValParams);
    oscTrigs.add(oscTrigParams);
}

AudioFeatureExtractor::~AudioFeatureExtractor(){
    
}

void AudioFeatureExtractor::update(){
    ofxOscMessage m, m2;
    m.setAddress("/FFTBandValues");
    m2.setAddress("/FFTBandTriggers");
    
    for (int i = 0; i < fftOct.nAverages; i++) {
        
        //Triggers
        if (fftOct.averages[i] > thresBand[i]){
            activateBandTrig[i] = true;
        } else {
            activateBandTrig[i] = false;
        }
        
        //Values
        if (oscSendValue[i]) {
            m.addFloatArg(fftOct.averages[i]);
            m2.addFloatArg(activateBandTrig[i]);
        }
    }
    
    sender.sendMessage(m);
    sender.sendMessage(m2);
}

void AudioFeatureExtractor::draw(){
    
    ofSetColor(255);
    
    ofDrawBitmapString("FFT Octaves, Num bands: " + ofToString(fftOct.nAverages), 50, 50);
    
    //FFT Octaves
    for(int i = 0; i < fftOct.nAverages; i++){
        ofSetColor(255);
        ofDrawRectangle(50 + (i * 30), 100, 30, -fftOct.averages[i]);
        ofDrawBitmapString(i, 60 + (i * 30), 130);
        
        ofSetColor(255,0,0);
        ofDrawLine(50 + (i * 30), 100 - thresBand[i], 80 + (i * 30), 100 - thresBand[i]);
        
        if (activateBandTrig[i]){
            ofDrawCircle(70 + (i * 30), 160, 10, 10);
        }
    }
    
    
    soundAnalysis.draw();
    oscVals.draw();
    oscTrigs.draw();
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

//
//  AudioFeatureExtractor.hpp
//  AudioFeatureExtractor
//
//  Created by Terry Clark on 16/06/2018.
//

#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxGui.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 8889

class AudioFeatureExtractor {
private:
    ofxOscSender sender;
    
    ofxPanel soundAnalysis, oscVals, oscTrigs;
    
    ofParameterGroup saParams, oscValParams, oscTrigParams;
    
    vector<ofParameter<float>> thresBand;
    vector<ofParameter<bool>> oscSendValue;
    vector<ofParameter<bool>> oscSendTrigger;
    
    vector<float> smoothedOct;
    vector<bool> activateBandTrig;
    
    float * lAudioIn;
    float * rAudioIn;
    
    double wave;
    
    ofSoundStream soundStream;
    
public:
    AudioFeatureExtractor();
    ~AudioFeatureExtractor();
    
    void update();
    void draw();
    void audioRequested(float * output, int bufferSize, int nChannels);
    void audioReceived(float * input, int bufferSize, int nChannels);

    maxiFFT fft;
    maxiFFTOctaveAnalyzer fftOct;
    
    int bufferSize;
    int sampleRate;
    int fftSize;
};

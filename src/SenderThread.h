#pragma once

#include "ofMain.h"

class SenderThread : public ofThread {
    public:
        SenderThread();
        ~SenderThread();
    
        // the thread function
        void threadedFunction();
    
        void setSerialInfo(ofSerialDeviceInfo deviceInfo);
    
        void setBrightness(float brightness);

        void sendData(const vector<ofFloatColor>& colors);
    
    private:
        unsigned int chunkSize; // Number of bytes to pass to write to the serial port at a time
        float delayTime;    // Milliseconds to wait between serial port writes
    
        float brightness;   // Output brightness scale, in percent
    
        ofSerialDeviceInfo deviceInfo;
        ofSerial serial;    // Serial object the BlinkyTape is connected through
        ofThreadChannel<vector<ofFloatColor> > dataChannel;
        void doTx(const vector<ofFloatColor>& colors);
};
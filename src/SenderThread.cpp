#include "senderThread.h"

SenderThread::SenderThread() {
    chunkSize = 200;
    delayTime = .005;
    
    // TODO: Wire this to the main thread
    brightness = 1;
}

SenderThread::~SenderThread() {
    dataChannel.close();
    waitForThread(true);
}

void SenderThread::setSerialInfo(ofSerialDeviceInfo deviceInfo) {
    if(isThreadRunning()) {
        return false;
    }
    
    this->deviceInfo = deviceInfo;
}

void SenderThread::threadedFunction() {
    
    serial.setup(deviceInfo.getDeviceID(), 115200);

    vector<ofFloatColor> colors;
    while(isThreadRunning() && serial.isInitialized()) {
        if(!dataChannel.receive(colors)) {
            break;
        }
        
        doTx(colors);
    }
    
    serial.close();
}

void SenderThread::setBrightness(float brightness) {
}

void SenderThread::sendData(const vector<ofFloatColor>& colors) {
    if(!isThreadRunning()) {
        return;
    }

    // Discard this frame if one is already pending
    if(!dataChannel.empty()) {
        return;
    }
    
    dataChannel.send(colors);
}

void SenderThread::doTx(const vector<ofFloatColor>& colors) {
    int bufferLength = (colors.size() * 3) + 1;
    unsigned char buffer[bufferLength];
    
    // TODO: We only support RGB for now.
    for(int index = 0; index < colors.size(); index++){
        float redCorrection = 1.8;
        float greenCorrection = 1.8;
        float blueCorrection = 2.1;
        
        buffer[(index * 3) + 0] = ofMap(pow(colors[index].r * brightness,redCorrection), 0, 1, 0, 254);
        buffer[(index * 3) + 1] = ofMap(pow(colors[index].g * brightness,greenCorrection), 0, 1, 0, 254);
        buffer[(index * 3) + 2] = ofMap(pow(colors[index].b * brightness,blueCorrection), 0, 1, 0, 254);
    }
    
    buffer[bufferLength-1] = 255;
    
    for (int chunk = 0; chunk * chunkSize < bufferLength; chunk++){
        int chunkLength = chunkSize;
        if ((chunk + 1) * chunkSize > bufferLength) {
            chunkLength = bufferLength - (chunk * chunkSize);
        }
        
        int bytesReceived = serial.writeBytes(buffer + chunk * chunkSize, chunkLength);
        
        if(chunkLength != bytesReceived){
            ofLogError("ofxBlinkyTape: sendData()")
                << "Error writing to BlinkyTape"
                << " expected=" << chunkLength << " got=" << bytesReceived;
            serial.close();
            return;
        }
        
        // TODO: a callback method here, to avoid blocking the main thread.
        float lastTime = ofGetElapsedTimef();
        while (lastTime + delayTime > ofGetElapsedTimef()){
        };
    }
}

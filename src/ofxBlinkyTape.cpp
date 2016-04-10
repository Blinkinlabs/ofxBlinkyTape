#include "ofxBlinkyTape.h"
#include "SenderThread.h"

bool sortDeviceInfo (ofxBlinkyTapeDeviceInfo i, ofxBlinkyTapeDeviceInfo j) {
    return (i.getDeviceName() < j.getDeviceName());
}

vector<ofxBlinkyTapeDeviceInfo> ofxBlinkyTape::getDeviceList(){
    ofSerial serial;
    vector<ofxBlinkyTapeDeviceInfo> deviceInfos = serial.getDeviceList();
    
    // TODO: Filter by VID/PID instead of device path for better cross-platform support
    for(int i = deviceInfos.size(); i--; i > -1){
        if(deviceInfos[i].getDevicePath().find("/dev/cu.usbmodem") == std::string::npos) {
            deviceInfos.erase(deviceInfos.begin() + i);
        }
    }
    
    std::sort (deviceInfos.begin(), deviceInfos.end(), sortDeviceInfo);
    
    return deviceInfos;
}

void ofxBlinkyTape::listDevices(){
    vector<ofxBlinkyTapeDeviceInfo> deviceInfos = getDeviceList();
    
    for(int i = 0; i < deviceInfos.size(); i++){
        ofLogNotice("ofxBlinkyTape") << "[" << i << "] = " << deviceInfos[i].getDeviceName();
    }
}

ofxBlinkyTape::ofxBlinkyTape(){
    senderThread = new SenderThread();
    
    brightness = 1;
}

ofxBlinkyTape::~ofxBlinkyTape(){
    close();
    
    delete senderThread;
}

bool ofxBlinkyTape::open(unsigned int index){
    vector<ofxBlinkyTapeDeviceInfo> deviceInfos = getDeviceList();
    
    if(index >= deviceInfos.size()){
        return false;
    }
    
    return open(deviceInfos[index]);
}

bool ofxBlinkyTape::open(ofxBlinkyTapeDeviceInfo deviceInfo){
    if(isConnected()) {
        return false;
    }
    
    senderThread->setSerialInfo(deviceInfo);
    senderThread->startThread(true, false);
    
    // TODO: test connection?
    return true;
}

void ofxBlinkyTape::close(){
    if(senderThread->isThreadRunning()) {
        senderThread->stopThread();
    }
}

bool ofxBlinkyTape::isConnected() const {
    return senderThread->isThreadRunning();
}


float ofxBlinkyTape::getBrightness() const{
    return brightness;
}

void ofxBlinkyTape::setBrightness(float brightness){
    this->brightness = brightness;
}

void ofxBlinkyTape::sendData(const vector<ofFloatColor>& colors){    
    senderThread->sendData(colors);
}
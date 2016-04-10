#pragma once

#include "ofMain.h"

typedef ofSerialDeviceInfo ofxBlinkyTapeDeviceInfo;

class SenderThread;

class ofxBlinkyTape {
    public:
        /// \name List Devices
        /// \{
        
        /// \brief This lists all the available BlinkyTape devices to the console or
        /// standard output
        ///
        /// On OSX and Linux this will return all the devices listed in /dev/cu.usb*,
        /// so you might want to compare it against a list of devices that you're
        /// expecting if you want to use it to dynamically connect to a
        /// device.
        ///
        /// TODO: Filter based on VID/PID to avoid conflicts
        static void listDevices();
    
        /// \brief Returns a vector of ofSerialDeviceInfo instances with the
        /// devicePath, deviceName, deviceID set
        static vector<ofSerialDeviceInfo> getDeviceList();

        /// \}
        /// \name Constructor and Destructor
        /// \{
        
        /// Initializes the BlinkyTape object, but doesn't actually open the
        /// connection to any devices. You'll need to use the open() method
        /// to do that
        ofxBlinkyTape();
        virtual ~ofxBlinkyTape();
    
        /// \}
        /// \name List Devices
        /// \{
    
        /// \brief Opens the BlinkyTape based on the order in which it is listed
        ///
        /// set the deviceIndex to choose a BlinkyTape, see listDevices()
        /// if you don't set the index, the first available BlinkyTape will be used
        ///
        /// The code bellow would open the first BlinkyTape found by the system:
        /// ~~~~{.cpp}
        /// ofxBlinkyTape myBlinkyTape;
        /// myBlinkyTape.open(0);
        /// ~~~~
        bool open(unsigned int deviceIndex = 0);
    
        /// \brief Opens the BlinkyTape with the given name
        ///
        /// On OSX and Linux, it might look like:
        /// ~~~~{.cpp}
        /// ofxBlinkyTape myBlinkyTape;
        /// myBlinkyTape.open("cu.usbmodem1D1171");
        /// ~~~~
        ///
        /// On Windows, like:
        /// ~~~~{.cpp}
        /// ofxBlinkyTape myBlinkyTape;
        /// myBlinkyTape.open("COM4");
        /// ~~~~
        bool open(ofxBlinkyTapeDeviceInfo deviceInfo);
    
        /// \brief Check if the BlinkyTape is connected
        bool isConnected() const;

        /// \brief Closes the connection to the BlinkyTape
        void close();
    
        /// \}
        /// \name Send Data
        /// \{
        
        /// \brief This writes a frame of pixels to the BlinkyTape
        ///
        /// ~~~~{.cpp}
        /// vector<ofFloatColor> colors;
        /// colors.push_back(ofColor::red);
        /// colors.push_back(ofColor::green);
        /// colors.push_back(ofColor::blue);
        /// myBlinkyTape.sendData(colors);
        /// ~~~~
        void sendData(const vector<ofFloatColor>& colors);
    
        /// \}
        /// \name Configuration
        /// \{
        
        /// \brief Output brightness scale, in percent
        float getBrightness() const;
        void setBrightness(float brightness);
        
    private:
        float brightness;   // Output brightness scale, in percent
    
        SenderThread* senderThread;
};
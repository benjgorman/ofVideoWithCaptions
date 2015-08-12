/**
 * Video Renderer Demo
 * ofxTimeline
 *
 * Shows how to use ofxTimeline to create video effects with a shader
 * that can be rendered to an image sequence
 */

#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"
#include "ofxVideoRecorder.h"

class testApp : public ofBaseApp{

  public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);


    void loadVideo(string videoPath);
    
    bool loaded;
    bool rendering;
    int currentRenderFrame;
    
    string renderFolder;
    ofTrueTypeFont font;
    
    //saves between loads
    ofxXmlSettings settings;
    
	ofxTimeline timeline;
    ofFbo frameBuffer;

    ofRectangle contentRectangle; //matched to video size
    ofRectangle outputRectangle;
    ofRectangle loadVideoButton;
    
    bool getCaptionStatus();
    
    void setCaptionStatus(bool isActive);
    
    void receivedBang(ofxTLBangEventArgs& bang);
    
    ofLight light;
    
    ////////////////////////////////
    ofVideoGrabber      vidGrabber;
    ofxVideoRecorder    vidRecorder;
    ofSoundStream       soundStream;
    bool bRecording;
    int sampleRate;
    int channels;
    string fileName;
    string fileExt;
    
    ofFbo recordFbo;
    ofPixels recordPixels;
    
    ofQTKitGrabber kit;
    
    
    
};

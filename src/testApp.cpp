/**
 * Video Renderer Demo
 * ofxTimeline
 *
 * Shows how to use ofxTimeline to create video effects with a shader
 * that can be rendered to an image sequence
 */

#include "testApp.h"
#define BUTTON_HEIGHT 30

bool recording=false;
bool timelineShown = true;

bool isCaptionActive;
std::string caption;
std::string old_caption;
std::string current_word;
int caption_width = 900;
int caption_number;

ofImage image;

//--------------------------------------------------------------
void testApp::setup()
{
    isCaptionActive = false;

	ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetFrameRate(60);
    ofSetFullscreen(false);
    
    vidRecorder.setFfmpegLocation(ofFilePath::getAbsolutePath("ffmpeg"));
    // use this is you have ffmpeg installed in your data folder
    
    fileName = "testMovie";
    fileExt = ".mov"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats
    
    // override the default codecs if you like
    // run 'ffmpeg -codecs' to find out what your implementation supports (or -formats on some older versions)
    vidRecorder.setVideoCodec("mpeg4");
    vidRecorder.setVideoBitrate("2500k");
    vidRecorder.setAudioCodec("mp3");
    vidRecorder.setAudioBitrate("192k");
    
    bRecording = true;
    ofEnableAlphaBlending();
    
    //set the timeline up with some default values
	timeline.setup();
    timeline.setFrameRate(60);
    //set big initial duration, longer than the video needs to be
	timeline.setDurationInFrames(20000);
	timeline.setLoopType(OF_LOOP_NONE);

    timeline.addFlags("caption");
    ofAddListener(timeline.events().bangFired, this, &testApp::receivedBang);
    
    loaded = false;

    //load the last video 
	if(settings.loadFile("settings.xml"))
    {
        string videoPath = settings.getValue("videoPath", "");
        if(videoPath != "")
        {
            loadVideo(videoPath);
        }
    }
    
    ofxTLVideoTrack* videoTrack = timeline.getVideoTrack("Video");
    
    //load our display font
    font.loadFont("GUI/helvetica.ttf", 25, true, true);
    //font.setGlobalDpi(120);
    
	font.setLineHeight(34.0f);
	font.setLetterSpacing(1.035);

}

int x_position = 0;

//--------------------------------------------------------------
void testApp::receivedBang(ofxTLBangEventArgs& bang)
{
    if(bang.track->getName() == "caption")
    {
        isCaptionActive = true;
        //sets the current word to be the flag
        current_word = bang.flag;
    }
}

//--------------------------------------------------------------
void setCaptionStatus(bool status)
{
    isCaptionActive = status;
}

//--------------------------------------------------------------
bool getCaptionStatus()
{
    if (isCaptionActive)
    {
        return true;
    }
    else
    {
        return false;
    }
}

ofImage img;

//--------------------------------------------------------------
void testApp::update()
{
    //update views
    if(!loaded)
    {
        contentRectangle = ofRectangle(0,0, 13.5, 9);
    }
    //calculate a the view for the movie, scaled into the center between the timeline and the buttons
    float availableHeight = ofGetHeight();// - timeline.getBottomLeft().y - BUTTON_HEIGHT;
    
    if(ofGetWidth() / availableHeight > contentRectangle.width/contentRectangle.height)
    {
        outputRectangle.height = availableHeight;
        outputRectangle.width = contentRectangle.width * availableHeight / contentRectangle.height;
    }
    else
    {
        outputRectangle.width  = ofGetWidth();
        outputRectangle.height = contentRectangle.height * ofGetWidth()/contentRectangle.width;
    }
    outputRectangle.x =  ofGetWidth()/2 - outputRectangle.width/2;
    
    outputRectangle.y =   ofGetHeight()/2 - outputRectangle.height/2;
    
    if (recording==true)
    {
        img.grabScreen(0, 0, ofGetWidth(),ofGetHeight());
        
        if(bRecording){
            vidRecorder.addFrame(img.getPixelsRef());
        }
        
    }
    
}

int length;
int lines = 0;
std::vector<string> totalLines;


//--------------------------------------------------------------
void testApp::draw()
{
    
	ofBackground(.15*255); //pro apps background color
    ofPushMatrix();
    
    ofSetColor(255, 255, 255);

    if(loaded)
    {
        timeline.getVideoPlayer("Video")->draw(outputRectangle);
        
        ofTranslate(ofGetWidth()/2-450, ofGetHeight()/2+250);
        ofPushStyle();
        
        if (isCaptionActive)
        {
            ofSetColor(0,0,0);
            
            
            for (int i=0; i<=lines; i++)
            {
                ofRectangle myRect(0, 0, caption_width, 40);
            
            
            //ofRect(myRect);
                
            
                myRect.setPosition(0, i*40);
                
                
                ofRect(myRect);
            }
            
            ofSetColor(255,255,255);
            
            
            // if the caption is active loop through the number of lines we have and add it to the current line number
            
            if (totalLines.size()==0)
            {
                totalLines.push_back("");
            }
            
            
            if (totalLines[lines]==old_caption + " " + current_word || totalLines[lines]== current_word)
            {
                //font.drawString(caption, ofGetWidth()/2-450, ofGetHeight()/2+30);
                for (int i=0; i<=lines; i++)
                {
                    font.drawString(totalLines[i], 0, 30+(i*30));
                    
                }
            }
            else
            {
                
                if (length > 36)
                {
                    length = 0;
                    lines++;
                    totalLines.push_back("");
                    old_caption = totalLines[lines];
                    totalLines[lines] = " " + current_word;
                    
                    for (int i=0; i<=lines; i++)
                    {

                        font.drawString(totalLines[i], 0, 30+(i*30));
                        
                    }
                    
                    length += current_word.length();
                }
                else
                {
                    
//                    old_caption = totalLines[lines];
//                    caption = caption + " " + current_word;
//                    font.drawString(caption, ofGetWidth()/2-450, ofGetHeight()/2+30);
//                    length += current_word.length();
                    
                    old_caption = totalLines[lines];
                    totalLines[lines] = totalLines[lines] + " " + current_word;
                    
                    for (int i=0; i<=lines; i++)
                    {
                        font.drawString(totalLines[i], 0, 30+(i*30));
                       
                    }
                    length += current_word.length();
                    
                }

                
            }
            
            
            
        }
        
        ofPopMatrix();
        ofPopStyle();
    }
    
    if (timelineShown ==true)
    {
        timeline.draw();
    }
}

//--------------------------------------------------------------
void testApp::loadVideo(string videoPath)
{
    ofxTLVideoTrack* videoTrack = timeline.getVideoTrack("Video");

    if(videoTrack == NULL)
    {
	    videoTrack = timeline.addVideoTrack("Video", videoPath);
        loaded = (videoTrack != NULL);
    }
    else
    {
        loaded = videoTrack->load(videoPath);
    }

    if(loaded)
    {
        contentRectangle = ofRectangle(0,0, videoTrack->getPlayer()->getWidth(), videoTrack->getPlayer()->getHeight());
        frameBuffer.allocate(contentRectangle.width, contentRectangle.height, GL_RGB);
        
        //timeline.clear();
        //At the moment with video and audio tracks
        //ofxTimeline only works correctly if the duration of the track == the duration of the timeline
        //plan is to be able to fix this but for now...
        timeline.setFrameRate(videoTrack->getPlayer()->getTotalNumFrames()/videoTrack->getPlayer()->getDuration());
        timeline.setDurationInFrames(videoTrack->getPlayer()->getTotalNumFrames());
        timeline.setTimecontrolTrack(videoTrack); //video playback will control the time
		timeline.bringTrackToTop(videoTrack);
    }
    else
    {
        videoPath = "";
    }
    settings.setValue("videoPath", videoPath);
    settings.saveFile();        
}

void testApp::keyPressed(int key)
{
    
    if (key == '}')
    {
        if (timelineShown)
        {
            timelineShown = false;
        }
        else
        {
            timelineShown = true;
        }
    }
    
    if(key == 'r')
    {
        if (recording==false)
        {
            recording = true;
            if(!vidRecorder.isInitialized())
            {
                
                
                //vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, ofGetWidth(), ofGetHeight(), 30, sampleRate, channels);
                
                vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, ofGetWidth(), ofGetHeight(), 60); // no audio
                //vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, 0,0,0, sampleRate, channels); // no video
                //          vidRecorder.setupCustomOutput(vidGrabber.getWidth(), vidGrabber.getHeight(), 30, sampleRate, channels, "-vcodec mpeg4 -b 1600k -acodec mp2 -ab 128k -f mpegts udp://localhost:1234"); // for custom ffmpeg output string (streaming, etc)
                
                vidRecorder.start();
            }
        }
        else
        {
            recording = false;
            vidRecorder.close();
            
            
        }
        
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
    if(dragInfo.files.size() > 0)
    {
		loadVideo(dragInfo.files[0]);
	}
}
	

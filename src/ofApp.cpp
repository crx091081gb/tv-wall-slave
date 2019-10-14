#include "ofApp.h"

//This app is a demo of the ability to play multiple files with the Non-Texture Player
//It requires multiple video files to be in /home/pi/videos/current
//There is a bit of glitching while the files switch

//This also demonstrates the ofxOMXPlayerListener pattern available

//If your app extends ofxOMXPlayerListener you will receive an event when the video ends or loops



//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    
	ofBackground(ofColor::black);
	consoleListener.setup(this);	
	
	//this will let us just grab a video without recompiling
    string directoryPath = ofToDataPath("data/movies", true);
	ofDirectory currentVideoDirectory(directoryPath);
	if (currentVideoDirectory.exists()) 
	{
		//currentVideoDirectory.allowExt("mp4");
		currentVideoDirectory.listDir();
		currentVideoDirectory.sort();
		files = currentVideoDirectory.getFiles();
		if (files.size()>0) 
		{
			videoCounter = 0;
			settings.videoPath = files[videoCounter].path();
			settings.useHDMIForAudio = false;	//default true
			settings.enableLooping = false;		//default true
			settings.enableTexture = false;		//default true
			settings.listener = this;			//this app extends ofxOMXPlayerListener so it will receive events ;
			omxPlayer.setup(settings);
		}		
	}else
    {
        ofLog() << directoryPath << "DOES NOT EXIST - does bin/data exist?";
    }

    doLoadNextMovie = false;

    // send on outport
	ofLog() << "sending OSC messages on port " << OUTPORT;
	sender.setup(BROADCAST, OUTPORT);
	// listen on the given port
	ofLog() << "listening for osc messages on port " << INPORT;
    receiver.setup(INPORT);

    if( wallSettings.loadFile("wallSettings.xml") ){
		ofLog() << "loaded wall settings";
	}else{
		ofLog() << "unable to load wall settings";
	}

	if( TVSettings.loadFile("TVSettings.xml") ){
		ofLog() << "loaded TV settings";
	}else{
		ofLog() << "unable to load TV settings";
	}

	tvNumber = TVSettings.getValue("TV", "");

	ofLog() << "TV number " << tvNumber;

	wallWidth = wallSettings.getValue("Wall:Width", 300);
	wallHeight = wallSettings.getValue("Wall:Height", 300);

	string queryWidth = tvNumber;
	queryWidth += ":Width";

	string queryHeight = tvNumber;
	queryHeight += ":Height";

	string queryOffsetX = tvNumber;
	queryOffsetX += ":OffsetX";

	string queryOffsetY = tvNumber;
	queryOffsetY += ":OffsetY";

	screenWidth = wallSettings.getValue(queryWidth, 100);
	screenHeight = wallSettings.getValue(queryHeight, 100);
	offsetX = wallSettings.getValue(queryOffsetX, 0);
	offsetX = wallSettings.getValue(queryOffsetY, 0);

	ofLog() << "Wall Width = " << wallWidth;
	ofLog() << "Wall Height = " << wallHeight;
	ofLog() << "Screen OffsetX = " << offsetX;
	ofLog() << "Screen OffsetY = " << offsetX;

    ofHideCursor();
}

//--------------------------------------------------------------
void ofApp::update()
{
	// check for waiting messages
	while(receiver.hasWaitingMessages()){

		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);

		// check for mouse moved message
		if(m.getAddress() == "/video"){
			int i = m.getArgAsInt(0);
			loadMovie(i);
		}
	}
	if (doLoadNextMovie) 
	{
		ofLogVerbose(__func__) << "doing reload";
		
		if(omxPlayer.isTextureEnabled())
		{
			//clear the texture if you want
			//omxPlayer.getTextureReference().clear();
		}
		//with the texture based player this must be done here - especially if the videos are different resolutions
		loadNextMovie();
	}
		
	
}


//--------------------------------------------------------------
void ofApp::draw(){
	
	//ofBackgroundGradient(ofColor::red, ofColor::black, OF_GRADIENT_CIRCULAR);
	
	//if(!omxPlayer.isTextureEnabled()) return;
	
	//omxPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
	int fileWidth = omxPlayer.getWidth();
	int fileHeight = omxPlayer.getHeight();
	//void drawCropped(float cropX, float cropY, float cropWidth, float cropHeight, float drawX, float drawY, float drawWidth, float drawHeight);
	float cropX = (float)fileWidth*((float)offsetX/(float)wallWidth);
	float cropY = (float)fileHeight*((float)offsetY/(float)wallHeight);
	float cropWidth = (float)fileWidth*((float)screenWidth/(float)wallWidth);
	float cropHeight = (float)fileHeight*((float)screenHeight/(float)wallHeight);
	//ofLog() << "fileWidth = " << fileWidth << " fileHeight = " << fileHeight;
	//ofLog() << "cropX = " << cropX << " cropY" << cropY << " cropWidth " << cropWidth << " cropHeight " << cropHeight;;
	omxPlayer.drawCropped(cropX, cropY, cropX + cropWidth, cropY + cropHeight, 0, 0, ofGetWidth(), ofGetHeight());
	//omxPlayer.drawCropped(960, 0, 1920, 512, 0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

	ofLogVerbose(__func__) << "key: " << key;
	switch (key) 
	{
		case 'n':
		{
			doLoadNextMovie = true;
			break;
		}
	}
}

void ofApp::onVideoEnd(ofxOMXPlayer* player)
{
    // ofLog() << "onVideoEnd: " << player->isLoopingEnabled();
    doLoadNextMovie = true;

}

void ofApp::onVideoLoop(ofxOMXPlayer* player)
{
    
}


void ofApp::onCharacterReceived(KeyListenerEventData& e)
{
	keyPressed((int)e.character);
}

void ofApp::loadNextMovie()
{
    if(videoCounter+1<files.size())
    {
        videoCounter++;
    }else
    {
        videoCounter = 0;
    }
    
    loadMovie(videoCounter);
    
    doLoadNextMovie = false;

    // ofxOscMessage m;
    // m.setAddress("/video");
    // m.addIntArg(videoCounter);
    // sender.sendMessage(m, false);
}

void ofApp::loadMovie(int i) {
    if (i < files.size()){
        ofLog() << "LOADING MOVIE" << files[videoCounter].path();
        omxPlayer.loadMovie(files[i].path());
    }

}



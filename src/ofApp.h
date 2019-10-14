#pragma once

#include "ofMain.h"
#include "ofxOMXPlayer.h"
#include "TerminalListener.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

#define INPORT 7777
#define OUTPORT 7778
#define BROADCAST "192.168.0.255"

class ofApp : public ofBaseApp, public ofxOMXPlayerListener, public KeyListener{

	public:

		void setup();
		void update();
		void draw();
	
	
		void keyPressed(int key);
		ofxOMXPlayer omxPlayer;
	
		void onVideoEnd(ofxOMXPlayer* player);
        void onVideoLoop(ofxOMXPlayer* player);

		
		vector<ofFile> files;
		int videoCounter;
	
		void onCharacterReceived(KeyListenerEventData& e);
		TerminalListener consoleListener;
		ofxOMXPlayerSettings settings;
	
		void loadNextMovie();
		void loadMovie(int i);

		ofxOscSender sender;
		ofxOscReceiver receiver;

		bool doLoadNextMovie;

		ofxXmlSettings TVSettings;

		std::string tvNumber;

		ofxXmlSettings wallSettings;
		
		int wallWidth;
		int wallHeight;
		int offsetX;
		int offsetY;
		int screenWidth;
		int screenHeight;

};


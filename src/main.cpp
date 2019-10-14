#include "ofMain.h"
#include "ofApp.h"

int main()
{
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetupOpenGL(720,576,OF_WINDOW);	
	//ofSetupOpenGL(1280, 1024, OF_WINDOW);
	ofRunApp( new ofApp());
}
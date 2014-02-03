#include "ofApp.h"


//------------------------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(1);
}

//------------------------------------------------------------------------------
void ofApp::update()
{
    ofFuture< Destructo > f0 = intString.execute(123);

    results.push_back(f0);


    std::vector<ofFuture<Destructo> >::iterator iter = results.begin();

    while(iter != results.end())
    {
        if((*iter).failed())
        {
            string s = (*iter).error();
            Exception* exc = (*iter).exception();
            cout << "ERROR: " << s << "|||" << exc->what() << endl;
            iter = results.erase(iter);
        }
        else if((*iter).available())
        {
            Destructo s = (*iter).data();
            cout << *s.getStringPtr() << endl;
            iter = results.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    ofFuture<std::string> f = voidString.execute();
    voidResults.push_back(f);

    std::vector<ofFuture<std::string> >::iterator iter2 = voidResults.begin();

    while(iter2 != voidResults.end())
    {
        if((*iter2).failed())
        {
            std::string s = (*iter2).error();
            Exception* exc = (*iter2).exception();
            cout << "ERROR: " << s << "|||" << exc->what() << endl;
            iter2 = voidResults.erase(iter2);
        }
        else if((*iter2).available())
        {
            std::string s = (*iter2).data();
            cout << s << endl;
            iter2 = voidResults.erase(iter2);
        }
        else
        {
            ++iter2;
        }
    }

}

//------------------------------------------------------------------------------
void ofApp::draw()
{
}

//------------------------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//------------------------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//------------------------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//------------------------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//------------------------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//------------------------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//------------------------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//------------------------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//------------------------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

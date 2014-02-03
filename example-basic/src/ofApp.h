
#pragma once


#include "ofMain.h"
#include "ofFuture.h"


class Destructo {
public:
    Destructo(const std::string& text): s(new std::string(text))
    {
    }

    Destructo(const Destructo& other ): s(new std::string(*other.s))
    {
    }

    virtual ~Destructo()
    {
        delete s;
    }

    Destructo& operator= (Destructo const& other)
    {
        Destructo temp(other);
        std::string *temp_s = temp.s;
        temp.s = s;
        s = temp_s;
        return *this;
    }

    std::string* getStringPtr() {
        return s;
    }

private:
    Destructo()
    {
    }

    std::string* s;
};

class ofFutureDestructoHandler: public ofBaseFutureHandler<Destructo,int>
{
    Destructo executeImpl(const int& input)
    {
        Poco::Thread::sleep(ofRandom(0,2000));
        Destructo dd(ofToString(input) + "_PROCESSED");
        return dd;
    }
};

class ofFutureNumberHandler: public ofBaseFutureHandler<string,void>
{
    std::string executeImpl(void)
    {
        float random = ofRandom(0,2000);
        float j = 0;

        for(int i = 0; i < 1000000; i++)
        {
            j += ofRandom(i);
        }

        Poco::Thread::sleep(random);

        return ofToString(j) + "XXX";
    }
};


class ofApp: public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofFutureDestructoHandler intString;
    ofFutureNumberHandler voidString;

    std::vector< ofFuture<Destructo> > results;
    std::vector< ofFuture<string> > voidResults;

};

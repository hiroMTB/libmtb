#pragma once

#include "cinder/audio/Node.h"

typedef std::shared_ptr<class  AudioOutput>	 AudioOutputRef;

using namespace ci;
using namespace ci::app;


class AudioOutput : public ci::audio::Node{
    
public:
    
     AudioOutput( const Format &format = Format() ) : Node( format ){}

    
    virtual void initialize() override{
        
    };
    
    virtual void process( ci::audio::Buffer * buffer )	override{
        
        const size_t numFrames = buffer->getNumFrames();
        float *data = buffer->getData();
        
        for( size_t i = 0; i < numFrames; i++ ) {
            for( size_t ch = 0; ch < buffer->getNumChannels(); ch++ ){
                
                const float * chData = mBuffer->getChannel( ch );
                data[ch*numFrames + i] = chData[ mAudioPos + i];
            }
        }
        
    };
    
    void setBuffer( ci::audio::Buffer *aBuffer ){
        mBuffer = aBuffer;
    }
    
    void setPosition( int aAudioPos ){
        mAudioPos = aAudioPos;
    }

    ci::audio::Buffer * mBuffer;

    int mAudioPos;
};

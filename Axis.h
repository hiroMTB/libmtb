#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Utilities.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

/*
 
                 Axis
 
       start -------------- head ---- end
 
 
 */

namespace n5{

class Axis{
    
public:
    
    void update( int frame ){
        past_pos = pos;
        past_speed = speed;
        past_accel = accel;
        
        pos   = animPos[frame];
        speed = abs( pos.x - past_pos.x);
        accel = abs( speed - past_speed );
        power = animPower[frame];
    }
    

    void check(){
        if( pos.x<left.x || (left.x+length)<pos.x ){
            cout << "Axis error : position over" << endl;
        }
        
        if( abs(speed) > speed_max ){
            cout << "Axis error : speed over" << endl;
        }
        
        if( abs(accel) > accel_max ){
            cout << "Axis error : Accel over" << endl;
        }
    }
    
    const int    fps       = 25;
    const double mmPerPix  = 18000.0 / 4320.0;           // 4.1666mm per pixel
    const double speed_max = 9000.0 / mmPerPix / fps;    //  9m/s,  86.4 pix/frame
    const double accel_max = 40000.0 / mmPerPix / fps;   // 40m/s2, 384 pix/fra,e
    const double length    = 5000.0 / mmPerPix;          // 1200px
    
    Vec2f pos, past_pos;     // world coord, pix
    Vec2f left;              // world coord, pix, left position of rail
    
    float speed, past_speed; // pix, x speed
    float accel, past_accel; // pix, x accel
    
    unsigned int power;  // DMX value, 0-255
    
    vector<Vec2f> animPos;
    vector<unsigned int> animPower;

};


}
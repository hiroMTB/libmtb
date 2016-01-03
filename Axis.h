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
    
    
    Axis( float _start, float _height )
    :
    start(_start),
    height(_height),
    end(start + length),
    center(start - length*0.5)
    {
        cout << "Axis : st:" << start << " - end:" << end << " L:" << length << ", sp:" << speed_max << ", acl = " << accel_max << endl;
    }

    void draw(){
        glColor3f( 0,1,0 );
        gl::drawSolidRect( Rectf( start, height-3, end, height+2) );
    }
 
    void move( float _dest ){
        
        dest = _dest;
        pos_p = pos;
        speed_p = speed;
        accel_p = accel;

        // motion pattern calculation
        float L = dest - pos;
        float Vmax = sqrt( L*accel_max );
        bool triangle = Vmax <= speed_max;
        float Vr = triangle ? Vmax : speed_max;
        float Ta = Vr / accel_max;
        float La = accel_max * Ta * Ta / 2;
        float Tr = triangle ? 2*Ta : Ta+L/Vr;
        
        accel = speed - speed_p;
        speed = pos - pos_p;
        pos = 123;
    }
    
    void check(){
        
        if( pos<0 || length<pos ){
            cout << "Axis error : position over" << endl;
        }
            
        if( abs(speed) > speed_max ){
            cout << "Axis error : speed over" << endl;
        }
        
        if( abs(accel) > accel_max ){
            cout << "Axis error : Accel over" << endl;
        }
    }
    
    // pixel
    const int fps         = 25;
    const float mmPerPix  = 18000.0f / 4320.0;
    const float length    = 5000.0 / mmPerPix;          //  5m     1200 pix
    const float speed_max = 9000.0 / mmPerPix / fps;    //  9m/s
    const float accel_max = 40000.0 / mmPerPix / fps;   // 40m/s2
    const float start;
    const float end;
    const float center;
    const float height;

    bool  isMoving  = false;
    float speed     = 0;
    float accel     = 0;
    float pos       = 0;
    float speed_p   = 0;
    float accel_p   = 0;
    float pos_p     = 0;
    float dest;
    
};


    vector<vector<Axis>> axes{
        { Axis(480,192), Axis(840,576), Axis(120,960), Axis(840,1344), Axis(480,1728) },
        { Axis(2640,192), Axis(2280,576), Axis(3000,960), Axis(2280,1344), Axis(2640,1728) }
    };

    void drawAxes(){
        for( auto a: axes )
            for( auto aa : a )
                aa.draw();
    }
}
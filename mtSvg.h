#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/svg/Svg.h"
#include "cinder/svg/SvgGl.h"

using namespace ci;
using namespace ci::app;

class mtSvg{
    
public:
    mtSvg(){};

    void load( fs::path path ){
        if( fs::exists(path) ){
            docref = svg::Doc::create( path );
            cout << "OK : SVG file load : " << path << endl;
        }else{
            cout << "Error : SVG file not exist : " << path << endl;
        }
    }
    
    void draw(){
        gl::draw( *docref );
    }
  
    svg::DocRef docref;
    
};
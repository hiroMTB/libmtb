#pragma once

#include <fstream>
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"
#include "cinder/Utilities.h"
#include "cinder/Xml.h"

using namespace ci;
using namespace ci::app;
using namespace std;

typedef std::function<float (float)> EaseFunc;

namespace mt {
    
    fs::path assetDir;
    
    gl::VboMesh::Layout getVboLayout(){
        gl::VboMesh::Layout layout;
        layout.setDynamicColorsRGBA();
        layout.setDynamicPositions();
        layout.setStaticIndices();
        return layout;
    }
    
    long getSeed(){
        time_t curr;
        tm local;
        time(&curr);
        local =*(localtime(&curr));
        //return local.tm_gmtoff;
        return local.tm_sec + local.tm_min;
    }
    
    string getTimeStamp(){        
        time_t curr;
        tm local;
        time(&curr);
        local =*(localtime(&curr));
        int month = local.tm_mon;
        int day = local.tm_mday;
        int hours = local.tm_hour;
        int min = local.tm_min;
        int sec = local.tm_sec;
        
        char stamp[16];
        sprintf(stamp, "%02d%02d_%02d%02d_%02d", month+1, day, hours, min, sec);
        return string(stamp);
    }
    
    string getTimeStampU(){
        return toString( time(NULL) );
    }
    
    void renderScreen( fs::path path, int frame ){
        string frame_name = "f_" + toString( frame ) + ".png";
        writeImage( path/frame_name, copyWindowSurface() );
        cout << "Render Image : " << frame << endl;
    }
    
    fs::path getAssetPath(){
        if( assetDir.string()=="" ){
            fs::path app = getAppPath();
            XmlTree xml( loadFile(expandPath(app/"../../../../../_project_settings.xml")) );
            XmlTree ast = xml.getChild("project_settings").getChild("assetDir");
            string st = ast.getValue<string>("error");
            if( st == "error"){
                //assetDir = expandPath("../../../../../assets");
                //return fs::path("/Volumes/StudioRK_node/n5/assets");
                printf( "Cant find assetDir, check you have _project_settings.xml\n" );
            }else{
                assetDir = st;
                printf( "assetDir \"%s\"\n", st.c_str() );
            }
        }
        return assetDir;
    }
    
    fs::path getRenderPath( string subdir_name="" ){
        fs::path app = getAppPath();
        if(subdir_name!="")
            return expandPath(app/"../../../../../_rtmp") / getTimeStamp() / subdir_name ;
        else
            return expandPath(app/"../../../../../_rtmp") / getTimeStamp();
    }
        
    void saveString( string str, fs::path path ){
        ofstream ost( path.string() );
        ost << str;
        ost.close();
    }
    
    float distanceToLine( const Ray &ray, const Vec3f &point){
        return cross(ray.getDirection(), point - ray.getOrigin()).length();
    }
    
    Vec3f dirToLine( const Vec3f &p0, const Vec3f &p1, const Vec3f &p2 ){
        Vec3f v = p2-p1;
        Vec3f w = p0-p1;
        double b = v.dot(w) / v.dot(v);
        return -w + b * v;
    }
    
    void fillSurface( Surface16u & sur, const ColorAf & col){

        Surface16u::Iter itr = sur.getIter();
        while (itr.line() ) {
            while( itr.pixel()){
                //setColorToItr( itr, col );
                sur.setPixel(itr.getPos(), col);
            }
        }
    }
    
    void drawCoordinate( float length=100 ){
        glBegin( GL_LINES ); {
            glColor3f( 1, 0, 0 );
            glVertex3f( 0, 0, 0 );
            glVertex3f( length, 0, 0 );
            glColor3f( 0, 1, 0 );
            glVertex3f( 0, 0, 0 );
            glVertex3f( 0, length, 0 );
            glColor3f(  0, 0, 1 );
            glVertex3f( 0, 0, 0 );
            glVertex3f( 0, 0, length );
        } glEnd();
    }
  
    void drawScreenGuide(){

        float w = getWindowWidth();
        float h = getWindowHeight();
        gl::pushMatrices();
        gl::setMatricesWindow( getWindowSize() );
        glBegin( GL_LINES ); {
            glColor3f( 1,1,1 );
            glVertex3f( w*0.5, 0, 0 );
            glVertex3f( w*0.5, h, 0 );
            glVertex3f( 0, h*0.5, 0 );
            glVertex3f( w, h*0.5, 0 );
        } glEnd();
        gl::popMatrices();
    }

    void loadColorSample( string fileName, vector<vector<Colorf>>& col){
        Surface sur( loadImage( getAssetPath()/fileName) );
        Surface::Iter itr = sur.getIter();
        
        int w = sur.getWidth();
        int h = sur.getHeight();
        
        col.assign( w, vector<Colorf>(h) );
        
        while ( itr.line() ) {
            
            while ( itr.pixel() ) {
                float r = itr.r() / 255.0f;
                float g = itr.g() / 255.0f;
                float b = itr.b() / 255.0f;
                
                Vec2i pos = itr.getPos();
                col[pos.x][pos.y].set( r, g, b );
            }
        }
        cout << "ColorSample Load Success: w:" << w << ", h " << h << endl;
    }

    clock_t timer;
    void timer_start(){
        timer = clock();
    }
    
    void timer_end(){
        double elapsed_sec = ((double)(clock() - timer)/CLOCKS_PER_SEC) * 1000.0 * 0.1; // dont know why but need *0.1
        cout << "Elapsed time : "  << elapsed_sec << " ms" << endl;
    }
    
    void setMatricesWindow( int screenWidth, int screenHeight, bool center, bool originUpperLeft=true, float near=-10000.0f, float far=10000.0f ){
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        
        if( originUpperLeft ){
            if( center )
                glOrtho( -screenWidth/2, screenWidth/2, screenHeight/2, -screenHeight/2, near, far );
            else
                glOrtho( 0, screenWidth, screenHeight, 0, near, far );            
        }else{
            if(center)
                glOrtho( -screenWidth/2, screenWidth/2, -screenHeight/2, screenHeight/2, near, far );
            else
                glOrtho( 0, screenWidth, 0, screenHeight, near, far );
        }
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
    }
}
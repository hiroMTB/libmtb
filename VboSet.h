#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/Vbo.h"

using namespace ci;
using namespace ci::app;
using namespace std;

/*
 
    VboSet
 
    Data continaer :
        gl::VboMeshRef  vbo;
        vector<Vec3f>   pos;
        vector<ColorAf> col;
        vector<int>     ind;
 
    Usage :
        1. addPos, addCol, addInd
        2. init
        3. updateVboPos, updateVboCol, updateVboInd
 
 */
class VboSet{

private:
    bool bNeedUpdatePos = false;
    bool bNeedUpdateCol = false;
    bool bNeedUpdateInd = false;
    
    vector<Vec3f>       pos;
    vector<ColorAf>     col;
    vector<uint32_t>    ind;

public:
    
    gl::VboMeshRef      vbo;
    gl::VboMesh::Layout lay;

    inline void addPos( const Vec3f & p )   { pos.push_back( p ); bNeedUpdatePos=true; }
    inline void addCol( const ColorAf & c ) { col.push_back( c ); bNeedUpdateCol=true; }
    inline void addInd( const uint32_t i )  { ind.push_back( i ); bNeedUpdateInd=true; }
    inline void addPos( const vector<Vec3f> & ps )   { pos.insert(pos.end(), ps.begin(), ps.end() ); bNeedUpdatePos=true; }
    inline void addCol( const vector<ColorAf> & cs ) { col.insert(col.end(), cs.begin(), cs.end() ); bNeedUpdateCol=true; }
    inline void addInd( const vector<uint32_t> &is ) { ind.insert(ind.end(), is.begin(), is.end() ); bNeedUpdateInd=true; }
    
    inline const vector<Vec3f>& getPos()    { return pos; };
    inline const vector<ColorAf>& getCol()  { return col; };
    inline const vector<uint32_t>& getInd() { return ind; };
    
    inline void writePos( int where, const Vec3f & p)    { pos[where] = p; bNeedUpdatePos=true; }
    inline void writeCol( int where, const ColorAf & c)  { col[where] = c; bNeedUpdateCol=true; }
    inline void writeInd( int where, const uint32_t & i) { ind[where] = i; bNeedUpdateInd=true; }
    
    void init( GLenum primitiveType, bool bStaticPos=true, bool bStaticCol=true ){
        bStaticPos ? lay.setStaticPositions()   : lay.setDynamicPositions();
        bStaticCol ? lay.setStaticColorsRGBA()  : lay.setDynamicColorsRGBA();
        //bStaticInd ? lay.setStaticIndices()     : lay.setDynamicIndices();
        lay.setStaticIndices();
        vbo = gl::VboMesh::create( pos.size(), ind.size(), lay, primitiveType );
        updateVboPos();
        updateVboCol();
        updateVboInd();
    }
    
    void updateVboPos(){
        if( bNeedUpdatePos && vbo ){
            
            if( lay.hasStaticPositions() ) {
                vbo->bufferPositions( pos );
            }else{
                gl::VboMesh::VertexIter itr = vbo->mapVertexBuffer();
                for( int i=0; !itr.isDone()&&i<pos.size(); i++ ){
                    itr.setPosition( pos[i] );
                    ++itr;
                }
            }
            bNeedUpdatePos = false;
        }
    }
    
    void updateVboCol(){
        if( bNeedUpdateCol && vbo ){
            
            if( lay.hasStaticColorsRGBA() ) {
                vbo->bufferColorsRGBA( col );
            }else{
                gl::VboMesh::VertexIter itr = vbo->mapVertexBuffer();
                for( int i=0; !itr.isDone()&&i<col.size(); i++ ){
                    itr.setColorRGBA( col[i] );
                    ++itr;
                }
            }
            bNeedUpdateCol = false;
        }
    }
    
    void updateVboInd(){
        if( bNeedUpdateInd && vbo ){
            vbo->bufferIndices( ind );
            bNeedUpdateInd = false;
        }
    }
    
    void draw(){
        if(vbo) gl::draw( vbo );
    }
    
    // does not work
    void drawRange( int start, int end){
        if(vbo) gl::drawRange( vbo, 0, ind.size(), start, end );
    }
    
    void resetVbo(){
        resetPos();
        resetCol();
        resetInd();
        vbo.reset();
    }
    
    void resetPos(){
        pos.clear();
        bNeedUpdatePos = true;
    }
    
    void resetCol(){
        col.clear();
        bNeedUpdateCol = true;
    }
    
    void resetInd(){
        ind.clear();
        bNeedUpdateInd = true;
    }
};
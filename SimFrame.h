#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

using namespace std;
using namespace boost;

namespace mt {

template <typename T>
class SimFrame{
    
public:
    
    typedef enum {
        DATA_RAW,
        DATA_MAP,
        DATA_LOG
    }DATA_TYPE;
    
    struct Pos{
    public:
        int x;
        int y;
        int z;
        int index;
    };

    DATA_TYPE data_type;
    unsigned int total_size;
    unsigned int load_size;
    unsigned int grid_size;
    unsigned int dimension;
    double min = numeric_limits<double>::max();
    double max = numeric_limits<double>::min();

    vector<Pos> pos;
    vector< vector<T> > data;
    
    void purseMinMax( filesystem::path dataSetDir ){
                
        filesystem::recursive_directory_iterator itr(dataSetDir), eof;

        for( ; itr!=eof; ++itr ){
            if( itr->path().filename().string().at(0) != '.' ){
                
                vector<double> raw_data;
                loadBin( itr->path().string(), raw_data );
                
                double file_min = numeric_limits<double>::max();
                double file_max = numeric_limits<double>::min();
                
                for( auto d : raw_data ){
                    file_min = std::min( file_min, d );
                    file_max = std::max( file_max, d );
                }
                //format fmt1("min: %1%,  max: %2%");
                //fmt1 % file_min % file_max;
                //cout << fmt1.str() << endl;
                
                // min/max value through all file in this dir
                min = std::min( min, file_min );
                max = std::max( max, file_max );
                //return;
            }
        }
    
        format fmt2("data set min: %1%,  max: %2%");
        fmt2 % min % max;
        cout << fmt2.str() << endl;
    }
    
    void load( string filePath, int boxelx, int boxely, int boxelz, double range_min, double range_max, SimFrame::DATA_TYPE dtype ){

        data_type = dtype;

        pos.clear();
        data.clear();
        
        vector<double> raw_data;
        loadBin( filePath, raw_data );
        
        total_size = raw_data.size();

        grid_size = boxelx * boxely * boxelz;
        if( total_size == grid_size ){
            dimension = 1;
        }else if( total_size == grid_size*2 ){
            dimension = 2;
        }else if( total_size == grid_size*3 ){
            dimension = 3;
        }else if( total_size == grid_size*4 ){
            dimension = 4;
        }else{
            cout << "Unmatch total num and boxel size" << endl;
            return;
        }

        for( int i=0; i<dimension; i++){
            data.push_back( vector<T>() );
        }
        
        cout << "Detect : " <<  dimension << " D parameter" << endl;
        
        for( int i=0; i<boxelx; i++ ){
            for( int j=0; j<boxely; j++ ){
                for( int k=0; k<boxelx; k++ ){

                    for( int d=0; d<dimension; d++){
                        int index = (i + j*boxely + k*boxelz*boxely)*dimension + d ;
                        
                        double datad = raw_data[index];
                        double thisdata;
                        
                        switch( data_type ){
                            case DATA_RAW:
                                thisdata = datad;
                                break;
                            case DATA_MAP:
                                thisdata = lmap(datad, min, max, 0.0, 1.0);
                                break;
                            case DATA_LOG:
                                double datad_map_1_10 = lmap(datad, min, max, 1.0, 10.0);
                                thisdata = log10(datad_map_1_10);
                                break;
                        }
                        
                        //if( range_min<=datad && datad<=range_max ){
                        {
                            
                            Pos p;
                            p.x=i; p.y=j; p.z=k; p.index=index;
                            pos.push_back( p );
                            
                            data[d].push_back( thisdata );
                        }
                    }
                }
            }
        }

        
        load_size = raw_data.size();
        cout << "Particle Visible/Total : " << load_size << "/" << total_size << endl;
        cout << "Visible Rate           : " << (float)load_size/total_size*100.0f << endl;
    }
    
private:

    void loadBin( string filePath, vector<double> & raw_data){
        
        cout << "loading bin : " << filePath << endl;
        
        std::ifstream is( filePath, std::ios::binary );
        if(is){
           // cout << "load OK Bin file" << endl;
        }else{
            cout << "load ERROR Bin file" << endl;
            return;
        }
        
        // get length of file:
        is.seekg (0, is.end);
        int fileSize = is.tellg();
        //cout << "length : " << fileSize << " byte" << endl;
        is.seekg (0, is.beg);
        
        unsigned int nRawData = fileSize / sizeof(double);  // 1D : 400*400*400 = 64,000,000
        raw_data.assign(nRawData, double(0));
        is.read(reinterpret_cast<char*>(&raw_data[0]), fileSize);
        is.close();
        //cout << "Load : " << nRawData << " double number" << endl;
    }
    
 
    // helper, copy from cinder
    double lmap(double val, double inMin, double inMax, double outMin, double outMax){
        return outMin + (outMax - outMin) * ((val - inMin) / (inMax - inMin));
    }
    
};

}
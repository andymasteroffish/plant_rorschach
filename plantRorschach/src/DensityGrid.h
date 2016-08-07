//
//  DensityGrid.h
//  plantRorschach
//
//  Created by Golden Ruby Games on 8/13/13.
//
//

#ifndef __plantRorschach__DensityGrid__
#define __plantRorschach__DensityGrid__

#include "ofMain.h"
#include "SpawnZone.h"

class Branch;

class DensityGrid{
public:
    
    void setup();
    void reset(int _maxDensity, int _whiteSpaceMaxValue);
    void draw();
    void drawInkBlot();
    void addDensityMarkerToGrid(SpawnZone newDensityMarker, ofVec2f anchorPos, float adjustedAngle, float baseScale);
    
    void applyImageToGrid(ofImage * pic);
    
    int getDensityForMarker(SpawnZone densityMarker, ofVec2f anchorPos, float adjustedAngle, float baseScale);
    
    void setColumnVals();
    
    int getUnusedTiles();
    
    #define GRID_W 50
    #define GRID_H 65
    int grid[GRID_W][GRID_H];
    int gridSize;
    ofVec2f gridOrigin;
    int leftPadding;
    
    int columnVals[GRID_W];
    
    int maxDensity;
    
    
    
    //base image
    //ofImage inkBlotPic;
    vector<ofImage> inkBlotPics;
    ofImage * curInkBlot;
    int whiteSpaceMaxValue;
};

#endif /* defined(__plantRorschach__DensityGrid__) */

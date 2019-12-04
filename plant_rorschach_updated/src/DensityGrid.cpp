//
//  DensityGrid.cpp
//  plantRorschach
//
//  Created by Golden Ruby Games on 8/13/13.
//
//

#include "DensityGrid.h"


void DensityGrid::setup(){
    gridSize = 20;
    leftPadding = 6;    //how far past the mirror line to extend the grid
    gridOrigin.set(-leftPadding*gridSize, -GRID_H*gridSize*0.5);
    
    
    //load the ink blot pics
    //inkBlotPic.setImageType(OF_IMAGE_GRAYSCALE);
    //inkBlotPic.loadImage("inkBlots/G2.png");
    
    ofDirectory imageDir;
    imageDir.listDir("inkBlots");
    //make the image vector the right size
    if (imageDir.size() > 0){
        inkBlotPics.assign(imageDir.size(), ofImage());
    }
    //add all of the files
    for (int i=0; i<imageDir.size(); i++){
        inkBlotPics[i].loadImage(imageDir.getPath((i)));
    }
    
    cout<<"hoop poop "<<imageDir.size()<<endl;
}


void DensityGrid::reset(int _maxDensity, int _whiteSpaceMaxValue){
    
    maxDensity = _maxDensity;
    whiteSpaceMaxValue = _whiteSpaceMaxValue;
    
    for (int x=0; x<GRID_W; x++){
        for (int y=0; y<GRID_H; y++){
            grid[x][y] = 0;
        }
    }
    
    //apply a random image
//    curInkBlot = &inkBlotPics[ (int)ofRandom(inkBlotPics.size())];
//    applyImageToGrid(curInkBlot);
    
}

void DensityGrid::draw(){
    
    ofSetColor(0);
    for (int x=0; x<=GRID_W; x++){
        ofDrawLine(gridOrigin.x+ x*gridSize, gridOrigin.y, gridOrigin.x+ x*gridSize, gridOrigin.y+GRID_H*gridSize);
    }
    
    for (int y=0; y<=GRID_H; y++){
        ofDrawLine(gridOrigin.x, gridOrigin.y + y*gridSize, gridOrigin.x+ GRID_W*gridSize, gridOrigin.y+ y*gridSize);
    }
    
    ofSetColor(255,0,0);
    for (int x=0; x<GRID_W; x++){
        for (int y=0; y<GRID_H; y++){
            if (grid[x][y]>0){
                ofDrawBitmapString(ofToString(grid[x][y]), gridOrigin.x+ x*gridSize + gridSize*0.4, gridOrigin.y+ y*gridSize + gridSize*0.8);
            }
            
        }
        
        //show the column's value
        ofDrawBitmapString(ofToString(columnVals[x]), gridOrigin.x+ x*gridSize + gridSize*0.4, gridOrigin.y-10);
    }
    
}

void DensityGrid::drawInkBlot(){
    //testing the ink blot
    ofSetColor(255);
    curInkBlot->draw(gridOrigin.x, gridOrigin.y, GRID_W*gridSize, GRID_H*gridSize);
}

void DensityGrid::addDensityMarkerToGrid(SpawnZone newDensityMarker, ofVec2f anchorPos, float adjustedAngle, float baseScale){
    
    ofVec2f centerPos = newDensityMarker.offset.getPoint(anchorPos, adjustedAngle, baseScale);
    
    int gridCenterX = (centerPos.x-gridOrigin.x)/gridSize;
    int gridCenterY = (centerPos.y-gridOrigin.y)/gridSize;
    
    int radius = (newDensityMarker.size*baseScale) / gridSize;
    
    int startX = MAX(0, gridCenterX-radius);
    int endX = MIN(GRID_W-1, gridCenterX+radius);
    int startY = MAX(0, gridCenterY-radius);
    int endY = MIN(GRID_H-1, gridCenterY+radius);
    
    for (int x=startX; x<=endX; x++){
        for (int y=startY; y<=endY; y++){
            if (ofDist(x,y, gridCenterX, gridCenterY) <= radius){
                grid[x][y] ++;
            }
        }
    }

}

//this assumes the image is exactly the same dimesnions as our grid.
void DensityGrid::applyImageToGrid(ofImage * pic){
    //black is the space we want to fill in
    //white is the space we seek to ignore
    
    int maxGridVal = whiteSpaceMaxValue;
    
    for (int x=0; x<GRID_W; x++){
        for (int y=0; y<GRID_H; y++){
            int pos = x + y*pic->getWidth();
            
            grid[x][y] = ofMap(pic->getColor(x, y).r, 0, 255, 0, maxGridVal);
            
        }
    }
    
    
    
}

int DensityGrid::getDensityForMarker(SpawnZone densityMarker, ofVec2f anchorPos, float adjustedAngle, float baseScale){
    int total = 0;
    
    ofVec2f centerPos = densityMarker.offset.getPoint(anchorPos, adjustedAngle, baseScale);
    
    int gridCenterX = (centerPos.x-gridOrigin.x)/gridSize;
    int gridCenterY = (centerPos.y-gridOrigin.y)/gridSize;
    
    int radius = (densityMarker.size*baseScale) / gridSize;
    
    int startX = gridCenterX-radius;
    int endX = gridCenterX+radius;
    int startY = gridCenterY-radius;
    int endY = gridCenterY+radius;
    
    for (int x=startX; x<=endX; x++){
        for (int y=startY; y<=endY; y++){
            
            if (ofDist(x,y, gridCenterX, gridCenterY) <= radius){
                
                //if it is out of bounds, return that it is very dense
                if (x<0 || x>= GRID_W || y<0 || y>=GRID_H){
                    total += 20;
                }
                //otherwise just return the grid value
                else{
                    total+= grid[x][y];
                }
            }
        }
    }
    
    return total;
}


void DensityGrid::setColumnVals(){
    
    for (int x=0; x<GRID_W; x++){
        columnVals[x] = 0;
        for (int y=0; y<GRID_H; y++){
            columnVals[x]+=grid[x][y];
        }
        
    }
    
}


int DensityGrid::getUnusedTiles(){
    
    int total = 0;
    
    for (int x=0; x<GRID_W; x++){
        for (int y=0; y<GRID_H; y++){
            if (grid[x][y] == 0){
                total++;
            }
        }
    }
    
    return total;
}







//
//  AnglePos.h
//  plantRorschach
//
//  Created by Golden Ruby Games on 8/12/13.
//
//

#ifndef plantRorschach_AnglePos_h
#define plantRorschach_AnglePos_h

#include "ofMain.h"

//quick helper class that can define a point based on the distance/angle relation to another point
class AnglePos{
public:
    float dist;
    float angle;
    
    AnglePos(){
        dist = 0;
        angle = 0;
    }
    
    AnglePos(ofVec2f a, ofVec2f b){
        setFromPoints(a,b); 
    }
    
    AnglePos(int x1, int y1, int x2, int y2){
        setFromPoints(x1,y1, x2,y2);
    }
    
    void setFromPoints(ofVec2f a, ofVec2f b){
        setFromPoints(a.x, a.y, b.x, b.y);
    }
    
    void setFromPoints(int x1, int y1, int x2, int y2){
        dist = ofDist(x1,y1, x2, y2);
        angle = atan2(y2-y1, x2-x1);
    }
    
    ofVec2f getPoint(ofVec2f origin, float originAngle, float scale=1){
        float endX = origin.x + cos(angle+originAngle)*dist*scale;
        float endY = origin.y + sin(angle+originAngle)*dist*scale;
        ofVec2f endPoint(endX,endY);
        return endPoint;
    }
};


#endif

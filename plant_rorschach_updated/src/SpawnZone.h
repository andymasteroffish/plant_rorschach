//
//  SpawnZone.h
//  plantRorschach
//
//  Created by Golden Ruby Games on 8/12/13.
//
//

#ifndef plantRorschach_SpawnZone_h
#define plantRorschach_SpawnZone_h

#include "ofMain.h"
#include "AnglePos.h"

//spawn zones are circle that show where on the image we can create new growth
struct SpawnZone{
public:
    AnglePos offset;
    float size;
    
    SpawnZone(){
        
    }
    
    SpawnZone(AnglePos _offset, float _size){
        offset = _offset;
        size = _size;
    }
    
    //this assumes that it is being set when there is no rotation
    SpawnZone(int x, int y, float _size){
        set(x,y,_size);
    }
    
    void set(int x, int y, float _size){
        offset.setFromPoints(0,0, x,y);
        size = _size;
    }

    
    void debugDraw(ofVec2f branchAnchor, float branchAngle, float branchScale){
        ofVec2f drawPos = offset.getPoint(branchAnchor, branchAngle, branchScale);
        ofCircle(drawPos.x, drawPos.y, size*branchScale);
    }
};

#endif

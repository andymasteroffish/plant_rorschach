//
//  BigFlowerBranch.h
//  plantRorschach
//
//  Created by Golden Ruby Games on 8/12/13.
//
//

#ifndef plantRorschach_BigFlowerBranch_h
#define plantRorschach_BigFlowerBranch_h

#include "ofMain.h"
#include "Branch.h"

class BigFlowerBranch : public Branch{
public:
    
    
    void typeSetup(ofVec2f imgAnchorPoint){
        //place some spawn zones
        addSpawnZone(400 - imgAnchorPoint.x, 165- imgAnchorPoint.y, 60);
        addSpawnZone(440 - imgAnchorPoint.x, 240-imgAnchorPoint.y, 60);
        addSpawnZone(430 - imgAnchorPoint.x, 400-imgAnchorPoint.y, 60);
        
        //place the density marker
        setDensityMarker(320- imgAnchorPoint.x, 290- imgAnchorPoint.y, 180*1.5);
    }
    
};

#endif

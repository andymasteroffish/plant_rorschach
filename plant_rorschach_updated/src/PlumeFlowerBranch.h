//
//  PlumeFlowerBranch.h
//  plantRorschach
//
//  Created by Golden Ruby Games on 8/12/13.
//
//

#ifndef plantRorschach_PlumeFlowerBranch_h
#define plantRorschach_PlumeFlowerBranch_h

#include "ofMain.h"
#include "Branch.h"

class PlumeFlowerBranch : public Branch{
public:
    
    
    void typeSetup(ofVec2f imgAnchorPoint){
        //place some spawn zones
        addSpawnZone(520 - imgAnchorPoint.x, 290-imgAnchorPoint.y, 60);
        addSpawnZone(430 - imgAnchorPoint.x, 195-imgAnchorPoint.y, 60);
        addSpawnZone(450 - imgAnchorPoint.x, 400-imgAnchorPoint.y, 60);
        
        setDensityMarker(325-imgAnchorPoint.x, 290-imgAnchorPoint.y, 260);
    }
    
};

#endif

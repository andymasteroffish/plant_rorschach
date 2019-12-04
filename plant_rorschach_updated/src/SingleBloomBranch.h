//
//  SingleBloom.h
//  plantRorschach
//
//  Created by Golden Ruby Games on 8/12/13.
//
//

#ifndef plantRorschach_SingleBloomBranch_h
#define plantRorschach_SingleBloomBranch_h

#include "ofMain.h"
#include "Branch.h"

class SingleBloomBranch : public Branch{
public:
    
    
    void typeSetup(ofVec2f imgAnchorPoint){
        //place some spawn zones
        addSpawnZone(455 - imgAnchorPoint.x, 275-imgAnchorPoint.y, 60);
        addSpawnZone(330 - imgAnchorPoint.x, 430-imgAnchorPoint.y, 60);
        
        setDensityMarker(390-imgAnchorPoint.x, 300-imgAnchorPoint.y, 200);
    }
    
};

#endif

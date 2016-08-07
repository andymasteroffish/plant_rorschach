//
//  TripleFlowerBranch.h
//  plantRorschach
//
//  Created by Golden Ruby Games on 8/12/13.
//
//

#ifndef plantRorschach_TripleFlowerBranch_h
#define plantRorschach_TripleFlowerBranch_h

#include "ofMain.h"
#include "Branch.h"

class TripleFlowerBranch : public Branch{
public:
    
    
    void typeSetup(ofVec2f imgAnchorPoint){
        //place some spawn zones
        addSpawnZone(420 - imgAnchorPoint.x, 210-imgAnchorPoint.y, 60);
        addSpawnZone(345 - imgAnchorPoint.x, 365-imgAnchorPoint.y, 60);
        
        setDensityMarker(340-imgAnchorPoint.x, 285-imgAnchorPoint.y, 210);
    }
    
};

#endif

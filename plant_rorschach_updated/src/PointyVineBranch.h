//
//  PointyVineBranch.h
//  plantRorschach
//
//  Created by Golden Ruby Games on 8/12/13.
//
//

#ifndef plantRorschach_PointyVineBranch_h
#define plantRorschach_PointyVineBranch_h

class PointyVineBranch : public Branch{
public:
    
    
    void typeSetup(ofVec2f imgAnchorPoint){
        //place some spawn zones
        addSpawnZone(410 - imgAnchorPoint.x, 296-imgAnchorPoint.y, 60);
        addSpawnZone(200 - imgAnchorPoint.x, 197-imgAnchorPoint.y, 60);
        
        setDensityMarker( 275-imgAnchorPoint.x, 290-imgAnchorPoint.y, 185);
    }
    
};

#endif

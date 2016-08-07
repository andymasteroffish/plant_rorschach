//
//  KindVineBranch.h
//  plantRorschach
//
//  Created by Golden Ruby Games on 8/12/13.
//
//

#ifndef plantRorschach_KindVineBranch_h
#define plantRorschach_KindVineBranch_h

class KindVineBranch : public Branch{
public:
    
    
    void typeSetup(ofVec2f imgAnchorPoint){
        //place some spawn zones
        addSpawnZone(445 - imgAnchorPoint.x, 358-imgAnchorPoint.y, 60);
        addSpawnZone(419 - imgAnchorPoint.x, 238-imgAnchorPoint.y, 60);
        
        setDensityMarker( 320-imgAnchorPoint.x, 260 -imgAnchorPoint.y, 180);
    }
    
};

#endif

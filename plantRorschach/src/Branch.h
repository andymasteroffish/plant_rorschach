//
//  Branch.h
//  plantRorschach
//
//  Created by Golden Ruby Games on 8/12/13.
//
//

#ifndef __plantRorschach__Branch__
#define __plantRorschach__Branch__

#include "ofMain.h"
#include "AnglePos.h"
#include "SpawnZone.h"

#include "DensityGrid.h"

class Branch{
public:
    
    float baseScale;    //how big this thing is when at rest
    float scale;
    
    float baseAngle;    //local rotation of this thing when at rest
    float angle;            //local angle
    float inheritedAngle;   //angle of the whole tree leading up to this point
    float adjustedAngle;    //angle+inheritedAngle
    
    int depth;  //how far down in the tree this is. Root is 0
    
    vector<Branch *> children;  //pointers to children
    
    ofVec2f anchorPos;  //where on the image this thing rotates and springs from
    ofVec2f inheritedAnchorPos;   //before it grows, we need to know where it will be for density checks
    
    //relaiton to parent
    AnglePos rootPos;   //moving from the parent's anchor, where is this branch's anchor
    
    //spawning new branches
    vector<SpawnZone>   spawnZones;
    
    //knowing where this thing is to try and avoid high density
    SpawnZone densityMarker;
    int densityTest;
    
    
    //growing
    float growTime;
    float growTimer;
    
    //swaying
    float swayTimer;
    float swaySpeed;    //a percentage aplied to the info passed down from panel
    float swayDist;     //a percentage aplied to the info passed down from panel
    float curSwayDist;  //interpolated value to smooth out changes in distance
    //getting sway info from parent (originally set in root and passed down)
    float panelSwayMinSpeed;
    float panelSwayMaxSpeed;
    float panelSwayDistRange;
    
    //the image
    ofImage * pic;
    
    //root
    bool isRoot;
    
    void setup(ofImage * _pic, ofVec2f imgAnchorPoint);
    void setAsRoot(ofVec2f origin, float startScale);
    void setAsChild(Branch * parent, AnglePos _rootPos, float _baseAngle, float _baseScale, int _depth);
    virtual void typeSetup(ofVec2f imgAnchorPoint){}
    
    void update(Branch * parent, float _inheritedAngle, float deltaTime);
    
    void draw(float breatheScale);
    void drawDebug();
    
    bool addChild(Branch * child, float minScaleAdjust, float maxScaleAdjust, float angleRange, DensityGrid * densityGrid);
    
    void addSpawnZone(int x, int y, float size);
    void setDensityMarker(int x, int y, float size);
    
    void startShrinking();
    
    void destroy();
    
    //DensityGrid testo;
    
    //let's try shrinking
    bool isShrinking;
    float shrinkTimer;
    bool doneShrinking;
    
};






#endif /* defined(__plantRorschach__Branch__) */

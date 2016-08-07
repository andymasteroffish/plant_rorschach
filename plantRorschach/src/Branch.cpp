//
//  Branch.cpp
//  plantRorschach
//
//  Created by Golden Ruby Games on 8/12/13.
//
//

#include "Branch.h"


void Branch::setup(ofImage * _pic, ofVec2f imgAnchorPoint){
    pic = _pic;
    typeSetup(imgAnchorPoint);
    
    isRoot = false;
    
    scale = 0;
    
    growTime = ofRandom(0.1, 0.5);
    growTimer = 0;
    
    swaySpeed = ofRandom(1);    //treated as a percent
    swayDist = ofRandom(-1,1);     //treated as a percent
    swayTimer = 0;
    curSwayDist = 0;
    
    isShrinking = false;
    shrinkTimer = 0;
    doneShrinking = false;
}

//sets this branch up to be the root of the images
void Branch::setAsRoot(ofVec2f origin, float startScale){
    baseAngle = 0;
    baseScale = startScale;
    
    rootPos.angle = 0;
    rootPos.dist = 0;
    
    angle = baseAngle;
    
    //testing
    //scale = baseScale;
    
    depth = 0;
    isRoot = true;
    
    anchorPos = origin;
    inheritedAnchorPos = anchorPos;
}

//set new parent will determine the ange and position of the child
void Branch::setAsChild(Branch * parent, AnglePos _rootPos, float _baseAngle, float _baseScale, int _depth){
    baseAngle = _baseAngle;
    baseScale = _baseScale;
    
    rootPos = _rootPos; //where this thing anchors relative to the paren't anchor/angle
    
    //for now, not moving anything
    angle = baseAngle;
    
    //figure out where this thing will wind up being
    inheritedAnchorPos = rootPos.getPoint(parent->inheritedAnchorPos, parent->adjustedAngle, parent->baseScale);
    
    //testing
    //scale = baseScale;
    
    depth = _depth;
}

void Branch::update(Branch * parent, float _inheritedAngle, float deltaTime){
    inheritedAngle = _inheritedAngle;
    
    bool canGrow = isRoot;
    
    //get some info from the aprent
    if (parent!=NULL){
        //set the anchor pos based on the paren't osiiton and rotation
        anchorPos = rootPos.getPoint(parent->anchorPos, parent->adjustedAngle, parent->scale);
        
        //get the panel info passed down from root
        panelSwayMinSpeed = parent->panelSwayMinSpeed;
        panelSwayMaxSpeed = parent->panelSwayMaxSpeed;
        panelSwayDistRange = parent->panelSwayDistRange;
        
        //if the parent is done growing, the children can start
        if (parent->growTimer >= parent->growTime){
            canGrow = true;
        }
    }
    
    
    //grow it if it is ready and hasn't grown yet
    if (growTimer < growTime && canGrow){
        growTimer += deltaTime;
        float prc = growTimer/growTime;
        prc = MIN(prc, 1);
        
        float flipRange = PI;
        float startAngle = baseAngle + (ofNoise(depth*2, baseScale, baseAngle) * flipRange - flipRange/2);
        angle = prc*baseAngle + (1-prc)*startAngle;
        
        scale = baseScale*prc;
    }
    
    //testing shrinking
    if (isShrinking){
        shrinkTimer += deltaTime;
        float prc = 1 - shrinkTimer/growTime;
        prc = MAX(prc, 0);
        
        if (prc == 0){
            doneShrinking = true;
        }
        
        float flipRange = PI;
        float startAngle = baseAngle + (ofNoise(depth*2, baseScale, baseAngle) * flipRange - flipRange/2);
        angle = prc*baseAngle + (1-prc)*startAngle;
        
        scale = baseScale*prc;
    }
    
    //wiggle it!
    float thisSwaySpeed = (1-swaySpeed)*panelSwayMinSpeed + swaySpeed*panelSwayMaxSpeed;
    curSwayDist  = 0.9*curSwayDist + (0.1)*(swayDist*panelSwayDistRange);
    swayTimer += deltaTime*thisSwaySpeed;
    angle = baseAngle + sin(swayTimer)*curSwayDist;
    
    //update the children
    adjustedAngle = inheritedAngle+angle;
    for (int i=0; i<children.size(); i++){
        children[i]->update(this, adjustedAngle, deltaTime);
    }
    
}

void Branch::draw(float breatheScale){
    
    
    //draw the children first 
    for (int i=0; i<children.size(); i++){
        children[i]->draw(breatheScale);
    }
    
    //draw itself
    //ofSetHexColor(0x4e662a);
    ofPushMatrix();
    ofTranslate(anchorPos.x, anchorPos.y);
    ofScale(scale + breatheScale, scale + breatheScale);
    ofRotate( ofRadToDeg(adjustedAngle));
    pic->draw(0,0);
    ofPopMatrix();
    
    
}

void Branch::drawDebug(){
    
    //draw the children debugs 
    for (int i=0; i<children.size(); i++){
        children[i]->drawDebug();
    }
    
//    //show spawn zones
//    ofSetColor(255,0,0,100);
//    for (int i=0; i<spawnZones.size(); i++){
//        spawnZones[i].debugDraw(anchorPos, adjustedAngle, scale);
//    }
    
    //show density markers
    ofSetColor(0,0,255,50);
    densityMarker.debugDraw(anchorPos, adjustedAngle, scale);
    
    
    //show depth
//    ofSetColor(0,0,255);
//    ofVec2f depthTextPos = spawnZones[0].offset.getPoint(anchorPos, adjustedAngle, scale);
//    ofDrawBitmapString(ofToString(depth), depthTextPos.x, depthTextPos.y);
    
    //show density
    ofSetColor(255,0,0);
    ofVec2f densityTextPos = densityMarker.offset.getPoint(anchorPos, adjustedAngle, scale);
    ofDrawBitmapString(ofToString(densityTest), densityTextPos.x, densityTextPos.y);
    
    //show the anchor
//    ofSetColor(255,0,0);
//    ofCircle(anchorPos.x,anchorPos.y, 3);
    
}

bool Branch::addChild(Branch * child, float minScaleAdjust, float maxScaleAdjust, float angleRange, DensityGrid * densityGrid){
    
    int numTries = 0;
    int maxNumTries = 10;
    
    bool goodPos = false;
    
    while (!goodPos && numTries<maxNumTries){
        numTries++;
    
        //select a spawn point
        int spawnID = ofRandom(spawnZones.size());
        
        //select an angle
        float newAngle = ofRandom(-angleRange, angleRange);
        
        //set the scale base don this branch'es scale
        float newScale = ofRandom(baseScale*minScaleAdjust, baseScale*maxScaleAdjust);
        //newScale = baseScale; //tetsing
        
        //give the child it's relative location
        child->setAsChild(this, spawnZones[spawnID].offset, newAngle, newScale, depth+1);
        //update it to actually set the position of the anchor
        child->update(this, adjustedAngle, 0);
        
        //how dense is this spot?
        int densityVal = densityGrid->getDensityForMarker(child->densityMarker, child->inheritedAnchorPos, child->adjustedAngle, child->baseScale);
        
        //see how it fits
        if (densityVal < densityGrid->maxDensity){
            goodPos = true;
        }
            
    }
    
    //if we found a good spot, add it
    if (goodPos){
        //add it to the list
        children.push_back(child);
        
        //testing
        child->densityTest = densityGrid->getDensityForMarker(child->densityMarker, child->inheritedAnchorPos, child->adjustedAngle, child->baseScale);
        
        //add it to the density grid
        densityGrid->addDensityMarkerToGrid(child->densityMarker, child->inheritedAnchorPos, child->adjustedAngle, child->baseScale);
        
        return true;
    }
    
    //tell test App to delet the child
    else{
        return false;
    }
}


//this happens relative to the anchor point of the image
void Branch::addSpawnZone(int x, int y, float size){
    SpawnZone newZone(x,y,size);
    spawnZones.push_back(newZone);
}

void Branch::setDensityMarker(int x, int y, float size){
    densityMarker.set(x,y,size);
}


void Branch::startShrinking(){
    isShrinking = true;
    for (int i=0; i<children.size(); i++){
        children[i]->startShrinking();
    }
}

//destroys branch and all of it's children
void Branch::destroy(){
    
    //kill all children
    for (int i=0; i<children.size(); i++){
        children[i]->destroy();
    }
    
    delete this;    //this feels like it could cause a memory leak
    
}






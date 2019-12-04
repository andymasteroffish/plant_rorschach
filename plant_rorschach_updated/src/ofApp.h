#pragma once

#include "ofMain.h"

#include "ofxControlPanel.h"

#include "AnglePos.h"

#include "Branch.h"
#include "TripleFlowerBranch.h"
#include "SingleBloomBranch.h"
#include "BigFlowerBranch.h"
#include "PlumeFlowerBranch.h"
#include "KindVineBranch.h"
#include "PointyVineBranch.h"

#include "DensityGrid.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void doAutoReset();
    void reset(int numTries);
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    Branch* addChildToBranch(Branch *parent);
    Branch* getNewBranch(bool allowVines);
    void fillTree(Branch * thisBranch, int depthSize, float minScale);
    
    void randomizeValues();
    void usePreset(int presetNum);
    
    
    ofVec2f origin;
    Branch * root;
    
    //#define NUM_PICS 8
    //ofImage pics[NUM_PICS];
#define NUM_BRANCH_TYPES 6
    float branchTickets[NUM_BRANCH_TYPES];  //likelihood of each branch type
    float totalBranchTickets;
    
    ofImage tripleFlowerPic;
    ofVec2f tripleFlowerAnchorPoint;
    ofImage singleBloomPic;
    ofVec2f singleBloomAnchorPoint;
    ofImage bigFlowerPic;
    ofVec2f bigFlowerAnchorPoint;
    ofImage plumeFlowerPic;
    ofVec2f plumeFlowerAnchorPoint;
    ofImage kindVinePic;
    ofVec2f kindVineAnchorPoint;
    ofImage pointyVinePic;
    ofVec2f pointyVineAnchorPoint;
    
    //tree generation
    int minChildrenPerBranch, maxChildrenPerBranch;
    //stuff we need to pass to branch when making a child
    float childMinScaleAdjust, childMaxScaleAdjust, childAngleRange;
    
    //density map
    DensityGrid densityGrid;
    
    
    //timing
    float prevFrameTime;
    float deltaTime;
    
    //breathing effect
    float breatheTimer;
    ofFbo fbo;
    ofShader    shader;
    float breatheSpeed;
    float breatheExpandSize;
    float breatheColorPush;
    
    
    //looking
    float camScale;
    bool draggingCam;
    ofVec2f camPos;
    ofVec2f oldCamPos;
    ofVec2f pressPos;   //where the mouse was pressed down
    
    //mirroring the screen
    ofTexture screenTex;
    
    //control panel
    ofxControlPanel panel;
    //presets
    vector<string> presetNames;
    
    vector< ofParameter<bool>> preset_toggles;
    ofParameter<float> cam_scale;
    ofParameter<bool> random_tree;
    
    ofParameter<bool> auto_reset;
    ofParameter<float> auto_reset_time;
    ofParameter<float> auto_reset_rest_time;
    
    ofParameter<bool> save_screen;
    
    ofParameter<int> max_depth;
    ofParameter<float> min_branch_scale;
    ofParameter<float> max_density;
    ofParameter<int> min_children_per_branch;
    ofParameter<int> max_children_per_branch;
    ofParameter<int> min_starting_branches;
    
    ofParameter<float> min_child_scale;
    ofParameter<float> max_child_scale;
    ofParameter<float> child_angle_range_deg;
    
    ofParameter<float> root_scale;
    
    vector< ofParameter<float>> branch_chance;
    
    ofParameter<float> sway_min_speed;
    ofParameter<float> sway_max_speed;
    ofParameter<float> sway_dist;
    ofParameter<float> root_sway_speed;
    ofParameter<float> root_sway_dist;
    
    ofParameter<float> breathe_speed;
    ofParameter<float> breathe_expand;
    ofParameter<float> breathe_color_push;
    
    ofParameter<float> white_space_val;
    ofParameter<float> max_unused_space;
    ofParameter<float> max_reset_tries;
    ofParameter<bool> show_ink_blot;
    
    //new shit
    bool autoResetIsOn;
    float autoResetTime;
    float autoResetTimer;
    float timeBetweenAutoResets;
    
    #define NUM_BG_PICS 5
    ofImage bgPics[NUM_BG_PICS];
    int curBgPic;
};

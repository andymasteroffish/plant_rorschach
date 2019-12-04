#include "ofApp.h"

bool showDebug = false;
bool showDensityGrid = false;
bool isRotating = false;
bool isMirroring = true;
bool showHelperInfo = false;
bool showPanel = false;

//--------------------------------------------------------------
void ofApp::setup(){
    
    cout<<ofGetWidth()<<" x "<<ofGetHeight()<<endl;
    
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetVerticalSync(true);
    
    ofBackground(240);
    
    for (int i=0; i<NUM_BG_PICS; i++){
        bgPics[i].load("bg/bg"+ofToString(i)+".jpg");
    }
    curBgPic = 2;
    
    screenTex.allocate(ofGetWidth()/2, ofGetHeight(), GL_RGB);
    
    fbo.allocate(ofGetWidth()/2, ofGetHeight());
    shader.setupShaderFromFile(GL_FRAGMENT_SHADER, "shaders/breather.frag");
    shader.linkProgram();
    
    presetNames.push_back("Standard");
    presetNames.push_back("Brambles");
    presetNames.push_back("Vines");
    presetNames.push_back("Open");
    presetNames.push_back("The_Beast");
    presetNames.push_back("The_Tall_Grass");
    
    //setup the control panel
    //ALL DEFAULT VALUES WILL BE RESET BY THE PRESET
    panel.setup("settings", 0, 0, 270, 700);
    panel.addPanel("Presets");//, 4, false);
    //panel.addPanel(
    
    //panel.addToggle("Generate New (space)", "NEW_TREE", false);
    
    panel.setWhichPanel(0);
    panel.setWhichColumn(0);
    
    for (int i=0; i<presetNames.size(); i++){
        preset_toggles.push_back(ofParameter<bool>(presetNames[i], false));
        panel.add(preset_toggles[i]);
        //panel.addToggle(presetNames[i], "PRESET_"+ofToString(i), false);
        //panel.addToggle("PRESET_"+ofToString(i), false);
        //panel.addToggle(presetNames[i], false);
    }
    
    //camera
    panel.add(cam_scale.set("CAM_SCALE", 0.5, 0.05, 2));
    //panel.addSlider("CAM_SCALE", 0.75, 0.05, 2);
    
    panel.add(random_tree.set("RANDOM_TREE", false));
    //panel.addToggle("RANDOM_TREE", false);
    
    panel.addLabel("Automating");
    panel.add(auto_reset.set("auto_reset", true));
    panel.add(auto_reset_time.set("auto_reset_time", 60, 1, 150));
    panel.add(auto_reset_rest_time.set("auto_reset_rest_time", 1.5, 0, 10));
//    panel.addToggle("auto_reset", true);
//    panel.addSlider("auto_reset_time", 20, 1, 150);
//    panel.addSlider("auto_reset_rest_time", 1.5, 0, 10);
    
    
    panel.addLabel("Press [space] any time to\ngenerate a new tree with the\ncurrent parameters");
    panel.addLabel("Press [H] to hide this panel");
    panel.addLabel("Drag screen to move camera");
    panel.addLabel("Zoom controlled by slider");
    
    panel.add(save_screen.set(false));
    //panel.addToggle("SAVE_SCREEN", false);
    
    //generation
    panel.setWhichPanel(1);
    
    panel.add( max_depth.set("MAX_DEPTH", 6, 1, 15));
    panel.add( min_branch_scale.set("MIN_BRANCH_SCALE", 0.1, 0.04, 0.4));
    
    panel.add( max_density.set("MAX_DENSITY", 0.55, 0, 1));
    
    panel.add( min_children_per_branch.set("MIN_CHILDREN_PER_BRANCH", 2, 0, 5));
    panel.add( max_children_per_branch.set("MAX_CHILDREN_PER_BRANCH", 3, 1, 5));
    panel.add( min_starting_branches.set("MIN_STARTING_BRANCHES", 2, 1, 5));
    
    panel.add( min_child_scale.set("MIN_CHILD_SCALE", 0.5, 0.1, 1.5));
    panel.add( max_child_scale.set("MAX_CHILD_SCALE", 1.2, 0.1, 2));
    panel.add( child_angle_range_deg.set("CHILD_ANGLE_RANGE_DEG", 45, 0, 135));
    
    panel.add( root_scale.set("ROOT_SCALE", 0.1, 0.15, 1));
//    panel.addSlider("MAX_DEPTH", 6, 1, 15);
//    panel.addSlider("MIN_BRANCH_SCALE", 0.1, 0.04, 0.4);
//
//    panel.addSlider("MAX_DENSITY", 0.55, 0, 1);
//
//    panel.addSlider("MIN_CHILDREN_PER_BRANCH", 2, 0, 5);
//    panel.addSlider("MAX_CHILDREN_PER_BRANCH", 3, 1, 5);
//    panel.addSlider("MIN_STARTING_BRANCHES", 2, 1, 5);
//
//    panel.addSlider("MIN_CHILD_SCALE", 0.5, 0.1, 1.5);
//    panel.addSlider("MAX_CHILD_SCALE", 1.2, 0.1, 2);
//    panel.addSlider("CHILD_ANGLE_RANGE_DEG", 45, 0, 135);
//
//    panel.addSlider("ROOT_SCALE", 0.1, 0.15, 1);
    
    //setting the chances for each branch type
    panel.addPanel("Branches", 4, false);
    panel.setWhichPanel("Branches");
    panel.addLabel("Likelihood of each type");
    for (int i=0; i<6; i++){
        string name = "BRANCH_CHANCE_"+ofToString(i);
        branch_chance.push_back( ofParameter<float>(name, 0.5, 0, 1));
        panel.add((branch_chance[i]));
    }
//    panel.addSlider("BRANCH_CHANCE_0", 0.5, 0, 1);
//    panel.addSlider("BRANCH_CHANCE_1", 0.5, 0, 1);
//    panel.addSlider("BRANCH_CHANCE_2", 0.5, 0, 1);
//    panel.addSlider("BRANCH_CHANCE_3", 0.5, 0, 1);
//    panel.addSlider("BRANCH_CHANCE_4", 0.5, 0, 1);
//    panel.addSlider("BRANCH_CHANCE_5", 0.5, 0, 1);
    
    //adjusting the tree after generating
    panel.addPanel("Movement", 4, false);
    panel.setWhichPanel("Movement");
    panel.add(sway_min_speed.set("SWAY_MIN_SPEED", 0.5, 0.1, 4));
    panel.add(sway_max_speed.set("SWAY_MAX_SPEED", 1.5, 0.1, 4));
    panel.add(sway_dist.set("SWAY_DIST", 0.03, 0, PI*0.1));
    panel.add(root_sway_speed.set("ROOT_SWAY_SPEED", 0.5, 0, 1));
    panel.add(root_sway_dist.set("ROOT_SWAY_DIST", 0.5, 0, 1));
//    panel.addSlider("SWAY_MIN_SPEED", 0.5, 0.1, 4);
//    panel.addSlider("SWAY_MAX_SPEED", 1.5, 0.1, 4);
//    panel.addSlider("SWAY_DIST", 0.03, 0, PI*0.1);
//    panel.addSlider("ROOT_SWAY_SPEED", 0.5, 0, 1);
//    panel.addSlider("ROOT_SWAY_DIST", 0.5, 0, 1);
    
    //breathing
    panel.add(breathe_speed.set("BREATHE_SPEED", 0, 0.1, 4));
    panel.add(breathe_expand.set("BREATHE_EXPAND", 0.0, 0, 0.05));
    panel.add(breathe_color_push.set("BREATHE_COLOR_PUSH", 0.0, 0, 0.5));
//    panel.addSlider("BREATHE_SPEED", 0, 0.1, 4);
//    panel.addSlider("BREATHE_EXPAND", 0.0, 0, 0.05);
//    panel.addSlider("BREATHE_COLOR_PUSH", 0.0, 0, 0.5);
    
    //setting the underlying image
    panel.addPanel("Ink Blot", 4, false);
    panel.setWhichPanel("Ink Blot");
    panel.add(white_space_val.set("WHITE_SPACE_VAL", 5, 0, 20));
    panel.add(max_unused_space.set("MAX_UNUSED_SPACE", 600, 100, 1500));
    panel.add(max_reset_tries.set("MAX_RESET_TRIES", 200, 1, 500));
    panel.add(show_ink_blot.set("SHOW_INK_BLOT", false));
//    panel.addSlider("WHITE_SPACE_VAL", 5, 0, 20);
//    panel.addSlider("MAX_UNUSED_SPACE", 600, 100, 1500);
//    panel.addSlider("MAX_RESET_TRIES", 200, 1, 500);
//    panel.addToggle("SHOW_INK_BLOT", false);
    
    usePreset(0);
    
    //new shit
    //panel.setupEvents();
    //panel.enableEvents();
    
    
    //setup the images
    tripleFlowerPic.load("pics/Animated Asset 1.png");
    tripleFlowerAnchorPoint.set(40, 315);
    tripleFlowerPic.setAnchorPoint(tripleFlowerAnchorPoint.x, tripleFlowerAnchorPoint.y);
    
    singleBloomPic.load("pics/Animated Asset 8.png");
    singleBloomAnchorPoint.set(41, 304);
    singleBloomPic.setAnchorPoint(singleBloomAnchorPoint.x, singleBloomAnchorPoint.y);
    
    bigFlowerPic.load("pics/Animated Asset 6.png");
    bigFlowerAnchorPoint.set(95, 400);
    bigFlowerPic.setAnchorPoint(bigFlowerAnchorPoint.x, bigFlowerAnchorPoint.y);
    
    plumeFlowerPic.load("pics/Animated Asset 4.png");
    plumeFlowerAnchorPoint.set(86, 336);
    plumeFlowerPic.setAnchorPoint(plumeFlowerAnchorPoint.x, plumeFlowerAnchorPoint.y);
    
    kindVinePic.load("pics/Animated Asset 3.png");
    kindVineAnchorPoint.set(25, 255);
    kindVinePic.setAnchorPoint(kindVineAnchorPoint.x, kindVineAnchorPoint.y);
    
    pointyVinePic.load("pics/Animated Asset 2.png");
    pointyVineAnchorPoint.set(60, 320);
    pointyVinePic.setAnchorPoint(pointyVineAnchorPoint.x, pointyVineAnchorPoint.y);
    
    
    //set the origin
    origin.set(-130, 0);
    
    
    //setup the density grid
    densityGrid.setup();
    
    //camera
    camScale = cam_scale.get();// panel.getValueF("CAM_SCALE");
    camPos.set(755, ofGetHeight()/2 / cam_scale.get());
    //camPos.set(1000, ofGetHeight()/2 / cam_scale.get());
    
    reset(0);
    
    autoResetTimer = 0;
    
    prevFrameTime = ofGetElapsedTimef();
}


//--------------------------------------------------------------
void ofApp::update(){
    deltaTime = ofGetElapsedTimef()-prevFrameTime;
    prevFrameTime = ofGetElapsedTimef();
    
    panel.update();
    
    autoResetIsOn = auto_reset.get();// panel.getValueB("auto_reset");
    autoResetTime = auto_reset_time.get();// panel.getValueF("auto_reset_time");
    timeBetweenAutoResets = auto_reset_rest_time.get();// panel.getValueF("auto_reset_rest_time");
    
    //some values can be adjusted in between spawns
    camScale = cam_scale.get();// panel.getValueF("CAM_SCALE");
    
    
    //moving the tree around
    root->panelSwayMinSpeed = sway_min_speed.get();// panel.getValueF("SWAY_MIN_SPEED");
    root->panelSwayMaxSpeed = sway_max_speed.get();// panel.getValueF("SWAY_MAX_SPEED");
    root->panelSwayDistRange = sway_dist.get();// panel.getValueF("SWAY_DIST");
    //setting the root's values
    root->swaySpeed = root_sway_speed.get();// panel.getValueF("ROOT_SWAY_SPEED");
    root->swayDist = root_sway_dist.get();// panel.getValueF("ROOT_SWAY_DIST");
    
    //breathing values
    breatheSpeed = breathe_speed.get();// panel.getValueF("BREATHE_SPEED");
    breatheExpandSize = breathe_expand.get();// panel.getValueF("BREATHE_EXPAND");
    breatheColorPush = breathe_color_push.get();// panel.getValueF("BREATHE_COLOR_PUSH");
    
    //check if they hit a preset button
    for (int i=0; i<presetNames.size(); i++){
        if (preset_toggles[i].get()){
        //if (panel.getValueB(presetNames[i])){
            //panel.setValueB(presetNames[i], false);
            preset_toggles[i].set(false);
            usePreset(i);
            root->destroy();
            reset(0);
        }
    }
    
    //check if they hit randomize
    if (random_tree.get()){
    //if (panel.getValueB("RANDOM_TREE")){
        //panel.setValueB("RANDOM_TREE", false);
        random_tree.set(false);
        root->destroy();
        randomizeValues();
        reset(0);
    }
    //cout<<"rand "<<panel.getValueB("TEST")<<endl;
    
    if (isRotating) {
        root->angle += deltaTime*PI*0.3;
    }
    
    //update the tree
    root->update(NULL, 0, deltaTime);
    
    
    //figure out breathing
    breatheTimer += deltaTime*breatheSpeed;
    
    float breathePrc;
    
    float inhaleTime = 1;
    float exhaleTime = 3;
    float restTime = 0.5;
    
    if (breatheTimer < inhaleTime){
        breathePrc = ofMap(breatheTimer, 0, inhaleTime, 0, 1);
    }
    else if (breatheTimer < inhaleTime+exhaleTime){
        breathePrc = ofMap(breatheTimer, inhaleTime, inhaleTime+exhaleTime, 1, 0, true);
    }
    else if (breatheTimer < inhaleTime+exhaleTime+restTime){
        breathePrc = 0;
    }
    else{
        breathePrc = 0;
        breatheTimer = 0;
    }
    
    float breatheScale = ofMap(breathePrc, 0, 1, 0, breatheExpandSize);
    
    //draw the tree into the FBO
    fbo.begin();
    
    ofClear(0);
    
    shader.begin();
    shader.setUniform1f("increaseVal", breathePrc* breatheColorPush );
    
    ofPushMatrix();
    ofTranslate(-ofGetWidth()/2,0);
    ofScale(camScale, camScale);
    ofTranslate(camPos.x, camPos.y);
    //draw the tree
    ofSetColor(255);
    root->draw(breatheScale);
    ofPopMatrix();
    
    shader.end();
    fbo.end();
    
    
    if (autoResetIsOn && !root->isShrinking){
        autoResetTimer += deltaTime;
        if (autoResetTimer >= autoResetTime){
            autoResetTimer = 0;
            root->startShrinking();
        }
    }
    
    if (root->doneShrinking){
        autoResetTimer += deltaTime;
        if (autoResetTimer >= timeBetweenAutoResets){
            doAutoReset();
            autoResetTimer = 0;
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);
    
    ofSetColor(255,255,255,70);
    bgPics[curBgPic].draw(0,0,ofGetWidth(), ofGetHeight());
    
    
    //are we saving a screenshot?
    bool takingScreenShot = false;
    
    if (save_screen.get()){
        save_screen.set(false);// panel.setValueB("SAVE_SCREEN", false);
        takingScreenShot = true;
    }
    
    
    //show the underlying ink blot if it's on
    if (show_ink_blot.get()){
        ofPushMatrix();
        
        ofScale(camScale, camScale);
        ofTranslate(camPos.x, camPos.y);
        
        densityGrid.drawInkBlot();
        
        ofPopMatrix();
    }
    
    
    //draw the tree
    ofSetHexColor(0x4e662a);
    
    fbo.draw(ofGetWidth()/2, 0);
    
    if (!takingScreenShot){
        ofPushMatrix();
        
        ofScale(camScale, camScale);
        ofTranslate(camPos.x, camPos.y);
        
        //debug stuff
        if (showDebug){
            root->drawDebug();
        }
        
        if (showDensityGrid){
            densityGrid.draw();
        }
        ofPopMatrix();
    }
    
    //snap shot it
    if (isMirroring){
        screenTex.loadScreenData(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
        //draw it flipped
        ofSetColor(255);
        screenTex.draw(ofGetWidth()/2,0,-screenTex.getWidth(), ofGetHeight());
    }
    
    //draw the control panel
    if (showPanel && !takingScreenShot){
        ofSetHexColor(0xffffff);
        ofSetLineWidth(1);
        panel.draw();
    }
    
    //take the screenshot if we're supposed to
    if (takingScreenShot){
        ofImage screenGrab;
        screenGrab.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        screenGrab.save("savedPics/img"+ofToString(ofGetSystemTime())+".png");
    }
    
    //some info
    if (showHelperInfo){
        ofSetColor(0,150);
        ofDrawRectangle(0,ofGetHeight()-30,ofGetWidth(),30);
        ofSetColor(255);
        string info = "Press SPACE to refresh. Press I to toggle guiding image. Press H to toggle control panel. Press S to take screen shot";
        ofDrawBitmapString(info, 15, ofGetHeight()-10);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'd'){
        showDebug = !showDebug;
    }
    if (key == 'r'){
        randomizeValues();
        //isRotating = !isRotating;
    }
    
    if (key == 'h'){
        showPanel = !showPanel;
    }
    
    if (key == 'g'){
        showDensityGrid = !showDensityGrid;
    }
    
    if (key == 'm'){
        isMirroring = !isMirroring;
    }
    
    
    if (key == 's'){
        save_screen.set(true);// panel.setValueB("SAVE_SCREEN", true);
    }
    
    if (key == 'i'){
        show_ink_blot.set(!show_ink_blot.get()); // panel.setValueB("SHOW_INK_BLOT", !panel.getValueB("SHOW_INK_BLOT"));
    }
     
    
    if (key == 'f'){
        ofToggleFullscreen();
    }
    
    if (key == 'z'){
        root->startShrinking();
    }
    
    if (key == 'b'){
        curBgPic = (int)ofRandom(NUM_BG_PICS);
    }
    
    if (key == 'a'){
        auto_reset.set( !autoResetIsOn);
        //panel.setValueB("auto_reset", !autoResetIsOn);
    }
    
    //cout<<"key "<<key<<endl;
    for(int i=0; i<presetNames.size(); i++){
        if (key == 49+i){
            preset_toggles[i].set(true);
            //panel.setValueB(presetNames[i], true);
        }
    }
    
    //space resets
    if (key == ' '){
        //root->destroy();
        doAutoReset();
        //reset(0);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

    
    if (draggingCam){
        camPos.x = oldCamPos.x + (x-pressPos.x) / (camScale);
        camPos.y = oldCamPos.y + (y-pressPos.y) / (camScale);
        
        cout<<"dat x "<<camPos.x<<endl;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (button == 2){
        draggingCam = true;
    }
    
    //FIX THIS SHIT
//    if (showPanel) {
//        draggingCam = !panel.mousePressed(x, y, button);
//    }else{
//        draggingCam = true;
//    }
    
    oldCamPos = camPos;
    pressPos.set(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    //if (showPanel) panel.mouseReleased();
    if (button == 2){
        draggingCam = false;
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    fbo.allocate(ofGetWidth()/2, ofGetHeight());
    screenTex.allocate(ofGetWidth()/2, ofGetHeight(), GL_RGB);
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::doAutoReset(){
    autoResetTimer = 0;
    //just turn one of the preset buttons on
    int presetID = (int)ofRandom(presetNames.size());
    //panel.setValueB(presetNames[presetID], true);
    preset_toggles[presetID].set(true);
}


//--------------------------------------------------------------
void ofApp::reset(int numTries){
    
    //clear the grid
    //curving the density value because there is much more granulaiton towards the end of the spectrum
    //int maxDensity = 99 * powf(panel.getValueF("MAX_DENSITY"), 2) + 1;
    int maxDensity = 99 * powf(max_density.get(), 2) + 1;
    densityGrid.reset(maxDensity, white_space_val.get());// panel.getValueI("WHITE_SPACE_VAL"));
    
    //set the branch tickets
    totalBranchTickets = 0;
    for (int i=0; i<NUM_BRANCH_TYPES; i++){
        branchTickets[i] = branch_chance[i].get();// panel.getValueF("BRANCH_CHANCE_"+ofToString(i));
        totalBranchTickets += branchTickets[i];
    }
    
    minChildrenPerBranch = min_children_per_branch.get();// panel.getValueI("MIN_CHILDREN_PER_BRANCH");
    maxChildrenPerBranch = max_children_per_branch.get();// panel.getValueI("MAX_CHILDREN_PER_BRANCH");
    
    childMinScaleAdjust = min_child_scale.get();// panel.getValueF("MIN_CHILD_SCALE");
    childMaxScaleAdjust = max_child_scale.get();//panel.getValueF("MAX_CHILD_SCALE");
    childAngleRange = ofDegToRad(child_angle_range_deg.get()); //ofDegToRad(panel.getValueF("CHILD_ANGLE_RANGE_DEG"));
    
    //make the root
    root = getNewBranch(false);
    
    //root->setAsRoot(origin, panel.getValueF("ROOT_SCALE"));
    root->setAsRoot(origin, root_scale.get());// panel.getValueF("ROOT_SCALE"));
    densityGrid.addDensityMarkerToGrid(root->densityMarker, root->anchorPos, root->adjustedAngle, root->baseScale);
    
    
    //fillTree(root, panel.getValueI("MAX_DEPTH"), panel.getValueF("MIN_BRANCH_SCALE"));
    fillTree(root, max_depth.get(), min_branch_scale.get());
    
    densityGrid.setColumnVals();
    
    
    //set the camera starting place
    //using a while loop is dumb an ineficient
    //this is pretty much all magic numbers and stupidity
    camPos.x = (ofGetWidth()*0.6)/camScale;
    float rootAnchorX = (root->anchorPos.x + camPos.x) * camScale;
    while (rootAnchorX > ofGetWidth()/2 - 10){
        camPos.x-=4;
        rootAnchorX = (root->anchorPos.x + camPos.x) * camScale;
    }
    
    camPos.y = ofGetHeight()/2 / camScale;
    
    //curBgPic = (int)ofRandom(NUM_BG_PICS);
    
    
    //IF YOU WANT TO USE INK BLOTS, THIS IS IMPORTANT AND YOU SHOULD PUT IT BACK
    
    //if there is too much unused space, try again
//    int maxUnusedSpace = panel.getValueF("MAX_UNUSED_SPACE");
//    //if it has been going too long, start makign it easier
//    if (numTries > panel.getValueI("MAX_RESET_TRIES")){
//        maxUnusedSpace += (numTries - panel.getValueI("MAX_RESET_TRIES"));
//        cout<<"new max unused "<<maxUnusedSpace<<endl;
//    }
//    //actually see if it qualifies
//    if (densityGrid.getUnusedTiles() > maxUnusedSpace){
//        reset(numTries+1);
//    }else{
//        cout<<"unused: "<<densityGrid.getUnusedTiles()<<"   after "<<numTries<<" attempts"<<endl;
//    }
    
}

//--------------------------------------------------------------
Branch* ofApp::addChildToBranch(Branch *parent){
    
    //create a child at random
    Branch * child = getNewBranch(true);
    
    //let the parent figure out how to add it to the tree
    if (parent->addChild(child, childMinScaleAdjust, childMaxScaleAdjust, childAngleRange, &densityGrid) ){
        return child;
    }else{
        delete child;
        return NULL;
    }
    
}

//--------------------------------------------------------------
Branch* ofApp::getNewBranch(bool allowVines){
    Branch * newBranch = NULL;
    
    bool goodSeleciton = false;
    
    int type = 0;
    while(!goodSeleciton){
        
        //setup the tickets
        float rand = ofRandom(totalBranchTickets);
        type = -1;
        
        while (rand > 0){
            type++;
            rand-=branchTickets[type];
        }
        
        if (allowVines || (type != 4 && type != 5)){
            goodSeleciton = true;
        }
        
    }
    
    if (type == 0){
        newBranch = new TripleFlowerBranch();
        newBranch->setup(&tripleFlowerPic, tripleFlowerAnchorPoint);
    }
    if (type == 1){
        newBranch = new SingleBloomBranch();
        newBranch->setup(&singleBloomPic, singleBloomAnchorPoint);
    }
    if (type == 2){
        newBranch = new BigFlowerBranch();
        newBranch->setup(&bigFlowerPic, bigFlowerAnchorPoint);
    }
    if (type == 3){
        newBranch = new PlumeFlowerBranch();
        newBranch->setup(&plumeFlowerPic, plumeFlowerAnchorPoint);
    }
    if (type == 4){
        newBranch = new KindVineBranch();
        newBranch->setup(&kindVinePic, kindVineAnchorPoint);
    }
    if (type == 5){
        newBranch = new PointyVineBranch();
        newBranch->setup(&pointyVinePic, pointyVineAnchorPoint);
    }
    
    if (newBranch==NULL){
        cout<<"WE FUCKED UP MAKING A BRANCH"<<endl;
    }
    
    return newBranch;
}

//--------------------------------------------------------------
void ofApp::fillTree(Branch * thisBranch, int depthSize, float minScale){
    
    //if we reached the goal or the min size, just return
    if (thisBranch->depth >= depthSize || thisBranch->baseScale < minScale){
        return;
    }
    
    //oethrwise give this thing some kids!
    int numKids = ofRandom(minChildrenPerBranch,maxChildrenPerBranch+1);
    
    //it sucks when the root only has one
    if (thisBranch->isRoot){
        numKids = min_starting_branches.get();// MAX(numKids, panel.getValueI("MIN_STARTING_BRANCHES"));
    }
    
    for (int i=0; i<numKids; i++){
        Branch* newBrach = addChildToBranch(thisBranch);
        if (newBrach != NULL){
            fillTree(newBrach, depthSize, minScale);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::usePreset(int presetNum){
    cout<<"set preset "<<presetNum<<endl;
    //standard
    if (presetNum==0){
        max_depth.set(4);// panel.set("MAX_DEPTH", 4);
        min_branch_scale.set(0.24);// panel.set("MIN_BRANCH_SCALE", 0.24);
        
        max_density.set(0.7);//) panel.set("MAX_DENSITY", 0.7);//0.28);
        
        min_children_per_branch.set(3);// panel.set("MIN_CHILDREN_PER_BRANCH", 3);
        max_children_per_branch.set(3);// panel.set("MAX_CHILDREN_PER_BRANCH", 3);
        min_starting_branches.set(5);// panel.set("MIN_STARTING_BRANCHES", 5);
        
        min_child_scale.set(0.61);// panel.set("MIN_CHILD_SCALE", 0.61);
        max_child_scale.set(1.01);// panel.set("MAX_CHILD_SCALE", 1.01);
        child_angle_range_deg.set(111);// panel.set("CHILD_ANGLE_RANGE_DEG", 111);
        
        root_scale.set(0.702);// panel.set("ROOT_SCALE", 0.702);
        
        //setting the chances for each branch type
        branch_chance[0].set(0.5);// panel.set("BRANCH_CHANCE_0", 0.5);
        branch_chance[1].set(0.5);// panel.set("BRANCH_CHANCE_1", 0.5);
        branch_chance[2].set(0.5);// panel.set("BRANCH_CHANCE_2", 0.5);
        branch_chance[3].set(0.5);// panel.set("BRANCH_CHANCE_3", 0.5);
        branch_chance[4].set(0.35);// panel.set("BRANCH_CHANCE_4", 0.35);
        branch_chance[5].set(0.35);// panel.set("BRANCH_CHANCE_5", 0.35);
        
        //adjusting the tree after generating
        sway_min_speed.set(0.5);// panel.set( "SWAY_MIN_SPEED", 0.5);
        sway_max_speed.set(1.5);// panel.set("SWAY_MAX_SPEED", 1.5);
        sway_dist.set(0.03);// panel.set("SWAY_DIST", 0.03);
        root_sway_speed.set(0.5);// panel.set("ROOT_SWAY_SPEED", 0.5);
        root_sway_dist.set(0.5);// panel.set("ROOT_SWAY_DIST", 0.5);
        
        //breathing
        breathe_speed.set(1.1);// panel.set("BREATHE_SPEED", 1.1);
        breathe_expand.set(0.011);// panel.set("BREATHE_EXPAND", 0.011);
        breathe_color_push.set(0.135);// panel.set("BREATHE_COLOR_PUSH", 0.135);
    }
    
    
    //brambles
    if (presetNum==1){
        max_depth.set(8);
        min_branch_scale.set(0.084);
        
        max_density.set( 0.6);
        
        min_children_per_branch.set(2);
        max_children_per_branch.set( 4);
        min_starting_branches.set(3);
        
        min_child_scale.set(0.64);
        max_child_scale.set(1.02);
        child_angle_range_deg.set(63);
        
        root_scale.set( 0.42);
        
        //setting the chances for each branch type
        branch_chance[0].set(0.15);
        branch_chance[1].set(0.15);
        branch_chance[2].set( 0.15);
        branch_chance[3].set(0.5);
        branch_chance[4].set( 0.5);
        branch_chance[5].set(0.5);
        
        //adjusting the tree after generating
        sway_min_speed.set( 0.56);
        sway_max_speed.set(0.82);
        sway_dist.set( 0.12);
        root_sway_speed.set(0.5);
        root_sway_dist.set(0.5);
    }
    
    
    //vines
    if (presetNum==2){
        max_depth.set(10);
        min_branch_scale.set( 0.1);
        
        max_density.set( 0.1);
        
        min_children_per_branch.set( 1);
        max_children_per_branch.set( 3);
        min_starting_branches.set( 4);
        
        min_child_scale.set( 0.54);
        max_child_scale.set( 1.37);
        child_angle_range_deg.set( 49);
        
        root_scale.set(0.35);
        
        //setting the chances for each branch type
        branch_chance[0].set( 0.19);
        branch_chance[1].set( 0.19);
        branch_chance[2].set( 0.0);
        branch_chance[3].set( 0.0);
        branch_chance[4].set( 0.74);
        branch_chance[5].set( 0.82);
        
        //adjusting the tree after generating
        sway_min_speed.set(  0.23);
        sway_max_speed.set( 1.1);
        sway_dist.set( 0.06);
        root_sway_speed.set( 0.5);
        root_sway_dist.set( 0.5);
    }
    
    //open
    if (presetNum==3){
        max_depth.set( 8);
        min_branch_scale.set( 0.152);
        
        max_density.set( 0.13);
        
        min_children_per_branch.set( 3);
        max_children_per_branch.set( 3);
        min_starting_branches.set( 3);
        
        min_child_scale.set(0.82);
        max_child_scale.set(1.18);
        child_angle_range_deg.set(70);
        
        root_scale.set(0.35);
        
        //setting the chances for each branch type
        branch_chance[0].set(0.66);
        branch_chance[1].set(0.61);
        branch_chance[2].set( 0.61);
        branch_chance[3].set(0.61);
        branch_chance[4].set(0.26);
        branch_chance[5].set(0.30);
        
        //adjusting the tree after generating
        sway_min_speed.set( 0.62);
        sway_max_speed.set( 1.55);
        sway_dist.set( 0.045);
        root_sway_speed.set(0.4);
        root_sway_dist.set(0.9);
    }
    
    
    //the beast
    if (presetNum==4){
        max_depth.set(8);
        min_branch_scale.set( 0.20);
        
        max_density.set( 0.41);
        
        min_children_per_branch.set( 1);
        max_children_per_branch.set( 3);
        min_starting_branches.set(4);
        
        min_child_scale.set( 0.69);
        max_child_scale.set( 1.14);
        child_angle_range_deg.set( 55);
        
        root_scale.set( 0.32);
        
        //setting the chances for each branch type
        branch_chance[0].set(0.5);
        branch_chance[1].set(0.5);
        branch_chance[2].set(0.5);
        branch_chance[3].set(0.5);
        branch_chance[4].set(0.5);
        branch_chance[5].set( 0.5);
        
        //adjusting the tree after generating
        sway_min_speed.set( 3.11);
        sway_max_speed.set(4.0);
        sway_dist.set( 0.314);
        root_sway_speed.set( 0.66);
        root_sway_dist.set(0.72);
    }
    
    
    //the tall grass
    if (presetNum==5){
        max_depth.set( 10);
        min_branch_scale.set( 0.130);
        
        max_density.set(0.70);
        
        min_children_per_branch.set( 2);
        max_children_per_branch.set( 3);
        min_starting_branches.set( 2);
        
        min_child_scale.set( 0.38);
        max_child_scale.set( 1.23);
        child_angle_range_deg.set( 61);
        
        root_scale.set(0.35);
        
        //setting the chances for each branch type
        branch_chance[0].set(0.07);
        branch_chance[1].set(0.07);
        branch_chance[2].set(0.0);
        branch_chance[3].set(0.0);
        branch_chance[4].set(1.0);
        branch_chance[5].set( 0.0);
        
        //adjusting the tree after generating
        sway_min_speed.set(0.5);
        sway_max_speed.set(1.5);
        sway_dist.set( 0.01);
        root_sway_speed.set(1.0);
        root_sway_dist.set(1.0);
    }
    
}

//--------------------------------------------------------------
void ofApp::randomizeValues(){
    //this is super hacky!
    //VALUE.MAX AND VALUE.MIN IN GUIVALUE.H WERE PROTECTED. YOU UNPRTOECTED IT!
    
    //this whole thing assumes each atribute only has one value, but for sliders, that's fine
    
//    for (int p=1; p<panel.panels.size(); p++){
//        for (int c=0; c<panel.panels[p]->children.size(); c++){
//            //make sure this isn't a label or something by making sure it has a value
//            if (panel.panels[p]->children[c]->value.value.size() > 0){
//                
//                float thisMin = panel.panels[p]->children[c]->value.min[0];
//                float thisMax = panel.panels[p]->children[c]->value.max[0];
//                
//                //set the damn thing
//                panel.panels[p]->children[c]->setValue(ofRandom(thisMin,thisMax), 0);
//                //cout<<panel.panels[p]->children[c]->value.max[0]<<endl;
//            }
//        }
//        
//    }
}



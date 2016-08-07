#include "ofApp.h"

bool showDebug = false;
bool showDensityGrid = false;
bool isRotating = false;
bool isMirroring = true;
bool showHelperInfo = false;
bool showPanel = false;

//--------------------------------------------------------------
void ofApp::setup(){
    
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
        //panel.addToggle(presetNames[i], "PRESET_"+ofToString(i), false);
        //panel.addToggle("PRESET_"+ofToString(i), false);
        panel.addToggle(presetNames[i], false);
    }
    
    //camera
    panel.addSlider("CAM_SCALE", 0.75, 0.05, 2);
    
    panel.addToggle("RANDOM_TREE", false);
    
    panel.addLabel("Automating");
    panel.addToggle("auto_reset", true);
    panel.addSlider("auto_reset_time", 20, 1, 150);
    panel.addSlider("auto_reset_rest_time", 1.5, 0, 10);
    
    
    panel.addLabel("Press [space] any time to\ngenerate a new tree with the\ncurrent parameters");
    panel.addLabel("Press [H] to hide this panel");
    panel.addLabel("Drag screen to move camera");
    panel.addLabel("Zoom controlled by slider");
    
    panel.addToggle("SAVE_SCREEN", false);
    
    //generation
    //panel.addPanel("Generation", 4, false);
    //panel.setWhichPanel("Generation");
    panel.setWhichPanel(1);
    
    panel.addSlider("MAX_DEPTH", 6, 1, 15);
    panel.addSlider("MIN_BRANCH_SCALE", 0.1, 0.04, 0.4);
    
    panel.addSlider("MAX_DENSITY", 0.55, 0, 1);
    
    panel.addSlider("MIN_CHILDREN_PER_BRANCH", 2, 0, 5);
    panel.addSlider("MAX_CHILDREN_PER_BRANCH", 3, 1, 5);
    panel.addSlider("MIN_STARTING_BRANCHES", 2, 1, 5);
    
    panel.addSlider("MIN_CHILD_SCALE", 0.5, 0.1, 1.5);
    panel.addSlider("MAX_CHILD_SCALE", 1.2, 0.1, 2);
    panel.addSlider("CHILD_ANGLE_RANGE_DEG", 45, 0, 135);
    
    panel.addSlider("ROOT_SCALE", 0.1, 0.15, 1);
    
    //setting the chances for each branch type
    panel.addPanel("Branches", 4, false);
    panel.setWhichPanel("Branches");
    panel.addLabel("Likelihood of each type");
    panel.addSlider("BRANCH_CHANCE_0", 0.5, 0, 1);
    panel.addSlider("BRANCH_CHANCE_1", 0.5, 0, 1);
    panel.addSlider("BRANCH_CHANCE_2", 0.5, 0, 1);
    panel.addSlider("BRANCH_CHANCE_3", 0.5, 0, 1);
    panel.addSlider("BRANCH_CHANCE_4", 0.5, 0, 1);
    panel.addSlider("BRANCH_CHANCE_5", 0.5, 0, 1);
    
    //adjusting the tree after generating
    panel.addPanel("Movement", 4, false);
    panel.setWhichPanel("Movement");
    panel.addSlider("SWAY_MIN_SPEED", 0.5, 0.1, 4);
    panel.addSlider("SWAY_MAX_SPEED", 1.5, 0.1, 4);
    panel.addSlider("SWAY_DIST", 0.03, 0, PI*0.1);
    panel.addSlider("ROOT_SWAY_SPEED", 0.5, 0, 1);
    panel.addSlider("ROOT_SWAY_DIST", 0.5, 0, 1);
    //breathing
    panel.addSlider("BREATHE_SPEED", 0, 0.1, 4);
    panel.addSlider("BREATHE_EXPAND", 0.0, 0, 0.05);
    panel.addSlider("BREATHE_COLOR_PUSH", 0.0, 0, 0.5);
    
    //setting the underlying image
    panel.addPanel("Ink Blot", 4, false);
    panel.setWhichPanel("Ink Blot");
    panel.addSlider("WHITE_SPACE_VAL", 5, 0, 20);
    panel.addSlider("MAX_UNUSED_SPACE", 600, 100, 1500);
    panel.addSlider("MAX_RESET_TRIES", 200, 1, 500);
    panel.addToggle("SHOW_INK_BLOT", false);
    
    usePreset(0);
    
    //new shit
    panel.setupEvents();
    panel.enableEvents();
    
    
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
    camScale = panel.getValueF("CAM_SCALE");
    camPos.set(755, ofGetHeight()/2 / panel.getValueF("CAM_SCALE"));
    
    reset(0);
    
    autoResetTimer = 0;
    
    prevFrameTime = ofGetElapsedTimef();
}


//--------------------------------------------------------------
void ofApp::update(){
    deltaTime = ofGetElapsedTimef()-prevFrameTime;
    prevFrameTime = ofGetElapsedTimef();
    
    panel.update();
    
    autoResetIsOn = panel.getValueB("auto_reset");
    autoResetTime = panel.getValueF("auto_reset_time");
    timeBetweenAutoResets = panel.getValueF("auto_reset_rest_time");
    
    //some values can be adjusted in between spawns
    camScale = panel.getValueF("CAM_SCALE");
    
    //moving the tree around
    root->panelSwayMinSpeed = panel.getValueF("SWAY_MIN_SPEED");
    root->panelSwayMaxSpeed = panel.getValueF("SWAY_MAX_SPEED");
    root->panelSwayDistRange = panel.getValueF("SWAY_DIST");
    //setting the root's values
    root->swaySpeed = panel.getValueF("ROOT_SWAY_SPEED");
    root->swayDist = panel.getValueF("ROOT_SWAY_DIST");
    
    //breathing values
    breatheSpeed = panel.getValueF("BREATHE_SPEED");
    breatheExpandSize = panel.getValueF("BREATHE_EXPAND");
    breatheColorPush = panel.getValueF("BREATHE_COLOR_PUSH");
    
    //check if they hit a preset button
    for (int i=0; i<presetNames.size(); i++){
        if (panel.getValueB(presetNames[i])){
            panel.setValueB(presetNames[i], false);
            usePreset(i);
            root->destroy();
            reset(0);
        }
    }
    
    //check if they hit randomize
    if (panel.getValueB("RANDOM_TREE")){
        panel.setValueB("RANDOM_TREE", false);
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
    if (panel.getValueB("SAVE_SCREEN")){
        panel.setValueB("SAVE_SCREEN", false);
        takingScreenShot = true;
    }
    
    
    //show the underlying ink blot if it's on
    if (panel.getValueB("SHOW_INK_BLOT")){
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
        panel.setValueB("SAVE_SCREEN", true);
    }
    
    if (key == 'i'){
        panel.setValueB("SHOW_INK_BLOT", !panel.getValueB("SHOW_INK_BLOT"));
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
        panel.setValueB("auto_reset", !autoResetIsOn);
    }
    
    //cout<<"key "<<key<<endl;
    for(int i=0; i<presetNames.size(); i++){
        if (key == 49+i){
            panel.setValueB(presetNames[i], true);
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
    //just turn one of the preset buttons on
    int presetID = (int)ofRandom(presetNames.size());
    panel.setValueB(presetNames[presetID], true);
    
}


//--------------------------------------------------------------
void ofApp::reset(int numTries){
    
    //clear the grid
    //curving the density value because there is much more granulaiton towards the end of the spectrum
    int maxDensity = 99 * powf(panel.getValueF("MAX_DENSITY"), 2) + 1;
    densityGrid.reset(maxDensity, panel.getValueI("WHITE_SPACE_VAL"));
    
    //set the branch tickets
    totalBranchTickets = 0;
    for (int i=0; i<NUM_BRANCH_TYPES; i++){
        branchTickets[i] = panel.getValueF("BRANCH_CHANCE_"+ofToString(i));
        totalBranchTickets += branchTickets[i];
    }
    
    minChildrenPerBranch = panel.getValueI("MIN_CHILDREN_PER_BRANCH");
    maxChildrenPerBranch = panel.getValueI("MAX_CHILDREN_PER_BRANCH");
    
    childMinScaleAdjust = panel.getValueF("MIN_CHILD_SCALE");
    childMaxScaleAdjust = panel.getValueF("MAX_CHILD_SCALE");
    childAngleRange = ofDegToRad(panel.getValueF("CHILD_ANGLE_RANGE_DEG"));
    
    //make the root
    root = getNewBranch(false);
    
    root->setAsRoot(origin, panel.getValueF("ROOT_SCALE"));
    densityGrid.addDensityMarkerToGrid(root->densityMarker, root->anchorPos, root->adjustedAngle, root->baseScale);
    
    
    fillTree(root, panel.getValueI("MAX_DEPTH"), panel.getValueF("MIN_BRANCH_SCALE"));
    
    densityGrid.setColumnVals();
    
    
    //set the camera starting place
    //using a while loop is dumb an ineficient
    //this is pretty much all magic numbers and stupidity
    camPos.x = 900/camScale;
    float rootAnchorX = (root->anchorPos.x + camPos.x) * camScale;
    while (rootAnchorX > ofGetWidth()/2){
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
        numKids = MAX(numKids, panel.getValueI("MIN_STARTING_BRANCHES"));
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
    
    //standard
    if (presetNum==0){
        panel.setValueI("MAX_DEPTH", 4);
        panel.setValueF("MIN_BRANCH_SCALE", 0.24);
        
        panel.setValueF("MAX_DENSITY", 0.7);//0.28);
        
        panel.setValueI("MIN_CHILDREN_PER_BRANCH", 3);
        panel.setValueI("MAX_CHILDREN_PER_BRANCH", 3);
        panel.setValueI("MIN_STARTING_BRANCHES", 5);
        
        panel.setValueF("MIN_CHILD_SCALE", 0.61);
        panel.setValueF("MAX_CHILD_SCALE", 1.01);
        panel.setValueF("CHILD_ANGLE_RANGE_DEG", 111);
        
        panel.setValueF("ROOT_SCALE", 0.702);
        
        //setting the chances for each branch type
        panel.setValueF("BRANCH_CHANCE_0", 0.5);
        panel.setValueF("BRANCH_CHANCE_1", 0.5);
        panel.setValueF("BRANCH_CHANCE_2", 0.5);
        panel.setValueF("BRANCH_CHANCE_3", 0.5);
        panel.setValueF("BRANCH_CHANCE_4", 0.35);
        panel.setValueF("BRANCH_CHANCE_5", 0.35);
        
        //adjusting the tree after generating
        panel.setValueF( "SWAY_MIN_SPEED", 0.5);
        panel.setValueF("SWAY_MAX_SPEED", 1.5);
        panel.setValueF("SWAY_DIST", 0.03);
        panel.setValueF("ROOT_SWAY_SPEED", 0.5);
        panel.setValueF("ROOT_SWAY_DIST", 0.5);
        
        //breathing
        panel.setValueF("BREATHE_SPEED", 1.1);
        panel.setValueF("BREATHE_EXPAND", 0.011);
        panel.setValueF("BREATHE_COLOR_PUSH", 0.135);
    }
    
    //brambles
    if (presetNum==1){
        panel.setValueI("MAX_DEPTH", 8);
        panel.setValueF("MIN_BRANCH_SCALE", 0.084);
        
        panel.setValueF("MAX_DENSITY", 0.6);
        
        panel.setValueI("MIN_CHILDREN_PER_BRANCH", 2);
        panel.setValueI("MAX_CHILDREN_PER_BRANCH", 4);
        panel.setValueI("MIN_STARTING_BRANCHES", 3);
        
        panel.setValueF("MIN_CHILD_SCALE", 0.64);
        panel.setValueF("MAX_CHILD_SCALE", 1.02);
        panel.setValueF("CHILD_ANGLE_RANGE_DEG", 63);
        
        panel.setValueF("ROOT_SCALE", 0.42);
        
        //setting the chances for each branch type
        panel.setValueF("BRANCH_CHANCE_0", 0.15);
        panel.setValueF("BRANCH_CHANCE_1", 0.15);
        panel.setValueF("BRANCH_CHANCE_2", 0.15);
        panel.setValueF("BRANCH_CHANCE_3", 0.5);
        panel.setValueF("BRANCH_CHANCE_4", 0.5);
        panel.setValueF("BRANCH_CHANCE_5", 0.5);
        
        //adjusting the tree after generating
        panel.setValueF( "SWAY_MIN_SPEED", 0.56);
        panel.setValueF("SWAY_MAX_SPEED", 0.82);
        panel.setValueF("SWAY_DIST", 0.12);
        panel.setValueF("ROOT_SWAY_SPEED", 0.5);
        panel.setValueF("ROOT_SWAY_DIST", 0.5);
    }
    
    //vines
    if (presetNum==2){
        panel.setValueI("MAX_DEPTH", 10);
        panel.setValueF("MIN_BRANCH_SCALE", 0.1);
        
        panel.setValueF("MAX_DENSITY", 0.1);
        
        panel.setValueI("MIN_CHILDREN_PER_BRANCH", 1);
        panel.setValueI("MAX_CHILDREN_PER_BRANCH", 3);
        panel.setValueI("MIN_STARTING_BRANCHES", 4);
        
        panel.setValueF("MIN_CHILD_SCALE", 0.54);
        panel.setValueF("MAX_CHILD_SCALE", 1.37);
        panel.setValueF("CHILD_ANGLE_RANGE_DEG", 49);
        
        panel.setValueF("ROOT_SCALE", 0.35);
        
        //setting the chances for each branch type
        panel.setValueF("BRANCH_CHANCE_0", 0.19);
        panel.setValueF("BRANCH_CHANCE_1", 0.19);
        panel.setValueF("BRANCH_CHANCE_2", 0.0);
        panel.setValueF("BRANCH_CHANCE_3", 0.0);
        panel.setValueF("BRANCH_CHANCE_4", 0.74);
        panel.setValueF("BRANCH_CHANCE_5", 0.82);
        
        //adjusting the tree after generating
        panel.setValueF( "SWAY_MIN_SPEED", 0.23);
        panel.setValueF("SWAY_MAX_SPEED", 1.1);
        panel.setValueF("SWAY_DIST", 0.06);
        panel.setValueF("ROOT_SWAY_SPEED", 0.5);
        panel.setValueF("ROOT_SWAY_DIST", 0.5);
    }
    
    //open
    if (presetNum==3){
        panel.setValueI("MAX_DEPTH", 8);
        panel.setValueF("MIN_BRANCH_SCALE", 0.152);
        
        panel.setValueF("MAX_DENSITY", 0.13);
        
        panel.setValueI("MIN_CHILDREN_PER_BRANCH", 3);
        panel.setValueI("MAX_CHILDREN_PER_BRANCH", 3);
        panel.setValueI("MIN_STARTING_BRANCHES", 3);
        
        panel.setValueF("MIN_CHILD_SCALE", 0.82);
        panel.setValueF("MAX_CHILD_SCALE", 1.18);
        panel.setValueF("CHILD_ANGLE_RANGE_DEG", 70);
        
        panel.setValueF("ROOT_SCALE", 0.35);
        
        //setting the chances for each branch type
        panel.setValueF("BRANCH_CHANCE_0", 0.66);
        panel.setValueF("BRANCH_CHANCE_1", 0.61);
        panel.setValueF("BRANCH_CHANCE_2", 0.61);
        panel.setValueF("BRANCH_CHANCE_3", 0.61);
        panel.setValueF("BRANCH_CHANCE_4", 0.26);
        panel.setValueF("BRANCH_CHANCE_5", 0.30);
        
        //adjusting the tree after generating
        panel.setValueF( "SWAY_MIN_SPEED", 0.62);
        panel.setValueF("SWAY_MAX_SPEED", 1.55);
        panel.setValueF("SWAY_DIST", 0.045);
        panel.setValueF("ROOT_SWAY_SPEED", 0.4);
        panel.setValueF("ROOT_SWAY_DIST", 0.9);
    }
    
    //the beast
    if (presetNum==4){
        panel.setValueI("MAX_DEPTH", 8);
        panel.setValueF("MIN_BRANCH_SCALE", 0.20);
        
        panel.setValueF("MAX_DENSITY", 0.41);
        
        panel.setValueI("MIN_CHILDREN_PER_BRANCH", 1);
        panel.setValueI("MAX_CHILDREN_PER_BRANCH", 3);
        panel.setValueI("MIN_STARTING_BRANCHES", 4);
        
        panel.setValueF("MIN_CHILD_SCALE", 0.69);
        panel.setValueF("MAX_CHILD_SCALE", 1.14);
        panel.setValueF("CHILD_ANGLE_RANGE_DEG", 55);
        
        panel.setValueF("ROOT_SCALE", 0.32);
        
        //setting the chances for each branch type
        panel.setValueF("BRANCH_CHANCE_0", 0.5);
        panel.setValueF("BRANCH_CHANCE_1", 0.5);
        panel.setValueF("BRANCH_CHANCE_2", 0.5);
        panel.setValueF("BRANCH_CHANCE_3", 0.5);
        panel.setValueF("BRANCH_CHANCE_4", 0.5);
        panel.setValueF("BRANCH_CHANCE_5", 0.5);
        
        //adjusting the tree after generating
        panel.setValueF("SWAY_MIN_SPEED", 3.11);
        panel.setValueF("SWAY_MAX_SPEED", 4.0);
        panel.setValueF("SWAY_DIST", 0.314);
        panel.setValueF("ROOT_SWAY_SPEED", 0.66);
        panel.setValueF("ROOT_SWAY_DIST", 0.72);
    }
    
    //the tall grass
    if (presetNum==5){
        panel.setValueI("MAX_DEPTH", 10);
        panel.setValueF("MIN_BRANCH_SCALE", 0.130);
        
        panel.setValueF("MAX_DENSITY", 0.70);
        
        panel.setValueI("MIN_CHILDREN_PER_BRANCH", 2);
        panel.setValueI("MAX_CHILDREN_PER_BRANCH", 3);
        panel.setValueI("MIN_STARTING_BRANCHES", 2);
        
        panel.setValueF("MIN_CHILD_SCALE", 0.38);
        panel.setValueF("MAX_CHILD_SCALE", 1.23);
        panel.setValueF("CHILD_ANGLE_RANGE_DEG", 61);
        
        panel.setValueF("ROOT_SCALE", 0.35);
        
        //setting the chances for each branch type
        panel.setValueF("BRANCH_CHANCE_0", 0.07);
        panel.setValueF("BRANCH_CHANCE_1", 0.07);
        panel.setValueF("BRANCH_CHANCE_2", 0.0);
        panel.setValueF("BRANCH_CHANCE_3", 0.0);
        panel.setValueF("BRANCH_CHANCE_4", 1.0);
        panel.setValueF("BRANCH_CHANCE_5", 0.0);
        
        //adjusting the tree after generating
        panel.setValueF("SWAY_MIN_SPEED", 0.5);
        panel.setValueF("SWAY_MAX_SPEED", 1.5);
        panel.setValueF("SWAY_DIST", 0.01);
        panel.setValueF("ROOT_SWAY_SPEED", 1.0);
        panel.setValueF("ROOT_SWAY_DIST", 1.0);
    }
    
    //path - THIS ONE SUCKS!
    if (presetNum==6){
        panel.setValueI("MAX_DEPTH", 12);
        panel.setValueF("MIN_BRANCH_SCALE", 0.04);
        
        panel.setValueF("MAX_DENSITY", 1);
        
        panel.setValueI("MIN_CHILDREN_PER_BRANCH", 0);
        panel.setValueI("MAX_CHILDREN_PER_BRANCH", 2);
        panel.setValueI("MIN_STARTING_BRANCHES", 1);
        
        panel.setValueF("MIN_CHILD_SCALE", 0.98);
        panel.setValueF("MAX_CHILD_SCALE", 1.08);
        panel.setValueF("CHILD_ANGLE_RANGE_DEG", 37);
        
        panel.setValueF("ROOT_SCALE", 0.3);
        
        //setting the chances for each branch type
        panel.setValueF("BRANCH_CHANCE_0", 0.5);
        panel.setValueF("BRANCH_CHANCE_1", 0.5);
        panel.setValueF("BRANCH_CHANCE_2", 0.17);
        panel.setValueF("BRANCH_CHANCE_3", 0.17);
        panel.setValueF("BRANCH_CHANCE_4", 0.5);
        panel.setValueF("BRANCH_CHANCE_5", 0.5);
        
        //adjusting the tree after generating
        panel.setValueF("SWAY_MIN_SPEED", 0.5);
        panel.setValueF("SWAY_MAX_SPEED", 1.5);
        panel.setValueF("SWAY_DIST", 0.03);
        panel.setValueF("ROOT_SWAY_SPEED", 0.5);
        panel.setValueF("ROOT_SWAY_DIST", 0.5);
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



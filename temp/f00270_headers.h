// f00293_gamesound.e
//

#ifndef LZZ_f00293_gamesound_e
#define LZZ_f00293_gamesound_e
#define LZZ_INLINE inline
class GameSound
{
public:
  sf::SoundBuffer buffer;
  sf::Sound sound;
  GameSound ();
  void init (string path);
  void setPitch (float val);
  void setPositionAndMinDis (float x, float y, float z, float w);
  void setLoop (bool val);
  void stop ();
  void play (float volume = 1.0f);
};
#undef LZZ_INLINE
#endif
// f00294_gamemusic.e
//

#ifndef LZZ_f00294_gamemusic_e
#define LZZ_f00294_gamemusic_e
#define LZZ_INLINE inline
class GameMusic
{
public:
  sf::Music sound;
  float volume;
  GameMusic ();
  void init (string path);
  void setLoop (bool val);
  void stop ();
  void setVolume (float _volume = 1.0f);
  void play (float _volume = 1.0f);
};
#undef LZZ_INLINE
#endif
// f00297_gamecamera.e
//

#ifndef LZZ_f00297_gamecamera_e
#define LZZ_f00297_gamecamera_e
#define LZZ_INLINE inline
class GameCamera
{
private:
public:
  GLfloat (unitPos) [3];
  float (accel) [3];
  float accelA;
  float accelB;
  float lastAccelA;
  float lastAccelB;
  float subjectDistance;
  GLfloat (rotation) [2];
  GameCamera ();
  void focusOn (int entID);
  void testCollision (float oldX, float oldY, float oldZ, float newX, float newY, float newZ, bool skipTest);
  void setUnitPosition (float x, float y, float z);
  void addUnitPosition (float x, float y, float z);
  void setRotation (float a, float b);
  void addRotation (float a, float b);
  void init ();
};
#undef LZZ_INLINE
#endif
// f00300_singleton.e
//

#ifndef LZZ_f00300_singleton_e
#define LZZ_f00300_singleton_e
#define LZZ_INLINE inline
class Singleton
{
public:
  struct UIQuad
  {
    fBoundingBox hitBounds;
    CharStruct * cs;
    int fontId;
  };
  struct UICont
  {
    UIComponent * uiComp;
    UIQuad bg;
    std::vector <UIQuad> charVec;
  };
  struct JSONStruct
  {
    JSONValue * jv;
  };
  struct UICStruct
  {
    UIComponent * uic;
    int nodeId;
  };
  struct CompareStruct
  {
    bool operator () (string const & first, string const & second);
  };
  CompareStruct compareStruct;
  typedef map <string, UICStruct>::iterator itUICStruct;
  typedef map <string, JSONStruct>::iterator itJSStruct;
  GameCamera * mainCamera;
  bool (keysPressed) [MAX_KEYS];
  double (keyDownTimes) [MAX_KEYS];
  unsigned char (keyMap) [MAX_KEYS];
  GLdouble (viewMatrixD) [16];
  GLdouble (projMatrixD) [16];
  Matrix4 viewMatrix;
  Matrix4 projMatrix;
  GLint (viewport) [4];
  E_OBJ activeObject;
  E_OBJ tempObj;
  eProgramState programState;
  eProgramAction (progActionsDown) [E_PS_SIZE * 256];
  eProgramAction (progActionsUp) [E_PS_SIZE * 256];
  EntSelection nearestLights;
  EntSelection highlightedEnts;
  EntSelection selectedEnts;
  GameEnt * selectedEnt;
  GameEnt * highlightedEnt;
  bool updateMatFlag;
  bool matVolLock;
  bool isMoving;
  bool perspectiveOn;
  bool (isInteractiveEnt) [E_CT_LENGTH];
  bool inputOn;
  bool pathfindingOn;
  bool isMacro;
  bool orgOn;
  bool autoScroll;
  bool cavesOn;
  bool bakeParamsOn;
  bool dragging;
  bool mouseMoved;
  bool changingGenVal;
  bool readyForRestart;
  bool forceRestart;
  bool hitGUI;
  bool draggingMap;
  bool guiLock;
  bool guiDirty;
  bool mirrorOn;
  bool applyToChildren;
  bool bShiftOld;
  bool bCtrlOld;
  bool bCtrl;
  bool bShift;
  bool testOn;
  bool emptyVDNotReady;
  bool radiosityOn;
  bool updateLock;
  bool isFullScreen;
  bool mapInvalid;
  bool wsBufferInvalid;
  bool forceGetPD;
  bool mouseLeftDown;
  bool mouseRightDown;
  bool pboSupported;
  bool notQuit;
  bool timerNotSet;
  bool lbDown;
  bool abDown;
  bool rbDown;
  bool mbDown;
  bool isBare;
  bool showMap;
  bool traceOn;
  bool waterOn;
  bool treesOn;
  bool firstRun;
  bool rotOn;
  bool doPageRender;
  bool markerFound;
  bool editPose;
  int fieldCallback;
  int mouseState;
  int lastW;
  int lastH;
  int (cdMap) [256];
  int maxLayerOver;
  int holderResolution;
  int visPageSizeInUnits;
  int bufferedPageSizeInUnits;
  int unitSizeInPixels;
  int holdersPerLot;
  int pixelsPerLot;
  int pixelsPerCell;
  int visPageSizeInPixels;
  int holderSizeInPixels;
  int currentStep;
  int maxChangesInPages;
  int maxChangesInHolders;
  int baseW;
  int baseH;
  int scaleFactor;
  int numDynLights;
  int iNumSteps;
  int curOrgId;
  int cellsPerLot;
  int extraRad;
  int defaultWinW;
  int defaultWinH;
  int guiWinW;
  int guiWinH;
  int shadersAreLoaded;
  int readyToRecompile;
  int lastPosX;
  int lastPosY;
  int frameCount;
  int maxHInPages;
  int maxWInPages;
  int screenWidth;
  int screenHeight;
  int mouseMovingSize;
  int mouseMovingLoc;
  int mouseMovingStepsBack;
  int mouseCount;
  int lastMouseX;
  int lastMouseY;
  int cellsPerNodeXY;
  int terDataVisPitchXY;
  int terDataBufPitchXY;
  int terDataBufPitchScaledXY;
  int cellsPerNodeZ;
  int terDataVisPitchZ;
  int terDataBufPitchZ;
  int terDataBufPitchScaledZ;
  int terDataBufAmount;
  int terDataVisSize;
  int terDataBufSize;
  int terDataTexScale;
  int terDataBufSizeScaled;
  int iNodeDivsPerLot;
  int holderSizeInPages;
  int volGenSuperMod;
  int volGenSuperRes;
  int matVolSize;
  int escCount;
  int * cdBuffer;
  intPair (entIdArr) [1024];
  uint palWidth;
  uint palHeight;
  uint blockShift;
  uint volGenFBOX;
  uint * terDataScaled;
  float lastx;
  float lasty;
  float FOV;
  float focalLength;
  float zoomDelta;
  float subjectDelta;
  float subjectZoom;
  float voxelSizeInWC;
  float msPerFrame;
  float cameraZoom;
  float targetZoom;
  float fogOn;
  float gridOn;
  float mapSampScale;
  float curBrushRad;
  float timeOfDay;
  float targetTimeOfDay;
  float gridSizeInPixels;
  float origWinW;
  float origWinH;
  float guiX;
  float guiY;
  float aspectRatio;
  float currentFBOResolutionX;
  float currentFBOResolutionY;
  float mouseX;
  float mouseY;
  float mouseXUp;
  float mouseYUp;
  float bufferMult;
  float holderSizeMB;
  float bestNodeDis;
  float heightOfNearPlane;
  float scrollDiv;
  float curMoveSpeed;
  float curMoveAccel;
  float masterVolume;
  float ambientVolume;
  float guiVolume;
  float musicVolume;
  float fxVolume;
  float * paramArr;
  float * voroArr;
  float * matCountArr;
  float * paramArrMap;
  float (clipDist) [2];
  double lastMoveTime;
  double curTime;
  double lastTime;
  double mdTime;
  double muTime;
  GameOrgNode * bestNode;
  GameOrgNode * selectedNode;
  GameOrgNode * lastSelNode;
  GameOrgNode * activeNode;
  FIVector4 targetCameraPos;
  FIVector4 lastCellPos;
  FIVector4 worldMarker;
  FIVector4 lookAtVec;
  FIVector4 targetLookAtVec;
  FIVector4 baseCameraPos;
  FIVector4 cameraPosAdjusted;
  FIVector4 baseScrollPos;
  FIVector4 worldSizeInPixels;
  FIVector4 mouseUpPD;
  FIVector4 spaceUpPD;
  FIVector4 mouseDownPD;
  FIVector4 mouseObjPD;
  FIVector4 mouseMovePD;
  FIVector4 tempVec1;
  FIVector4 tempVec2;
  FIVector4 tempVec3;
  FIVector4 worldSizeInTerData;
  FIVector4 worldSizeInUnits;
  FIVector4 worldSizeInPages;
  FIVector4 worldSizeInLots;
  FIVector4 worldSizeInCells;
  FIVector4 worldSizeInHolders;
  FIVector4 worldSizeInBlocks;
  FIVector4 (moveNodes) [2];
  FIVector4 (voroVecArr) [125];
  floatAndIndex (indexArr) [125];
  FIVector4 mouseStart;
  FIVector4 mouseEnd;
  FIVector4 mapFreqs;
  FIVector4 mapAmps;
  FIVector4 * mouseMoving;
  FIVector4 mouseVel;
  FIVector4 bufferDim;
  FIVector4 bufferDimTarg;
  FIVector4 bufferDimHalf;
  FIVector4 bufferModDim;
  FIVector4 origin;
  FIVector4 lastModXYZ;
  FIVector4 panMod;
  FIVector4 dMod;
  FIVector4 modXYZ;
  FIVector4 matVolDim;
  uint * matVol;
  std::vector <UICont*> (guiLayers) [MAX_UI_LAYERS];
  std::vector <RotationInfo> rotMatStack;
  std::vector <DynObject *> dynObjects;
  PathHolder charPathHolder;
  PathHolder splitPathHolder;
  float floorHeightInCells;
  float roofHeightInCells;
  float wallRadInCells;
  int cellsPerHolder;
  int cellsPerPage;
  int unitsPerCell;
  int blockSizeInLots;
  int blockSizeInCells;
  int blockSizeInHolders;
  int blockSizeInPages;
  int blockSizeInPixels;
  Image * imageHM0;
  Image * imageHM1;
  Image * cloudImage;
  GameEnt baseEnt;
  GameEnt * currentActor;
  GamePlant * (gamePlants) [E_PT_LENGTH/2];
  Shader * curShaderPtr;
  string currentFieldString;
  string curShader;
  string allText;
  string stringBuf;
  string curCallback;
  string (cbDataStrings) [10];
  string guiSaveLoc;
  PoolManager * gpuPool;
  PoolManager * entityPool;
  vector <UIComponent*> compStack;
  vector <string> splitStrings;
  vector <string> shaderStrings;
  vector <string> shaderTextureIds;
  map <string, Shader*> shaderMap;
  map <string, UICStruct> compMap;
  map <string, string> includeMap;
  map <string, FBOSet> fboMap;
  FBOSet (vgFBOArr) [MAX_VG_FBOS];
  FBOSet (vgtFBOArr) [MAX_VGT_FBOS];
  GLuint fsqDL;
  GLuint volId;
  GLuint volIdLinear;
  GLuint volIdEmpty;
  GLuint volIdEmptyLinear;
  GLuint volIdMat;
  GLuint volGenId;
  uint * lookup2to3;
  unsigned char * resultImage;
  materialNode * matSlice0;
  materialNode * matSlice1;
  materialNode * matSlice2;
  charArr nullBuffer;
  charArr lastImageBuffer;
  charArr lastJSONBufferGUI;
  JSONValue * rootObjJS;
  JSONValue * guiRootJS;
  Timer myTimer;
  Timer scrollTimer;
  Timer moveTimer;
  GameWorld * gw;
  float (lightArr) [MAX_LIGHTS * 16];
  int numLights;
  bool multiLights;
  int * rbStack;
  int * rbHeightStack;
  TerTexture (terTextures) [MAX_TER_TEX];
  GameOrg * testHuman;
  GameGUI * mainGUI;
  UIComponent * mapComp;
  UIComponent * mainMenu;
  UIComponent * ddMenu;
  UIComponent * fieldMenu;
  UIComponent * fieldText;
  FontWrapper * (fontWrappers) [EFW_LENGTH];
  GameMusic * (music) [EML_LENGTH];
  map <string, GameSound> soundMap;
  map <string, StyleSheet> styleSheetMap;
  map <string, JSONStruct> internalJSON;
  map <string, JSONStruct> externalJSON;
  Singleton ();
  void init (int _defaultWinW, int _defaultWinH, int _scaleFactor);
  void prepSound (string soundName);
  void playSoundPosAndPitch (string soundName, FIVector4 * listenerPos, FIVector4 * soundPos, float variance = 0.0f, float volume = 1.0f);
  void playSound (string soundName, float volume = 1.0f);
  void playSoundEvent (char const * eventName, bool suppress = false);
  void setCurrentActor (GameEnt * ge);
  void updateMatVol ();
  void dispatchEvent (int button, int state, float x, float y, UIComponent * comp, bool automated = false, bool preventRefresh = false);
  StyleSheet * getNewStyleSheet (string ssName);
  void initStyleSheet ();
  int requestTerIndex (int requestingBlockId);
  static void qNormalizeAngle (int & angle);
  ~ Singleton ();
  void setProgAction (eProgramState ps, unsigned char kc, eProgramAction pa, bool isDown);
  void setProgActionAll (unsigned char kc, eProgramAction pa, bool isDown);
  void drawCrossHairs (FIVector4 originVec, float radius);
  void drawLine (FIVector4 * p0, FIVector4 * p1);
  void drawCubeCentered (FIVector4 * originVec, float radius);
  void drawBoxUp (FIVector4 originVec, float radiusX, float radiusY, float diamZ);
  void drawBox (FIVector4 * minVec, FIVector4 * maxVec, int faceFlag = 2);
  void getMaterialString ();
  void refreshIncludeMap ();
  void doShaderRefresh (bool doBake);
  void setWH (int w, int h);
  void sampleFBODirect (FBOSet * fbos, int offset = 0);
  void unsampleFBODirect (FBOSet * fbos, int offset = 0);
  void bindFBODirect (FBOSet * fbos, int doClear = 1);
  FBOSet * getFBOByName (string & fboName);
  void sampleFBO (string fboName, int offset = 0, int swapFlag = -1);
  void unsampleFBO (string fboName, int offset = 0, int swapFlag = -1);
  FBOSet * getFBOSet (string fboName);
  FBOWrapper * getFBOWrapper (string fboName, int offset);
  void copyFBO (string src, string dest, int num = 0);
  void copyFBO2 (string src, string dest);
  void bindFBO (string fboName, int swapFlag = -1);
  void unbindFBO ();
  void bindShader (string shaderName);
  void unbindShader ();
  void setShaderArrayfVec3 (string paramName, float * x, int count);
  void setShaderArrayfVec4 (string paramName, float * x, int count);
  void setShaderMatrix4x4 (string paramName, float * x, int count);
  void setShaderArray (string paramName, float * x, int count);
  GLint getShaderLoc (string paramName);
  void setShaderFloat (string paramName, float x);
  void setShaderInt (string paramName, int x);
  void setShaderfVec2 (string paramName, FIVector4 * v);
  void setShaderVec2 (string paramName, float x, float y);
  void setShaderVec3 (string paramName, float x, float y, float z);
  void setShaderfVec3 (string paramName, FIVector4 * v);
  void setShaderVec4 (string paramName, float x, float y, float z, float w);
  void setShaderfVec4 (string paramName, FIVector4 * v);
  void setShaderFloatUB (string paramName, float x);
  void setShaderfVec4UB (string paramName, FIVector4 * v);
  void updateUniformBlock (int ubIndex, int ubDataSize = -1);
  void invalidateUniformBlock (int ubIndex);
  void beginUniformBlock (int ubIndex);
  bool wasUpdatedUniformBlock (int ubIndex);
  void setShaderTexture (int multitexNumber, uint texId);
  void setShaderTexture3D (int multitexNumber, uint texId);
  bool shiftDown ();
  bool ctrlDown ();
  bool altDown ();
  void drawQuadBounds (float fx1, float fy1, float fx2, float fy2, float fz);
  void drawFSQuad ();
  void drawFSQuadOffset (float xOff, float yOff, float zm);
  void drawFBO (string fboName, int ind, float zm, int swapFlag = -1);
  void drawFBOOffsetDirect (FBOSet * fbos, int ind, float xOff, float yOff, float zm);
  void drawFBOOffset (string fboName, int ind, float xOff, float yOff, float zm);
  float getTerHeightScaled (float val);
  float getSLNormalized ();
  float getSLInPixels ();
  float getHeightAtPixelPos (float x, float y, bool dd = false);
  void transformOrg (GameOrg * curOrg);
  void angleToVec (FIVector4 * fv, float xr, float yr);
  void vecToAngle (FIVector4 * fv, FIVector4 * ta);
  void syncObjects (FIVector4 * bp);
  void updateCamVals ();
  void moveCamera (FIVector4 * pModXYZ);
  GameOrgNode * getMirroredNode (GameOrgNode * curNode);
  void applyNodeChanges (GameOrgNode * _curNode, float dx, float dy);
  void moveObject (float dx, float dy);
  void updateMultiLights ();
  void toggleFullScreen ();
  void restartGen (bool instantRefresh, bool clearEverything);
  void setObjToElevationBase (FIVector4 * obj);
  void setCameraToElevationBase ();
  void setCameraToElevation ();
  void processSpecialKeys (int key, int _x, int _y);
  void updateCS ();
  void processInput (unsigned char key, bool keyDown, int x, int y);
  void keyboardUp (unsigned char key, int _x, int _y);
  void keyboardDown (unsigned char key, int _x, int _y);
  void runReport ();
  void getPixData (FIVector4 * toVector, int _xv, int _yv, bool forceUpdate, bool isObj);
  void mouseMove (int _x, int _y);
  void makeDirty ();
  void setSelNode (GameOrgNode * newNode);
  void worldToScreenBase (FIVector4 * sc, FIVector4 * wc);
  float getShortestAngle (float begInRad, float endInRad, float amount);
  void handleMovement ();
  bool anyMenuVisible ();
  void mouseClick (int button, int state, int _x, int _y);
  void resetActiveNode ();
  void updateNearestOrgNode (bool setActive, FIVector4 * mousePosWS);
  void findNearestOrgNode (GameOrgNode * curNode, FIVector4 * mousePosWS);
  void getJVNodeByString (JSONValue * rootNode, JSONValue * * resultNode, string stringToSplit);
  JSONValue * fetchJSONData (string dataFile);
  bool processJSONFromString (string * sourceBuffer, JSONValue * * destObj);
  bool processJSON (charArr * sourceBuffer, charArr * saveBuffer, JSONValue * * destObj);
  bool loadJSON (string path, JSONValue * * destObj);
  void setGUIText (string key, string stringValue, float floatValue = 0.0f, bool applyVal = false, bool applyString = true);
  float getGUIValue (string key);
  UIComponent * getGUIComp (string key);
  void setGUIValue (string key, float floatValue, bool dispatchEvent = false, bool preventRefresh = false);
  void loadGUIValues (bool applyValues = false);
  void saveExternalJSON ();
  void saveGUIValues ();
  void updateGUI ();
  void beginFieldInput (string defString, int cb);
  void processFieldInput (unsigned char key);
  void endFieldInput (bool success);
  void saveOrg ();
  void loadOrg ();
  void loadGUI ();
  bool loadFile (string fnString, charArr * dest);
  bool saveFileString (string fileName, string * source);
  bool saveFile (char * fileName, charArr * source);
  void updateAmbientSounds ();
  void frameUpdate ();
  void display ();
  void setMatrices (int w, int h);
  void reshape (int w, int h);
  void idleFunc ();
};
#undef LZZ_INLINE
#endif
// f00305_shader.e
//

#ifndef LZZ_f00305_shader_e
#define LZZ_f00305_shader_e
#define LZZ_INLINE inline
class Shader
{
private:
  unsigned int shader_id;
  unsigned int shader_vp;
  unsigned int shader_fp;
public:
  int curUBIndex;
  map <string, float> paramMap;
  map <string, float> paramMapTemp;
  vector <string> paramVec;
  vector <UniformBuffer> uniVec;
  Singleton * singleton;
  Shader (Singleton * _singleton);
  static char * textFileRead (char const * fileName);
  static void validateShader (GLuint shader, char const * file = 0);
  static int validateProgram (GLuint program);
  int countOc (string * src, string testStr);
  void init (string _shaderFile, bool doBake, map <string, string> * includeMap);
  ~ Shader ();
  unsigned int id ();
  void bind ();
  void updateUniformBlock (int ubIndex, int ubDataSize);
  void invalidateUniformBlock (int ubIndex);
  void beginUniformBlock (int ubIndex);
  bool wasUpdatedUniformBlock (int ubIndex);
  void unbind ();
  void setTexture (GLchar const * name, int texUnit, int texId);
  void setVec (GLchar const * name, GLfloat const * vecData, int vecSize);
  void setVecString (string name, GLfloat const * vecData, int vecSize);
  void setShaderMatrix4x4 (string paramName, float * x, int count);
  void setShaderArrayfVec4 (string paramName, float * x, int count);
  void setShaderArrayfVec3 (string paramName, float * x, int count);
  void setShaderArray (string paramName, float * x, int count);
  GLint getShaderLoc (string paramName);
  void setShaderFloat (string paramName, float x);
  void setShaderVec2 (string paramName, float x, float y);
  void setShaderVec3 (string paramName, float x, float y, float z);
  void setShaderVec4 (string paramName, float x, float y, float z, float w);
  void setShaderInt (string paramName, int x);
  void setShaderfVec2 (string paramName, FIVector4 * f);
  void setShaderfVec3 (string paramName, FIVector4 * f);
  void setShaderfVec4 (string paramName, FIVector4 * f);
  void setShaderFloatUB (string paramName, float x);
  void setShaderfVec4UB (string paramName, FIVector4 * f);
};
#undef LZZ_INLINE
#endif
// f00310_pooledresource.e
//

#ifndef LZZ_f00310_pooledresource_e
#define LZZ_f00310_pooledresource_e
#define LZZ_INLINE inline
class PooledResource
{
private:
  FBOSet * (fboSet) [MAX_LAYERS];
  uint * (cpuSet) [MAX_LAYERS];
public:
  intPair usedById;
  bool isCPU;
  Singleton * singleton;
  GLuint (holderDL) [MAX_MIP_LEV*MAX_LAYERS];
  bool listGenerated;
  PooledResource ();
  FBOSet * getFBOS (int fboNum);
  void init (Singleton * _singleton, bool _isCPU, int _sizeX, int _sizeY);
  void bindHolderDL (int mipLev, int layer);
  void unbindHolderDL (int mipLev, int layer);
};
#undef LZZ_INLINE
#endif
// f00315_poolmanager.e
//

#ifndef LZZ_f00315_poolmanager_e
#define LZZ_f00315_poolmanager_e
#define LZZ_INLINE inline
class PoolManager
{
public:
  int poolItemsCreated;
  bool isCPU;
  list <int> holderPoolIds;
  vector <intPair> orderedIds;
  vector <PooledResource *> holderPoolItems;
  Singleton * singleton;
  GameWorld * gw;
  bool isEntity;
  int sizeX;
  int sizeY;
  PoolManager ();
  void init (Singleton * _singleton, bool _isEntity, bool _isCPU, int _sizeX, int _sizeY);
  float getMaxMem ();
  float getTotMemUsed ();
  void reorderIds ();
  int findFurthestHolderId ();
  int requestPoolId (int blockId, int holderId);
};
#undef LZZ_INLINE
#endif
// f00316_poolpagemanager.e
//

#ifndef LZZ_f00316_poolpagemanager_e
#define LZZ_f00316_poolpagemanager_e
#define LZZ_INLINE inline
class PoolPageManager
{
public:
  PoolPageManager ();
};
#undef LZZ_INLINE
#endif
// f00320_fontwrapper.e
//

#ifndef LZZ_f00320_fontwrapper_e
#define LZZ_f00320_fontwrapper_e
#define LZZ_INLINE inline
class FontWrapper
{
public:
  Singleton * singleton;
  charArr lastJSONBuffer;
  JSONValue * jsRoot;
  Image * fontImage;
  float ascender;
  float descender;
  float fontHeight;
  float maxWidth;
  float fontScale;
  float additionalOffset;
  bool isIcons;
  CharStruct (charVals) [4096];
  FontWrapper ();
  void init (Singleton * _singleton, string fontName, bool _isIcons, float _fontScale, float _additionalOffset = 0.0f);
};
#undef LZZ_INLINE
#endif
// f00325_uicomponent.e
//

#ifndef LZZ_f00325_uicomponent_e
#define LZZ_f00325_uicomponent_e
#define LZZ_INLINE inline
class UIComponent
{
private:
  UIComponent * valuePtr;
  float privValueX;
  float privValueY;
  std::vector <int> _children;
  std::vector <int> _floatingChildren;
public:
  Singleton * singleton;
  Singleton::UIQuad thisUIQuad;
  Singleton::UICont thisUICont;
  string uid;
  string ss;
  string text;
  string label;
  string dataFile;
  string dataRef;
  string dataKey;
  string valRef;
  int matCode;
  int parentId;
  int nodeId;
  int index;
  int fillDir;
  int layer;
  int hoverType;
  int guiClass;
  Singleton::UIQuad * curQuad;
  bool selected;
  bool foundParent;
  bool foundValuePtr;
  bool visible;
  iVector2 align;
  FIVector4 valVec;
  FIVector4 valVecMask;
  JSONValue * jvNodeNoTemplate;
  fVector2 scrollMaskY;
  fVector2 dragStart;
  fVector2 lastDrag;
  fVector2 dragOffset;
  fVector2 totOffset;
  fVector2 targScrollOffset;
  fVector2 scrollOffset;
  fVector2 floatOffset;
  fVector2 originPos;
  fVector2 resultDimInPixels;
  fVector2 textDimInPixels;
  fVector2 rmDimInPixels;
  fVector2 minDimInPixels;
  fVector2 maxDimInPixels;
  fVector2 fillRatioDim;
  FontWrapper * curFont;
  FontWrapper * curFontIcons;
  bool dragging;
  bool contOnStack;
  bool overChild;
  bool overSelf;
  bool singleLine;
  bool hasBackground;
  bool mouseOver;
  bool mouseDown;
  bool wasHit;
  bool isDirty;
  bool isFloating;
  bool dataLoaded;
  uint flags;
  float divisions;
  float paddingInPixels;
  float borderInPixels;
  float marginInPixels;
  float spaceForCharsInPixels;
  iVector2 spacing;
  std::vector <string> lineVec;
  std::vector < std::vector<string> > wordVec;
  std::vector <float> linePitchVec;
  fBoundingBox hitBounds;
  StyleSheetResult resSS;
  UIComponent ();
  void init (Singleton * _singleton, int _parentId, int _nodeId, int _index, JSONValue * _jvNodeNoTemplate, bool _isFloating, string * stringVals, double * floatVals);
  UIComponent * getChild (int ind);
  UIComponent * getFloatingChild (int ind);
  int getChildCount ();
  int getFloatingChildCount ();
  float getDimYClamped (float val);
  float getResultDimYClamped ();
  string findKeyString (int valEnum);
  void updateLinkedValues (bool isRead = false);
  void setValueIndex (int ind, float val);
  float getValueIndex (int ind);
  float getValueIndexPtr (int ind);
  void setValue (float _value, bool doEventDispatch = false, bool preventRefresh = false);
  float getValue ();
  void setValueY (float _value, bool doEventDispatch = false, bool preventRefresh = false);
  float getValueY ();
  UIComponent * getParent ();
  UIComponent * findParentByUID (string parUID);
  UIComponent * getValuePtr ();
  UIComponent * findNodeByString (string _uid);
  float getMinWidth ();
  float getMinHeight ();
  int addChild (int _parentId, int _nodeId, string * stringVals, double * floatVals, bool _isFloating, JSONValue * _jvNodeNoTemplate);
  void setOrigPos ();
  void applyHeight ();
  void applyWidth ();
  void gatherDirty (vector <UIComponent*> * dirtyVec);
  void clearDirty ();
  void alignToComp (UIComponent * myComp);
  void layout ();
  void updateOffset ();
  void updateValue (float x, float y);
  void runReport ();
  void clearOver ();
  bool findMaxLayer (float x, float y, float xTransformed, float yTransformed);
  void testOver (float x, float y);
  bool testHit (int button, int state, float x, float y);
  UIComponent * getHighestCont (UIComponent * curNode, int genCount);
  void setText (string _text);
  void updateVecs ();
  void renderAll ();
  void updateSS ();
  float getLineOffset (int lineCount);
  float lengthOfWord (int i, int j, bool isIcon);
  int maxCharsForWord (int i, int j);
  void renderText (bool getDimensions);
};
#undef LZZ_INLINE
#endif
// f00330_gamegui.e
//

#ifndef LZZ_f00330_gamegui_e
#define LZZ_f00330_gamegui_e
#define LZZ_INLINE inline
class GameGUI
{
private:
public:
  Singleton * singleton;
  JSONValue * jvRoot;
  JSONValue * jvTemplates;
  JSONValue * jvSounds;
  bool isReady;
  bool isLoaded;
  fVector2 mouseTrans;
  int guiRenderCount;
  int nodeCount;
  vector <UIComponent*> dirtyVec;
  string (tempStrings) [10];
  string (stringVals) [E_GST_LENGTH];
  double (floatVals) [E_GFT_LENGTH];
  GameGUI ();
  void init (Singleton * _singleton);
  bool compChildStr (string childStr);
  JSONValue * findNearestKey (JSONValue * jv, string key);
  void addChildFromJSON (JSONValue * jv, int curParentId, bool isFloating = false);
  void guiFromJSON (JSONValue * jv);
  void doRefresh ();
  void testOver (int x, int y);
  bool testHit (int button, int state, int x, int y);
  UIComponent * findNodeByString (string _uid);
  void renderCharAt (UIComponent * uiComp, CharStruct * cs, FontWrapper * activeFont, float px, float py, float shadowOffset);
  void renderQuad (UIComponent * uiComp, fBoundingBox fbb, float shadowOffset);
  void renderQuadDirect (UIComponent * uiComp);
  void runReport ();
  void renderGUI (int activeFBO);
};
LZZ_INLINE bool GameGUI::compChildStr (string childStr)
                                                  {
		return tempStrings[E_GDS_CHILD_TYPE].compare(childStr) == 0;
	}
#undef LZZ_INLINE
#endif
// f00338_gameorgnode.e
//

#ifndef LZZ_f00338_gameorgnode_e
#define LZZ_f00338_gameorgnode_e
#define LZZ_INLINE inline
class GameOrgNode
{
private:
public:
  int nodeName;
  GameOrgNode * parent;
  std::vector <GameOrgNode*> children;
  FIVector4 (orgVecs) [E_OV_LENGTH];
  FIVector4 * readTBN;
  FIVector4 * writeTBN;
  FIVector4 (tbnBaseTrans) [3];
  FIVector4 (tbnRotA) [3];
  FIVector4 (tbnRotB) [3];
  FIVector4 (tbnRotC) [3];
  FIVector4 (tbnTrans) [3];
  FIVector4 (orgTrans) [3];
  GameOrgNode (GameOrgNode * _parent, int _nodeName, float _material, float _rotThe, float _rotPhi, float _rotRho, float _tanLengthInCells0, float _bitLengthInCells0, float _norLengthInCells0, float _tanLengthInCells1, float _bitLengthInCells1, float _norLengthInCells1, float _tanX, float _tanY, float _tanZ, float _bitX, float _bitY, float _bitZ, float _norX, float _norY, float _norZ);
  GameOrgNode * addChild (int _nodeName, float _material, float _rotThe, float _rotPhi, float _rotRho, float _tanLengthInCells0, float _bitLengthInCells0, float _norLengthInCells0, float _tanLengthInCells1, float _bitLengthInCells1, float _norLengthInCells1, float _tanX, float _tanY, float _tanZ, float _bitX, float _bitY, float _bitZ, float _norX, float _norY, float _norZ);
  GameOrgNode * getNode (int _nodeName);
  void doTransform (Singleton * singleton);
};
#undef LZZ_INLINE
#endif
// f00339_gameorg.e
//

#ifndef LZZ_f00339_gameorg_e
#define LZZ_f00339_gameorg_e
#define LZZ_INLINE inline
class GameOrg
{
public:
  Singleton * singleton;
  GamePageHolder * gph;
  GameOrgNode * baseNode;
  FIVector4 basePosition;
  JSONValue * rootObj;
  float defVecLength;
  float gv (float * vals);
  static float const baseMat;
  GameOrg ();
  void init (Singleton * _singleton);
  void loadFromFile (string fileName);
  void jsonToNode (JSONValue * * parentObj, GameOrgNode * curNode);
  void saveToFile (string fileName);
  void nodeToJSON (JSONValue * * parentObj, GameOrgNode * curNode);
  void initHuman ();
};
#undef LZZ_INLINE
#endif
// f00340_gameplantnode.e
//

#ifndef LZZ_f00340_gameplantnode_e
#define LZZ_f00340_gameplantnode_e
#define LZZ_INLINE inline
class GamePlantNode
{
public:
  FIVector4 begPoint;
  FIVector4 endPoint;
  FIVector4 tangent;
  FIVector4 baseShoot;
  float shootLength;
  float begThickness;
  float endThickness;
  float midThickness;
  float sphereRad;
  FIVector4 startEndWidth;
  FIVector4 upVec;
  GamePlantNode * parent;
  GamePlantNode * children;
  int maxChildren;
  int numChildren;
  GamePlantNode ();
  void updateTangent (float angleInRadians);
  void init (GamePlantNode * _parent, int _maxChildren, int _numChildren);
};
#undef LZZ_INLINE
#endif
// f00341_gameplant.e
//

#ifndef LZZ_f00341_gameplant_e
#define LZZ_f00341_gameplant_e
#define LZZ_INLINE inline
class GamePlant
{
public:
  Singleton * singleton;
  FIVector4 origin;
  FIVector4 tempv0;
  FIVector4 tempv1;
  FIVector4 tempv2;
  FIVector4 tempv3;
  FIVector4 rootVec;
  FIVector4 trunkVec;
  GamePlantNode * trunkNode;
  GamePlantNode * rootsNode;
  PlantRules * rootRules;
  PlantRules * trunkRules;
  static PlantRules (allPlantRules) [E_PT_LENGTH];
  static void initAllPlants (Singleton * _singleton);
  float gv (float * vals);
  GamePlant ();
  void init (Singleton * _singleton, PlantRules * _rootRules, PlantRules * _trunkRules, FIVector4 * _origin);
  void initBase (PlantRules * rules, GamePlantNode * curNode, FIVector4 * baseVec);
  void applyRules (PlantRules * rules, GamePlantNode * curParent, int curGen, int maxGen, float totLength, float maxLength);
};
#undef LZZ_INLINE
#endif
// f00350_gamepage.e
//

#ifndef LZZ_f00350_gamepage_e
#define LZZ_f00350_gamepage_e
#define LZZ_INLINE inline
class GamePage
{
private:
  uint * volData;
  uint * volDataLinear;
public:
  Singleton * singleton;
  int thisPageId;
  int terRes;
  int bufferedPageSizeInUnits;
  FIVector4 offsetInUnits;
  FIVector4 offsetInPages;
  FIVector4 offsetInPagesLocal;
  float pageDepth;
  int iVolumeSize;
  int * cellData;
  bool hasSolids;
  bool hasTrans;
  bool hasGeom;
  bool hasLines;
  bool hasTerrain;
  bool hasWater;
  bool hasWindow;
  bool hasTree;
  bool isDirty;
  int voroCount;
  bool volDataModified;
  bool isRendering;
  bool isEntity;
  int paramsPerEntry;
  int numEntries;
  int totParams;
  int maxEntries;
  FloatVec (vertices) [MAX_MIP_LEV*MAX_LAYERS];
  bool threadRunning;
  float unitSizeInPixels;
  GameWorld * gw;
  FIVector4 worldMinVisInPixels;
  FIVector4 worldMaxVisInPixels;
  FIVector4 worldMinBufInPixels;
  FIVector4 worldMaxBufInPixels;
  FIVector4 centerPosition;
  FIVector4 worldUnitMin;
  FIVector4 worldUnitMax;
  FIVector4 voroSize;
  FIVector4 curPos;
  FIVector4 randNum;
  FIVector4 testNum;
  FIVector4 newPos;
  FIVector4 tempVec1;
  FIVector4 tempVec2;
  GamePageHolder * parentGPH;
  GameBlock * parentBlock;
  GamePage ();
  void init (Singleton * _singleton, GamePageHolder * _parentGPH, int _thisPageId, int offsetX, int offsetY, int offsetZ, int oxLoc, int oyLoc, int ozLoc, bool _isEntity = false);
  uint * getVolData ();
  uint * getVolDataLinear ();
  void copyToTexture (bool isForEmptyVD);
  void setFalse ();
  void addEntityGeom (bool justTesting);
  void addGeom (bool justTesting);
  void getVoroPoints ();
  void addAllGeom ();
  void generateVolume (bool dd = false);
  int getIndex (int i, int j, int k, int p);
  void getPoints (int fboNum);
  ~ GamePage ();
};
#undef LZZ_INLINE
#endif
// f00351_gamepageholder.e
//

#ifndef LZZ_f00351_gamepageholder_e
#define LZZ_f00351_gamepageholder_e
#define LZZ_INLINE inline
class GamePageHolder
{
public:
  int iPageDataVolume;
  int holderSizeInPages;
  int usingPoolId;
  int blockId;
  int holderId;
  int sres;
  bool hasTrans;
  bool hasSolids;
  bool childrenDirty;
  bool (hasVerts) [MAX_LAYERS];
  bool refreshDL;
  std::vector <GameEnt *> entityGeom;
  int entityGeomCounter;
  FIVector4 offsetInHolders;
  FIVector4 gphMinInPixels;
  FIVector4 gphMaxInPixels;
  FIVector4 gphCenInPixels;
  FIVector4 offsetInBlocks;
  FIVector4 origOffset;
  FIVector4 tempVec;
  FIVector4 tempVec2;
  PooledResource * gpuRes;
  Singleton * singleton;
  intPairVec (containsEntIds) [E_ET_LENGTH];
  GamePage * * pageData;
  bool readyForClear;
  bool isEntity;
  float holderSizeInPixels;
  float halfHolderSizeInPixels;
  GamePageHolder ();
  void init (Singleton * _singleton, int _blockId, int _holderId, int trueX, int trueY, int trueZ, bool _isEntity = false);
  void doRefreshDL ();
  GamePage * getPageAtCoordsLocal (int x, int y, int z, bool createOnNull = false);
  void refreshGeom ();
  void clearSet ();
  int passiveRefresh ();
  void refreshChildren (bool refreshImmediate, bool clearEverything = false, bool refreshUnderground = false);
  void addNewGeom (int _curBT, int _curAlign, float _baseOffset, FIVector4 * _p1, FIVector4 * _p2, FIVector4 * _rad, FIVector4 * _cornerRad, FIVector4 * _visInsetFromMin, FIVector4 * _visInsetFromMax, FIVector4 * _powerVals, FIVector4 * _powerVals2, FIVector4 * _thickVals, FIVector4 * _matParams, FIVector4 * _centerPoint, FIVector4 * _anchorPoint, int _minRot, int _maxRot);
  void addNewLinesGeom (GameOrgNode * curNode, float scale);
  void fetchGeom ();
  void unbindGPUResources ();
};
#undef LZZ_INLINE
#endif
// f00352_gameblock.e
//

#ifndef LZZ_f00352_gameblock_e
#define LZZ_f00352_gameblock_e
#define LZZ_INLINE inline
class GameBlock
{
public:
  Singleton * singleton;
  int blockId;
  int blockSizeInHolders;
  int blockSizeInLots;
  int terDataBufAmount;
  bool forceUpdate;
  float (trilin) [8];
  int terDataVisPitchXY;
  float fTerDataVisPitchXY;
  int terDataBufPitchXY;
  int terDataBufPitchScaledXY;
  int terDataVisPitchZ;
  float fTerDataVisPitchZ;
  int terDataBufPitchZ;
  int terDataBufPitchScaledZ;
  int terDataTexScale;
  int terDataVisSize;
  int terDataBufSize;
  int blockSizeInPixels;
  int iHolderSize;
  int maxFloors;
  float fBlockSizeInPixels;
  int (dirModX) [6];
  int (dirModY) [6];
  int (dirModZ) [6];
  int (opDir) [6];
  float pixelsPerCell;
  float floorHeightInCells;
  float floorHeight;
  float roofHeight;
  bool (isBuilding) [E_CT_LENGTH];
  bool (isRoad) [E_CT_LENGTH];
  bool (isRoom) [E_CT_LENGTH];
  bool (isBase) [E_CT_LENGTH];
  bool (isTraversable) [E_CT_LENGTH];
  int (ctClasses) [E_CT_LENGTH];
  int (layerHash) [E_CT_LENGTH];
  FIVector4 anchorPoint;
  FIVector4 p1;
  FIVector4 p2;
  FIVector4 newP1;
  FIVector4 newP2;
  FIVector4 rad;
  FIVector4 visInsetFromMin;
  FIVector4 visInsetFromMax;
  FIVector4 cornerRad;
  FIVector4 thickVals;
  FIVector4 powerVals;
  FIVector4 powerVals2;
  FIVector4 matParams;
  FIVector4 centerPoint;
  FIVector4 lightVec;
  FIVector4 offsetInBlocks;
  FIVector4 offsetInBlocksWrapped;
  FIVector4 origin;
  FIVector4 blockMinBufInPixels;
  FIVector4 blockMaxBufInPixels;
  FIVector4 tempVec;
  FIVector4 tempVec2;
  FIVector4 tempVec3;
  FIVector4 tempVec4;
  FIVector4 tempVecB;
  FIVector4 tempVecB2;
  FIVector4 tempVecB3;
  GameEnt baseEnt;
  EntVec (gameEnts) [E_ET_LENGTH];
  GameWorld * gw;
  GamePageHolder * * holderData;
  BuildingNode * buildingData;
  MapNode * mapData;
  uint * terData;
  BuildingCon * curCon;
  GameBlock ();
  void init (Singleton * _singleton, int _blockId, int _x, int _y, int _xw, int _yw);
  int getNodeIndexClamped (int _x, int _y, int _z);
  int getNodeIndex (int x, int y, int z, int bufAmount);
  int touchesPlanarBN (int x, int y, int z, int buildingType, int bufAmount);
  int touchesPlanarClass (int x, int y, int z, int classType, int bufAmount);
  bool isLCorner (int x, int y, int z, int classType, bool includeSingle);
  void applyWingValues (int _x1, int _y1, int _z1, int _x2, int _y2, int _z2, int cnum, bool isWingBeg, bool isWingEnd, float multiplier);
  void addPlantNodes (GamePlantNode * curPlantNode, FIVector4 * orig, float scale);
  void addNewGeom (int _curBT, int _curAlign, float _baseOffset, FIVector4 * _p1, FIVector4 * _p2, FIVector4 * _rad, FIVector4 * _cornerRad, FIVector4 * _visInsetFromMin, FIVector4 * _visInsetFromMax, FIVector4 * _powerVals, FIVector4 * _powerVals2, FIVector4 * _thickVals, FIVector4 * _matParams, FIVector4 * _centerPoint, FIVector4 * _anchorPoint, int _minRot, int _maxRot);
  void connectNodes (int _x1, int _y1, int _z1, int _x2, int _y2, int _z2, int ct, int id = -1, int _heightDelta = 0, int _direction = 0, float _wallRadInCells = -1.0f, unsigned int _nodeFlags = 0);
  int getMapNodeIndex (int x, int y, int bufAmount);
  int touchesMap (int x, int y, int buildingType, int bufAmount);
  int touchesWithinRadMap (int x, int y, int buildingType, int rad, int bufAmount);
  int touches2Map (int x, int y, int buildingType, int bufAmount);
  void connectMapNodes (int _x1, int _y1, int _x2, int _y2, int buildingType, int id, int bufAmount);
  int getAdjustedHeightInHolders (int xInHolders, int yInHolders);
  float fIsNearTerrain (FIVector4 * worldMinVisInPixels);
  void refreshHoldersInArea (FIVector4 * worldPos);
  void modifyTerrain (FIVector4 * worldPos, bool doSub);
  int isNearTerrain (FIVector4 * worldPosInPix);
  int findNearestNode (FIVector4 * worldPositionInPixelsIn, FIVector4 * posInNodesOut, FIVector4 * posInPixelsOut);
  void nodeIndexToWorldSpaceInPixels (int ind, FIVector4 * posInPixelsOut);
  bool hasTerrainBelow (int i, int j, int k, int conDir, bool bothEnds);
  bool hasRoomBelow (int i, int j, int k, int conDir);
  bool isSupportedBelow (int i, int j, int k, int conDir, bool bothEnds);
  bool touchesRoomOnLevel (int i, int j, int k);
  bool touchesBaseOnLevel (int i, int j, int k, int layer);
  bool buildingAbove (int x, int y, int z);
  int copyTerToTexture ();
  void makeMazeUG ();
};
#undef LZZ_INLINE
#endif
// f00380_gameworld.e
//

#ifndef LZZ_f00380_gameworld_e
#define LZZ_f00380_gameworld_e
#define LZZ_INLINE inline
class GameWorld
{
public:
  int numProvinces;
  int seaLevel;
  int seaSlack;
  int pageCount;
  int mapSwapFlag;
  int visPageSizeInUnits;
  int blockSizeInHolders;
  int iBlockSize;
  int renderMethod;
  int iBufferSize;
  int maxThreads;
  int availThreads;
  int visPageSizeInPixels;
  int holderSizeInPages;
  int lightCount;
  int MIN_MIP;
  int MAX_MIP;
  int AVG_MIP;
  int blockMip;
  int hmChannel;
  int idChannel;
  int densityChannel;
  int blockChannel;
  int stChannel;
  int btChannel;
  int pathChannel;
  int houseChannel;
  float mapWidth;
  float mapHeight;
  int * curDiagram;
  int * provinceGrid;
  int * provinceX;
  int * provinceY;
  bool doDrawFBO;
  bool lastProcResult;
  bool mapLockOn;
  bool foundPath;
  float mapStep;
  int (dirFlags) [4];
  int (dirFlagsOp) [4];
  int (dirFlagsO) [4];
  int (dirFlagsOpO) [4];
  int (dirModX) [6];
  int (dirModY) [6];
  int (dirModZ) [6];
  int (opDir) [4];
  GameEnt * (activeLights) [MAX_EVAL_LIGHTS];
  int dirFlagClear;
  int visFlag;
  int visFlagO;
  int activeFBO;
  bool noiseGenerated;
  std::vector <coordAndIndex> roadCoords;
  std::vector <GamePageHolder *> holdersToRefresh;
  std::vector <int> ocThreads;
  std::vector <GameEnt> gameActors;
  float ppCell;
  FIVector4 lScreenCoords;
  FIVector4 cScreenCoords;
  FIVector4 worldSizeInPages;
  FIVector4 worldSizeInHolders;
  FIVector4 worldSizeInBlocks;
  FIVector4 entMin;
  FIVector4 entMax;
  FIVector4 camPagePos;
  FIVector4 camHolderPos;
  FIVector4 camBlockPos;
  FIVector4 iPixelWorldCoords;
  FIVector4 pagePos;
  FIVector4 unitPos;
  FIVector4 lastUnitPos;
  FIVector4 lastPagePos;
  FIVector4 tempVec1;
  FIVector4 tempVec2;
  FIVector4 tempVec3;
  FIVector4 (blockPos) [2];
  FIVector4 (nodePos) [2];
  FIVector4 (nodePosInPixels) [2];
  PathNode (blockAndIndexPath) [2];
  PathNode (blockAndIndexSplitPath) [2];
  FIVector4 (lineSeg) [2];
  PathHolder * finalPath;
  int (nodeInd) [2];
  GameBlock * (blockRef) [2];
  FIVector4 minv;
  FIVector4 maxv;
  FIVector4 tempVec;
  FIVector4 unitPosMin;
  FIVector4 unitPosMax;
  FIVector4 unitPosCenter;
  FIVector4 startBounds;
  FIVector4 endBounds;
  FIVector4 * lightPos;
  FIVector4 * globLightPos;
  FIVector4 lightPosBase;
  FIVector4 * cameraPos;
  FIVector4 * curBoxPos;
  FIVector4 tv0;
  FIVector4 tv1;
  FIVector4 tv2;
  FIVector4 tv3;
  FIVector4 tv4;
  FIVector4 tv5;
  Singleton * singleton;
  GameBlock * * blockData;
  FBOWrapper * curFBO;
  FBOWrapper * curFBO2;
  GameWorld ();
  void init (Singleton * _singleton);
  GameBlock * getBlockAtCoords (int xInBlocks, int yInBlocks, bool createOnNull = false);
  GamePageHolder * getHolderAtCoords (int x, int y, int z, bool createOnNull = false);
  uint getTerDataAtCoords (int x, int y, int z);
  GamePageHolder * getHolderAtId (intPair id);
  GameBlock * getBlockAtId (int id);
  GamePage * getPageAtIndex (int ind);
  int getCellAtCoords (FIVector4 * coords);
  GamePage * getPageAtCoords (int x, int y, int z, bool createOnNull = false);
  bool checkBounds (int k, int km);
  void update ();
  void toggleVis (GameEnt * se);
  void ensureBlocks ();
  void findNearestEnt (EntSelection * entSelection, int entType, int maxLoadRad, int radStep, FIVector4 * testPoint, bool onlyInteractive = false, bool ignoreDistance = false);
  bool procPages ();
  bool addHolderToRefresh (GamePageHolder * toAdd);
  int getHoldersInEnt (GameEnt * gg);
  void refreshHoldersInList (bool doImmediate, bool clearEverything);
  void actionOnHolders (int action, bool instantRefresh = false, bool clearEverything = false);
  void combineHolders ();
  void drawOrg (GameOrg * curOrg, bool drawAll);
  void drawNodeEnt (GameOrgNode * curNode, FIVector4 * basePosition, float scale, int drawMode, bool drawAll);
  void clearVisitedPaths (PathHolder * pathHolder);
  void clearPathList (PathHolder * pathHolder);
  float getIdealPathLength (PathNode * blockAndIndex);
  int findAIPathRBT (PathHolder * pathHolder, PathNode * blockAndIndex, float _pathSlack);
  void drawPathLine (PathHolder * curPath, int r, int g, int b, float lw, float zoff);
  void drawAIPath (PathHolder * pathHolder, PathHolder * splitPathHolder);
  void makeFall (GameEnt * ge);
  void moveCell (GameEnt * ge, int x, int y, int z);
  void moveCellRotated (GameEnt * ge, int dirMod);
  int testHit (GameEnt * ge);
  void renderGeom ();
  void modifyUnit (FIVector4 * fPixelWorldCoordsBase, E_BRUSH brushAction);
  float weighPath (float x1, float y1, float x2, float y2, float rad, bool doSet, bool isOcean);
  float findBestPath (float x1, float y1, float x2, float y2, int generation, int roadIndex, bool doSet, bool isOcean);
  void initMap ();
  void drawMap ();
  void doBlur (string fboName, int _baseFBO = 0);
  void updateLights ();
  void postProcess ();
  ~ GameWorld ();
};
#undef LZZ_INLINE
#endif
 

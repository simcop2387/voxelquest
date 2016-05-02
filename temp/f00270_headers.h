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
  float curVolume;
  GameSound ();
  void init (string path);
  void setPitch (float val);
  void setPositionAndMinDis (float x, float y, float z, float w);
  void setLoop (bool val);
  void stop ();
  void setVolumeSmooth (float volume = 1.0f, float decay = 0.01f);
  void setVolume (float volume = 1.0f);
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
// f00300_singleton.e
//

#ifndef LZZ_f00300_singleton_e
#define LZZ_f00300_singleton_e
#define LZZ_INLINE inline
class Singleton
{
private:
  FIVector4 * cameraPos;
  FIVector4 tempLerpPos;
  FIVector4 camLerpPos;
  FIVector4 resultCameraPos;
  FIVector4 targetCameraPos;
  FIVector4 baseCameraPos;
public:
  struct UIQuad
  {
    fBoundingBox hitBounds;
    CharStruct * cs;
    int fontId;
  };
  struct UICont
  {
    UIQuad bg;
    std::vector <UIQuad> charVec;
  };
  struct JSONStruct
  {
    JSONValue * jv;
  };
  struct UICStruct
  {
    int nodeId;
  };
  struct CompStruct
  {
    bool isValid;
    UIComponent * data;
  };
  DynBuffer * myDynBuffer;
  struct CompareStruct
  {
    bool operator () (string const & first, string const & second);
  };
  CompareStruct compareStruct;
  typedef map <string, UICStruct>::iterator itUICStruct;
  typedef map <string, JSONStruct>::iterator itJSStruct;
  float (mipDis) [8];
  bool (keysPressed) [MAX_KEYS];
  double (keyDownTimes) [MAX_KEYS];
  unsigned char (keyMap) [KEYMAP_LENGTH];
  bool (keyMapResultZipped) [KEYMAP_LENGTH];
  bool (keyMapResultUnzipped) [KEYMAP_LENGTH];
  int (keyMapCoolDown) [KEYMAP_LENGTH];
  int (keyMapMaxCoolDown) [KEYMAP_LENGTH];
  GLdouble (viewMatrixD) [16];
  float (viewMatrixDI) [16];
  GLdouble (projMatrixD) [16];
  Matrix4 lightView;
  Matrix4 identMatrix;
  Matrix4 viewMatrix;
  Matrix4 projMatrix;
  Matrix4 pmMatrix;
  std::vector <Matrix4> objMatrixStack;
  Matrix4 curObjMatrix;
  Matrix4 curMVP;
  Matrix3 curObjMatrix3;
  Matrix4 tempObjMatrix;
  Matrix4 lightSpaceMatrix;
  Matrix4 lightSpaceMatrixLow;
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
  PatternStruct (patterns) [E_PAT_LENGTH*4];
  TBOWrapper limbTBO;
  float (limbTBOData) [MAX_LIMB_DATA_IN_BYTES];
  std::vector <ObjectStruct> tempPrimList;
  TBOWrapper primTBO;
  float (primTBOData) [MAX_PRIM_DATA_IN_BYTES];
  bool (settings) [E_BS_LENGTH];
  bool fpsTest;
  bool commandOn;
  bool sphereMapOn;
  bool waitingOnDestruction;
  bool lightChanged;
  bool updateShadows;
  bool updateHolderLookat;
  bool isPressingMove;
  bool doPathReport;
  bool refreshPaths;
  bool placingTemplate;
  bool smoothMove;
  bool ignoreFrameLimit;
  bool autoMove;
  bool allInit;
  bool updateMatFlag;
  bool matVolLock;
  bool isMoving;
  bool perspectiveOn;
  bool lastPersp;
  bool (isInteractiveEnt) [E_CT_LENGTH];
  bool inputOn;
  bool isMacro;
  bool cavesOn;
  bool bakeParamsOn;
  bool dragging;
  bool mouseMoved;
  bool hitGUI;
  bool draggingMap;
  bool guiDirty;
  bool applyToChildren;
  bool bShiftOld;
  bool bCtrlOld;
  bool bCtrl;
  bool bShift;
  bool emptyVDNotReady;
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
  bool firstRun;
  bool rotOn;
  bool doPageRender;
  bool markerFound;
  bool frameMouseMove;
  bool depthInvalidRotate;
  bool depthInvalidMove;
  bool lastDepthInvalidMove;
  bool drawOrient;
  bool multiLights;
  bool timeMod;
  int forceShadowUpdate;
  int destructCount;
  int curPrimTemplate;
  int geomStep;
  int earthMod;
  int currentTick;
  int curPattern;
  int curPatternRot;
  int curSelPrim;
  int limbArrPos;
  int primArrPos;
  int cacheVersion;
  int holderLoadCount;
  int bakeTicks;
  int tbTicks;
  int tempCounter;
  int actorCount;
  int polyCount;
  int fdWritePos;
  int fdReadPos;
  int fpsCountMax;
  int fpsCount;
  int medianCount;
  int maxHolderDis;
  int fieldCallback;
  int mouseState;
  int lastW;
  int lastH;
  int maxLayerOver;
  int pathFindingStep;
  int baseW;
  int baseH;
  int scaleFactor;
  int numDynLights;
  int iNumSteps;
  int curOrgId;
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
  int iNodeDivsPerHolder;
  int matVolSize;
  int escCount;
  int mapPitch;
  int lastNodeId;
  int diagCount;
  int frameSkip;
  int frameSkipCount;
  int cellsPerHolder;
  int cellsPerHolderPad;
  int voxelsPerHolder;
  int voxelsPerHolderPad;
  int cellsPerBlock;
  int holdersPerBlock;
  int holdersPerChunk;
  int chunksPerBlock;
  int chunksPerWorld;
  int cellsPerChunk;
  int cellsPerWorld;
  int holdersPerWorld;
  int blocksPerWorld;
  int voxelsPerCell;
  int paddingInCells;
  PaddedData (pdPool) [MAX_PDPOOL_SIZE];
  intPair (entIdArr) [1024];
  uint palWidth;
  uint palHeight;
  uint blockShift;
  uint * terDataScaled;
  GLfloat (camRotation) [2];
  GLfloat (curCamRotation) [2];
  uint (naUintData) [8];
  int (naIntData) [8];
  float (naFloatData) [8];
  float (conVals) [E_CONST_LENGTH];
  float gammaVal;
  float lastMouseOrigX;
  float lastMouseOrigY;
  float globWheelDelta;
  float amountInvalidMove;
  float amountInvalidRotate;
  float sphereMapPrec;
  float heightMapMaxInCells;
  float resultShake;
  float cameraShake;
  float lastx;
  float lasty;
  float FOV;
  float focalLength;
  float zoomDelta;
  float subjectDelta;
  float subjectZoom;
  float targetSubjectZoom;
  float cameraZoom;
  float targetZoom;
  float curBrushRad;
  float timeOfDay;
  float targetTimeOfDay;
  float origWinW;
  float origWinH;
  float guiX;
  float guiY;
  float aspectRatio;
  float currentFBOResolutionX;
  float currentFBOResolutionY;
  float curPrimMod;
  float mouseX;
  float mouseY;
  float mouseXUp;
  float mouseYUp;
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
  float * paramArrGeom;
  float * primArr;
  float * limbArr;
  float * splashArr;
  float * explodeArr;
  float * voroArr;
  float * matCountArr;
  float * paramArrMap;
  float (clipDist) [2];
  float MAX_TRAVEL_DIS;
  double timeDelta;
  double curTime;
  float smoothTime;
  double pauseTime;
  double clickTime;
  double mdTime;
  double muTime;
  double (clickTimeLR) [2];
  double (mdTimeLR) [2];
  double (muTimeLR) [2];
  FIVector4 (geomPoints) [E_GEOM_POINTS_LENGTH];
  FIVector4 (colVecs) [16];
  FIVector4 geomOrigOffset;
  FIVector4 lastSend;
  FIVector4 lastMouseZO;
  FIVector4 lastLightPos;
  FIVector4 lightPos;
  FIVector4 lightLookAt;
  FIVector4 lastHolderPos;
  FIVector4 lightVec;
  FIVector4 lightVecOrig;
  FIVector4 (dirVecs) [6];
  FIVector4 lastCellPos;
  FIVector4 worldMarker;
  FIVector4 lookAtVec;
  FIVector4 lookAtVec2D;
  FIVector4 baseScrollPos;
  PixData spaceUpPixData;
  PixData mouseUpPixData;
  PixData mouseDownPixData;
  PixData mouseMovePixData;
  FIVector4 tempVec1;
  FIVector4 tempVec2;
  FIVector4 tempVec3;
  FIVector4 (voroVecArr) [125];
  floatAndIndex (indexArr) [125];
  FIVector4 mouseStart;
  FIVector4 mouseEnd;
  FIVector4 mouseMoveVec;
  FIVector4 mapFreqs;
  FIVector4 mapAmps;
  FIVector4 * mouseMoving;
  FIVector4 rasterLowDim;
  FIVector4 bufferDim;
  FIVector4 bufferDimTarg;
  FIVector4 bufferDimHalf;
  FIVector4 bufferModDim;
  FIVector4 bufferRenderDim;
  FIVector4 origin;
  FIVector4 panMod;
  FIVector4 dMod;
  FIVector4 modXYZ;
  FIVector4 matVolDim;
  uint * matVol;
  ThreadWrapper threadNetSend;
  ThreadWrapper threadNetRecv;
  std::list <KeyStackEvent> keyStack;
  ThreadPoolWrapper * threadPoolSpeech;
  std::vector <ExplodeStruct> explodeStack;
  std::vector <DebrisStruct> debrisStack;
  std::vector <FIVector4> primTemplateStack;
  std::vector <SphereStruct> sphereStack;
  std::vector <int> (guiLayers) [MAX_UI_LAYERS];
  std::vector <RotationInfo> rotMatStack;
  std::vector <DynObject *> dynObjects;
  VBOWrapper fsQuad;
  VBOWrapper zoCube;
  VBOWrapper zoCubes;
  float floorHeightInCells;
  float roofHeightInCells;
  float wallRadInCells;
  Image * imageVoro;
  Image * imageHM0;
  Image * imageHM1;
  Image * cloudImage;
  GamePageHolder * closestHolder;
  GamePlant * (gamePlants) [E_PT_LENGTH/2];
  Shader * curShaderPtr;
  string jsonPostString;
  string currentFieldString;
  string curShader;
  string allText;
  string stringBuf;
  string curCallback;
  string (cbDataStrings) [10];
  string guiSaveLoc;
  string curCLFull;
  string curCLBaseDir;
  string curCLWorldDir;
  VolumeWrapper * (volumeWrappers) [E_VW_LENGTH];
  vector <CompStruct> compStack;
  vector <int> emptyStack;
  vector <string> jsonPostStack;
  vector <string> splitStrings;
  vector <string> shaderStrings;
  vector <string> shaderTextureIds;
  map <string, Shader*> shaderMap;
  map <string, UICStruct> compMap;
  map <string, string> includeMap;
  map <string, FBOSet> fboMap;
  GLuint fsqDL;
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
  JSONValue * constRootJS;
  JSONValue * cacheMetaJS;
  HPClock bulletTimer;
  Timer fpsTimer;
  Timer shakeTimer;
  Timer myTimer;
  Timer scrollTimer;
  Timer moveTimer;
  VBOGrid myVBOGrid;
  GameWorld * gw;
  GameEntManager * gem;
  GamePhysics * gamePhysics;
  GameFluid * (gameFluid) [E_FID_LENGTH];
  GameLogic * gameLogic;
  GameNetwork * gameNetwork;
  GameAI * gameAI;
  VIStruct (chunkVI) [NUM_MIP_LEVELS_WITH_FIRST];
  float (lightArr) [MAX_LIGHTS * 16];
  int numLights;
  int * rbStack;
  int * rbHeightStack;
  TerTexture (terTextures) [MAX_TER_TEX];
  GameGUI * mainGUI;
  UIComponent * mapComp;
  UIComponent * (menuList) [E_FM_LENGTH];
  UIComponent * contMenuBar;
  UIComponent * ddMenuBar;
  UIComponent * fieldText;
  FontWrapper * (fontWrappers) [EFW_LENGTH];
  GameMusic * (music) [EML_LENGTH];
  map <string, GameSound> soundMap;
  map <string, StyleSheet> styleSheetMap;
  map <string, JSONStruct> externalJSON;
  Singleton ();
  FIVector4 btvConv;
  FIVector4 * BTV2FIV (btVector3 btv);
  void init (int _defaultWinW, int _defaultWinH, int _scaleFactor);
  void listVoices ();
  bool speak (string speechString);
  void applyPat (int patInd, int patShape, int rot, int x, int y, int val, int rad);
  void getVoroOffsets ();
  void generatePatterns ();
  int placeInStack ();
  int placeInLayer (int nodeId, int layer);
  void initAllMatrices ();
  void funcNT2 ();
  void startNT2 ();
  bool stopNT2 ();
  void funcNT ();
  void startNT ();
  bool stopNT ();
  void prepSound (string soundName);
  void playSoundEnt (string soundName, BaseObj * ge = NULL, float variance = 0.0f, float volume = 1.0f, bool doLoop = false);
  void playSoundPosAndPitch (string soundName, FIVector4 * listenerPos, FIVector4 * soundPos, float variance = 0.0f, float volume = 1.0f, bool doLoop = false);
  void updateSoundPosAndPitch (string soundName, FIVector4 * listenerPos, FIVector4 * soundPos, float volume = 1.0f, float decay = 0.01f);
  void playSound (string soundName, float volume = 1.0f);
  void playSoundEvent (char const * eventName, bool suppress = false);
  void updateMatVol ();
  void toggleDDMenu (int x, int y, bool toggled);
  void dispatchEvent (int button, int state, float x, float y, UIComponent * comp, bool automated = false, bool preventRefresh = false);
  StyleSheet * getNewStyleSheet (string ssName);
  void initStyleSheet ();
  static void qNormalizeAngle (int & angle);
  ~ Singleton ();
  void setProgAction (eProgramState ps, unsigned char kc, eProgramAction pa, bool isDown);
  void setProgActionAll (unsigned char kc, eProgramAction pa, bool isDown);
  void drawCrossHairs (FIVector4 originVec, float radius);
  void drawLine (FIVector4 * p0, FIVector4 * p1);
  void drawCubeCentered (FIVector4 * originVec, float radius);
  void drawBoxUp (FIVector4 originVec, float radiusX, float radiusY, float diamZ);
  void drawBoxMinMax (btVector3 v0, btVector3 v1);
  void drawBoxRad (btVector3 v0, btVector3 v1);
  void drawBox (FIVector4 * v0, FIVector4 * v1, int faceFlag = 2);
  void getMaterialString ();
  void updatePrimArr ();
  void updatePrimTBOData ();
  bool getPrimTemplateString ();
  void refreshIncludeMap ();
  void doShaderRefresh (bool doBake);
  void setWH (int w, int h);
  void sampleFBODirect (FBOSet * fbos, int offset = 0, int _minOff = 0, int _maxOff = -1 /* read max */);
  void unsampleFBODirect (FBOSet * fbos, int offset = 0, int _minOff = 0, int _maxOff = -1 /* read max */);
  void getMatrixFromFBO (string fboName);
  void bindFBODirect (FBOSet * fbos, int doClear = 1);
  FBOSet * getFBOByName (string & fboName);
  void sampleFBO (string fboName, int offset = 0, int swapFlag = -1, int minOff = 0, int maxOff = -1);
  void unsampleFBO (string fboName, int offset = 0, int swapFlag = -1, int minOff = 0, int maxOff = -1);
  FBOSet * getFBOSet (string fboName);
  FBOWrapper * getFBOWrapper (string fboName, int offset);
  void copyFBO (string src, string dest, int num = 0);
  void copyFBO2 (string src, string dest, int num1 = 0, int num2 = 1);
  void copyFBO3 (string src, string dest, int num1 = 0, int num2 = 1, int num3 = 2);
  void bindFBO (string fboName, int swapFlag = -1, int doClear = 1);
  void unbindFBO ();
  void bindShader (string shaderName);
  void unbindShader ();
  void setShaderArrayfVec3 (string paramName, float * x, int count);
  void setShaderArrayfVec4 (string paramName, float * x, int count);
  void setShaderMatrix4x4 (string paramName, float * x, int count);
  void setShaderMatrix3x3 (string paramName, float * x, int count);
  void setShaderArray (string paramName, float * x, int count);
  GLint getShaderLoc (string paramName);
  void setShaderFloat (string paramName, float x);
  void setShaderInt (string paramName, int x);
  void setShaderfVec2 (string paramName, FIVector4 * v);
  void setShaderVec2 (string paramName, float x, float y);
  void setShaderVec3 (string paramName, float x, float y, float z);
  void setShaderfVec3 (string paramName, FIVector4 * v);
  void setShaderbtVec3 (string paramName, btVector3 v);
  void setShaderVec4 (string paramName, float x, float y, float z, float w);
  void setShaderfVec4 (string paramName, FIVector4 * v);
  void setShaderFloatUB (string paramName, float x);
  void setShaderfVec4UB (string paramName, FIVector4 * v);
  void updateUniformBlock (int ubIndex, int ubDataSize = -1);
  void invalidateUniformBlock (int ubIndex);
  void beginUniformBlock (int ubIndex);
  bool wasUpdatedUniformBlock (int ubIndex);
  void setShaderTBO (int multitexNumber, GLuint tbo_tex, GLuint tbo_buf, bool isFloat);
  void setShaderTexture (int multitexNumber, uint texId);
  void setShaderTexture3D (int multitexNumber, uint texId);
  bool shiftDown ();
  bool ctrlDown ();
  bool altDown ();
  void drawQuadWithCoords (FIVector4 * p0, FIVector4 * p1, FIVector4 * p2, FIVector4 * p3, float tx1, float ty1, float tx2, float ty2);
  void drawQuadBounds (float fx1, float fy1, float fx2, float fy2, float fz);
  void drawFSQuad ();
  void drawFSQuadOffset (float xOff, float yOff, float zm);
  void drawFBO (string fboName, int ind, float zm, int swapFlag = -1);
  void drawFBOOffsetDirect (FBOSet * fbos, int ind, float xOff, float yOff, float zm);
  void drawFBOOffset (string fboName, int ind, float xOff, float yOff, float zm);
  float getTerHeightScaled (float val);
  float getSLNormalized ();
  float getSeaHeightScaled ();
  float getHeightAtPixelPos (float x, float y, bool dd = false);
  void angleToVec (FIVector4 * fv, float xr, float yr);
  void vecToAngle (FIVector4 * fv, FIVector4 * ta);
  void syncObjects ();
  void updateCamVals ();
  void moveCamera (FIVector4 * pModXYZ);
  GLfloat getCamRot (int ind);
  void moveObject (float dx, float dy);
  void updateMultiLights ();
  void toggleFullScreen ();
  void setCameraToElevation ();
  btVector3 screenToWorld (float mx, float my, float camAng);
  btVector3 getRayTo (float x, float y);
  void runReport ();
  void updateCS ();
  void getMarkerPos (int x, int y);
  void holderReport ();
  void speakSetting (int settingName);
  void toggleSetting (int settingName, bool withVoice = true);
  void setSetting (int settingName, bool value, bool withVoice = true);
  void resetGeom ();
  void stopAllThreads ();
  void processInput (unsigned char key, bool keyDown, int x, int y);
  void getPixData (PixData * toPixData, int _xv, int _yv, bool forceUpdate);
  float getMinGeom (int baseIndex);
  FIVector4 * getGeomRef (int templateId, int enumVal);
  void setFXYZWGeom (int baseIndex, FIVector4 * baseVec);
  void setFXYGeom (int baseIndex, float xv, float yv);
  void setFXGeom (int baseIndex, float xv);
  void setFYGeom (int baseIndex, float yv);
  void setFZGeom (int baseIndex, float zv);
  void setFWGeom (int baseIndex, float wv);
  float getFXGeom (int baseIndex);
  float getFYGeom (int baseIndex);
  float getFZGeom (int baseIndex);
  float getFWGeom (int baseIndex);
  void processSpecialKeys (int key, int _x, int _y);
  void keyboardUp (unsigned char key, int _x, int _y);
  void keyboardDown (unsigned char key, int _x, int _y);
  void updateCurGeom (int x, int y);
  void mouseMove (int _x, int _y);
  void mouseClick (int button, int state, int _x, int _y);
  bool feetContact (BaseObj * ge);
  void flushKeyStack ();
  void applyKeyAction (bool isReq, int actorId, uint keyFlags, float camRotX, float camRotY);
  void gatherKeyActions ();
  void handleMovement ();
  bool anyMenuVisible ();
  void performCamShake (BaseObj * ge, float fp);
  void explodeBullet (BaseObj * ge);
  void getJVNodeByString (JSONValue * rootNode, JSONValue * * resultNode, string stringToSplit);
  string makePretty (string sourceString, string remString);
  void cleanJVPointer (JSONValue * * jv);
  void getSpecialData (int datEnum, string datString);
  void updateStatGUI ();
  void updateStatusHUD ();
  void showHudMenu (bool visible);
  void showStatMenu (bool visible);
  void refreshContainers (bool onMousePos);
  JSONValue * fetchJSONData (string dataFile, bool doClean, JSONValue * params = NULL);
  bool processJSONFromString (string * sourceBuffer, JSONValue * * destObj);
  void specialReplace (string & allTextString, string preDelim, string pstDelim);
  void jsonPostProc ();
  bool processJSON (charArr * sourceBuffer, charArr * saveBuffer, JSONValue * * destObj);
  void doAlert ();
  bool loadJSON (string path, JSONValue * * destObj);
  void setGUIText (string key, string stringValue, float floatValue = 0.0f, bool applyVal = false, bool applyString = true);
  float getGUIValue (string key);
  UIComponent * getGUIComp (string key);
  void setGUIValue (string key, float floatValue, bool dispatchEvent = false, bool preventRefresh = false);
  void loadValuesGUI (bool applyValues = false);
  void saveExternalJSON ();
  void saveGUIValues ();
  void updateGUI ();
  void beginFieldInput (string defString, int cb);
  void processFieldInput (unsigned char key);
  void endFieldInput (bool success);
  void saveOrg ();
  void loadOrg ();
  float getConst (string conName);
  int iGetConst (int ev);
  void loadConstants ();
  void loadGUI ();
  string loadFileString (string fnString);
  std::ifstream::pos_type filesize (char const * filename);
  bool checkCacheEntry (int blockId, int chunkId, int holderId, bool doErase = false);
  bool loadCacheEntry (int blockId, int chunkId, int holderId);
  bool saveCacheEntry (int blockId, int chunkId, int holderId);
  bool loadCacheMetaData ();
  bool saveCacheMetaData ();
  void clearCache ();
  bool updateCurCacheLoc ();
  bool createFolder (string folderNameStr);
  bool saveFloatArray (string fileName, float * data, int dataSizeInFloats);
  bool loadFloatArray (string fileName, float * data, int dataSizeInFloats);
  bool loadFile (string fnString, charArr * dest);
  bool saveFileString (string fileName, string * source);
  bool saveFile (char * fileName, charArr * source);
  float getUnderWater ();
  void updateAmbientSounds ();
  void checkFluid (GameFluid * gf);
  void frameUpdate (bool doFrameRender);
  int getAvailPD ();
  FIVector4 * cameraGetPos ();
  FIVector4 * cameraGetPosNoShake ();
  float getTargetTimeOfDay ();
  void updateBullets ();
  void idleFunc ();
  void display (bool doFrameRender);
  bool gluInvertMatrix (double const (m) [16], float (invOut) [16]);
  int getMatrixInd (int col, int row);
  void ComputeFOVProjection (float * result, float fov, float aspect, float nearDist, float farDist, bool leftHanded);
  void getLSMatrix (Matrix4 & lsMat, float orthoSize);
  void updateLightPos ();
  void setMatrices (int w, int h);
  void reshape (int w, int h);
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
  string localString;
  Shader (Singleton * _singleton);
  static char * textFileRead (char const * fileName);
  static void validateShader (GLuint shader, char const * file = 0);
  static int validateProgram (GLuint program);
  int countOc (string * src, string testStr);
  void init (string shaderName, bool doBake, map <string, string> * includeMap);
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
  void setShaderMatrix3x3 (string paramName, float * x, int count);
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
  void setShaderbtVec3 (string paramName, btVector3 f);
  void setShaderFloatUB (string paramName, float x);
  void setShaderfVec4UB (string paramName, FIVector4 * f);
};
#undef LZZ_INLINE
#endif
// f00307_dynbuffer.e
//

#ifndef LZZ_f00307_dynbuffer_e
#define LZZ_f00307_dynbuffer_e
#define LZZ_INLINE inline
class DynBuffer
{
public:
  int SCREEN_WIDTH;
  int SCREEN_HEIGHT;
  float CAMERA_DISTANCE;
  int TEXT_WIDTH;
  int TEXT_HEIGHT;
  int IMAGE_WIDTH;
  int IMAGE_HEIGHT;
  int CHANNEL_COUNT;
  int DATA_SIZE;
  void * font;
  GLuint (pboIds) [2];
  GLuint textureId;
  GLubyte * imageData;
  int screenWidth;
  int screenHeight;
  bool mouseLeftDown;
  bool mouseRightDown;
  float mouseX;
  float mouseY;
  float cameraAngleX;
  float cameraAngleY;
  float cameraDistance;
  bool pboSupported;
  int pboMode;
  int drawMode;
  glInfo glInfo;
  DynBuffer ();
  void setVsync (bool enabled);
  ~ DynBuffer ();
  bool initSharedMem ();
  void clearSharedMem ();
  void initLights ();
  void setCamera (float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
  void updatePixels (GLubyte * dst, int size);
  void showTransferRate ();
  void toOrtho ();
  void toPerspective ();
  void displayCB ();
  void reshapeCB (int width, int height);
  void idleCB ();
  void keyboardCB (unsigned char key, int x, int y);
  void mouseCB (int button, int state, int x, int y);
  void mouseMotionCB (int x, int y);
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
// f00323_gameoctree.e
//

#ifndef LZZ_f00323_gameoctree_e
#define LZZ_f00323_gameoctree_e
#define LZZ_INLINE inline
class GameOctree
{
public:
  Singleton * singleton;
  int rootPtr;
  vector <OctNode> octNodes;
  int dimInVoxels;
  int maxDepth;
  GameOctree ();
  ~ GameOctree ();
  void init (Singleton * _singleton, int _dimInVoxels, int reserveSize);
  int allocNode (int parent);
  int getNode (VectorI3 * pos);
  int addNode (VectorI3 * pos, bool & wasNew);
  void clearAllNodes ();
};
#undef LZZ_INLINE
#endif
// f00324_gamevoxelwrap.e
//

#ifndef LZZ_f00324_gamevoxelwrap_e
#define LZZ_f00324_gamevoxelwrap_e
#define LZZ_INLINE inline
class GameVoxelWrap
{
public:
  Singleton * singleton;
  VoxelBuffer * voxelBuffer;
  PaddedData * basePD;
  PaddedDataEntry * baseData;
  GamePageHolder * gph;
  int lastFFSteps;
  int curPD;
  float fVoxelsPerCell;
  int voxelsPerCell;
  int cellsPerHolder;
  int cellsPerHolderPad;
  int voxelsPerHolderPad;
  int voxelsPerHolder;
  int paddingInCells;
  int paddingInVoxels;
  ivec3 offsetInCells;
  ivec3 offsetInVoxels;
  vec3 fOffsetInVoxels;
  vec3 oneVec;
  vec3 halfOff;
  vec3 crand0;
  vec3 crand1;
  vec3 crand2;
  FBOWrapper * hmFBO;
  GameVoxelWrap ();
  void init (Singleton * _singleton);
  void fillVec ();
  void process (GamePageHolder * _gph);
  bool findNextCoord (ivec3 * voxResult);
  bool inBounds (ivec3 * pos, int minB, int maxB);
  int getNode (ivec3 * pos);
  void floodFill (ivec3 startVox);
  bool isInvSurfaceVoxel (ivec3 * pos, int ignorePtr, int & curPtr, bool checkVisited);
  bool isSurfaceVoxel (ivec3 * pos, int & curPtr, bool checkVisited);
  int getVoxelAtCoord (ivec3 * pos);
  float sampLinear (vec3 * pos, vec3 offset);
  PaddedDataEntry * getPadData (int ii, int jj, int kk);
  float rand2D (vec3 co);
  vec3 randPN (vec3 co);
  void getVoro (vec3 * worldPos, vec3 * worldClosestCenter, vec3 * otherData, float fSpacing);
  void calcVoxel (ivec3 * _pos, int octPtr, int VLIndex);
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
  Singleton::UICont uiCont;
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
  int layerId;
  int hoverType;
  int guiClass;
  bool forceDragUpdate;
  bool selected;
  bool foundParent;
  bool foundValuePtr;
  bool visible;
  bool enabled;
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
  void clearChildren ();
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
  void updateTextNumber ();
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
  int addChild (int _lastIndex, int _parentId, string * stringVals, double * floatVals, bool _isFloating, JSONValue * _jvNodeNoTemplate);
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
  vector <UIComponent*> dirtyVec;
  string (tempStrings) [10];
  string (stringVals) [E_GST_LENGTH];
  double (floatVals) [E_GFT_LENGTH];
  GameGUI ();
  void init (Singleton * _singleton);
  JSONValue * findNearestKey (JSONValue * jv, string key);
  void addChildFromJSON (int lastIndex, JSONValue * jv, int curParentId, bool isFloating = false);
  void deleteJSONNodes (JSONValue * jv);
  void clearRenderOrder ();
  void deleteNodes (UIComponent * curNode);
  void refreshNode (UIComponent * oldNode);
  void guiFromJSON (JSONValue * jv);
  void doRefresh ();
  void testOver (int x, int y);
  bool testHit (int button, int state, int x, int y);
  UIComponent * findNodeByString (string _uid);
  void renderCharAt (UIComponent * uiComp, CharStruct * cs, FontWrapper * activeFont, float px, float py, float shadowOffset);
  void renderQuad (UIComponent * uiComp, fBoundingBox fbb, float shadowOffset);
  void renderQuadDirect (UIComponent * uiComp);
  void runReport ();
  void renderGUI ();
};
#undef LZZ_INLINE
#endif
// f00332_gamenetwork.e
//

#ifndef LZZ_f00332_gamenetwork_e
#define LZZ_f00332_gamenetwork_e
#define LZZ_INLINE inline
class GameNetwork
{
public:
  Singleton * singleton;
  static int const FRAME_SIZE_IN_BYTES = 256;
  static int const TOT_BUFFER_SIZE = FRAME_SIZE_IN_BYTES*256;
  FIVector4 (tempVecs) [8];
  WSADATA wsaData;
  SOCKET ConnectSocket;
  int recvPosInBytes;
  int sendPosInBytes;
  int recvConsumedInBytes;
  int sendConsumedInBytes;
  uint * (uintPtr) [8];
  int * (intPtr) [8];
  float * (floatPtr) [8];
  char (recvbuf) [TOT_BUFFER_SIZE];
  char (sendbuf) [TOT_BUFFER_SIZE];
  bool isConnected;
  std::vector <NetworkAction> netSendStack;
  GameNetwork ();
  void init (Singleton * _singleton);
  void getIntFloatLen (int opCode, int * uintLen, int * intLen, int * floatLen);
  void addNetworkAction (int opCode, uint * naUintData, int * naIntData, float * naFloatData);
  void addNetworkActionForSend (NetworkAction * na);
  void applyNetworkActions ();
  int socketConnect (bool doConnect);
  void socketRecv ();
  void socketSend ();
  void checkBufferLengthSend ();
  void checkBufferLengthRecv ();
  void flushNetworkActions ();
  void updateSend ();
  void updateRecv ();
};
#undef LZZ_INLINE
#endif
// f00335_gameai.e
//

#ifndef LZZ_f00335_gameai_e
#define LZZ_f00335_gameai_e
#define LZZ_INLINE inline
class GameAI
{
public:
  Singleton * singleton;
  std::vector <intVec> tokenToRules;
  std::vector <VToken> intToVToken;
  std::vector <VNode*> tokenIndexToVar;
  std::vector <AssignStruct> assignStack;
  map <string,int> stringToVTokenIndex;
  string tempVarStr;
  VNode * kbCompiled;
  VNode * qbCompiled;
  JSONValue * jvKB;
  GameAI ();
  void init (Singleton * _singleton);
  string getResolvedString (VNode * tempNode);
  bool setEqualTo (VNode * destNodeFinal, VNode * srceNodeFinal, int curGen);
  void rollBack (int curGen);
  bool attemptUnify (VNode * nodeToUnify, VNode * testNode, int curGen);
  VNode * drillDown (VNode * curVar);
  bool testEqual (VNode * testNode);
  bool isBetween (VNode * testNode);
  bool searchToUnify (VNode * nodeToUnifyBase, int curGen, int curOffset = 0);
  void printChain (VNode * curVar);
  void runQueries ();
  bool compileVocab (JSONValue * uncompiledNode, VNode * curVNode);
  bool checkVocab (JSONValue * jv);
  void buildRule (VNode * curNode, int ruleNumber, bool isIfThen);
  void buildRuleSet (VNode * curNode);
  void numberRule (VNode * curNode, int ruleNumber);
  int numberRuleSet (VNode * curNode, int offset);
  void printRule (VNode * curNode, int offset);
  void printRuleSet (VNode * curNode);
  bool parseKB ();
  void traceRules ();
  void getKB ();
};
#undef LZZ_INLINE
#endif
// f00337_gamefluid.e
//

#ifndef LZZ_f00337_gamefluid_e
#define LZZ_f00337_gamefluid_e
#define LZZ_INLINE inline
class GameFluid
{
public:
  Singleton * singleton;
  uint * (volDataPrim) [E_PL_LENGTH];
  int forceFullRefresh;
  int volSizePrim;
  int volSizePrimBuf;
  int bufAmount;
  int vspMin;
  int vspMax;
  int * fluidData;
  int * extraData;
  int shrinkCount;
  int immobileHeight;
  int immobileInd;
  int maxWaterHeight;
  int maxWaterInd;
  std::vector <int> indexStack;
  int curGeomCount;
  int UNIT_MIN;
  int UNIT_MAX;
  int UNIT_INSIDE;
  int watchMinX;
  int watchMaxX;
  int watchMinY;
  int watchMaxY;
  int watchMinZ;
  int watchMaxZ;
  int totSize;
  FIVector4 readMIP;
  FIVector4 writeMIP;
  FIVector4 dirtyMin;
  FIVector4 dirtyMax;
  FIVector4 tempMin;
  FIVector4 tempMax;
  FIVector4 tempMin2;
  FIVector4 tempMax2;
  FIVector4 minV;
  FIVector4 maxV;
  bool waitingOnThreads;
  bool fluidChanged;
  bool posShifted;
  bool hasRead;
  bool invalidated;
  float F_UNIT_MAX;
  float F_UNIT_MIN;
  std::vector <FluidStruct> fsVec;
  int * fluidIds;
  int * idealCellIds;
  FluidPlane fluidPlane;
  bool waterTickReady;
  bool cycleTerminated;
  bool readyForTermination;
  bool fluidReading;
  bool proceedingToRead;
  bool modifiedGeom;
  bool readyForTBOUpdate;
  bool firstVPUpdate;
  int volSizePrimMacro;
  int floatsInPrimMacro;
  int floatsPerPrimEntry;
  int primsPerMacro;
  int primDiv;
  int waterTick;
  int waterTickMax;
  int waterTickMaxDiv;
  int cellsPerHolder;
  int cellsPerBlock;
  int internalPrimFormat;
  int precPrimFormat;
  int mainId;
  int (volSizes) [E_FID_LENGTH];
  int (mipSizes) [E_FID_LENGTH];
  float waterLerp;
  float * tboData;
  GLuint (volIdPrim) [E_PL_LENGTH];
  TBOWrapper tboWrapper;
  std::vector <PushModStruct> pmStack;
  FIVector4 volMinInPixels;
  FIVector4 volMaxInPixels;
  FIVector4 volMinReadyInPixels;
  FIVector4 volMaxReadyInPixels;
  FIVector4 camPosVP;
  FIVector4 campPosVPDump;
  FIVector4 newCamPos;
  FIVector4 lastCamPos;
  FIVector4 camPosVPInPixels;
  FIVector4 lastCamPosVP;
  FIVector4 curDirtyMax;
  FIVector4 curDirtyMin;
  FIVector4 curWaterMax;
  FIVector4 curWaterMin;
  FIVector4 tempBoundsMin;
  FIVector4 tempBoundsMax;
  ThreadWrapper threadLoader;
  ThreadWrapper threadTex;
  ThreadWrapper threadFluid;
  int getFluidId (int groupNum, int ind);
  int getFIDSize (int groupNum);
  void fidPushBack (int groupNum, int val);
  int * fidGetBeg (int groupNum);
  int * fidGetEnd (int groupNum);
  int getIdealCellId (int groupNum, int ind);
  int getICISize (int groupNum);
  void iciPushBack (int groupNum, int val);
  GameFluid ();
  void init (Singleton * _singleton, int _mainId);
  void flushActionStack ();
  void pushPlaceTemplate (bool isReq, FIVector4 * newPos, int pt, int orientation);
  bool addPrimObj (FIVector4 * pos, int tempId, int uid);
  void flushAllGeom ();
  void addGeom (FIVector4 * newPos, int templateId, int orientation);
  void fetchGeom ();
  void setupPrimTexture ();
  bool anyThreadsRunning ();
  void flushStacks ();
  bool updateAll ();
  void copyPrimTexture (int ox, int oy, int oz, int dim, uint * * myData);
  void updateTBOData (bool firstTime, bool reloadTemplates);
  void terminateCycle ();
  void beginFluidRead (FIVector4 * _campPosVPDump);
  bool tryToEndRead ();
  void tryToEndThreads ();
  void proceedWithRead ();
  void endFluidRead ();
  void shiftRegion ();
  void funcFT ();
  void startFT ();
  bool stopFT ();
  void funcTL ();
  void startTL ();
  bool stopTL ();
  void funcTT ();
  void startTT ();
  bool stopTT ();
  void getPrimData (int n);
  void writeFluidData ();
  void prereadFluidData ();
  void readFluidData ();
  bool passesCheck (int n);
  bool updateFluidData ();
  bool findStableRegions (int startInd, int newId);
  bool floodFillId (int startInd, int newId);
  bool inBounds (int i, int j, int k);
  void roundBox (FIVector4 * absVecFromCenter, FIVector4 * innerBoxRad, FIVector4 * cornerDisThicknessPower, bool & isInObj, bool & isInside);
  void clearAllGeom ();
  void clearInsideValues ();
  void fillCurrentGeom (int templateId, FIVector4 * templatePos);
  void resetDirtyRegion ();
  void shrinkDirtyRegion ();
  void maxDirtyRegion ();
};
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
  FIVector4 tempFI;
  btVector3 tbnOffset;
  btVector3 tempOffset;
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
  void flipOrient (float newVal);
  void doTransform (Singleton * singleton, GameOrgNode * tempParent);
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
  GameOrgNode * baseNode;
  GameOrgNode * (allNodes) [E_BONE_C_END];
  float (wepLengths) [E_BONE_C_END];
  FIVector4 basePosition;
  JSONValue * rootObj;
  PoseKey basePose;
  PoseKey targetPose;
  int ownerUID;
  int entType;
  int subType;
  int stepCount;
  double totTime;
  float defVecLength;
  float gv (float * vals);
  static float const baseMat;
  GameOrg ();
  void init (Singleton * _singleton, int _ownerUID, int _entType, int _subType);
  void jsonToNode (JSONValue * * parentObj, GameOrgNode * curNode, bool notThePose);
  void setBinding (int actorId, bool val);
  int getPoseUID ();
  void loadOrgFromFile (string fileName, bool notThePose);
  void saveOrgToFile (string fileName);
  BaseObj * getOwner ();
  void setTPG (int _targetPoseGroup, int _targetPoseRLBN);
  void setToPose (GameOrg * otherOrg, float lerpAmount, int boneId = -1);
  void updatePose (double curTimeStep);
  void nodeToJSON (JSONValue * * parentObj, GameOrgNode * curNode);
  void updateHandleOffset ();
  void initWeapon ();
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
// f00347_gamephysrig.e
//

#ifndef LZZ_f00347_gamephysrig_e
#define LZZ_f00347_gamephysrig_e
#define LZZ_INLINE inline
class GamePhysRig
{
public:
  Singleton * singleton;
  btDynamicsWorld * m_ownerWorld;
  std::vector <RigJointStruct> rigJoints;
  int geId;
  btVector3 origOffset;
  GameOrg * baseOrg;
  BaseObj * baseEnt;
  void updatePivot (int jointId);
  int addJoint (int nodeName, int parentId, int jointType, float mass, GameOrgNode * curNode);
  void initFromOrg (GameOrgNode * curNode, int curParent);
  void reinit ();
  GamePhysRig (Singleton * _singleton, int _geId, btDynamicsWorld * ownerWorld, btVector3 const & positionOffset);
  void removeAllBodies ();
  virtual ~ GamePhysRig ();
};
#undef LZZ_INLINE
#endif
// f00349_gameentmanager.e
//

#ifndef LZZ_f00349_gameentmanager_e
#define LZZ_f00349_gameentmanager_e
#define LZZ_INLINE inline
class GameEntManager
{
public:
  Singleton * singleton;
  bool curActorNeedsRefresh;
  bool orgOn;
  bool isDraggingObject;
  bool firstPerson;
  bool takingTurn;
  int weaponToPlace;
  int activeActorUID;
  int curActorUID;
  int curPoseType;
  int highlightedLimb;
  int highlightedLimb2;
  int gameObjCounter;
  int lastObjInd;
  int selObjInd;
  int actObjInd;
  int draggingFromInd;
  int draggingToInd;
  int draggingFromType;
  int draggingToType;
  int turnListInd;
  float subjectDistance;
  float lastSubjectDistance;
  FIVector4 tempVec1;
  FIVector4 tempVec2;
  FIVector4 tempVec3;
  map <BaseObjType, BaseObj> gameObjects;
  vector <BaseObjType> visObjects;
  vector <int> turnList;
  GameOrgNode * bestNode;
  GameOrgNode * selectedNode;
  GameOrgNode * lastSelNode;
  GameOrgNode * activeNode;
  std::vector <GamePhysRig*> gamePhysRigs;
  std::vector <GameOrg*> gameOrgs;
  std::vector <GameOrg*> gamePoses;
  PoseInfo (gamePoseInfo) [E_PG_LENGTH];
  PoseKey (curPose) [E_ENTTYPE_LENGTH];
  EntPool (entPoolStack) [E_ENTTYPE_LENGTH];
  JSONValue * poseRootJS;
  int (entIdToIcon) [MAX_OBJ_TYPES];
  int (iconToEntId) [MAX_ICON_ID];
  bool (isContainer) [MAX_OBJ_TYPES];
  string (objStrings) [MAX_OBJ_TYPES];
  GameEntManager ();
  void init (Singleton * _singleton);
  BaseObj * getCurActor ();
  BaseObj * getActiveActor ();
  void refreshActiveId ();
  void endHumanTurn ();
  void cycleTurn ();
  void nextTurn ();
  void refreshTurnList ();
  void setTurnBased (bool newVal);
  void checkActorRefresh ();
  void closeAllContainers ();
  bool anyContainerOpen ();
  void togglePoseEdit ();
  void loadDefaultPose (int actorId);
  void applyNonPoseData ();
  void setFirstPerson (bool _newVal);
  int getCurActorUID ();
  void updateOrgMat (UIComponent * comp);
  void doDrag ();
  void endDrag (int upInd);
  bool handleGUI (UIComponent * comp, bool mouseUpEvent, bool mouseDownEvent, bool noTravel, bool wasDoubleClick);
  BaseObj * getEquipped (BaseObj * parentObj);
  void updateDragInfo (int bestInd, bool lbDown, bool wasDoubleClick);
  int getRandomContId ();
  int getRandomNPCId ();
  int getRandomMonsterId ();
  int getRandomObjId ();
  void fillWithRandomObjects (int parentUID, int gen);
  void removeEntity (bool isReq, int ind);
  bool isRecycledFunc (int poolId);
  BaseObjType placeNewEnt (bool isReq, int et, FIVector4 * cellPos, bool isHidden = false);
  void performDrag (bool isReq, int _draggingFromInd, int _draggingFromType, int _draggingToInd, int _draggingToType, FIVector4 * _worldMarker);
  void setCurActor (int newUID);
  void toggleFirstPerson ();
  void toggleActorSel ();
  void setSelInd (int ind);
  void closeContainer (int i);
  void toggleCont (int contIndex, bool onMousePos);
  void addVisObject (BaseObjType _uid, bool isRecycled);
  bool removeVisObject (BaseObjType _uid, bool isRecycled);
  bool areEnemies (int actorUID1, int actorUID2);
  bool areFriends (int actorUID1, int actorUID2);
  int getUnitDisXY (btVector3 p1, btVector3 p2);
  btVector3 getUnitDistance (int actorUID1, int actorUID2);
  int getClosestActor (int actorId, int objType, float maxDis, uint flags);
  GameOrg * getCurOrg ();
  BaseObj * getActorRef (int uid);
  bool combatMode ();
  bool isSwingingWeapon (int actorId, int handNum);
  bool isPunching (int actorId, int handNum);
  bool isKicking (int actorId, int handNum);
  void setSwing (float _mx, float _my, int actorId, int handNum, bool isKick);
  void nextSwing (int actorId, int handNum);
  void makeShoot (int actorId, int bulletType);
  void bindPose (int actorId, int handNum, bool bindOn);
  void makeGrab (int actorId, int _handNum);
  void makeDropAll (int actorId);
  void makeThrow (int actorId, int _handNum);
  void changeStatus (int actorId, int status, int modVal);
  void makeSwing (int actorId, int handNum);
  void makeTurnTowardsTB (int actorId, btVector3 actorTargVec);
  BaseObj * getEntAtUnitPos (btVector3 pos);
  void makeTurnTB (int actorId, int modVal);
  btVector3 getOffsetTB (btVector3 orig, int dir, float amount);
  bool makeMoveTB (int actorId, int modVal);
  void makeTurn (int actorId, float dirFactor);
  void makeMoveVec (int actorId, btVector3 moveVec);
  void makeMove (int actorId, btVector3 moveDir, bool relative, bool delayed);
  void makeJump (int actorId, int isUp, float jumpFactor);
  void makeHit (bool tb, int attackerId, int victimId, int weaponId);
  GameOrgNode * getMirroredNode (GameOrgNode * curNode);
  void refreshActor (int actorId);
  void applyNodeChanges (GameOrgNode * _curNode, float dx, float dy);
  void transformOrg (GameOrg * curOrg, GameOrgNode * tempParent);
  void resetActiveNode ();
  bool updateNearestOrgNode (bool setActive);
  void saveOrgFromMenu (string currentFieldString);
  void loadOrgFromMenu (string currentFieldString);
  void makeDirty ();
  void setSelNode (GameOrgNode * newNode);
  bool hasRLBN (int rlbnRes, int k);
  void loadPoseInfo (bool justRefresh);
  GameOrg * getPose (int targPoseGroup, int targRLBN, int targStep);
  string getPoseString (int targPoseGroup, int targRLBN, int targStep);
  GameOrg * getCurrentPose ();
  string getCurrentPoseString ();
  int getActionStateFromPose (int poseNum);
  void changePose (int amount);
  void saveCurrentPose ();
  void getIndexForPose (PoseKey * tempPose);
  void setPoseFromIndex (int i);
  int getPoseType (int poseIndex);
  void saveEveryPose ();
  void loadNonPoseData (int npdPose, int npdSide, int npdStep);
  void loadCurrentPose ();
  int numberIcons (int pCurCount, int x1, int y1, int x2, int y2);
  string getStringForObjectId (int objectId);
  void initAllObjects ();
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
private:
  int * pathData;
  int * cellData;
  int * extrData;
public:
  bool listGenerated;
  bool readyToRender;
  bool listEmpty;
  bool hasData;
  bool hasPath;
  bool hasCache;
  bool wasStacked;
  bool isDirty;
  bool lockWrite;
  int (begMip) [NUM_MIP_LEVELS_WITH_FIRST];
  int (endMip) [NUM_MIP_LEVELS_WITH_FIRST];
  vector <float> vertexVec;
  VolumeWrapper * terVW;
  GameVoxelWrap * voxelWrap;
  std::vector <ObjectStruct> tempObjects;
  std::vector <intPair> objectOrder;
  int curPD;
  int blockId;
  int chunkId;
  int holderId;
  int pathSize;
  int totIdealNodes;
  int totGroupIds;
  int cellDataSize;
  int cellsPerHolder;
  int visitId;
  bool pathsInvalid;
  bool idealPathsInvalid;
  bool pathsReady;
  bool idealPathsReady;
  uint holderFlags;
  std::vector <int> indexStack;
  std::vector <GroupIdStruct> groupIdStack;
  std::vector <GroupInfoStruct> groupInfoStack;
  std::vector <ConnectingNodeStruct> bestConnectingNodes;
  std::vector <int> collideIndices;
  FIVector4 offsetInHolders;
  FIVector4 gphMinInCells;
  FIVector4 gphMaxInCells;
  FIVector4 gphCenInCells;
  FIVector4 origOffset;
  Singleton * singleton;
  bool wasGenerated;
  void reset (bool destroyCache);
  GamePageHolder ();
  void init (Singleton * _singleton, int _blockId, int _chunkId, int _holderId, int trueX, int trueY, int trueZ);
  void makeDirty ();
  void gatherObjects ();
  int getCellAtCoordsLocal (int xx, int yy, int zz);
  int getCellAtInd (int ind);
  void getArrAtInd (int ind, int * tempCellData, int * tempCellData2);
  void setArrAtInd (int ind, int * tempCellData = NULL, int * tempCellData2 = NULL);
  void clearPathPreserve ();
  void clearPathSizes ();
  void checkData (bool checkPath);
  void clearGroupFlags (int targId);
  int floodFillAtInd (int firstInd, int newId, bool findCenter, GroupInfoStruct * curGI);
  void findIdealNodes ();
  int getGroupId (int pathDataIndex);
  int getInfoPD (int pathDataIndex);
  GroupIdStruct * getInfo (int pathDataIndex);
  void getInfoReport (int pathDataIndex);
  btVector3 holderIndToBTV (GamePageHolder * curPointHolder, int curPointIndex);
  void sortConNodes (GamePageHolder * endHolder, int endInd);
  void linkRegions ();
  bool prepPathRefresh (int rad);
  void refreshPaths ();
  void genCellData ();
  void bindPD (int pd);
  void unbindPD ();
  void applyFill ();
  void fillVBO ();
  PaddedDataEntry * getPadData (int ii, int jj, int kk);
  int gatherData ();
  bool checkCache ();
  void generateList ();
  void wrapPolys ();
};
LZZ_INLINE PaddedDataEntry * GamePageHolder::getPadData (int ii, int jj, int kk)
                                                                   {
		
		int cellsPerHolderPad = singleton->cellsPerHolderPad;
		int paddingInCells = singleton->paddingInCells;
		
		int i = ii + paddingInCells;
		int j = jj + paddingInCells;
		int k = kk + paddingInCells;
		
		return &(
			singleton->pdPool[curPD].data[
				i + j*cellsPerHolderPad + k*cellsPerHolderPad*cellsPerHolderPad
			]	
		);
	}
#undef LZZ_INLINE
#endif
// f00352_gamechunk.e
//

#ifndef LZZ_f00352_gamechunk_e
#define LZZ_f00352_gamechunk_e
#define LZZ_INLINE inline
class GameChunk
{
public:
  Singleton * singleton;
  VBOWrapper (vboWrapper) [NUM_MIP_LEVELS_WITH_FIRST];
  int lastPointCount;
  int mipLev;
  int changeTick;
  bool readyToRender;
  bool listEmpty;
  bool changeFlag;
  bool isDirty;
  int iHolderSize;
  int holdersPerChunk;
  GamePageHolder * * holderData;
  FIVector4 offsetInChunks;
  FIVector4 chunkCenInCells;
  int chunkId;
  int blockId;
  std::vector <ObjectStruct> localObjects;
  GameChunk ();
  void init (Singleton * _singleton, int _blockId, int _chunkId, int trueX, int trueY, int trueZ);
  void makeDirty ();
  VBOWrapper * getCurVBO ();
  void drawLoadingHolders ();
  void checkHolders ();
  void reset ();
  void fillVBO ();
};
#undef LZZ_INLINE
#endif
// f00353_gameblock.e
//

#ifndef LZZ_f00353_gameblock_e
#define LZZ_f00353_gameblock_e
#define LZZ_INLINE inline
class GameBlock
{
public:
  Singleton * singleton;
  int blockId;
  int holdersPerBlock;
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
  float plantScale;
  int terDataTexScale;
  int terDataVisSize;
  int terDataBufSize;
  int cellsPerBlock;
  int maxFloors;
  float fCellsPerBlock;
  int (dirModX) [6];
  int (dirModY) [6];
  int (dirModZ) [6];
  int (opDir) [6];
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
  int iChunkSize;
  GameChunk * * chunkData;
  BuildingNode * buildingData;
  MapNode * mapData;
  uint * terData;
  BuildingCon * curCon;
  GameBlock ();
  void init (Singleton * _singleton, int _blockId, int _x, int _y, int _z, int _xw, int _yw, int _zw);
  int getNodeIndexClamped (int _x, int _y, int _z);
  int getNodeIndex (int x, int y, int z, int bufAmount);
  int touchesPlanarBN (int x, int y, int z, int buildingType, int bufAmount);
  int touchesPlanarClass (int x, int y, int z, int classType, int bufAmount);
  int touchesPlanarClass2 (int x, int y, int z, int classType, int bufAmount);
  bool isLCorner (int x, int y, int z, int classType, bool includeSingle);
  void applyWingValues (int _x1, int _y1, int _z1, int _x2, int _y2, int _z2, int cnum, bool isWingBeg, bool isWingEnd, float multiplier);
  void addPlantNodes (GamePlantNode * curPlantNode, FIVector4 * orig, float scale);
  void addNewGeom (int _curBT, int _curAlign, float _baseOffset, FIVector4 * _p1, FIVector4 * _p2, FIVector4 * _rad, FIVector4 * _cornerRad, FIVector4 * _visInsetFromMin, FIVector4 * _visInsetFromMax, FIVector4 * _powerVals, FIVector4 * _powerVals2, FIVector4 * _thickVals, FIVector4 * _matParams, FIVector4 * _anchorPoint, int _minRot, int _maxRot);
  void connectNodes (int _x1, int _y1, int _z1, int _x2, int _y2, int _z2, int ct, int id = -1, int _heightDelta = 0, int _direction = 0, float _wallRadInCells = -1.0f, unsigned int _nodeFlags = 0);
  int getMapNodeIndex (int x, int y, int bufAmount);
  int touchesMap (int x, int y, int buildingType, int bufAmount);
  int touchesWithinRadMap (int x, int y, int buildingType, int rad, int bufAmount);
  int touches2Map (int x, int y, int buildingType, int bufAmount);
  void connectMapNodes (int _x1, int _y1, int _x2, int _y2, int buildingType, int id, int bufAmount);
  int getAdjustedHeightInHolders (int xInHolders, int yInHolders);
  float fIsNearTerrain (FIVector4 * worldMinVisInPixels);
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
  void makeMazeUG ();
};
#undef LZZ_INLINE
#endif
// f00355_volumewrapper.e
//

#ifndef LZZ_f00355_volumewrapper_e
#define LZZ_f00355_volumewrapper_e
#define LZZ_INLINE inline
class VolumeWrapper
{
public:
  FBOSet fboSet;
  GLuint volId;
  FIVector4 terGenDim;
  FIVector4 genPosMin;
  FIVector4 genPosMax;
  bool isFloat;
  bool isReady;
  VolumeWrapper ();
  void init (int z, GLenum clampMethod, bool _isFloat, int filterType);
  void copyFloatArr (float * floatArr);
  void copyCharArr (unsigned char * charArr);
};
#undef LZZ_INLINE
#endif
// f00360_threadpoolwrapper.e
//

#ifndef LZZ_f00360_threadpoolwrapper_e
#define LZZ_f00360_threadpoolwrapper_e
#define LZZ_INLINE inline
class ThreadPoolWrapper
{
public:
  int maxThreads;
  int (intData) [THREAD_DATA_COUNT];
  string (stringData) [THREAD_DATA_COUNT];
  ThreadWrapper * threadPool;
  Singleton * singleton;
  bool singleThreaded;
  std::vector <int> availIds;
  ThreadPoolWrapper ();
  void init (Singleton * _singleton, int _maxThreads, bool _singleThreaded);
  void doSpeak (string speechString);
  void funcTP (int threadId);
  void startTP (int threadId);
  bool stopTP (int threadId);
  bool anyThreadAvail ();
  bool startThread (bool checkAvail = true);
  bool anyRunning ();
  void stopAll ();
  ~ ThreadPoolWrapper ();
};
#undef LZZ_INLINE
#endif
// f00370_gamelogic.e
//

#ifndef LZZ_f00370_gamelogic_e
#define LZZ_f00370_gamelogic_e
#define LZZ_INLINE inline
class GameLogic
{
public:
  Singleton * singleton;
  std::vector <PathResult> pathSearchStack;
  std::vector <PathResult> pathFinalStack;
  std::list <LoadHolderStruct> holderStack;
  ThreadPoolWrapper * threadPoolPath;
  ThreadPoolWrapper * threadPoolList;
  FIVector4 minv;
  FIVector4 maxv;
  PathInfo testPath;
  std::vector <btVector3> tempStack;
  int idCounter;
  GamePageHolder * globEndHolder;
  int globEndGroupId;
  bool globFoundTarg;
  bool dirtyStack;
  bool allowThreadCreation;
  GameLogic ();
  void setEntTargPath (int sourceUID, int destUID);
  void init (Singleton * _singleton);
  void applyTBBehavior ();
  void applyBehavior ();
  GamePageHolder * getHolderById (int blockId, int chunkId, int holderId);
  GamePageHolder * getHolderByPR (PathResult * pr);
  bool holdersEqual (GamePageHolder * h0, GamePageHolder * h1);
  void addHolderToStack (GamePageHolder * curHolder);
  void remHolderFromStack (int opCode);
  void fillAllPaths (GamePageHolder * begHolder, GamePageHolder * endHolder, int begInd, int endInd, int opCode);
  bool addGroupToStack (ConnectingNodeStruct * testConNode, GamePageHolder * curHolder, int groupId, GamePageHolder * lastHolder, int lastGroupId, int lastIndex);
  void remGroupFromStack (int opCode);
  void fillAllGroups (GamePageHolder * begHolder, GamePageHolder * endHolder, int begInd, int endInd, int opCode);
  bool findNaivePath (PathInfo * pathInfo);
  bool findBestPath (PathInfo * pathInfo, GamePageHolder * closestHolder, GamePageHolder * closestHolder2, int bestInd, int bestInd2);
  void drawFinalPath (PathInfo * pathInfo);
  void getPath (PathInfo * pathInfo);
  void update ();
  void drawLineAtIndices (GamePageHolder * curPointHolder, int curPointIndex, GamePageHolder * curPointHolder2, int curPointIndex2);
  btVector3 holderIndToBTV (GamePageHolder * curPointHolder, int curPointIndex, bool addHalfOff);
  void drawPointAtIndex (GamePageHolder * curPointHolder, int curPointIndex, int r, int g, int b, float rad);
  void getPointsForPath (GamePageHolder * curHolderFrom, int _curInd, PathInfo * pathInfo, bool reverseOrder);
  void drawRegions (int offX, int offY, int offZ);
  int getClosestPathRad (btVector3 cpBTV, GamePageHolder * & closestHolder);
  bool anyThreadsRunning ();
  void freePD ();
  void processCurHolder (GamePageHolder * curHolder, bool doPaths);
  void loadNearestHolders (bool doUpdate);
};
#undef LZZ_INLINE
#endif
// f00373_myshapedrawer.e
//

#ifndef LZZ_f00373_myshapedrawer_e
#define LZZ_f00373_myshapedrawer_e
#define LZZ_INLINE inline
class MyShapeDrawer
{
public:
  btAlignedObjectArray <ShapeCache*> m_shapecaches;
  Singleton * singleton;
  MyShapeDrawer (Singleton * _singleton);
  void init ();
  bool enableTexture (bool enable);
  bool hasTextureEnabled () const;
  void drawCoordSystem ();
  void drawSphere (btScalar radius, int lats, int longs);
  ShapeCache * cache (btConvexShape * shape);
  void renderSquareA (float x, float y, float z);
  void glDrawVector (btVector3 const & v);
  void setId (int bodyUID, int limbUID);
  void updateMat2 ();
  void updateMat ();
  void pushNewMat (btScalar * m);
  void popMat ();
  void drawOrient (int uid);
  void drawOpenGL (btScalar * m, btCollisionShape const * shape, btVector3 const & color, int debugMode, btVector3 const & worldBoundsMin, btVector3 const & worldBoundsMax, int uid);
  ~ MyShapeDrawer ();
  void drawSceneInternal (btDiscreteDynamicsWorld const * dynamicsWorld, int pass);
  void drawScene (btDiscreteDynamicsWorld const * dynamicsWorld, bool useShadows);
};
LZZ_INLINE void MyShapeDrawer::glDrawVector (btVector3 const & v)
                                                             { glVertex3d(v[0], v[1], v[2]); }
#undef LZZ_INLINE
#endif
// f00374_myglhelper.e
//

#ifndef LZZ_f00374_myglhelper_e
#define LZZ_f00374_myglhelper_e
#define LZZ_INLINE inline
class MyGLHelper : public GUIHelperInterface
{
public:
  MyGLHelperInternalData * m_data;
  Singleton * singleton;
  MyGLHelper (Singleton * _singleton, CommonGraphicsApp * glApp);
  ~ MyGLHelper ();
  struct CommonRenderInterface * getRenderInterface ();
  void createRigidBodyGraphicsObject (btRigidBody * body, btVector3 const & color);
  void createCollisionObjectGraphicsObject (btCollisionObject * body, btVector3 const & color);
  int registerGraphicsShape (float const * vertices, int numvertices, int const * indices, int numIndices);
  int registerGraphicsInstance (int shapeIndex, float const * position, float const * quaternion, float const * color, float const * scaling);
  static void createCollisionShapeGraphicsObjectInternal (btCollisionShape * collisionShape, btTransform const & parentTransform, btAlignedObjectArray <GLInstanceVertex> & verticesOut, btAlignedObjectArray <int> & indicesOut);
  void createCollisionShapeGraphicsObject (btCollisionShape * collisionShape);
  void syncPhysicsToGraphics (btDiscreteDynamicsWorld const * rbWorld);
  void render (btDiscreteDynamicsWorld const * rbWorld);
  void createPhysicsDebugDrawer (btDiscreteDynamicsWorld * rbWorld);
  struct Common2dCanvasInterface * get2dCanvasInterface ();
  CommonParameterInterface * getParameterInterface ();
  void setUpAxis (int axis);
  void resetCamera (float camDist, float pitch, float yaw, float camPosX, float camPosY, float camPosZ);
  void autogenerateGraphicsObjects (btDiscreteDynamicsWorld * rbWorld);
  void drawText3D (char const * txt, float posX, float posY, float posZ, float size);
  struct CommonGraphicsApp * getAppInterface ();
};
#undef LZZ_INLINE
#endif
// f00375_gamephysics.e
//

#ifndef LZZ_f00375_gamephysics_e
#define LZZ_f00375_gamephysics_e
#define LZZ_INLINE inline
class GamePhysics
{
public:
  Singleton * singleton;
  BenchmarkDemo * example;
  MyOGLApp * myOGLApp;
  GUIHelperInterface * guiHelper;
  float (myMat) [16];
  float BASE_ENT_HEIGHT;
  float BASE_ENT_RAD;
  float CONTACT_THRESH;
  Matrix4 myMatrix4;
  Vector4 myVector4;
  Vector4 resVector4;
  btRigidBody * lastBodyPick;
  int lastBodyUID;
  btVector3 orig;
  btVector3 xyMask;
  btVector3 zMask;
  GamePhysics ();
  void init (Singleton * _singleton);
  void pickBody (FIVector4 * mmPD);
  void collectDebris ();
  void beginDrop ();
  void remBoxFromObj (BaseObjType _uid);
  void addBoxFromObj (BaseObjType _uid, bool refreshLimbs);
  void flushImpulses ();
  void procCol (BaseObj * * geArr, BodyStruct * * curBodyArr);
  void collideWithWorld (double curStepTime);
  void remFarAway ();
  void updateAll ();
  ~ GamePhysics ();
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
  bool skippedPrim;
  int numProvinces;
  int seaLevel;
  int seaSlack;
  int pageCount;
  int mapSwapFlag;
  int holdersPerBlock;
  int shiftCounter;
  int holdersPerChunk;
  int chunksPerWorld;
  int chunksPerBlock;
  int renderCount;
  float invalidCount;
  float invalidCountMax;
  int iBlockSize;
  int renderMethod;
  int iBufferSize;
  int maxThreads;
  int availThreads;
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
  std::vector <intTrip> gamePageHolderList;
  std::vector <coordAndIndex> roadCoords;
  std::vector <int> ocThreads;
  btVector3 (offsetVal) [4];
  string (curTargFBO) [3];
  string (curDepthFBO) [3];
  GamePageHolder * blockHolder;
  GamePageHolder * holderInFocus;
  FIVector4 lScreenCoords;
  FIVector4 cScreenCoords;
  int cellsPerWorld;
  int holdersPerWorld;
  int blocksPerWorld;
  FIVector4 entMin;
  FIVector4 entMax;
  FIVector4 camHolderPos;
  FIVector4 camBlockPos;
  FIVector4 camChunkPos;
  FIVector4 iPixelWorldCoords;
  FIVector4 pagePos;
  FIVector4 unitPos;
  FIVector4 lastUnitPos;
  FIVector4 tempVec1;
  FIVector4 tempVec2;
  FIVector4 tempVec3;
  FIVector4 tempVec4;
  FIVector4 holderLookAt;
  FIVector4 (blockPos) [2];
  FIVector4 (nodePos) [2];
  FIVector4 (nodePosInPixels) [2];
  FIVector4 (lineSeg) [2];
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
  BaseObjType lastHitObjUID;
  GameWorld ();
  void init (Singleton * _singleton);
  GameBlock * getBlockAtCoords (int xInBlocks, int yInBlocks, int zInBlocks, bool createOnNull = false);
  void clearAllHolders ();
  GameChunk * getChunkAtCoords (int x, int y, int z, bool createOnNull = false);
  GamePageHolder * getHolderAtCoords (int x, int y, int z, bool createOnNull = false);
  GameChunk * getChunkAtId (int blockId, int chunkId);
  GamePageHolder * getHolderAtId (int blockId, int chunkId, int holderId);
  GameBlock * getBlockAtId (int id);
  int getCellInd (btVector3 cParam, GamePageHolder * & curHolder);
  int getCellInd (GamePageHolder * & curHolder, int xv, int yv, int zv);
  int getCellAtCoords (int xv, int yv, int zv);
  float getCellAtCoordsLin (btVector3 pos);
  btVector3 getNormalAtCoord (btVector3 coord, float * cellVal);
  void setArrAtCoords (int xv, int yv, int zv, int * tempCellData, int * tempCellData2);
  void getArrAtCoords (int xv, int yv, int zv, int * tempCellData, int * tempCellData2);
  void fireEvent (BaseObjType uid, int opCode, float fParam);
  void doMedian ();
  void preUpdate ();
  void update ();
  void toggleVis (GameEnt * se);
  void ensureBlocks ();
  void drawVol (VolumeWrapper * curVW, FIVector4 * minc, FIVector4 * maxc, bool copyToTex, bool forceFinish, bool getVoro = false, bool getBlockHolders = false);
  void updateLimbTBOData (bool showLimbs);
  void drawPrim (bool doSphereMap, bool doTer, bool doPoly);
  void drawOrg (GameOrg * curOrg, bool drawAll);
  void drawNodeEnt (GameOrgNode * curNode, FIVector4 * basePosition, float scale, int drawMode, bool drawAll);
  void polyCombine ();
  void drawPolys (string fboName, int minPeel, int maxPeel);
  void rastChunk (int rad, uint flags);
  void renderGeom ();
  void updateMouseCoords (FIVector4 * fPixelWorldCoordsBase);
  float weighPath (float x1, float y1, float x2, float y2, float rad, bool doSet, bool isOcean);
  float findBestPath (float x1, float y1, float x2, float y2, int generation, int roadIndex, bool doSet, bool isOcean);
  void initMap ();
  void drawMap ();
  void doBlur (string fboName, int _baseFBO = 0);
  void drawBasicPrims (bool doShadow);
  void rasterHolders (bool doShadow);
  void renderDebug ();
  void finalStep (bool postToScreen);
  void postProcess (bool postToScreen);
  ~ GameWorld ();
};
#undef LZZ_INLINE
#endif
 

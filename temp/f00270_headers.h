// f00290_wsrequesthandler.e
//

#ifndef LZZ_f00290_wsrequesthandler_e
#define LZZ_f00290_wsrequesthandler_e
#define LZZ_INLINE inline
class WebSocketRequestHandler : public HTTPRequestHandler
{
public:
  WebSocketServer * ws_ptr;
  WebSocketRequestHandler (WebSocketServer * _ws_ptr);
  ~ WebSocketRequestHandler ();
  void handleRequest (HTTPServerRequest & request, HTTPServerResponse & response);
};
#undef LZZ_INLINE
#endif
// f00291_rhfactory.e
//

#ifndef LZZ_f00291_rhfactory_e
#define LZZ_f00291_rhfactory_e
#define LZZ_INLINE inline
class RequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
  WebSocketServer * ws_ptr;
  RequestHandlerFactory (WebSocketServer * _ws_ptr);
  HTTPRequestHandler * createRequestHandler (HTTPServerRequest const & request);
};
#undef LZZ_INLINE
#endif
// f00292_wsserver.e
//

#ifndef LZZ_f00292_wsserver_e
#define LZZ_f00292_wsserver_e
#define LZZ_INLINE inline
class WebSocketServer : public Poco::Util::ServerApplication
{
public:
  bool dataReady;
  bool isWorking;
  bool isJSON;
  int MAX_FRAME_SIZE;
  charArr recBuffer;
  charArr okBuffer;
  WebSocketServer ();
  ~ WebSocketServer ();
protected:
  void initialize (Application & self);
  void uninitialize ();
  void defineOptions (OptionSet & options);
  void handleOption (std::string const & name, std::string const & value);
  void displayHelp ();
  int main (std::vector <std::string> const & args);
private:
  bool _helpRequested;
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
  E_OBJ activeObject;
  E_OBJ tempObj;
  E_MOUSE_STATE mouseState;
  E_GRASS_STATE grassState;
  eProgramState programState;
  eProgramAction (progActionsDown) [E_PS_SIZE*256];
  eProgramAction (progActionsUp) [E_PS_SIZE*256];
  bool emptyVDNotReady;
  bool radiosityOn;
  bool updateLock;
  bool isFullScreen;
  bool changesMade;
  bool mapInvalid;
  bool bufferInvalid;
  bool wsBufferInvalid;
  bool forceGetPD;
  bool mouseLeftDown;
  bool mouseRightDown;
  bool pboSupported;
  bool notQuit;
  bool timerNotSet;
  bool lbDown;
  bool rbDown;
  bool mbDown;
  bool isZooming;
  bool isPanning;
  bool softMode;
  bool isBare;
  bool reportPagesDrawn;
  bool showMap;
  bool traceOn;
  bool waterOn;
  bool firstRun;
  int maxLayers;
  int maxChanges;
  int maxPooledRes;
  int poolItemsCreated;
  int baseW;
  int baseH;
  int scaleFactor;
  int activeMode;
  int visPageSizeInPixels;
  int visPageSizeInUnits;
  int unitSizeInPixels;
  int maxHeightInUnits;
  int holdersPerLot;
  int pixelsPerLot;
  int pixelsPerMeter;
  int pixelsPerUnit;
  int metersPerLot;
  int unitsPerMeter;
  int unitsPerLot;
  int extraRad;
  int defaultWinW;
  int defaultWinH;
  int shadersAreLoaded;
  int readyToRecompile;
  int lastPosX;
  int lastPosY;
  int frameCount;
  int maxH;
  int maxW;
  int screenWidth;
  int screenHeight;
  int mouseMovingSize;
  int mouseMovingLoc;
  int mouseMovingStepsBack;
  int mouseCount;
  int lastMouseX;
  int lastMouseY;
  int bufferedPageSizeInUnits;
  int voroSize;
  int geomCounter;
  int bufferMultRec;
  int holderSizeInPages;
  int holderSizeInPixels;
  uint volGenFBOX;
  uint palWidth;
  uint palHeight;
  uint blockShift;
  float directPass;
  float fogOn;
  float gridOn;
  float mapSampScale;
  float curBrushRad;
  float diskOn;
  float currentFBOResolutionX;
  float currentFBOResolutionY;
  float mouseX;
  float mouseY;
  float mouseXUp;
  float mouseYUp;
  float cameraZoom;
  float targetZoom;
  float curTime;
  float lastTime;
  float myDelta;
  float mdTime;
  float muTime;
  float heightmapMax;
  float bufferMult;
  float holderSizeMB;
  float * paramArr;
  float * matCountArr;
  int * geomIDArr;
  float * paramArrMap;
  FIVector4 minBoundsInPixels;
  FIVector4 maxBoundsInPixels;
  FIVector4 mouseUpPD;
  FIVector4 mouseDownPD;
  FIVector4 mouseMovePD;
  FIVector4 grassWH;
  FIVector4 worldSizeInPages;
  FIVector4 worldSizeInLots;
  FIVector4 worldSizeInHolders;
  FIVector4 worldSizeInBlocks;
  std::vector <DynObject*> dynObjects;
  int iNodeDivsPerLot;
  int blockSizeInLots;
  int blockSizeInHolders;
  int blockSizeInPages;
  int blockSizeInPixels;
  int maxFloors;
  FIVector4 mouseStart;
  FIVector4 mouseEnd;
  FIVector4 mapFreqs;
  FIVector4 mapAmps;
  FIVector4 * mouseMoving;
  FIVector4 mouseVel;
  FIVector4 worldSeed;
  FIVector4 bufferDim;
  FIVector4 bufferDimHalf;
  FIVector4 origin;
  FIVector4 lastModXYZ;
  FIVector4 panMod;
  Image * imageHM0;
  Image * imageHM1;
  Image * cloudImage;
  GameGeom * highlightedGeom;
  GameGeom * selectedGeom;
  Shader * curShaderPtr;
  string curShader;
  string allText;
  list <int> holderPoolIds;
  vector <intPair> orderedIds;
  vector <PooledResource*> holderPoolItems;
  vector <string> shaderStrings;
  vector <string> fboStrings;
  vector <string> shaderTextureIDs;
  map <string, Shader*> shaderMap;
  map <string, FBOSet*> fboMap;
  string curVGString;
  GLuint volID;
  GLuint volIDLinear;
  GLuint volIDEmpty;
  GLuint volIDEmptyLinear;
  GLuint volGenID;
  GLuint terrainID;
  GLuint volTris;
  GLuint sliceTris;
  GLuint (grassTris) [MAX_GRASS_LEV];
  uint * lookup2to3;
  unsigned char * resultImage;
  charArr nullBuffer;
  charArr lastImageBuffer;
  charArr lastJSONBuffer;
  JSONValue * rootObj;
  WebSocketServer * myWS;
  Timer myTimer;
  GameWorld * gw;
  float (lightArr) [MAX_LIGHTS*16];
  int numLights;
  Singleton ();
  void init (int _defaultWinW, int _defaultWinH, int _scaleFactor, WebSocketServer * _myWS);
  void reorderIds ();
  int findFurthestHolderId ();
  int requestPoolId (int blockID, int holderID);
  static void qNormalizeAngle (int & angle);
  void perspectiveProjection ();
  void orthographicProjection ();
  ~ Singleton ();
  float genRand (float LO, float HI);
  void setProgAction (eProgramState ps, unsigned char kc, eProgramAction pa, bool isDown);
  void setProgActionAll (unsigned char kc, eProgramAction pa, bool isDown);
  void createGrassList (int index);
  void drawCrossHairs (FIVector4 originVec, float radius);
  void drawCubeCentered (FIVector4 originVec, float radius);
  void drawBoxUp (FIVector4 originVec, float radiusX, float radiusY, float diamZ);
  void drawBox (FIVector4 * minVec, FIVector4 * maxVec);
  float glslMod (float x, float y);
  void createVTList ();
  void doShaderRefresh ();
  void setMatrices (int w, int h);
  void setWH (int w, int h);
  void sampleFBODirect (FBOSet * fbos, int offset = 0);
  void unsampleFBODirect (FBOSet * fbos, int offset = 0);
  void bindFBODirect (FBOSet * fbos, int doClear = 1);
  void sampleFBO (string fboName, int offset = 0, int swapFlag = -1);
  void unsampleFBO (string fboName, int offset = 0, int swapFlag = -1);
  FBOSet * getFBOSet (string fboName);
  FBOWrapper * getFBOWrapper (string fboName, int offset);
  void copyFBO (string src, string dest);
  void copyFBO2 (string src, string dest);
  void bindFBO (string fboName, int swapFlag = -1);
  void unbindFBO ();
  void bindShader (string shaderName);
  void unbindShader ();
  void setShaderArrayfVec3 (string paramName, float * x, int count);
  void setShaderArrayfVec4 (string paramName, float * x, int count);
  void setShaderArray (string paramName, float * x, int count);
  void setShaderFloat (string paramName, float x);
  void setShaderInt (string paramName, int x);
  void setShaderfVec2 (string paramName, FIVector4 * v);
  void setShaderVec2 (string paramName, float x, float y);
  void setShaderVec3 (string paramName, float x, float y, float z);
  void setShaderfVec3 (string paramName, FIVector4 * v);
  void setShaderVec4 (string paramName, float x, float y, float z, float w);
  void setShaderfVec4 (string paramName, FIVector4 * v);
  void setShaderTexture (int multitexNumber, uint texID);
  void setShaderTexture3D (int multitexNumber, uint texID);
  bool shiftDown ();
  bool ctrlDown ();
  bool altDown ();
  void drawQuadBounds (float fx1, float fy1, float fx2, float fy2);
  void drawFSQuad (float zoom);
  void drawFSQuadOffset (float xOff, float yOff, float zoom);
  void drawFBO (string fboName, int ind, float zoom, int swapFlag = -1);
  void drawFBOOffsetDirect (FBOSet * fbos, int ind, float xOff, float yOff, float zoom);
  void drawFBOOffset (string fboName, int ind, float xOff, float yOff, float zoom);
  float getSeaLevelInPixels ();
  float getCityHeight ();
  float getHeightAtPixelPos (float x, float y, bool ignoreCity = false);
  void moveCamera (FIVector4 * modXYZ);
  void moveObject (float dx, float dy, float zoom);
  void toggleFullScreen ();
  void updateVG ();
  void setCameraToElevation ();
  void processSpecialKeys (int key, int _x, int _y);
  void processKey (unsigned char key, int _x, int _y, bool isPressed);
  void keyboardUp (unsigned char key, int _x, int _y);
  void keyboardDown (unsigned char key, int _x, int _y);
  void getPixData (FIVector4 * toVector, int xv, int yv);
  GameGeom * findNearestGeom (FIVector4 * testPoint);
  void mouseMove (int _x, int _y);
  void worldToScreen (FIVector4 * sc, FIVector4 * wc);
  void screenToWorld (FIVector4 * tc, FIVector4 * wc);
  void mouseClick (int button, int state, int _x, int _y);
  void processB64 (charArr * sourceBuffer, charArr * saveBuffer);
  bool processJSON (charArr * sourceBuffer, charArr * saveBuffer);
  void loadAllData ();
  void saveAllData ();
  bool loadFile (char * fileName, charArr * dest);
  bool saveFile (char * fileName, charArr * source);
  void display ();
  void reshape (int w, int h);
  void idleFunc ();
};
LZZ_INLINE float Singleton::glslMod (float x, float y)
                                           {
		return x - y * floor(x/y);
    }
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
public:
  intPair usedByHolderId;
  Singleton * singleton;
  PooledResource ();
  FBOSet * getFBOS (int fboNum);
  void init (Singleton * _singleton);
};
#undef LZZ_INLINE
#endif
// f00350_gamepage.e
//

#ifndef LZZ_f00350_gamepage_e
#define LZZ_f00350_gamepage_e
#define LZZ_INLINE inline
class GamePage : public Poco::Runnable
{
private:
  uint * volData;
  uint * volDataLinear;
public:
  Singleton * singleton;
  int thisPageId;
  int bufferedPageSizeInUnits;
  FIVector4 offsetInUnits;
  FIVector4 offsetInPages;
  FIVector4 offsetInPagesLocal;
  float pageDepth;
  float seaHeightInPixels;
  int iVolumeSize;
  bool hasGeom;
  bool hasTerrain;
  bool hasWater;
  bool hasWindow;
  bool hasTree;
  bool volDataModified;
  bool isRendering;
  int paramsPerEntry;
  int numEntries;
  int totParams;
  int maxEntries;
  int maxHeightInUnits;
  FIVector4 worldSeed;
  bool threadRunning;
  float unitSizeInPixels;
  E_STATES curState;
  E_STATES nextState;
  FIVector4 worldMinVisInPixels;
  FIVector4 worldMaxVisInPixels;
  FIVector4 worldMinBufInPixels;
  FIVector4 worldMaxBufInPixels;
  FIVector4 scaleAndOffset;
  FIVector4 worldUnitMin;
  FIVector4 worldUnitMax;
  E_FILL_STATE fillState;
  GamePageHolder * parentGPH;
  uint * getVolData ();
  uint * getVolDataLinear ();
  GamePage ();
  void init (Singleton * _singleton, GamePageHolder * _parentGPH, int _thisPageId, int offsetX, int offsetY, int offsetZ, int oxLoc, int oyLoc, int ozLoc);
  void copyToTexture (bool isForEmptyVD);
  void addGeom (bool justTesting);
  void generateVolume ();
  void getCoords ();
  ~ GamePage ();
  void run ();
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
  int blockID;
  int holderID;
  bool isDirty;
  bool hasTrans;
  bool hasSolids;
  FIVector4 offsetInHolders;
  FIVector4 gphMinInPixels;
  FIVector4 gphMaxInPixels;
  FIVector4 offsetInBlocks;
  PooledResource * gpuRes;
  Singleton * singleton;
  std::vector <intPair> containsGeomIds;
  GamePage * * pageData;
  GamePageHolder ();
  void init (Singleton * _singleton, int _blockID, int _holderID, int trueX, int trueY, int trueZ);
  void clearSet (bool forceClear);
  void refreshChildren (bool refreshImmediate);
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
  int blockID;
  int blockSizeInHolders;
  int blockSizeInLots;
  FIVector4 offsetInBlocks;
  FIVector4 offsetInBlocksWrapped;
  std::vector <GameGeom*> gameGeom;
  int iHolderSize;
  int maxFloors;
  GamePageHolder * * holderData;
  int iNodeDivsPerLot;
  int iBuildingNodeSize;
  int iBuildingNodesPerSide;
  int iBuildingNodesPerSideM1;
  float fBuildingNodesPerSideM1;
  BuildingNode * buildingData;
  GLubyte * terrainHeights;
  GameWorld * gw;
  BuildingNode nullNode;
  GameBlock ();
  void init (Singleton * _singleton, int _blockID, int _x, int _y, int _xw, int _yw);
  BuildingNode * getNode (int x, int y);
  BuildingNodeProp * getPropAtLevel (int x, int y, int dir, int lev, int nodeType);
  BuildingNodeProp * getPropAtIndLevel (int i, int dir, int lev, int nodeType);
  int touches (int x, int y, int buildingType);
  int touchesHeight (int x, int y, int buildingType);
  int touchDir (int x, int y, int buildingType);
  bool testHeight (int _x1, int _y1, int _x2, int _y2, int heightVal);
  int touchesCenter (int x, int y, int buildingType);
  int sameHeight (int x, int y);
  int touches2 (int x, int y, int buildingType);
  void connectNodes (int _x1, int _y1, int _x2, int _y2, int buildingType, int id);
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
  int ((diagrams) [E_RENDER_LENGTH]) [E_STATE_LENGTH];
  int renderMethod;
  int iBufferSize;
  int maxThreads;
  int availThreads;
  int visPageSizeInPixels;
  int holderSizeInPages;
  int MIN_MIP;
  int MAX_MIP;
  int AVG_MIP;
  int hmChannel;
  int idChannel;
  int densityChannel;
  int blockChannel;
  int stChannel;
  int btChannel;
  int pathChannel;
  int houseChannel;
  int * curDiagram;
  int * provinceGrid;
  int * provinceX;
  int * provinceY;
  bool doDrawFBO;
  bool lastProcResult;
  bool mapLockOn;
  float mapStep;
  float mapTrans;
  float newZoom;
  int (dirFlags) [4];
  int (dirFlagsOp) [4];
  int (dirFlagsO) [4];
  int (dirFlagsOpO) [4];
  int (dirModX) [4];
  int (dirModY) [4];
  int (opDir) [4];
  int dirFlagClear;
  int visFlag;
  int visFlagO;
  int activeFBO;
  bool noiseGenerated;
  bool wavesGenerated;
  std::vector <coordAndIndex> roadCoords;
  std::vector <GamePageHolder*> holdersToRefresh;
  vector <int> ocThreads;
  FIVector4 lScreenCoords;
  FIVector4 cScreenCoords;
  FIVector4 worldSizeInPages;
  FIVector4 worldSizeInHolders;
  FIVector4 worldSizeInBlocks;
  FIVector4 geomMin;
  FIVector4 geomMax;
  FIVector4 camPagePos;
  FIVector4 camHolderPos;
  FIVector4 cutHolderPos;
  FIVector4 camBlockPos;
  FIVector4 iPixelWorldCoords;
  FIVector4 pagePos;
  FIVector4 unitPos;
  FIVector4 lastUnitPos;
  FIVector4 lastPagePos;
  FIVector4 minLRInPixels;
  FIVector4 maxLRInPixels;
  FIVector4 minv;
  FIVector4 maxv;
  FIVector4 tempVec;
  FIVector4 unitPosMin;
  FIVector4 unitPosMax;
  FIVector4 unitPosMinIS;
  FIVector4 unitPosMaxIS;
  FIVector4 startBounds;
  FIVector4 endBounds;
  FIVector4 * fogPos;
  FIVector4 * cutPos;
  FIVector4 * lightPos;
  FIVector4 * globLightPos;
  FIVector4 lightPosBase;
  FIVector4 * cameraPos;
  Singleton * singleton;
  GameBlock * * blockData;
  FBOWrapper * curFBO;
  FBOWrapper * curFBO2;
  Poco::ThreadPool threadpool;
  GameWorld ();
  void init (Singleton * _singleton);
  int wrapCoord (int val, int mv);
  GameBlock * getBlockAtCoords (int xInBlocks, int yInBlocks, bool createOnNull = false);
  GamePageHolder * getHolderAtCoords (int x, int y, int z, bool createOnNull = false);
  GamePageHolder * getHolderAtID (intPair id);
  GamePage * getPageAtIndex (int ind);
  GamePage * getPageAtCoords (int x, int y, int z, bool createOnNull = false);
  bool checkBounds (int k);
  void update ();
  bool processPages ();
  bool addHolderToRefresh (GamePageHolder * toAdd);
  int getHoldersInGeom (GameGeom * gg);
  void refreshHoldersInList (bool doImmediate);
  void actionOnHolders (int action);
  void drawHolder (GamePageHolder * gp, int curLayer);
  void combineBuffers ();
  void renderGeom ();
  void modifyUnit (FIVector4 * fPixelWorldCoordsBase, E_BRUSH brushAction);
  void renderWorldSpaceGPU (float processPagesFBO, float processGeomFBO, float processWaterFBO);
  void renderWorldSpace ();
  void renderGrass ();
  float weighPath (float x1, float y1, float x2, float y2, float rad, bool doSet, bool isOcean);
  float findBestPath (float x1, float y1, float x2, float y2, int generation, int roadIndex, bool doSet, bool isOcean);
  void initMap ();
  void drawMap ();
  void doBlur (string fboName, float blurAmount);
  void postProcess ();
  ~ GameWorld ();
};
#undef LZZ_INLINE
#endif
 

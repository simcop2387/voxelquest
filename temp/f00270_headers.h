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
  bool isFullScreen;
  bool changesMade;
  bool bufferInvalid;
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
  bool (keyDownArr) [MAX_KEYS];
  bool wsBufferInvalid;
  bool softMode;
  bool isBare;
  bool reportPagesDrawn;
  int poolItemsCreated;
  int baseW;
  int baseH;
  int scaleFactor;
  int activeMode;
  int visPageSizeInPixels;
  int visPageSizeInUnits;
  int unitSizeInPixels;
  int bufferMult;
  int maxHeightInUnits;
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
  uint volGenFBOSize;
  uint slicesPerPitch;
  uint palWidth;
  uint palHeight;
  float curBrushRad;
  float diskOn;
  float grassHeight;
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
  FIVector4 activeObjectPos;
  FIVector4 minBoundsInPixels;
  FIVector4 maxBoundsInPixels;
  FIVector4 mouseUpPD;
  FIVector4 mouseDownPD;
  FIVector4 mouseMovePD;
  FIVector4 worldSizeInPages;
  FIVector4 cameraPos;
  FIVector4 lightPos;
  FIVector4 mouseStart;
  FIVector4 mouseEnd;
  FIVector4 * mouseMoving;
  FIVector4 mouseVel;
  FIVector4 worldSeed;
  FIVector4 fogPos;
  FIVector4 bufferDim;
  FIVector4 bufferDimHalf;
  FIVector4 origin;
  FIVector4 lastModXYZ;
  FIVector4 panMod;
  string curShader;
  string allText;
  list <int> pagePoolIds;
  vector <PooledResource*> pagePoolItems;
  vector <string> shaderStrings;
  vector <string> fboStrings;
  vector <string> shaderTextureIDs;
  map <string, Shader*> shaderMap;
  map <string, FBOSet*> fboMap;
  GLuint volTris;
  GLuint grassTris;
  uint * lookup2to3;
  GLuint lookup2to3ID;
  unsigned char * resultImage;
  charArr nullBuffer;
  charArr lastImageBuffer;
  charArr lastJSONBuffer;
  JSONValue * rootObj;
  WebSocketServer * myWS;
  Timer myTimer;
  GameWorld * gw;
  std::vector <GameGeom*> gameGeom;
  Singleton ();
  void init (int _defaultWinW, int _defaultWinH, int _scaleFactor, WebSocketServer * _myWS);
  void sendPoolIdToFront (int id);
  int requestPoolId (int requestingPageId);
  static void qNormalizeAngle (int & angle);
  void setupLookups ();
  void perspectiveProjection ();
  void orthographicProjection ();
  ~ Singleton ();
  float genRand (float LO, float HI);
  void setProgAction (eProgramState ps, unsigned char kc, eProgramAction pa, bool isDown);
  void setProgActionAll (unsigned char kc, eProgramAction pa, bool isDown);
  void keySetup ();
  void createGrassList ();
  void drawCubeCentered (FIVector4 originVec, float radius);
  void drawBox (FIVector4 minVec, FIVector4 maxVec);
  void createVTList ();
  void doShaderRefresh ();
  void setMatrices (int w, int h);
  void setWH (int w, int h);
  void sampleFBODirect (FBOSet * fbos, int offset = 0);
  void unsampleFBODirect (FBOSet * fbos, int offset = 0);
  void bindFBODirect (FBOSet * fbos);
  void sampleFBO (string fboName, int offset = 0);
  void unsampleFBO (string fboName, int offset = 0);
  FBOWrapper * getFBOWrapper (string fboName, int offset);
  void bindFBO (string fboName);
  void unbindFBO ();
  void bindShader (string shaderName);
  void unbindShader ();
  void setShaderArray (string paramName, float * x, int count);
  void setShaderFloat (string paramName, float x);
  void setShaderfVec2 (string paramName, FIVector4 * v);
  void setShaderVec2 (string paramName, float x, float y);
  void setShaderVec3 (string paramName, float x, float y, float z);
  void setShaderfVec3 (string paramName, FIVector4 * v);
  void setShaderVec4 (string paramName, float x, float y, float z, float w);
  void setShaderfVec4 (string paramName, FIVector4 * v);
  void setShaderTexture (uint texID, int multitexNumber);
  void setShaderTexture3D (uint texID, int multitexNumber);
  bool shiftDown ();
  bool ctrlDown ();
  bool altDown ();
  void drawFSQuad (float zoom);
  void drawFSQuadOffset (float xOff, float yOff, float zoom);
  void drawFBO (string fboName, int ind, float zoom);
  void drawFBOOffsetDirect (FBOSet * fbos, int ind, float xOff, float yOff, float zoom);
  void drawFBOOffset (string fboName, int ind, float xOff, float yOff, float zoom);
  void moveCamera (FIVector4 * modXYZ);
  void moveObject (float dx, float dy, float zoom);
  void doAction (eProgramAction pa);
  void processSpecialKeys (int key, int _x, int _y);
  void processKey (unsigned char key, int _x, int _y, bool isPressed);
  void keyboardUp (unsigned char key, int _x, int _y);
  void keyboardDown (unsigned char key, int _x, int _y);
  int clamp (int val, int min, int max);
  float clampf (float val, float min, float max);
  void getPixData (FIVector4 * toVector, int xv, int yv);
  void mouseMove (int _x, int _y);
  void worldToScreen (FIVector4 * sc, FIVector4 * wc);
  void screenToWorld (FIVector4 * tc, FIVector4 * wc);
  void mouseClick (int button, int state, int _x, int _y);
  void processB64 (charArr * sourceBuffer, charArr * saveBuffer);
  void processJSON (charArr * sourceBuffer, charArr * saveBuffer);
  void loadAllData ();
  void saveAllData ();
  bool loadFile (char * fileName, charArr * dest);
  bool saveFile (char * fileName, charArr * source);
  void display ();
  void reshape (int w, int h);
  void idleFunc ();
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
public:
  FBOSet * fboSet;
  uint volID;
  uint volIDLinear;
  int bufferedPageSizeInUnits;
  int usedByPageId;
  Singleton * singleton;
  PooledResource ();
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
public:
  Singleton * singleton;
  int thisPageId;
  int usingPoolId;
  PooledResource * gpuRes;
  int threshVal;
  int bufferedPageSizeInUnits;
  FIVector4 offsetInUnits;
  int iVolumeSize;
  uint * volData;
  uint * volDataLinear;
  int paramsPerEntry;
  int paramArrLen;
  int totParams;
  float * paramArr;
  int maxHeightInUnits;
  int totLenO2;
  int totLenVisO2;
  FIVector4 worldSeed;
  bool isDirty;
  bool threadRunning;
  float unitSizeInPixels;
  E_STATES curState;
  E_STATES nextState;
  FIVector4 worldMinVisInPixels;
  FIVector4 worldMaxVisInPixels;
  FIVector4 worldMinBufInPixels;
  FIVector4 worldMaxBufInPixels;
  FIVector4 worldUnitMin;
  FIVector4 worldUnitMax;
  E_FILL_STATE fillState;
  GamePage ();
  void init (Singleton * _singleton, int _thisPageId, FIVector4 * _offsetInUnits);
  uint NumberOfSetBits (uint i);
  uint clamp (uint val);
  void createSimplexNoise ();
  void unbindGPUResources ();
  void copyToTexture ();
  void generateVolume ();
  ~ GamePage ();
  void run ();
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
  int pageCount;
  int visPageSizeInUnits;
  int iVolumeSize;
  int ((diagrams) [E_RENDER_LENGTH]) [E_STATE_LENGTH];
  int * curDiagram;
  int renderMethod;
  bool doDrawFBO;
  bool lastProcResult;
  bool updatePoolOrder;
  vector <int> ocThreads;
  FIVector4 lScreenCoords;
  FIVector4 aoScreenCoords;
  FIVector4 worldSizeInPages;
  FIVector4 curPos;
  FIVector4 camPagePos;
  FIVector4 iPixelWorldCoords;
  FIVector4 pagePos;
  FIVector4 unitPos;
  FIVector4 lastUnitPos;
  FIVector4 lastPagePos;
  FIVector4 tempVec;
  FIVector4 unitPosMin;
  FIVector4 unitPosMax;
  FIVector4 unitPosMinIS;
  FIVector4 unitPosMaxIS;
  FIVector4 startBounds;
  FIVector4 endBounds;
  Singleton * singleton;
  GamePage * * worldData;
  int iBufferSize;
  Poco::ThreadPool threadpool;
  int maxThreads;
  int availThreads;
  int visPageSizeInPixels;
  GameWorld ();
  bool checkBounds (int i, int j, int k);
  void resetToState (E_STATES resState);
  void init (Singleton * _singleton);
  void update ();
  bool processPages ();
  void renderPages ();
  void drawPage (GamePage * gp, int dx, int dy, int dz);
  void combineBuffers ();
  void renderGeom ();
  void modifyUnit (FIVector4 * fPixelWorldCoordsBase, E_BRUSH brushAction);
  void renderWorldSpace ();
  void renderGrass ();
  void postProcess ();
  ~ GameWorld ();
};
#undef LZZ_INLINE
#endif
 

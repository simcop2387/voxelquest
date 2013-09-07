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
  bool (keyDownArr) [MAX_KEYS];
  bool wsBufferInvalid;
  int curBrushRad;
  float diskOn;
  float grassHeight;
  bool showUI;
  bool isBare;
  GLuint volTris;
  GLuint grassTris;
  vector <string> shaderStrings;
  vector <string> fboStrings;
  vector <string> shaderTextureIDs;
  map <string, Shader*> shaderMap;
  map <string, FBOSet*> fboMap;
  string curShader;
  string allText;
  eProgramState programState;
  eProgramAction (progActionsDown) [E_PS_SIZE*256];
  eProgramAction (progActionsUp) [E_PS_SIZE*256];
  bool isFullScreen;
  int baseW;
  int baseH;
  E_MOUSE_STATE mouseState;
  int scaleFactor;
  int visPageSizeInPixels;
  int visPageSizeInUnits;
  int unitSizeInPixels;
  int bufferMult;
  int maxHeightInUnits;
  int extraRad;
  int defaultWinW;
  int defaultWinH;
  float currentFBOResolutionX;
  float currentFBOResolutionY;
  float mouseX;
  float mouseY;
  float mouseXUp;
  float mouseYUp;
  bool shiftPressed;
  bool ctrlPressed;
  bool altPressed;
  float cameraZoom;
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
  uint * lookup2to3;
  GLuint lookup2to3ID;
  int shadersAreLoaded;
  int readyToRecompile;
  FIVector4 bufferDim;
  FIVector4 bufferDimHalf;
  bool lbDown;
  bool rbDown;
  bool mbDown;
  FIVector4 worldSeed;
  Timer myTimer;
  float curTime;
  float lastTime;
  E_GRASS_STATE grassState;
  float myDelta;
  int frameCount;
  bool changesMade;
  bool bufferInvalid;
  int maxH;
  int maxW;
  int screenWidth;
  int screenHeight;
  bool mouseLeftDown;
  bool mouseRightDown;
  bool pboSupported;
  bool notQuit;
  bool timerNotSet;
  GameWorld * gw;
  int lastPosX;
  int lastPosY;
  FIVector4 fogPos;
  static void qNormalizeAngle (int & angle);
  void setupLookups ();
  void perspectiveProjection ();
  void orthographicProjection ();
  Singleton ();
  ~ Singleton ();
  float genRand (float LO, float HI);
  void setProgAction (eProgramState ps, unsigned char kc, eProgramAction pa, bool isDown);
  void setProgActionAll (unsigned char kc, eProgramAction pa, bool isDown);
  void keySetup ();
  void createGrassList ();
  void drawCubeCentered (FIVector4 origin, float radius);
  void drawBox (FIVector4 minVec, FIVector4 maxVec);
  void createVTList ();
  void init (int _defaultWinW, int _defaultWinH, int _scaleFactor);
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
  void setShaderFloat (string paramName, float x);
  void setShaderfVec2 (string paramName, FIVector4 * v);
  void setShaderVec2 (string paramName, float x, float y);
  void setShaderVec3 (string paramName, float x, float y, float z);
  void setShaderfVec3 (string paramName, FIVector4 * v);
  void setShaderVec4 (string paramName, float x, float y, float z, float w);
  void setShaderfVec4 (string paramName, FIVector4 * v);
  void setShaderTexture (uint texID, int multitexNumber);
  void setShaderTexture3D (uint texID, int multitexNumber);
  void drawFSQuad (float zoom);
  void drawFSQuadOffset (float xOff, float yOff, float zoom);
  void drawFBO (string fboName, int ind, float zoom);
  void drawFBOOffsetDirect (FBOSet * fbos, int ind, float xOff, float yOff, float zoom);
  void drawFBOOffset (string fboName, int ind, float xOff, float yOff, float zoom);
  void moveObject (float dx, float dy, float zoom);
  void doAction (eProgramAction pa);
  void processSpecialKeys (int key, int _x, int _y);
  void processKey (unsigned char key, int _x, int _y, bool isPressed);
  void keyboardUp (unsigned char key, int _x, int _y);
  void keyboardDown (unsigned char key, int _x, int _y);
  int clamp (int val, int min, int max);
  void getPixData (FIVector4 * toVector, int xv, int yv);
  void mouseMove (int _x, int _y);
  void worldToScreen (FIVector4 * sc, FIVector4 * wc);
  void screenToWorld (FIVector4 * tc, FIVector4 * wc);
  void mouseClick (int button, int state, int _x, int _y);
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
  PooledResource ();
  void init ();
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
  int threshVal;
  int iDim;
  FIVector4 offsetInUnits;
  int iVolumeSize;
  uint * volData;
  Singleton * singleton;
  FBOSet * fboSet;
  uint volID;
  uint volIDLinear;
  int maxHeightInUnits;
  int totLenO2;
  int totLenVisO2;
  FIVector4 worldSeed;
  bool isDirty;
  bool threadRunning;
  float unitSizeInPixels;
  E_STATES curState;
  E_STATES nextState;
  FIVector4 worldMin;
  FIVector4 worldMax;
  FIVector4 worldUnitMin;
  FIVector4 worldUnitMax;
  E_FILL_STATE fillState;
  GamePage ();
  void init (Singleton * _singleton, FIVector4 * _offsetInUnits);
  uint NumberOfSetBits (uint i);
  uint clamp (uint val);
  void createSimplexNoise ();
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
  vector <int> ocThreads;
  FIVector4 screenCoords;
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
  void update (bool changesMade, bool bufferInvalid);
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
 

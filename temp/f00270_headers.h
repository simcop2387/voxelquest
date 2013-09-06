// f00300_singleton.e
//

#ifndef LZZ_f00300_singleton_e
#define LZZ_f00300_singleton_e
#define LZZ_INLINE inline
class Singleton
{
public:
  E_OBJ activeObject;
  bool (keyDownArr) [MAX_KEYS];
  bool wsBufferInvalid;
  float diskOn;
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
  int scaleFactor;
  int iRSize;
  int iPageSize;
  int unitSize;
  int unitScale;
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
  FIVector4 minBoundsInPixels;
  FIVector4 maxBoundsInPixels;
  FIVector4 mouseUpPD;
  FIVector4 mouseDownPD;
  FIVector4 mouseMovePD;
  FIVector4 activeObjPos;
  FIVector4 gwSize;
  FIVector4 cameraPos;
  FIVector4 lightPos;
  uint * lookup2to3;
  GLuint lookup2to3ID;
  int shadersAreLoaded;
  int readyToRecompile;
  FIVector4 bufferDim;
  bool lbDown;
  bool rbDown;
  bool mbDown;
  float seedX;
  float seedY;
  float seedZ;
  Timer myTimer;
  float curTime;
  float lastTime;
  E_GRASS_STATE grassState;
  float myDelta;
  int frameCount;
  bool changesMade;
  bool bufferInvalid;
  int maxH;
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
// f00350_gamepage.e
//

#ifndef LZZ_f00350_gamepage_e
#define LZZ_f00350_gamepage_e
#define LZZ_INLINE inline
class GamePage : public Poco::Runnable
{
public:
  int iDim;
  FIVector4 iOff;
  int iVolumeSize;
  int iRSize2;
  uint * volData;
  Singleton * singleton;
  FBOSet * fboSet;
  uint volID;
  uint volIDLinear;
  bool isDirty;
  float unitSize;
  E_STATES curState;
  E_STATES nextState;
  FIVector4 worldMin;
  FIVector4 worldMax;
  FIVector4 worldUnitMin;
  FIVector4 worldUnitMax;
  E_FILL_STATE fillState;
  GamePage ();
  void init (Singleton * _singleton, int _iDim, FIVector4 _iOff, int _iRSize2);
  uint NumberOfSetBits (uint i);
  uint clamp (uint val);
  float sqrtFast (float x);
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
  int iPageSize;
  int iVolumeSize;
  int renderRad;
  int loadRad;
  int ((diagrams) [E_RENDER_LENGTH]) [E_STATE_LENGTH];
  int * curDiagram;
  int renderMethod;
  bool doDrawFBO;
  FIVector4 screenCoords;
  FIVector4 gwSize;
  FIVector4 minWithinLR;
  FIVector4 maxWithinLR;
  FIVector4 curPos;
  FIVector4 camPagePos;
  FIVector4 iPixelWorldCoords;
  FIVector4 pagePos;
  FIVector4 unitPos;
  FIVector4 lastUnitPos;
  FIVector4 lastPagePos;
  Singleton * singleton;
  GamePage * * worldData;
  int iBufferSize;
  Poco::ThreadPool threadpool;
  int iRSize;
  GameWorld ();
  bool checkBounds (int i, int j, int k);
  void resetToState (E_STATES resState);
  void init (Singleton * _singleton);
  void update (bool changesMade, bool bufferInvalid, int maxH);
  bool processPages ();
  void renderPages (int maxH);
  void drawPage (GamePage * gp, int dx, int dy, int dz);
  void combineBuffers ();
  void renderGeom ();
  void modifyUnit (FIVector4 * fPixelWorldCoordsBase, bool doAdd);
  void renderWorldSpace ();
  void renderGrass ();
  void postProcess ();
  ~ GameWorld ();
};
#undef LZZ_INLINE
#endif
 

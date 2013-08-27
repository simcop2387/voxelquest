// f00300_singleton.e
//

#ifndef LZZ_f00300_singleton_e
#define LZZ_f00300_singleton_e
#define LZZ_INLINE inline
class Singleton
{
public:
  bool (keyDownArr) [MAX_KEYS];
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
  fVector3 cameraPos;
  float cameraZoom;
  uint * lookup2to3;
  GLuint lookup2to3ID;
  int shadersAreLoaded;
  int readyToRecompile;
  bool lbDown;
  bool rbDown;
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
  void setProgAction (eProgramState ps, unsigned char kc, eProgramAction pa, bool isDown);
  void setProgActionAll (unsigned char kc, eProgramAction pa, bool isDown);
  void keySetup ();
  void init (int _defaultWinW, int _defaultWinH);
  void doShaderRefresh ();
  void setMatrices (int w, int h);
  void setWH (int w, int h);
  void sampleFBODirect (FBOSet * fbos);
  void unsampleFBODirect (FBOSet * fbos);
  void bindFBODirect (FBOSet * fbos);
  void sampleFBO (string fboName);
  void unsampleFBO (string fboName);
  void bindFBO (string fboName);
  void unbindFBO ();
  void bindShader (string shaderName);
  void unbindShader ();
  void setShaderFloat (string paramName, float x);
  void setShaderVec2 (string paramName, float x, float y);
  void setShaderVec3 (string paramName, float x, float y, float z);
  void setShaderVec4 (string paramName, float x, float y, float z, float w);
  void setShaderTexture (uint texID, int multitexNumber);
  void setShaderTexture3D (uint texID, int multitexNumber);
  void drawFSQuad (float zoom);
  void drawFSQuadOffset (float xOff, float yOff, float zoom);
  void drawFBO (string fboName, int ind, float zoom);
  void drawFBOOffsetDirect (FBOSet * fbos, int ind, float xOff, float yOff, float zoom);
  void drawFBOOffset (string fboName, int ind, float xOff, float yOff, float zoom);
  void moveCamera (float dx, float dy, float zoom);
  void doAction (eProgramAction pa);
  void processSpecialKeys (int key, int x, int y);
  void processKey (unsigned char key, int x, int y, bool isPressed);
  void keyboardUp (unsigned char key, int x, int y);
  void keyboardDown (unsigned char key, int x, int y);
  void mouseMove (int x, int y);
  void mouseClick (int button, int state, int x, int y);
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
  int iVoroDim;
  iVector3 iOff;
  int iVolumeSize;
  int iVoroSize;
  int iRenderSize;
  float origHeight;
  uint * volData;
  uint * voroData;
  Singleton * singleton;
  FBOSet * fboSet;
  uint volID;
  uint voroID;
  E_STATES curState;
  E_STATES nextState;
  GamePage ();
  void init (Singleton * _singleton, int _iDim, iVector3 _iOff, int _iRenderSize);
  uint NumberOfSetBits (uint i);
  uint clamp (uint val);
  float sqrtFast (float x);
  void createSimplexNoise ();
  void renderVolume (float texMin, float texMax);
  void copyToTexture ();
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
  iVector3 iDim;
  int iPageSize;
  int iVolumeSize;
  int loadRad;
  int loadRadZ;
  int renderRad;
  float unitScale;
  int ((diagrams) [E_RENDER_LENGTH]) [E_STATE_LENGTH];
  int * curDiagram;
  int renderMethod;
  bool doDrawFBO;
  iVector3 minWithinLR;
  iVector3 maxWithinLR;
  Singleton * singleton;
  GamePage * * worldData;
  GamePage * * texData;
  int iBufferSize;
  iVector3 origin;
  iVector3 cameraPos;
  iVector3 playerPos;
  iVector2 iScreenDim;
  fVector2 fScreenDim;
  Poco::ThreadPool threadpool;
  GameWorld ();
  int checkBounds (int i, int j, int k);
  void resetToState (E_STATES resState);
  void init (iVector3 _iDim, Singleton * _singleton, int _renderMethod);
  void update (bool changesMade, bool bufferInvalid, int maxH);
  bool processPages ();
  void renderPages (float zoom, int maxH);
  void drawPage (GamePage * gp, int dx, int dy, int dz);
  void postProcess (float zoom);
  void setWH (int width, int height);
  ~ GameWorld ();
};
#undef LZZ_INLINE
#endif
 

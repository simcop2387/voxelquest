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
// f00295_gamegeom.e
//

#ifndef LZZ_f00295_gamegeom_e
#define LZZ_f00295_gamegeom_e
#define LZZ_INLINE inline
class GameGeom
{
private:
  FIVector4 boundsMinInPixels;
  FIVector4 boundsMaxInPixels;
  FIVector4 visMinInPixels;
  FIVector4 visMaxInPixels;
public:
  FIVector4 (geomParams) [E_GP_LENGTH];
  FIVector4 anchorPointInPixels;
  FIVector4 moveMinInPixels;
  FIVector4 moveMaxInPixels;
  GameLight * light;
  bool visible;
  bool hasAnchor;
  int rotDir;
  int minRot;
  int maxRot;
  int curRot;
  int buildingType;
  int id;
  int globalId;
  GameGeom ();
  void init (int _id);
  float getRand ();
  FIVector4 tempVec1;
  FIVector4 tempVec2;
  FIVector4 * getBoundsMinInPixels ();
  FIVector4 * getBoundsMaxInPixels ();
  FIVector4 * getVisMinInPixels ();
  FIVector4 * getVisMaxInPixels ();
  FIVector4 * getBoundsMinInPixelsT ();
  FIVector4 * getBoundsMaxInPixelsT ();
  FIVector4 * getVisMinInPixelsT ();
  FIVector4 * getVisMaxInPixelsT ();
  int getClampedRot ();
  void rotate (int mod, bool ignoreConstraints);
  void initBounds (int _buildingType, int _id, int _globalId, int alignBottomMiddleTop, float _zOffset, FIVector4 * p1, FIVector4 * p2, FIVector4 * rad, FIVector4 * _cornerDisInPixels, FIVector4 * _visInsetFromMin, FIVector4 * _visInsetFromMax, FIVector4 * _powerVals, FIVector4 * _powerVals2, FIVector4 * _thickVals, FIVector4 * _matParams, FIVector4 * _centerPoint, FIVector4 * _anchorPoint, int _minRot, int _maxRot);
  void initTree (int _buildingType, int _id, int _globalId, int alignBottomMiddleTop, float _zOffset, FIVector4 * p0, FIVector4 * p1, FIVector4 * p2, float radP0, float radP1, float sphereRad, FIVector4 * _matParams);
  void applyTransform (int rotMod, bool ignoreConstraints);
  void initAnchorPoint (FIVector4 * _anchorPointInPixels, int _minRot, int _maxRot);
};
#undef LZZ_INLINE
#endif
// f00297_gameent.e
//

#ifndef LZZ_f00297_gameent_e
#define LZZ_f00297_gameent_e
#define LZZ_INLINE inline
class GameEnt
{
private:
public:
  FIVector4 (entParams) [vLength];
  GameEnt ();
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
  eProgramState programState;
  eProgramAction (progActionsDown) [E_PS_SIZE * 256];
  eProgramAction (progActionsUp) [E_PS_SIZE * 256];
  bool bShift;
  bool tiltChanged;
  bool testOn;
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
  bool isBare;
  bool reportPagesDrawn;
  bool showMap;
  bool traceOn;
  bool waterOn;
  bool treesOn;
  bool firstRun;
  bool rotOn;
  int currentStep;
  int maxLayers;
  int maxChangesInHolders;
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
  int numDynLights;
  int iNumSteps;
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
  int bufferedPageSizeInUnits;
  int voroSize;
  int geomCounter;
  int lightCounter;
  int metersPerNodeXY;
  int terDataVisPitchXY;
  int terDataBufPitchXY;
  int terDataBufPitchScaledXY;
  int metersPerNodeZ;
  int terDataVisPitchZ;
  int terDataBufPitchZ;
  int terDataBufPitchScaledZ;
  int terDataBufAmount;
  int terDataVisSize;
  int terDataBufSize;
  int terDataTexScale;
  int terDataBufSizeScaled;
  uint * terDataScaled;
  int iNodeDivsPerLot;
  int selGeomListInd;
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
  float timeOfDay;
  float targetTimeOfDay;
  float gridSizeInPixels;
  float origWinW;
  float origWinH;
  double curMS;
  float tiltAmount;
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
  float bufferMult;
  float holderSizeMB;
  float * paramArr;
  float * voroArr;
  float * matCountArr;
  int * geomIdArr;
  float * paramArrMap;
  FIVector4 worldSizeInPixels;
  FIVector4 mouseUpPD;
  FIVector4 mouseDownPD;
  FIVector4 mouseMovePD;
  FIVector4 tempVec1;
  FIVector4 tempVec2;
  FIVector4 worldSizeInPages;
  FIVector4 worldSizeInLots;
  FIVector4 worldSizeInHolders;
  FIVector4 worldSizeInBlocks;
  FIVector4 (moveNodes) [2];
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
  FIVector4 dMod;
  FIVector4 modXYZ;
  std::vector <DynObject *> dynObjects;
  std::vector <GameGeom *> selGeomList;
  PathHolder charPathHolder;
  PathHolder splitPathHolder;
  float floorHeightInMeters;
  float roofHeightInMeters;
  float wallRadInMeters;
  int blockSizeInLots;
  int blockSizeInMeters;
  int blockSizeInHolders;
  int blockSizeInPages;
  int blockSizeInPixels;
  Image * basicFont;
  Image * imageHM0;
  Image * imageHM1;
  Image * cloudImage;
  GameGeom * highlightedGeom;
  GameGeom * selectedGeom;
  Shader * curShaderPtr;
  string curShader;
  string allText;
  PoolManager * gpuPool;
  PoolManager * entityPool;
  vector <string> shaderStrings;
  vector <string> fboStrings;
  vector <string> shaderTextureIds;
  map <string, Shader *> shaderMap;
  map <string, FBOSet *> fboMap;
  GLuint volId;
  GLuint volIdLinear;
  GLuint volIdEmpty;
  GLuint volIdEmptyLinear;
  GLuint volGenId;
  GLuint volTris;
  GLuint sliceTris;
  uint * lookup2to3;
  unsigned char * resultImage;
  charArr nullBuffer;
  charArr lastImageBuffer;
  charArr lastJSONBuffer;
  JSONValue * rootObj;
  WebSocketServer * myWS;
  Timer myTimer;
  GameWorld * gw;
  float (lightArr) [MAX_LIGHTS * 16];
  int numLights;
  bool multiLights;
  int * rbHeightStack;
  TerTexture (terTextures) [MAX_TER_TEX];
  GameGUI * mainGUI;
  Singleton ();
  void init (int _defaultWinW, int _defaultWinH, int _scaleFactor, WebSocketServer * _myWS);
  int requestTerIndex (int requestingBlockId);
  static void qNormalizeAngle (int & angle);
  void perspectiveProjection ();
  void orthographicProjection ();
  ~ Singleton ();
  float genRand (float LO, float HI);
  void setProgAction (eProgramState ps, unsigned char kc, eProgramAction pa, bool isDown);
  void setProgActionAll (unsigned char kc, eProgramAction pa, bool isDown);
  void drawCrossHairs (FIVector4 originVec, float radius);
  void drawLine (FIVector4 * p0, FIVector4 * p1);
  void drawCubeCentered (FIVector4 * originVec, float radius);
  void drawBoxUp (FIVector4 originVec, float radiusX, float radiusY, float diamZ);
  void drawBox (FIVector4 * minVec, FIVector4 * maxVec, int faceFlag = 2);
  float glslMod (float x, float y);
  void createVTListTilt ();
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
  void drawFSQuad (float zoom);
  void drawFSQuadOffset (float xOff, float yOff, float zoom);
  void drawFBO (string fboName, int ind, float zoom, int swapFlag = -1);
  void drawFBOOffsetDirect (FBOSet * fbos, int ind, float xOff, float yOff, float zoom);
  void drawFBOOffset (string fboName, int ind, float xOff, float yOff, float zoom);
  float getTerHeightScaled (float val);
  float getSLNormalized ();
  float getSLInPixels ();
  float getHeightAtPixelPos (float x, float y, bool dd = false);
  void moveCamera (FIVector4 * pModXYZ);
  void moveObject (float dx, float dy, float zoom);
  void updateMultiLights ();
  void toggleFullScreen ();
  void setCameraToElevationBase ();
  void setCameraToElevation ();
  void processSpecialKeys (int key, int _x, int _y);
  void processKey (unsigned char key, int _x, int _y, bool isPressed);
  void keyboardUp (unsigned char key, int _x, int _y);
  void keyboardDown (unsigned char key, int _x, int _y);
  void getPixData (FIVector4 * toVector, int xv, int yv);
  GameGeom * findNearestGeom (FIVector4 * testPoint, bool createList = false);
  void mouseMove (int _x, int _y);
  void screenToWorldBase (FIVector4 * tc, FIVector4 * wc);
  void worldToScreenBase (FIVector4 * sc, FIVector4 * wc);
  void worldToScreen (FIVector4 * sc, FIVector4 * wc, bool centerIsOrigin = false);
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
		return x - y * floor(x / y);
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
  uint * (cpuSet) [MAX_LAYERS];
public:
  intPair usedByHolderId;
  bool isCPU;
  Singleton * singleton;
  PooledResource ();
  FBOSet * getFBOS (int fboNum);
  void init (Singleton * _singleton, bool _isCPU);
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
  PoolManager ();
  void init (Singleton * _singleton, bool _isCPU);
  float getMaxMem ();
  float getTotMemUsed ();
  void reorderIds ();
  int findFurthestHolderId ();
  int requestPoolId (int blockId, int holderId);
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
public:
  Singleton * singleton;
  int uid;
  bool mouseOver;
  bool mouseDown;
  bool wasHit;
  float value;
  float divisions;
  iVector2 align;
  int fillDir;
  iVector2 originPos;
  iVector2 resultDim;
  string text;
  int charScale;
  int charsPerLine;
  int maxLines;
  iVector2 charDim;
  iVector2 spacing;
  std::vector <string> lineVec;
  std::vector < std::vector<string> > wordVec;
  iBoundingBox hitBounds;
  StyleSheet * mainSS;
  StyleSheetResult resSS;
  enum eAlignH
  {
    E_ALIGN_LEFT,
    E_ALIGN_RIGHT,
    E_ALIGN_CENTER
  };
  enum eAlignV
  {
    E_ALIGN_TOP,
    E_ALIGN_MIDDLE,
    E_ALIGN_BOTTOM
  };
  enum eFillDir
  {
    E_FILL_HORIZONTAL,
    E_FILL_VERTICAL
  };
  UIComponent ();
  void init (Singleton * _singleton, StyleSheet * _mainSS, string _text, int _originPosX, int _originPosY, int _charsPerLine = 40, int _maxLines = 5, int _alignX = E_ALIGN_LEFT, int _alignY = E_ALIGN_TOP, int _fillDir = E_FILL_HORIZONTAL, int _charScale = 1, int _spacingX = 0, int _spacingY = 0);
  void updateValue (int x, int y);
  void testOver (int x, int y);
  bool testHit (int button, int state, int x, int y);
  void setText (string _text);
  void refresh ();
  void renderCharAt (int c, int px, int py);
  void renderQuad (int ix0, int iy0, int ix1, int iy1);
  void render (bool visible);
};
LZZ_INLINE void UIComponent::renderCharAt (int c, int px, int py)
                                                        {
		
		iBoundingBox ibbPos;
		fBoundingBox fbbPos;

		fBoundingBox fbbSamplePos;

		ibbPos.xMin = px;
		ibbPos.yMin = py;
		ibbPos.xMax = px+charDim.x*charScale;
		ibbPos.yMax = py+charDim.y*charScale;

		if (c > 32 && c < 126) {
			fbbSamplePos.xMin = ((float)((c-32)*charDim.x - 1))/656.0;
		}
		else {
			fbbSamplePos.xMin = 0.0f;
		}

		fbbSamplePos.xMax = ((float)((c+1-32)*charDim.x - 1))/656.0;
		fbbSamplePos.yMin = 0.0f;
		fbbSamplePos.yMax = 1.0f;

		float x0 = ((float)ibbPos.xMin)/singleton->origWinW;
		float x1 = ((float)ibbPos.xMax)/singleton->origWinW;
		float y0 = ((float)ibbPos.yMin)/singleton->origWinH;
		float y1 = ((float)ibbPos.yMax)/singleton->origWinH;

		x0 = (x0-0.5f)*2.0f;
		x1 = (x1-0.5f)*2.0f;
		y0 = ((1.0f-y0) - 0.5f)*2.0f;
		y1 = ((1.0f-y1) - 0.5f)*2.0f;

		
		//dimensions
		glMultiTexCoord4f(1, charDim.x*charScale, charDim.y*charScale, 0.0f, 0.0f);

		glMultiTexCoord4f(4, 1.0f, 1.0f, 1.0f, 1.0f);
		glMultiTexCoord4f(5, 1.0f, 1.0f, 1.0f, 1.0f);
		//border color
		glMultiTexCoord4f(6, 1.0f, 1.0f, 1.0f, 1.0f);
		//misc
		glMultiTexCoord4f(7, 0.0f, 0.0f, 0.0f, 0.0f);
		

		glMultiTexCoord4f(2, resSS.props[E_SS_BGCOLTEXT1_R], resSS.props[E_SS_BGCOLTEXT1_G], resSS.props[E_SS_BGCOLTEXT1_B], resSS.props[E_SS_BGCOLTEXT1_A]);
		glMultiTexCoord4f(3, resSS.props[E_SS_FGCOLTEXT1_R], resSS.props[E_SS_FGCOLTEXT1_G], resSS.props[E_SS_FGCOLTEXT1_B], resSS.props[E_SS_FGCOLTEXT1_A]);
		
		
		glMultiTexCoord4f(0, fbbSamplePos.xMin, fbbSamplePos.yMin, 0.0f, 1.0f);
		glVertex3f (  x0, y1, -1.0f );
		glMultiTexCoord4f(0, fbbSamplePos.xMax, fbbSamplePos.yMin, 1.0f, 1.0f);
		glVertex3f (  x1, y1, -1.0f );

		glMultiTexCoord4f(2, resSS.props[E_SS_BGCOLTEXT0_R], resSS.props[E_SS_BGCOLTEXT0_G], resSS.props[E_SS_BGCOLTEXT0_B], resSS.props[E_SS_BGCOLTEXT0_A]);
		glMultiTexCoord4f(3, resSS.props[E_SS_FGCOLTEXT0_R], resSS.props[E_SS_FGCOLTEXT0_G], resSS.props[E_SS_FGCOLTEXT0_B], resSS.props[E_SS_FGCOLTEXT0_A]);		

		glMultiTexCoord4f(0, fbbSamplePos.xMax, fbbSamplePos.yMax, 1.0f, 0.0f);
		glVertex3f (  x1, y0, -1.0f );
		glMultiTexCoord4f(0, fbbSamplePos.xMin, fbbSamplePos.yMax, 0.0f, 0.0f);
		glVertex3f (  x0, y0, -1.0f );

	}
LZZ_INLINE void UIComponent::renderQuad (int ix0, int iy0, int ix1, int iy1)
                                                                   {


		


		float x0 = ((float)ix0)/singleton->origWinW;
		float x1 = ((float)ix1)/singleton->origWinW;
		float y0 = ((float)iy0)/singleton->origWinH;
		float y1 = ((float)iy1)/singleton->origWinH;

		x0 = (x0-0.5f)*2.0f;
		x1 = (x1-0.5f)*2.0f;
		y0 = ((1.0f-y0) - 0.5f)*2.0f;
		y1 = ((1.0f-y1) - 0.5f)*2.0f;

		
		//dimensions
		glMultiTexCoord4f(1, ix1-ix0, iy1-iy0, resSS.props[E_SS_BORDER], resSS.props[E_SS_CORNERRAD]);

		//glMultiTexCoord4f(4, 1.0f, 1.0f, 1.0f, 1.0f);
		glMultiTexCoord4f(5, 1.0f, 1.0f, 1.0f, 1.0f);
		
		//border color
		glMultiTexCoord4f(6, resSS.props[E_SS_BDCOL_R], resSS.props[E_SS_BDCOL_G], resSS.props[E_SS_BDCOL_B], resSS.props[E_SS_BDCOL_A]);
		//misc
		glMultiTexCoord4f(7, resSS.props[E_SS_ROUNDNESS], value, 1.0f, 1.0f);


		//bg
		glMultiTexCoord4f(2, resSS.props[E_SS_BGCOL1_R], resSS.props[E_SS_BGCOL1_G], resSS.props[E_SS_BGCOL1_B], resSS.props[E_SS_BGCOL1_A]);
		//fg
		glMultiTexCoord4f(3, resSS.props[E_SS_FGCOL1_R], resSS.props[E_SS_FGCOL1_G], resSS.props[E_SS_FGCOL1_B], resSS.props[E_SS_FGCOL1_A]);
		//tg
		glMultiTexCoord4f(4, resSS.props[E_SS_TGCOL1_R], resSS.props[E_SS_TGCOL1_G], resSS.props[E_SS_TGCOL1_B], resSS.props[E_SS_TGCOL1_A]);
		
		
		glMultiTexCoord4f(0, 0.0f, 0.0f, 0.0f, 1.0f);
		glVertex3f (  x0, y1, -1.0f );
		glMultiTexCoord4f(0, 0.0f, 0.0f, 1.0f, 1.0f);
		glVertex3f (  x1, y1, -1.0f );

		//bg
		glMultiTexCoord4f(2, resSS.props[E_SS_BGCOL0_R], resSS.props[E_SS_BGCOL0_G], resSS.props[E_SS_BGCOL0_B], resSS.props[E_SS_BGCOL0_A]);
		//fg
		glMultiTexCoord4f(3, resSS.props[E_SS_FGCOL0_R], resSS.props[E_SS_FGCOL0_G], resSS.props[E_SS_FGCOL0_B], resSS.props[E_SS_FGCOL0_A]);
		//tg
		glMultiTexCoord4f(4, resSS.props[E_SS_TGCOL0_R], resSS.props[E_SS_TGCOL0_G], resSS.props[E_SS_TGCOL0_B], resSS.props[E_SS_TGCOL0_A]);

		glMultiTexCoord4f(0, 0.0f, 0.0f, 1.0f, 0.0f);
		glVertex3f (  x1, y0, -1.0f );
		glMultiTexCoord4f(0, 0.0f, 0.0f, 0.0f, 0.0f);
		glVertex3f (  x0, y0, -1.0f );


	}
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
  UIComponent frameRateTB;
  UIComponent testTB;
  StyleSheet mainSS;
  GameGUI ();
  void init (Singleton * _singleton);
  void testOver (int x, int y);
  bool testHit (int button, int state, int x, int y);
  void renderGUI (bool showGUI);
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
  int bufferedPageSizeInUnits;
  FIVector4 offsetInUnits;
  FIVector4 offsetInPages;
  FIVector4 offsetInPagesLocal;
  float pageDepth;
  int iVolumeSize;
  bool hasGeom;
  bool hasTerrain;
  bool hasWater;
  bool hasWindow;
  bool hasTree;
  bool nearTerrain;
  bool nearAir;
  int voroCount;
  bool volDataModified;
  bool isRendering;
  bool isEntity;
  int paramsPerEntry;
  int numEntries;
  int totParams;
  int maxEntries;
  int maxHeightInUnits;
  FIVector4 worldSeed;
  bool threadRunning;
  float unitSizeInPixels;
  GameWorld * gw;
  FIVector4 worldMinVisInPixels;
  FIVector4 worldMaxVisInPixels;
  FIVector4 worldMinBufInPixels;
  FIVector4 worldMaxBufInPixels;
  FIVector4 centerPosition;
  FIVector4 scaleAndOffset;
  FIVector4 worldUnitMin;
  FIVector4 worldUnitMax;
  FIVector4 voroSize;
  FIVector4 curPos;
  FIVector4 posFloored;
  FIVector4 randNum;
  FIVector4 testNum;
  FIVector4 newPos;
  FIVector4 tempVec1;
  FIVector4 tempVec2;
  GamePageHolder * parentGPH;
  GameBlock * parentBlock;
  uint * getVolData ();
  uint * getVolDataLinear ();
  GamePage ();
  void init (Singleton * _singleton, GamePageHolder * _parentGPH, int _thisPageId, int offsetX, int offsetY, int offsetZ, int oxLoc, int oyLoc, int ozLoc, bool _isEntity = false);
  void copyToTexture (bool isForEmptyVD);
  void addEntityGeom (bool justTesting);
  void addGeom (bool justTesting);
  void getVoroPoints ();
  void generateVolume ();
  void getCoords ();
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
  bool isDirty;
  bool hasTrans;
  bool hasSolids;
  bool underground;
  std::vector <GameGeom *> entityGeom;
  int entityGeomCounter;
  int entType;
  FIVector4 offsetInHolders;
  FIVector4 gphMinInPixels;
  FIVector4 gphMaxInPixels;
  FIVector4 offsetInBlocks;
  PooledResource * gpuRes;
  Singleton * singleton;
  std::vector <intPair> containsGeomIds;
  GamePage * * pageData;
  bool isEntity;
  GamePageHolder ();
  void init (Singleton * _singleton, int _blockId, int _holderId, int trueX, int trueY, int trueZ, bool _isEntity = false, bool _entType = E_ET_TEST);
  GamePage * getPageAtCoordsLocal (int x, int y, int z, bool createOnNull = false);
  void clearSet (bool forceClear);
  void refreshChildren (bool refreshImmediate);
  void addNewGeom (int _curBT, int _curAlign, float _baseOffset, FIVector4 * _p1, FIVector4 * _p2, FIVector4 * _rad, FIVector4 * _cornerRad, FIVector4 * _visInsetFromMin, FIVector4 * _visInsetFromMax, FIVector4 * _powerVals, FIVector4 * _powerVals2, FIVector4 * _thickVals, FIVector4 * _matParams, FIVector4 * _centerPoint, FIVector4 * _anchorPoint, int _minRot, int _maxRot);
  void fetchEntityGeom ();
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
  int localGeomCounter;
  int lightCounter;
  int terDataBufAmount;
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
  float pixelsPerMeter;
  float floorHeightInMeters;
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
  FIVector4 moveMinInPixels;
  FIVector4 moveMaxInPixels;
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
  std::vector <GameGeom *> gameGeom;
  std::vector <GameLight *> gameLights;
  std::vector <GameEnt *> gameEnts;
  GamePlant oakTree;
  GamePlant bareTree;
  GameWorld * gw;
  GamePageHolder * * holderData;
  BuildingNode * buildingData;
  MapNode * mapData;
  uint * terData;
  BuildingCon * curCon;
  GameBlock ();
  void init (Singleton * _singleton, int _blockId, int _x, int _y, int _xw, int _yw);
  int getNodeIndex (int x, int y, int z, int bufAmount);
  int touchesPlanarBN (int x, int y, int z, int buildingType, int bufAmount);
  int touchesPlanarClass (int x, int y, int z, int classType, int bufAmount);
  bool isLCorner (int x, int y, int z, int classType, bool includeSingle);
  void applyWingValues (int _x1, int _y1, int _z1, int _x2, int _y2, int _z2, int cnum, bool isWingBeg, bool isWingEnd, float multiplier);
  void addNewGeom (int _curBT, int _curAlign, float _baseOffset, FIVector4 * _p1, FIVector4 * _p2, FIVector4 * _rad, FIVector4 * _cornerRad, FIVector4 * _visInsetFromMin, FIVector4 * _visInsetFromMax, FIVector4 * _powerVals, FIVector4 * _powerVals2, FIVector4 * _thickVals, FIVector4 * _matParams, FIVector4 * _centerPoint, FIVector4 * _anchorPoint, int _minRot, int _maxRot);
  void connectNodes (int _x1, int _y1, int _z1, int _x2, int _y2, int _z2, int ct, int id = -1, int _heightDelta = 0, int _direction = 0, float _wallRadInMeters = -1.0f);
  int getMapNodeIndex (int x, int y, int bufAmount);
  int touchesMap (int x, int y, int buildingType, int bufAmount);
  int touches2Map (int x, int y, int buildingType, int bufAmount);
  void connectMapNodes (int _x1, int _y1, int _x2, int _y2, int buildingType, int id, int bufAmount);
  void isNearTerrain (FIVector4 * worldPositionInPixels, bool & nearT, bool & nearA);
  int findNearestNode (FIVector4 * worldPositionInPixelsIn, FIVector4 * posInNodesOut, FIVector4 * posInPixelsOut);
  void nodeIndexToWorldSpaceInPixels (int ind, FIVector4 * posInPixelsOut);
  bool hasTerrainBelow (int i, int j, int k, int conDir, bool bothEnds);
  bool hasRoomBelow (int i, int j, int k, int conDir);
  bool isSupportedBelow (int i, int j, int k, int conDir, bool bothEnds);
  bool touchesRoomOnLevel (int i, int j, int k);
  bool touchesBaseOnLevel (int i, int j, int k, int layer);
  bool buildingAbove (int x, int y, int z);
  int copyTerToTexture ();
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
  int * curDiagram;
  int * provinceGrid;
  int * provinceX;
  int * provinceY;
  bool doDrawFBO;
  bool lastProcResult;
  bool mapLockOn;
  bool foundPath;
  float mapStep;
  float mapTrans;
  float newZoom;
  int (dirFlags) [4];
  int (dirFlagsOp) [4];
  int (dirFlagsO) [4];
  int (dirFlagsOpO) [4];
  int (dirModX) [6];
  int (dirModY) [6];
  int (dirModZ) [6];
  int (opDir) [4];
  GameLight * (activeLights) [MAX_EVAL_LIGHTS];
  int dirFlagClear;
  int visFlag;
  int visFlagO;
  int activeFBO;
  bool noiseGenerated;
  bool wavesGenerated;
  std::vector <coordAndIndex> roadCoords;
  std::vector <GamePageHolder *> holdersToRefresh;
  GamePageHolder (entHolders) [E_ET_LENGTH];
  std::vector <GameEnt> entList;
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
  FIVector4 minLRInPixels;
  FIVector4 maxLRInPixels;
  FIVector4 minLRInHolders;
  FIVector4 maxLRInHolders;
  FIVector4 minv;
  FIVector4 maxv;
  FIVector4 tempVec;
  FIVector4 unitPosMin;
  FIVector4 unitPosMax;
  FIVector4 startBounds;
  FIVector4 endBounds;
  FIVector4 * fogPos;
  FIVector4 * cutPos;
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
  GamePageHolder * getHolderAtId (intPair id);
  GameBlock * getBlockAtId (int id);
  GamePage * getPageAtIndex (int ind);
  GamePage * getPageAtCoords (int x, int y, int z, bool createOnNull = false);
  bool checkBounds (int k);
  void update ();
  bool processPages ();
  bool addHolderToRefresh (GamePageHolder * toAdd);
  int getHoldersInGeom (GameGeom * gg);
  void refreshHoldersInList (bool doImmediate);
  void actionOnHolders (int action);
  void drawHolder (GamePageHolder * gp, int curLayer, float xoff, float yoff, float zoff);
  void combineBuffers ();
  void clearVisitedPaths (PathHolder * pathHolder);
  void clearPathList (PathHolder * pathHolder);
  float getIdealPathLength (PathNode * blockAndIndex);
  int findAIPathRBT (PathHolder * pathHolder, PathNode * blockAndIndex, float _pathSlack);
  void drawPathLine (PathHolder * curPath, int r, int g, int b, float lw, float zoff);
  void drawAIPath (PathHolder * pathHolder, PathHolder * splitPathHolder);
  void renderGeom ();
  void modifyUnit (FIVector4 * fPixelWorldCoordsBase, E_BRUSH brushAction);
  void renderWorldSpaceGPU (float processPagesFBO, float processGeomFBO, float processWaterFBO);
  void getWorldSpaceBuffer ();
  float weighPath (float x1, float y1, float x2, float y2, float rad, bool doSet, bool isOcean);
  float findBestPath (float x1, float y1, float x2, float y2, int generation, int roadIndex, bool doSet, bool isOcean);
  void initMap ();
  void drawMap ();
  void doBlur (string fboName, float blurAmount);
  void updateLights ();
  void postProcess ();
  ~ GameWorld ();
};
#undef LZZ_INLINE
#endif
 

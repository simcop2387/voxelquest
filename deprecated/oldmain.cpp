

class GameCamera {
private:
	
public:
	GLfloat unitPos[3];
	
	
	float accel[3];
	float accelA;
	float accelB;
	float lastAccelA;
	float lastAccelB;
	
	GLfloat rotation[2];
	GLfloat clipDist[2];
	
	GameCamera() {
		
	}
	
	
	
	void focusOn(int entID) {
		
	}
	
	void setClipDist(float n, float f) {
		clipDist[0] = n;
		clipDist[1] = f;
	}
	
	void testCollision(float oldX, float oldY, float oldZ, float newX, float newY, float newZ, bool skipTest) {
		float oldPos[3] = {oldX,oldY,oldZ};
		float testPos[3] = {newX,newY,newZ};
		int i;
		
		
		if (skipTest) {
			for (i = 0; i < 3; i++) {
				
				accel[i] += (testPos[i]-oldPos[i])/4.0;
				
				accel[i] = accel[i]/1.1;
				
				unitPos[i] += accel[i];
			}
		}
		
		
	}
	
	void setUnitPosition(float x, float y, float z) {
		
		unitPos[0] = x;
		unitPos[1] = y;
		unitPos[2] = z;
	}
	
	void addUnitPosition(float x, float y, float z) {
		
		float oldX = unitPos[0];
		float oldY = unitPos[1];
		float oldZ = unitPos[2];
		
		float newX = unitPos[0]+x*timeValSec;
		float newY = unitPos[1]+y*timeValSec;
		float newZ = unitPos[2]+z*timeValSec;
		
		testCollision(oldX, oldY, oldZ, newX, newY, newZ, true);
		
	}
	
	void setRotation(float a, float b) {
		rotation[0] = a;
		rotation[1] = b;
		
		accelA = 0.0f;
		accelB = 0.0f;
		lastAccelA = 0.0f;
		lastAccelB = 0.0f;
	}
	
	void addRotation(float a, float b) {
		
		
		accelA += a;//*min(timeValSec,1.0f);
		accelB += b;//*min(timeValSec,1.0f);
		
		accelA = accelA / 1.5f;
		accelB = accelB / 1.5f;
		
		rotation[0] += accelA;
		rotation[1] += accelB;
	}
	
	void init() {
		
		accel[0] = 0.0f;
		accel[1] = 0.0f;
		accel[2] = 0.0f;
		
		setRotation(0.0f, 180.0f);
		setClipDist(0.1f, gfCLIP_DIST);
		setUnitPosition(200.0f,200.0f,200.0f);
		
	}
	
};

class GameManager {
private:

public:
	
	GameManager* thisGM;
	GameCamera* mainCamera;
	PageManager* mainPM;
	GameGUI* mainGUI;

	Shader shader;
	Shader shaderWater;
	int mouseState;
	
	float fogRed;
	float fogGreen;
	float fogBlue;
	GLfloat lastx;
	GLfloat lasty;

	bool loadingEnabled;
	bool guiEnabled;
	bool isMoving;
	bool isFullScreen;
	bool shiftPressed;
	bool ctrlPressed;
	bool altPressed;
	
	bool keysPressed[256];
	unsigned char keyMap[256];

	enum keyMappings {
		KEYMAP_UP,
		KEYMAP_DOWN,
		KEYMAP_FORWARD,
		KEYMAP_BACKWARD,
		KEYMAP_LEFT,
		KEYMAP_RIGHT,
		KEYMAP_FULLSCREEN,
		KEYMAP_TOGGLE_LOAD,
		KEYMAP_MIP,
		KEYMAP_RENDER_GUI
	};

	int selMipLevel;





	void preInit() {
		
		globalTimer.start();
		
		int i;
		int j;
		//int k;

		giGUI_IDS = 0;
		
		mouseState = E_MOUSE_UP;

		timeValSec = 0.0f;
		curFPS = 0.0f;
		timeVal = 0.0f;

		selMipLevel = 0;
		loadingEnabled = true;
		guiEnabled = true;
		
		thisGM = this;
		
		
		
		float nLerpVal;
		float iLerpVal;


		int numMats = 5;		
		float texVals[] = {
			0.0f/255.0f,	 0.0f/255.0f, 		0.0f/255.0f,
			0.0f/255.0f,	 0.0f/255.0f, 		0.0f/255.0f,

			0.0f/255.0f,	 32.0f/255.0f, 		64.0f/255.0f,
			0.0f/255.0f,	 128.0f/255.0f, 	255.0f/255.0f,
			
			12.0f/255.0f,	 39.0f/255.0f, 		0.0f/255.0f,
			113.0f/255.0f,	 166.0f/255.0f, 	53.0f/255.0f,
			
			0.0f/255.0f,	 0.0f/255.0f, 		0.0f/255.0f,
			255.0f/255.0f,	 255.0f/255.0f, 	255.0f/255.0f,
			
			21.0f/255.0f,	 21.0f/255.0f, 		9.0f/255.0f,
			111.0f/255.0f,	 98.0f/255.0f, 		79.0f/255.0f
			
		};

		
		
		
		for (i = 0; i < giNUM_THREADS; i++) {
			ptBusy[i] = false;

			for (j = 0; j < giNUM_THREAD_PARAMS; j++) {
				ptParams[i].fParams[j] = 0.0f;
			}
			
		}
		
		
		
		
		for (i = 0; i < giNUM_MATERIALS; i++) {
			for (j = 0; j < giMATERIAL_SLOTS; j++) {
				mainMaterials[i].r[j] = 0.0f;
				mainMaterials[i].g[j] = 0.0f;
				mainMaterials[i].b[j] = 0.0f;
			}
		}
		
		for (i = 0; i < numMats; i++) {
			for (j = 0; j < giMATERIAL_SLOTS; j++) {
				nLerpVal = ((float)j)/((float)(giMATERIAL_SLOTS-1));
				iLerpVal = 1.0-nLerpVal;
				
				mainMaterials[i].r[j] = texVals[i*6]*iLerpVal + texVals[i*6+3]*nLerpVal;
				mainMaterials[i].g[j] = texVals[i*6+1]*iLerpVal + texVals[i*6+3+1]*nLerpVal;
				mainMaterials[i].b[j] = texVals[i*6+2]*iLerpVal + texVals[i*6+3+2]*nLerpVal;
			}
		}
		
		
		mainGUI = NULL;
		
		

		gvLightDir.setXYZ(0.7,-0.5,1.0);
		gvLightDir.makeUnitVector();

		fogRed = 135.0f/255.0f;
		fogGreen = 160.0f/255.0f;
		fogBlue = 1.0f;
		
		lastx = 0;
		lasty = 0;

		isMoving = false;
		isFullScreen = false;

		shiftPressed = false;
		ctrlPressed = false;
		altPressed = false;
		
		mainPM = new PageManager();
		mainPM->init();
		
		mainCamera = new GameCamera();
		mainCamera->init();
		
	}
	
	void postInit(void) {
	    glEnable(GL_DEPTH_TEST);
	    glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	    shader.init("shaders/shader.c");
	    shaderWater.init("shaders/shaderWater.c");

		int i;

		for (i = 0; i < 256; i++) {
			keysPressed[i] = false;
		}

		keyMap[KEYMAP_UP] = 'a';
		keyMap[KEYMAP_DOWN] = 'z';
		keyMap[KEYMAP_FORWARD] = 'e';
		keyMap[KEYMAP_BACKWARD] = 'd';
		keyMap[KEYMAP_LEFT] = 's';
		keyMap[KEYMAP_RIGHT] = 'f';
		keyMap[KEYMAP_FULLSCREEN] = 'p';
		keyMap[KEYMAP_TOGGLE_LOAD] = 'l';
		
		keyMap[KEYMAP_MIP] = 'm';
		keyMap[KEYMAP_RENDER_GUI] = '\t';

	}
	
	GameManager() {
		
	}


	void processSpecialKeys(int key, int x, int y) {

	/*
	GLUT_KEY_F1		F1 function key
	GLUT_KEY_F2		F2 function key
	GLUT_KEY_F3		F3 function key
	GLUT_KEY_F4		F4 function key
	GLUT_KEY_F5		F5 function key
	GLUT_KEY_F6		F6 function key
	GLUT_KEY_F7		F7 function key
	GLUT_KEY_F8		F8 function key
	GLUT_KEY_F9		F9 function key
	GLUT_KEY_F10		F10 function key
	GLUT_KEY_F11		F11 function key
	GLUT_KEY_F12		F12 function key
	GLUT_KEY_LEFT		Left function key
	GLUT_KEY_RIGHT		Right function key
	GLUT_KEY_UP		Up function key
	GLUT_KEY_DOWN		Down function key
	GLUT_KEY_PAGE_UP	Page Up function key
	GLUT_KEY_PAGE_DOWN	Page Down function key
	GLUT_KEY_HOME		Home function key
	GLUT_KEY_END		End function key
	GLUT_KEY_INSERT		Insert function key
	*/

		switch(key) {
			case GLUT_KEY_F1 :

				break;
			case GLUT_KEY_F2 :

				break;
			case GLUT_KEY_F3 :

				break;
		}
	}

	void processKey(unsigned char key, int x, int y, bool isPressed) {
		keysPressed[key] = isPressed;
	}

	void keyboardUp(unsigned char key, int x, int y) {
		processKey(key,x,y,false);

		int keymod = glutGetModifiers();

		if (keymod&GLUT_ACTIVE_SHIFT) {
			shiftPressed = false;
		}
		if (keymod&GLUT_ACTIVE_CTRL) {
			ctrlPressed = false;
		}
		if (keymod&GLUT_ACTIVE_ALT) {
			altPressed = false;
		}
	}


	void doTerminate() {
		if (isFullScreen) {
			//glutLeaveGameMode();
		}

		exit(0);
	}

	void keyboardDown(unsigned char key, int x, int y) {

		if (key==27) {
			doTerminate();
		}
		
		if (key==keyMap[KEYMAP_TOGGLE_LOAD]) {
			loadingEnabled = !loadingEnabled;
			cout << "LOADING ENABLED: " << loadingEnabled << "\n";
		}
		if (key==keyMap[KEYMAP_RENDER_GUI]) {
			guiEnabled = !guiEnabled;
		}
		
		if (key == keyMap[KEYMAP_MIP]) {
			selMipLevel++;
			
			if (selMipLevel >= giMIP_LEVELS) {
				selMipLevel = 0;
			}
		}

		if (key==keyMap[KEYMAP_FULLSCREEN]) {
			isFullScreen = !isFullScreen;
			if (isFullScreen) {
				//glutGameModeString( "1280x800:32@75" ); //the settings for fullscreen mode
				//glutEnterGameMode();
				glutFullScreen();
			}
			else {
				glutReshapeWindow((int)windowSize[0], (int)windowSize[1]);
				//glutLeaveGameMode();
			}
		}

		processKey(key,x,y,true);

		int keymod = glutGetModifiers();

		if (keymod&GLUT_ACTIVE_SHIFT) {
			shiftPressed = true;
		}
		if (keymod&GLUT_ACTIVE_CTRL) {
			ctrlPressed = true;
		}
		if (keymod&GLUT_ACTIVE_ALT) {
			altPressed = true;
		}

	}


	void mouseMovementWithButton(int x, int y) {
		mainGUI->testOver(x,y);

		if (isMoving) {
			int diffx=x-lastx;
			int diffy=y-lasty;
			lastx=x;
			lasty=y;
			
			mainCamera->addRotation((float) diffy, (float) diffx);
		}
	}

	void mouseMovementWithoutButton(int x, int y) {
		mainGUI->testOver(x,y);
	}

	void mouseClick(int button, int state, int x, int y) {

		bool hitGUI = mainGUI->testHit(button, state, x, y);

		if (hitGUI) {
			
		}
		else {
			if (state == GLUT_DOWN) {
				if (button == GLUT_LEFT_BUTTON) {

				}

				lastx=x;
				lasty=y;

				isMoving = true;
			}
			if (state == GLUT_UP) {
				isMoving = false;
			}
		}

		

	}

	void handleMovement(void) {

		
		
		float xrotrad = (mainCamera->rotation[0] / 180 * M_PI);
		float yrotrad = (mainCamera->rotation[1] / 180 * M_PI);
		
		

		float xmod = 0.0f;
		float ymod = 0.0f;
		float zmod = 0.0f;
		float moveSpeed;

		if (keysPressed[keyMap[KEYMAP_UP]]) {
			ymod += 1.0f;
		}

		if (keysPressed[keyMap[KEYMAP_DOWN]]) {
			ymod -= 1.0f;
		}

		if (keysPressed[keyMap[KEYMAP_FORWARD]]) {

			
			xmod += float(sin(yrotrad));
			zmod -= float(cos(yrotrad));
			ymod -= float(sin(xrotrad));
		}

		if (keysPressed[keyMap[KEYMAP_BACKWARD]]) {
			
			xmod -= float(sin(yrotrad));
			zmod += float(cos(yrotrad));
			ymod += float(sin(xrotrad));
		}

		if (keysPressed[keyMap[KEYMAP_RIGHT]]) {
			
			xmod += float(cos(yrotrad));
			zmod += float(sin(yrotrad));
		}

		if (keysPressed[keyMap[KEYMAP_LEFT]]) {
			
			xmod -= float(cos(yrotrad));
			zmod -= float(sin(yrotrad));
		}

		moveSpeed = 1.0f;
		
		mainCamera->addUnitPosition(xmod*moveSpeed, ymod*moveSpeed, zmod*moveSpeed);
	}
	

	void idleFunc(void) {
		//mainCamera->addRotation(0.0f, 0.0f);
		//handleMovement();
	}




	void display(void) {

		int i;
		float timeValGlobal = globalTimer.getElapsedTimeInMilliSec();


		if (mainGUI == NULL) {
			mainGUI = new GameGUI();
			mainGUI->init(windowSize[0],windowSize[1]);
		}

		GLfloat fogColor[] = {fogRed, fogGreen, fogBlue};
		
		
		timer.start();

		glClearColor (fogRed,fogGreen,fogBlue,1.0);
	    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		mainCamera->addRotation(0.0f, 0.0f);
		handleMovement();
		
		mainPM->setPosition(mainCamera->unitPos[0], mainCamera->unitPos[1], mainCamera->unitPos[2]);

		if (loadingEnabled) {
			mainPM->update();
		}
		


		////
		glLoadIdentity();
		glRotatef(mainCamera->rotation[0],1.0,0.0,0.0);
		glRotatef(mainCamera->rotation[1],0.0,1.0,0.0);

		glTranslated(-mainCamera->unitPos[0],-mainCamera->unitPos[1],-mainCamera->unitPos[2]);
		

		shader.bind();		
		shader.setVec("cameraPos",mainCamera->unitPos,3);
		shader.setVec("fogColor",fogColor,3);
		shader.setVec("clipDist",mainCamera->clipDist,2);
		shader.setVec("pageSize",&gfUNITS_PER_PAGE,1);
		mainPM->doRender(E_RENDER_SOLID);
		shader.unbind();


		
		glEnable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		shaderWater.bind();

		shaderWater.setVec("timeVal",&timeValGlobal,1);
		shaderWater.setVec("cameraPos",mainCamera->unitPos,3);
		shaderWater.setVec("fogColor",fogColor,3);
		shaderWater.setVec("clipDist",mainCamera->clipDist,2);
		shaderWater.setVec("pageSize",&gfUNITS_PER_PAGE,1);
		mainPM->doRender(E_RENDER_WATER);
		shaderWater.unbind();
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		

		
		mainGUI->renderGUI(guiEnabled);


		
	    glutSwapBuffers();
		
		
		
		timer.stop();
		timeVal = timer.getElapsedTimeInMilliSec();
		
		timeValSec = timeVal*60.0/1000.0f;
		
		

		totalFPS += 1000.0f/timeVal;

		if (frameCount%fpsNumFrames == 0) {
			curFPS = totalFPS/((float)fpsNumFrames);
			totalFPS = 0.0f;
		}
		
		frameCount++;
		
		
	}

	void reshape (int w, int h) {
		glViewport (0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
		glMatrixMode (GL_PROJECTION); //set the matrix to projection
		glLoadIdentity ();
		gluPerspective (70, (GLfloat)w / (GLfloat)h, mainCamera->clipDist[0], mainCamera->clipDist[1]); //set the perspective (angle of sight, width, height, , depth)
		glMatrixMode (GL_MODELVIEW); //set the matrix back to model
	}
	
};
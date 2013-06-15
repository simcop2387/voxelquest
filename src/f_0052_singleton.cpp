class Singleton {
public:

	vector<string> shaderStrings;
	vector<string> fboStrings;
	vector<string> shaderTextureIDs;

	map<string, Shader*> shaderMap;
	map<string, FBOSet*> fboMap;

	string curShader;
	string allText;

	eProgramState programState;

	eProgramAction progActionsDown[E_PS_SIZE*256];
	eProgramAction progActionsUp[E_PS_SIZE*256];

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

    uint* lookup2to3;
    //uint* lookup3to2;
    GLuint lookup2to3ID;
    //GLuint lookup3to2ID;

	int shadersAreLoaded;
	int readyToRecompile;



	Singleton() {

	}
	~Singleton() {

	}

	inline void setProgAction(eProgramState ps, unsigned char kc, eProgramAction pa, bool isDown) {

		if (isDown) {
			progActionsDown[ps*256 + kc] = pa;
		}
		else {
			progActionsUp[ps*256 + kc] = pa;
		}
	}

	inline void setProgActionAll(unsigned char kc, eProgramAction pa, bool isDown) {
		int i;

		for (i = 0; i < E_PS_SIZE; i++) {
			setProgAction((eProgramState)i, kc, pa, isDown);
		}

	}

	void initKeys() {

		setProgActionAll(27,E_PA_QUIT, false);
		setProgActionAll('p',E_PA_TOGGLE_FULLSCREEN, false);
		setProgActionAll('r',E_PA_REFRESH, false);

	}

    
    
	void init(int _defaultWinW, int _defaultWinH) {

		defaultWinW = _defaultWinW;
		defaultWinH = _defaultWinH;

		isFullScreen = false;
	    shadersAreLoaded = 0;
	    readyToRecompile = 0;

		programState = E_PS_IN_GAME;

	    cameraPos.x = 0.0f;
	    cameraPos.y = 0.0f;
	    cameraZoom = 1.0f;

	    shadersAreLoaded = 0;
	    readyToRecompile = 0;

	    mouseX = 0.0f;
	    mouseY = 0.0f;
	    mouseXUp = 0.0f;
	    mouseYUp = 0.0f;

	    shiftPressed = false;
		ctrlPressed = false;
		altPressed = false;


	    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // 4-byte pixel alignment
	    glEnable(GL_TEXTURE_2D);
	    glDisable(GL_DEPTH_TEST);
	    glDisable(GL_CULL_FACE);
	    glDisable(GL_LIGHTING);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    glDisable(GL_BLEND);




	    fboStrings.push_back("testFBO");
	    fboStrings.push_back("resultFBO");
	    fboStrings.push_back("volGenFBO");

	    shaderStrings.push_back("shaderWater");

	    /*
	    shaderStrings.push_back("ShaderTarg2");
	    shaderStrings.push_back("ShaderLighting");
	    shaderStrings.push_back("GenerateVolume");
	    shaderStrings.push_back("RenderVolume");
	    shaderStrings.push_back("Simplex2D");
	    shaderStrings.push_back("CalcFlow");
	    shaderStrings.push_back("Erode");
	    shaderStrings.push_back("DLA");
	    shaderStrings.push_back("MapShader");
	    */

	    shaderTextureIDs.push_back("Texture0");
	    shaderTextureIDs.push_back("Texture1");
	    shaderTextureIDs.push_back("Texture2");
	    shaderTextureIDs.push_back("Texture3");
	    shaderTextureIDs.push_back("Texture4");
	    shaderTextureIDs.push_back("Texture5");
	    shaderTextureIDs.push_back("Texture6");
	    shaderTextureIDs.push_back("Texture7");


	    int i;

	    //initLookups();

	    for (i = 0; i < shaderStrings.size(); i++) {
	        shaderMap.insert(  pair<string,Shader*>(shaderStrings[i], NULL)  );
	    }
	    doShaderRefresh();

	    //fboSize = 512;
	    //bufsPerFBO = 2;

	    for (i = 0; i < fboStrings.size(); i++) {
	        fboMap.insert(  pair<string, FBOSet*>(fboStrings[i], new FBOSet())  );
	    }

	    //init(int _numBufs, int _width, int _height, int _bytesPerChannel);
	    fboMap["testFBO"]->init(2, 1024, 1024, 1);
	    fboMap["resultFBO"]->init(1, 1024, 1024, 1);
	    fboMap["volGenFBO"]->init(1, 4096, 4096, 1);
	    



	}

	void doShaderRefresh() {
	    std::cout << "doShaderRefresh\n";

	    readyToRecompile = 0;

	    int i;
	    int j;


	    for (i = 0; i < shaderStrings.size(); i++) {
	        if (shaderMap[ shaderStrings[i] ]) {
	            delete shaderMap[ shaderStrings[i] ];
	        }
	        shaderMap[ shaderStrings[i] ] = new Shader( ("shaders/"+shaderStrings[i]+".c").c_str() );
	    }
	    
	}




	
	void setMatrices(int w, int h) {
		glViewport(0,0,w,h);
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity ();
	    glMatrixMode (GL_PROJECTION);
	    glLoadIdentity ();
	}

	void setWH(int w, int h) {
	    baseW = w;
	    baseH = h;
	}

	void sampleFBODirect(FBOSet* fbos) {
	    int i;
	    if (shadersAreLoaded) {
	        for (i = 0; i < fbos->numBufs; i++) {
	            setShaderTexture(shaderTextureIDs[i], fbos->fbos[i].color_tex, i);
	        }
	    }
	}

	void unsampleFBODirect(FBOSet* fbos) {
	    int i;
	    if (shadersAreLoaded) {
	        for (i = fbos->numBufs - 1; i >= 0; i--) {
	            setShaderTexture(shaderTextureIDs[i], 0, i);
	        }
	    }
	}

	void bindFBODirect(FBOSet* fbos) {
	    setMatrices(fbos->width,fbos->height);
	    fbos->bind(1);
	    currentFBOResolutionX = fbos->width;
	    currentFBOResolutionY = fbos->height;
	}

	////

	void sampleFBO(string fboName) {
	    FBOSet* fbos = fboMap[fboName];
	    sampleFBODirect(fbos);
	}

	void unsampleFBO(string fboName) {
	    
	    FBOSet* fbos = fboMap[fboName];
	    unsampleFBODirect(fbos);
	}

	void bindFBO(string fboName) {
	    
	    FBOSet* fbos = fboMap[fboName];
	    bindFBODirect(fbos);
	}

	void unbindFBO() {
	    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	    setMatrices(baseW,baseH);
	    
	}



	void bindShader(string shaderName) {

	    if (shadersAreLoaded) {
	        curShader = shaderName;
	        shaderMap[curShader]->bind();
	    }
	    

	}

	void unbindShader() {
	    if (shadersAreLoaded) {
	        shaderMap[curShader]->unbind();
	    }
	    
	}

	void setShaderFloat(string paramName, float x) {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x);

	        shaderMap[curShader]->setShaderFloat(paramName, x);
	    }
	}
	void setShaderVec2(string paramName, float x, float y) {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y);
	        shaderMap[curShader]->setShaderVec2(paramName, x, y);
	    }
	}
	void setShaderVec3(string paramName, float x, float y, float z) {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y);
	        shaderMap[curShader]->setShaderVec3(paramName, x, y, z);
	    }
	}
	void setShaderVec4(string paramName, float x, float y, float z, float w) {
	    if (shadersAreLoaded) {
	        //shaderMap[curShader]->setUniformValue(shaderMap[curShader]->uniformLocation(paramName),x,y,z,w);
	        shaderMap[curShader]->setShaderVec4(paramName, x, y, z, w);
	    }
	}

	void setShaderTexture(string shaderSamplerName, uint texID, int multitexNumber) {
	    if (shadersAreLoaded) {
	        glActiveTexture(GL_TEXTURE0 + multitexNumber);
	        glBindTexture(GL_TEXTURE_2D, texID);
	        //glUniform1iARB(shaderMap[curShader]->uniformLocation(shaderSamplerName),multitexNumber);
	        shaderMap[curShader]->setShaderInt(shaderSamplerName,multitexNumber);
	    }
	}

	void setShaderTexture3D(string shaderSamplerName, uint texID, int multitexNumber) {
	    if (shadersAreLoaded) {
	        glActiveTexture(GL_TEXTURE0 + multitexNumber);
	        glBindTexture(GL_TEXTURE_3D, texID);
	        //glUniform1iARB(shaderMap[curShader]->uniformLocation(shaderSamplerName),multitexNumber);
	        shaderMap[curShader]->setShaderInt(shaderSamplerName,multitexNumber);
	    }
	}



	void drawFSQuad(float zoom) {
	    drawFSQuadOffset(0.0f,0.0f,zoom);
	}

	void drawFSQuadOffset(float xOff, float yOff, float zoom) {
	    float fx1 = (xOff-1.0f)*zoom;
	    float fy1 = (yOff-1.0f)*zoom;
	    float fx2 = (xOff+1.0f)*zoom;
	    float fy2 = (yOff+1.0f)*zoom;

	    glColor4f(1, 1, 1, 1);
	    glBegin(GL_QUADS);
	    glNormal3f(0, 0, 1);
	    
	    glTexCoord2f(0.0f, 0.0f);
	    glVertex3f(fx1,fy1,0.0f);
	    
	    glTexCoord2f(1.0f, 0.0f);
	    glVertex3f(fx2,fy1,0.0f);
	    
	    glTexCoord2f(1.0f, 1.0f);
	    glVertex3f(fx2,fy2,0.0f);
	    
	    glTexCoord2f(0.0f, 1.0f);
	    glVertex3f(fx1,fy2,0.0f);
	    
	    glEnd();
	}

	void drawFBO(string fboName, int ind, float zoom) {
	    drawFBOOffset(fboName, ind, 0.0f, 0.0f, zoom);
	}

	void drawFBOOffsetDirect(FBOSet* fbos, int ind, float xOff, float yOff, float zoom) {

	    glBindTexture(GL_TEXTURE_2D, fbos->fbos[ind].color_tex);
	    //glClearColor(0.2,0.2,0.2,0.0);
	    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    drawFSQuadOffset(xOff,yOff,zoom);
	    glBindTexture(GL_TEXTURE_2D, 0);
	}

	void drawFBOOffset(string fboName, int ind, float xOff, float yOff, float zoom) {
	    FBOSet* fbos = fboMap[fboName];
	    drawFBOOffsetDirect(fbos, ind, xOff, yOff, zoom);
	}


	void moveCamera(float dx, float dy, float zoom) {
	    cameraPos.x += dx/(baseW*zoom/2.0);
	    cameraPos.y -= dy/(baseH*zoom/2.0);
	}


	void doAction(eProgramAction pa) {
		switch (pa) {
			case E_PA_QUIT:
				if (isFullScreen) {
					//glutLeaveGameMode();
				}

				exit(0);
			break;
			case E_PA_TOGGLE_FULLSCREEN:
				isFullScreen = !isFullScreen;

				if (isFullScreen) {
					//glutGameModeString( "1280x800:32@75" ); //the settings for fullscreen mode
					//glutEnterGameMode();
					glutFullScreen();
				}
				else {
					glutReshapeWindow(defaultWinW, defaultWinH);
					//glutLeaveGameMode();
				}

			break;
			case E_PA_REFRESH:
				cameraPos.x = 0.0f;
            	cameraPos.y = 0.0f;
            	cameraZoom = 1.0f;
            	doShaderRefresh();
			break;
			case E_PA_SIZE:
				//unused
			break;

		}
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
		
	}
	void keyboardUp(unsigned char key, int x, int y) {

		int keymod = glutGetModifiers();

		if (keymod&GLUT_ACTIVE_SHIFT) {} else {
			shiftPressed = false;
		}
		if (keymod&GLUT_ACTIVE_CTRL) {} else {
			ctrlPressed = false;
		}
		if (keymod&GLUT_ACTIVE_ALT) {} else {
			altPressed = false;
		}

		doAction(progActionsUp[((int)programState)*256 + key]);
	}
	void keyboardDown(unsigned char key, int x, int y) {

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

		doAction(progActionsDown[((int)programState)*256 + key]);
	}
	void mouseMovementWithButton(int x, int y) {
		
	}
	void mouseMovementWithoutButton(int x, int y) {
		
	}
	void mouseClick(int button, int state, int x, int y) {
		if (state == GLUT_DOWN) {
			if (button == GLUT_LEFT_BUTTON) {

			}
		}
		if (state == GLUT_UP) {
			
		}
	}
	void display(void) {
		
	}
	void reshape(int w, int h) {
		setWH(w,h);
		setMatrices(baseW,baseH);
	}
	void idleFunc(void) {
		
	}

};


/*
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();

    if (event->buttons() & Qt::LeftButton) {
        singleton->fMouseDown = 1.0;
    } else if (event->buttons() & Qt::RightButton) {

    }
}

void GLWidget::wheelEvent(QWheelEvent *event){
  myDelta += event->delta()/300.0f;
  singleton->cameraZoom = pow(2.0, myDelta);

}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    singleton->fMouseDown = 0.0;

    singleton->mouseXUp = event->x();
    singleton->mouseYUp = event->y();

    if (event->buttons() & Qt::LeftButton) {
        singleton->fMouseDown = 1.0;

        singleton->moveCamera((float)dx, (float)dy, singleton->cameraZoom);
    } else if (event->buttons() & Qt::RightButton) {
        singleton->mouseX = event->x();
        singleton->mouseY = event->y();

        
    }
    lastPos = event->pos();
}
*/





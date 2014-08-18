
class GameGUI {
private:
	
public:
	Singleton* singleton;

	//Image* textImage;
	//GLuint guiTextureId;

	//Shader shaderGUI;

	UIComponent frameRateTB;
	UIComponent testTB;

	StyleSheet mainSS;

	GameGUI() {
		
	}

	void init(Singleton* _singleton) {

		singleton = _singleton;

		//textImage = loadBMP("basicFont.bmp");
		//guiTextureId = loadTexture(textImage);
	    //shaderGUI.init( "shaderGUI.c");
	    mainSS.init();





		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOL0_R] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOL0_G] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOL0_B] = 0.7f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOL0_A] = 1.0f;

		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOL1_R] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOL1_G] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOL1_B] = 0.7f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOL1_A] = 0.5f;

		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOL0_R] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOL0_G] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOL0_B] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOL0_A] = 0.0f;

		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOL1_R] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOL1_G] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOL1_B] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOL1_A] = 0.0f;



		mainSS.compStates[E_COMP_UP].props[E_SS_TGCOL0_R] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_TGCOL0_G] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_TGCOL0_B] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_TGCOL0_A] = 1.0f;

		mainSS.compStates[E_COMP_UP].props[E_SS_TGCOL1_R] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_TGCOL1_G] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_TGCOL1_B] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_TGCOL1_A] = 0.5f;

		

		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOLTEXT0_R] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOLTEXT0_G] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOLTEXT0_B] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOLTEXT0_A] = 0.0f;

		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOLTEXT1_R] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOLTEXT1_G] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOLTEXT1_B] = 0.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BGCOLTEXT1_A] = 0.0f;

		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOLTEXT0_R] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOLTEXT0_G] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOLTEXT0_B] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOLTEXT0_A] = 1.0f;

		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOLTEXT1_R] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOLTEXT1_G] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOLTEXT1_B] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_FGCOLTEXT1_A] = 1.0f;

		mainSS.compStates[E_COMP_UP].props[E_SS_BDCOL_R] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BDCOL_G] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BDCOL_B] = 1.0f;
		mainSS.compStates[E_COMP_UP].props[E_SS_BDCOL_A] = 1.0f;



	    mainSS.compStates[E_COMP_UP].props[E_SS_PADDING] = 2.0f;
	    mainSS.compStates[E_COMP_UP].props[E_SS_BORDER] = 1.0f;
	    mainSS.compStates[E_COMP_UP].props[E_SS_MARGIN] = 2.0f;
	    mainSS.compStates[E_COMP_UP].props[E_SS_CORNERRAD] = 8.0f;
	    mainSS.compStates[E_COMP_UP].props[E_SS_ROUNDNESS] = 1.0f;

	    mainSS.compStates[E_COMP_OVER].copyFrom(& (mainSS.compStates[E_COMP_UP]) );
	    mainSS.compStates[E_COMP_DOWN].copyFrom(& (mainSS.compStates[E_COMP_UP]) );

	    
	    mainSS.compStates[E_COMP_OVER].props[E_SS_BGCOL0_R] = 0.0f;
		mainSS.compStates[E_COMP_OVER].props[E_SS_BGCOL0_G] = 0.0f;
		mainSS.compStates[E_COMP_OVER].props[E_SS_BGCOL0_B] = 1.0f;
		mainSS.compStates[E_COMP_OVER].props[E_SS_BGCOL0_A] = 1.0f;

		mainSS.compStates[E_COMP_OVER].props[E_SS_BGCOL1_R] = 0.0f;
		mainSS.compStates[E_COMP_OVER].props[E_SS_BGCOL1_G] = 0.0f;
		mainSS.compStates[E_COMP_OVER].props[E_SS_BGCOL1_B] = 1.0f;
		mainSS.compStates[E_COMP_OVER].props[E_SS_BGCOL1_A] = 0.7f;


		mainSS.compStates[E_COMP_DOWN].props[E_SS_BGCOL0_R] = 0.2f;
		mainSS.compStates[E_COMP_DOWN].props[E_SS_BGCOL0_G] = 0.2f;
		mainSS.compStates[E_COMP_DOWN].props[E_SS_BGCOL0_B] = 1.0f;
		mainSS.compStates[E_COMP_DOWN].props[E_SS_BGCOL0_A] = 1.0f;

		mainSS.compStates[E_COMP_DOWN].props[E_SS_BGCOL1_R] = 0.2f;
		mainSS.compStates[E_COMP_DOWN].props[E_SS_BGCOL1_G] = 0.2f;
		mainSS.compStates[E_COMP_DOWN].props[E_SS_BGCOL1_B] = 1.0f;
		mainSS.compStates[E_COMP_DOWN].props[E_SS_BGCOL1_A] = 0.5f;




	    frameRateTB.init(
	    	singleton,
	    	&mainSS,
	    	"0.0f",
	    	20,
	    	20
	    );
		testTB.init(
			singleton,
			&mainSS,
			"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi nec ligula ac massa consectetur consectetur. Sed vel malesuada arcu. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque laoreet lorem sed nisi blandit malesuada. Donec consequat urna et metus feugiat faucibus vulputate tortor blandit.\n\nCras dignissim, sapien ut tristique faucibus, enim augue venenatis neque, vel condimentum lacus est vitae urna...",
			20,
			80
		);
	}

	void testOver(int x, int y) {
		testTB.testOver(x, y);
	}

	bool testHit(int button, int state, int x, int y) {
		return testTB.testHit(button, state, x, y);
	}

	void renderGUI(bool showGUI) {

		glEnable (GL_BLEND);

		if (singleton->frameCount % 60 == 0) {
			string s1 = floatToString(1000.0f/singleton->curMS);//curFPS
			string s2 = " FPS; ";
			string s3 = floatToString(singleton->curMS);
			string s4 = " MS ";
			frameRateTB.setText(s1+s2+s3+s4);
		}
		

		singleton->bindShader("GUIShader");
		singleton->setShaderTexture(0,singleton->basicFont->tid);
		
		glBegin (GL_QUADS);
			if (showGUI) {
				//testTB.render(true);
				frameRateTB.render(true);
			}
		glEnd ();

		//shaderGUI.unbind();
		singleton->setShaderTexture(0,0);
		singleton->unbindShader();


		glDisable(GL_BLEND);
	}

};


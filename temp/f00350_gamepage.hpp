




class GamePage: public Poco::Runnable {
public:

	int iDim;
	int iVoroDim;
	iVector3 iOff;
	int iVolumeSize;
	int iVoroSize;
	int iRenderSize;
	float origHeight;
	uint* volData;
	uint* voroData;
	Singleton* singleton;
	FBOSet* fboSet;
	uint volID;
	uint voroID;
	E_STATES curState;
	E_STATES nextState;

	


	GamePage() {

	}

	void init(Singleton* _singleton, int _iDim, iVector3 _iOff, int _iRenderSize) {
		int i;

		curState = E_STATE_INIT_BEG;
		nextState = E_STATE_WAIT;

		singleton = _singleton;

		

		iDim = _iDim;
		iOff = _iOff;

		int iBuf = iDim/2;
		int tmp = ((iOff.z * (256/iDim)) >> 8);
		origHeight = tmp;
		origHeight = origHeight/255.0;
		iDim += iBuf*2;

		iOff.x -= iBuf;
		iOff.y -= iBuf;
		iOff.z -= iBuf;

		iVolumeSize = iDim*iDim*iDim;
		volData = new uint[iVolumeSize];
		for (i = 0; i < iVolumeSize; i++) {
			volData[i] = 0;
		}



		uint voroPerUnit = 2;
		iVoroDim = iDim;
		iVoroSize = iVoroDim*iVoroDim*iVoroDim;
		voroData = new uint[iVoroSize];
		
		for (i = 0; i < iVoroSize; i++) {
			voroData[i] = 0;
		}
		


		iRenderSize = _iRenderSize;

		if (iRenderSize == -1) {
			iRenderSize = iDim*2;
		}

		
		int renderDim2 = iRenderSize*iRenderSize;

		fboSet = NULL;
		fboSet = new FBOSet();
		fboSet->init(2,iRenderSize,iRenderSize,1);
		glGenTextures(1,&volID);
		glGenTextures(1,&voroID);
		

		curState = E_STATE_INIT_END;
	}

	uint NumberOfSetBits(uint i)
	{
	    i = i - ((i >> 1) & 0x55555555);
	    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
	    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
	}

	uint clamp(uint val) {
		if (val > 255) {
			val = 255;
		}
		if (val < 0) {
			val = 0;
		}
		return val;
	}

	//32 bit only
	float sqrtFast(float x) {
	    unsigned int i = *(unsigned int*) &x; 
	    // adjust bias
	    i  += 127 << 23;
	    // approximation of square root
	    i >>= 1; 
	    return *(float*) &i;
	}
	


	void createSimplexNoise() {


		curState = E_STATE_CREATESIMPLEXNOISE_BEG;

		int i, j, k, m;


		int totLen = iDim;


		int ind = 0;

		uint tmp;

		float fx;
		float fy;
		float fz;

		uint randOff[3];

		const float RAND_MOD[9] = {
			3456.0f, 5965.0f, 45684.0f,
			4564.0f, 1234.0f, 6789.0f,
			4567.0f, 67893.0f, 13245.0f
		};

		
		float thresh;
		float testVal;


		
		
		for (j = 0; j < totLen; j++) {
			fy = j + iOff.y;

			for (i = 0; i < totLen; i++) {
				fx = i + iOff.x;
				
				for (k = 0; k < totLen; k++) {
					fz = k + iOff.z;

					ind = k*totLen*totLen + j*totLen + i;

					
					
					if (fx < 0.0f || fy < 0.0f || fz < 0.0f ) {
						volData[ind] = 0;
					}
					else {
						testVal = simplexScaledNoise(
											4.0f, //octaves
											0.5f, //persistence (amount added in each successive generation)
											1.0f/64.0f, //scale (frequency)
											0.0f,
											1.0f,
											fx+332.0f,
											fy+997.0f,
											fz+4444.0f
										);
						

						thresh = fz/255.0;
						tmp = clamp(testVal*255.0*(1.0-thresh*thresh*thresh));
						volData[ind] = (tmp<<24);

						//(tmp<<24)|(randOff[2]<<16)|(randOff[1]<<8)|randOff[0];
					}
					
					

					
				}
			}
		}

		totLen = iVoroDim;

		ind = 0;

		for (j = 0; j < totLen; j++) {
			fy = j + iOff.y;

			for (i = 0; i < totLen; i++) {
				fx = i + iOff.x;
				
				for (k = 0; k < totLen; k++) {
					fz = k + iOff.z;

					ind = k*totLen*totLen + j*totLen + i;

					if (fx < 0.0f || fy < 0.0f || fz < 0.0f ) {
						voroData[ind] = 0;
					}
					else {

						for (m = 0; m < 3; m++) {
							randOff[m] = clamp(simplexScaledNoise(
												1.0f, //octaves
												1.0f, //persistence (amount added in each successive generation)
												1.0f/4.0, //scale (frequency)
												0.0f,
												1.0f,
												fx+RAND_MOD[m*3+0],
												fy+RAND_MOD[m*3+1],
												fz+RAND_MOD[m*3+2]
											)*255.0);
						}
						
						voroData[ind] = (randOff[2]<<16)|(randOff[1]<<8)|randOff[0];
					}

					
				}
			}
		}
		


		curState = E_STATE_CREATESIMPLEXNOISE_END;


	}
	

	void renderVolume(float texMin, float texMax) {

		float fx1 = -1.0f;
		float fy2 = -1.0f;
		float fx2 = 1.0f;
		float fy1 = 1.0f;

		float centerX = (fx1 + fx2)/2.0f;
		float centerY = (fy1 + fy2)/2.0f;

		float fy25 = fy1*0.75f + fy2*0.25f;
		float fy75 = fy1*0.25f + fy2*0.75f;

		float coordsX[8];
		float coordsY[8];

		coordsX[0] = centerX;
		coordsY[0] = centerY;

		coordsX[1] = centerX;
		coordsY[1] = fy1;

		coordsX[2] = fx2;
		coordsY[2] = fy25;

		coordsX[3] = fx2;
		coordsY[3] = fy75;

		coordsX[4] = centerX;
		coordsY[4] = fy2;

		coordsX[5] = fx1;
		coordsY[5] = fy75;

		coordsX[6] = fx1;
		coordsY[6] = fy25;

		coordsX[7] = coordsX[1];
		coordsY[7] = coordsY[1];

		float backfaceX[8];
		float backfaceY[8];
		float backfaceZ[8];

		

		backfaceX[0] = texMin;
		backfaceY[0] = texMin;
		backfaceZ[0] = texMin;
		backfaceX[1] = texMin;
		backfaceY[1] = texMin;
		backfaceZ[1] = texMax;
		backfaceX[2] = texMax;
		backfaceY[2] = texMin;
		backfaceZ[2] = texMax;
		backfaceX[3] = texMax;
		backfaceY[3] = texMin;
		backfaceZ[3] = texMin;
		backfaceX[4] = texMax;
		backfaceY[4] = texMax;
		backfaceZ[4] = texMin;
		backfaceX[5] = texMin;
		backfaceY[5] = texMax;
		backfaceZ[5] = texMin;
		backfaceX[6] = texMin;
		backfaceY[6] = texMax;
		backfaceZ[6] = texMax;
		backfaceX[7] = backfaceX[1];
		backfaceY[7] = backfaceY[1];
		backfaceZ[7] = backfaceZ[1];


	    glBegin(GL_TRIANGLE_FAN);
	    glNormal3f(0, 0, 1);

	    int i;

	    for (i = 0; i < 8; i++) {

	    	glColor4f(backfaceX[i], backfaceY[i], backfaceZ[i], 1.0f);

	    	glMultiTexCoord3f( GL_TEXTURE0, backfaceX[i], backfaceY[i], backfaceZ[i]);
			
			if (i == 0) {
				//glColor4f((backfaceX[i]+1.0f)/2.0f, (backfaceY[i]+1.0f)/2.0f, (backfaceZ[i]+1.0f)/2.0f, 1.0f);
				glMultiTexCoord3f( GL_TEXTURE1, 1.0f, 1.0f, 1.0f);
			}
			else {
				glMultiTexCoord3f( GL_TEXTURE1, backfaceX[i], backfaceY[i], backfaceZ[i]);
			}

	    	glVertex3f(coordsX[i],coordsY[i],0.0f);
	    }
	    
	    glEnd();

	}


	void copyToTexture() {

		curState = E_STATE_COPYTOTEXTURE_BEG;

		//copy volData to 3d Texture
		glBindTexture(GL_TEXTURE_3D,volID);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, iDim, iDim, iDim, 0, GL_RGBA, GL_UNSIGNED_BYTE, volData);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_LINEAR
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_BORDER
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_3D,0);


		//copy voroData to 3d Texture
		glBindTexture(GL_TEXTURE_3D,voroID);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, iVoroDim, iVoroDim, iVoroDim, 0, GL_RGBA, GL_UNSIGNED_BYTE, voroData);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_LINEAR
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_BORDER
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_3D,0);


		
		
		//render 3D texture to 2D, interpolate data
		singleton->bindShader("GenerateVolume");
		singleton->bindFBO("volGenFBO");
		singleton->setShaderTexture3D(volID, 0);
		singleton->setShaderTexture3D(voroID, 1);
		singleton->setShaderTexture(singleton->lookup2to3ID, 2);

		//singleton->setShaderFloat("unitsPerDim", iDim.x);
		singleton->drawFSQuad(1.0f);

		singleton->setShaderTexture3D(0, 0);
		singleton->setShaderTexture3D(0, 1);
		singleton->setShaderTexture(0, 2);

		singleton->unbindFBO();
		singleton->unbindShader();

		//ray trace new texture, generate normals, AO, depth, etc
		singleton->bindShader("RenderVolume");
		singleton->bindFBODirect(fboSet);
		singleton->sampleFBO("volGenFBO");
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		singleton->setShaderFloat("curHeight",origHeight);
		renderVolume(0.0f,1.0f);
		singleton->unsampleFBO("volGenFBO");
		singleton->unbindFBO();
		singleton->unbindShader();
		



		/*
		//ray trace new texture, generate normals, AO, depth, etc
		singleton->bindShader("RenderVolume");
		singleton->bindFBODirect(fboSet);
	
		singleton->setShaderTexture3D("Texture0", volID, 0);
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		singleton->setShaderFloat("curHeight",origHeight);
		renderVolume(0.0f,1.0f);
		singleton->setShaderTexture3D("Texture0", 0, 0);

		singleton->unbindFBO();
		singleton->unbindShader();
		*/




		curState = E_STATE_COPYTOTEXTURE_END;
	}

	~GamePage() {

		if (volData) {
			delete[] volData;
		}
		if (voroData) {
			delete[] voroData;
		}
		

		if (volID) {
			glDeleteTextures(1, &volID);
		}
		if (voroID) {
			glDeleteTextures(1, &voroID);
		}
	}




	void run() {
		switch (nextState) {
			case E_STATE_CREATESIMPLEXNOISE_LAUNCH:
				createSimplexNoise();
			break;

			default:

			break;
		}
	}

};
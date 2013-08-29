




class GamePage: public Poco::Runnable {
public:

	int iDim;
	//int iVoroDim;
	iVector3 iOff;
	int iVolumeSize;
	//int iVoroSize;
	int iRenderSize;
	float origHeight;
	uint* volData;
	//uint* voroData;
	Singleton* singleton;
	FBOSet* fboSet;
	uint volID;
	uint volIDLinear;
	//uint voroID;
	E_STATES curState;
	E_STATES nextState;

	float voroScale;
	

	E_FILL_STATE fillState;

	GamePage() {

	}



	void init(Singleton* _singleton, int _iDim, iVector3 _iOff, int _iRenderSize) {
		int i;

		curState = E_STATE_INIT_BEG;
		nextState = E_STATE_WAIT;

		singleton = _singleton;

		voroScale = 1.0;
		

		iDim = _iDim;
		iOff = _iOff;

		
		int tmp = ((iOff.z * (256/iDim)) >> 8);
		origHeight = tmp;
		origHeight = origHeight/255.0;



		int iBuf = iDim/2;

		iDim += iBuf*2;
		
		//iOff.x -= iBuf;
		//iOff.y -= iBuf;
		//iOff.z -= iBuf;

		iVolumeSize = iDim*iDim*iDim;
		volData = new uint[iVolumeSize];
		for (i = 0; i < iVolumeSize; i++) {
			volData[i] = 0;
		}


		/*
		iVoroDim = iDim;
		iVoroSize = iVoroDim*iVoroDim*iVoroDim;
		voroData = new uint[iVoroSize];
		
		for (i = 0; i < iVoroSize; i++) {
			voroData[i] = 0;
		}
		*/
		


		iRenderSize = _iRenderSize;

		if (iRenderSize == -1) {
			iRenderSize = iDim*2;
		}

		
		int renderDim2 = iRenderSize*iRenderSize;

		fboSet = NULL;
		fboSet = new FBOSet();
		fboSet->init(2,iRenderSize,iRenderSize,1);
		glGenTextures(1,&volID);
		glGenTextures(1,&volIDLinear);
		//glGenTextures(1,&voroID);
		

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

		bool isBlank = true;
		bool isFull = true;
		

		curState = E_STATE_CREATESIMPLEXNOISE_BEG;

		int i, j, k, m;

		float maxGenHeight = 32.0f;

		int totLen = iDim;
		int totLenO2 = iDim/2;
		float fTotLen = (float)totLen;
		float fTotLenT2 = fTotLen*2.0f;

		int ind = 0;

		uint tmp;

		float fx, fy, fz;
		float fx2, fy2, fz2;

		uint randOff[3];

		float ijkVals[3];

		const float RAND_MOD[9] = {
			3456.0f, 5965.0f, 45684.0f,
			4564.0f, 1234.0f, 6789.0f,
			4567.0f, 67893.0f, 13245.0f
		};

		
		float thresh;
		float testVal;

		if (iOff.z- totLenO2 > maxGenHeight) {
			isBlank = true;
			isFull = false;
		}
		else {
			for (j = 0; j < totLen; j++) {

				ijkVals[1] = (float)j;

				fy = (j - totLenO2) + iOff.y;
				fy2 = (j - totLenO2)*voroScale + iOff.y;

				for (i = 0; i < totLen; i++) {

					ijkVals[0] = (float)i;

					fx = (i - totLenO2) + iOff.x;
					fx2 = (i - totLenO2)*voroScale + iOff.x;
					
					for (k = 0; k < totLen; k++) {

						ijkVals[2] = (float)k;

						fz = (k - totLenO2) + iOff.z;
						fz2 = (k - totLenO2)*voroScale + iOff.z;

						ind = k*totLen*totLen + j*totLen + i;

						
						
						if (fx < 0.0f || fy < 0.0f || fz < 0.0f ) {
							volData[ind] = 0;
						}
						else {
							testVal = simplexScaledNoise(
												4.0f, //octaves
												0.5f, //persistence (amount added in each successive generation)
												1.0f/32.0f, //scale (frequency)
												0.0f,
												1.0f,
												fx+singleton->seedX,
												fy+singleton->seedY,
												fz+singleton->seedZ
											);
							

							thresh = (fz/maxGenHeight);
							if (thresh > 1.0) {
								thresh = 1.0;
							}
							tmp = clamp(testVal*255.0*(1.0-thresh*thresh*thresh));

							if (tmp >= 127) {
								isBlank = false;
							}
							else {
								isFull = false;
							}
							


							for (m = 0; m < 3; m++) {
								randOff[m] = clamp((simplexScaledNoise(
																			1.0f, //octaves
																			1.0f, //persistence (amount added in each successive generation)
																			1.0f/4.0, //scale (frequency)
																			0.0f,
																			1.0f,
																			fx2+RAND_MOD[m*3+0],
																			fy2+RAND_MOD[m*3+1],
																			fz2+RAND_MOD[m*3+2]
																		) + ijkVals[m])*255.0/fTotLen);
							}

							if (tmp%16 > 1) {
								volData[ind] = (tmp<<24)|(randOff[2]<<16)|(randOff[1]<<8)|randOff[0];
							}
							else {
								volData[ind] = (tmp<<24);
							}

							
						}
						
						

						
					}
				}
			}
		}
		
		
		
		
		

		if (isBlank||isFull) {

			if (isBlank) {
				fillState = E_FILL_STATE_EMPTY;
			}
			if (isFull) {
				fillState = E_FILL_STATE_FULL;
			}

			curState = E_STATE_LENGTH;
		}
		else {
			fillState = E_FILL_STATE_PARTIAL;
			curState = E_STATE_CREATESIMPLEXNOISE_END;
		}

		


	}


	void copyToTexture() {

		curState = E_STATE_COPYTOTEXTURE_BEG;

		//copy volData to 3d Texture
		glBindTexture(GL_TEXTURE_3D,volID);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, iDim, iDim, iDim, 0, GL_RGBA, GL_UNSIGNED_BYTE, volData);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_LINEAR
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_NEAREST
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_BORDER
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_3D,0);

		glBindTexture(GL_TEXTURE_3D,volIDLinear);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, iDim, iDim, iDim, 0, GL_RGBA, GL_UNSIGNED_BYTE, volData);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_LINEAR
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_BORDER
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_3D,0);

		curState = E_STATE_COPYTOTEXTURE_END;

	}

	void generateVolume() {

		curState = E_STATE_GENERATEVOLUME_BEG;

		/*
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
		*/


		
		
		//render 3D texture to 2D, interpolate data
		singleton->bindShader("GenerateVolume");
		singleton->bindFBO("volGenFBO");
		singleton->setShaderTexture3D(volID, 0);
		singleton->setShaderTexture3D(volIDLinear, 1);
		singleton->setShaderTexture(singleton->lookup2to3ID, 2);

		singleton->setShaderFloat("unitsPerDim", iDim);
		singleton->setShaderFloat("voroScale", voroScale);
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
		glCallList(singleton->volTris);
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
		glCallList(singleton->volTris);
		singleton->setShaderTexture3D("Texture0", 0, 0);

		singleton->unbindFBO();
		singleton->unbindShader();
		*/




		curState = E_STATE_GENERATEVOLUME_END;
	}

	~GamePage() {

		if (volData) {
			delete[] volData;
		}
		
		/*if (voroData) {
			delete[] voroData;
		}*/
		

		if (volID) {
			glDeleteTextures(1, &volID);
		}
		if (volIDLinear) {
			glDeleteTextures(1, &volIDLinear);
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
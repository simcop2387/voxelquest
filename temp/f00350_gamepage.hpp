




class GamePage: public Poco::Runnable {
public:

	int iDim;
	FIVector4 iOff;
	int iVolumeSize;
	int iRSize2;
	//float curHeight;
	//float heightOfVol;
	uint* volData;
	Singleton* singleton;
	FBOSet* fboSet;
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

	GamePage() {

	}



	void init(Singleton* _singleton, int _iDim, FIVector4 _iOff, int _iRSize2) {
		int i;

		isDirty = false;

		curState = E_STATE_INIT_BEG;
		nextState = E_STATE_WAIT;

		singleton = _singleton;


		iDim = _iDim;
		iOff = _iOff;

		iRSize2 = _iRSize2;

		//heightOfVol = (((float)iRSize2)/2.0f - 1.0f)/255.0f;

		
		unitSize = (float)(singleton->unitSize);

		//curHeight = ((iOff.z * (256/iDim)) >> 8);
		//curHeight = curHeight/255.0;



		int iBuf = iDim/2;
		iDim += iBuf*2;

		iVolumeSize = iDim*iDim*iDim;
		volData = new uint[iVolumeSize];
		for (i = 0; i < iVolumeSize; i++) {
			volData[i] = 0;
		}





		

		fboSet = NULL;
		

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
		int totLenM1 = totLen-1;
		int totLenO2 = totLen/2;
		int totLenO3 = (totLen*3)/4;
		int totLenO4 = totLen/4;
		float fTotLen = (float)totLen;
		float fTotLenT2 = fTotLen*2.0f;

		int ind = 0;

		uint tmp;
		uint tmp2;

		float fx, fy, fz;

		uint randOff[3];

		float ijkVals[3];

		const float RAND_MOD[9] = {
			3456.0f, 5965.0f, 45684.0f,
			4564.0f, 1234.0f, 6789.0f,
			4567.0f, 67893.0f, 13245.0f
		};


		

		/*
		worldMin.setFX( ((0 + totLenO4) + iOff.getFX())*unitSize );
		worldMin.setFY( ((0 + totLenO4) + iOff.getFY())*unitSize );
		worldMin.setFZ( ((0 + totLenO4) + iOff.getFZ())*unitSize );

		worldMax.setFX( ((totLen - totLenO4) + iOff.getFX())*unitSize );
		worldMax.setFY( ((totLen - totLenO4) + iOff.getFY())*unitSize );
		worldMax.setFZ( ((totLen - totLenO4) + iOff.getFZ())*unitSize );
		*/

		worldMin.copyFrom(&iOff);
		worldMax.copyFrom(&iOff);
		worldMin.addXYZ( -totLenO4 );
		worldMax.addXYZ(  totLenO4 );
		worldMin.multXYZ((float)unitSize);
		worldMax.multXYZ((float)unitSize);


		worldUnitMin.copyFrom(&iOff);
		worldUnitMax.copyFrom(&iOff);
		worldUnitMin.addXYZ( -totLenO2 );
		worldUnitMax.addXYZ(  totLenO2-1 );


		
		float thresh;
		float testVal;
		float testVal2;

		if (false) { //(iOff.z - totLenO2*2.0f > maxGenHeight) {
			isBlank = true;
			isFull = false;
		}
		else {
			for (j = 0; j < totLen; j++) {

				ijkVals[1] = (float)j;

				fy = (j - totLenO2) + iOff.getFY();

				for (i = 0; i < totLen; i++) {

					ijkVals[0] = (float)i;

					fx = (i - totLenO2) + iOff.getFX();
					
					for (k = 0; k < totLen; k++) {

						ijkVals[2] = (float)k;

						fz = (k - totLenO2) + iOff.getFZ();

						ind = k*totLen*totLen + j*totLen + i;

						
						
						if (fx < 0.0f || fy < 0.0f || fz < 0.0f ) {
							volData[ind] = 0;
						}
						else {

							thresh = (fz/maxGenHeight);
							if (thresh > 1.0) {
								thresh = 1.0;
							}



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

							testVal2 = simplexScaledNoise(
												4.0f, //octaves
												0.5f, //persistence (amount added in each successive generation)
												1.0f/32.0f, //scale (frequency)
												0.0f,
												1.0f,
												fx+singleton->seedX - 1.0f,
												fy+singleton->seedY - 1.0f,
												fz+singleton->seedZ - 1.0f
											);
							

							
							tmp = clamp(testVal*255.0*(1.0-thresh*thresh*thresh));
							tmp2 = clamp(testVal2*255.0*(1.0-thresh*thresh*thresh));


							if (i >= totLenO4 && i <= totLenO3) {
								if (j >= totLenO4 && j <= totLenO3) {
									if (k >= totLenO4 && k <= totLenO3) {
										if (tmp > 126) {
											//isBlank = false;
										}
										else {
											isFull = false;
										}
									}
								}
							}

							
							if (tmp > 126 || tmp2 > 126) {
								isBlank = false;
							}
							


							for (m = 0; m < 3; m++) {
								randOff[m] = clamp((simplexScaledNoise(
																			1.0f, //octaves
																			1.0f, //persistence (amount added in each successive generation)
																			1.0f/4.0, //scale (frequency)
																			0.0f,
																			1.0f,
																			fx+RAND_MOD[m*3+0],
																			fy+RAND_MOD[m*3+1],
																			fz+RAND_MOD[m*3+2]
																		) + ijkVals[m])*255.0f/fTotLen);
							}

							if ( (tmp%16 > 6) && ( (i+j+k)%2 == 0) ) {
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

		if (fboSet == NULL) {
			
			fboSet = new FBOSet();
			fboSet->init(2,iRSize2,iRSize2,1,false);
			glGenTextures(1,&volID);
			glGenTextures(1,&volIDLinear);

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

			TOT_GPU_MEM_USAGE += ((float)(iDim*iDim*iDim*4*2))/(1024.0f*1024.0f);

		}
		else {


			/*
			void glTexImage3D(	GLenum target,
			 	GLint level,
			 	GLint internalFormat,
			 	GLsizei width,
			 	GLsizei height,
			 	GLsizei depth,
			 	GLint border,
			 	GLenum format,
			 	GLenum type,
			 	const GLvoid * data);

			*/

			glBindTexture(GL_TEXTURE_3D,volID);
				glTexSubImage3D(
					GL_TEXTURE_3D,
					0,
					
					0,
					0,
					0,

					iDim,
					iDim,
					iDim,

					GL_RGBA,
					GL_UNSIGNED_BYTE,

					volData
				);

			glBindTexture(GL_TEXTURE_3D,0);
			glBindTexture(GL_TEXTURE_3D,volIDLinear);
				glTexSubImage3D(
					GL_TEXTURE_3D,
					0,
					
					0,
					0,
					0,

					iDim,
					iDim,
					iDim,

					GL_RGBA,
					GL_UNSIGNED_BYTE,

					volData
				);
			glBindTexture(GL_TEXTURE_3D,0);

	
			
			//GLenum target,
			//GLint level,
			
			//GLint xoffset,
			//GLint yoffset,
			//GLint zoffset,
			
			//GLsizei width,
			//GLsizei height,
			//GLsizei depth,
			
			//GLenum format,
			//GLenum type,
			
			//const GLvoid * data


		}

		curState = E_STATE_COPYTOTEXTURE_END;

	}

	void generateVolume() {

		curState = E_STATE_GENERATEVOLUME_BEG;
		
		
		//render 3D texture to 2D, interpolate data
		singleton->bindShader("GenerateVolume");
		singleton->bindFBO("volGenFBO");
		singleton->setShaderTexture3D(volID, 0);
		singleton->setShaderTexture3D(volIDLinear, 1);
		singleton->setShaderTexture(singleton->lookup2to3ID, 2);

		singleton->setShaderFloat("unitsPerDim", iDim);
		singleton->setShaderfVec3("worldMin", &(worldMin));
		singleton->setShaderfVec3("worldMax", &(worldMax));

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
		//singleton->setShaderFloat("curHeight",curHeight);
		//singleton->setShaderFloat("heightOfVol",heightOfVol);
		singleton->setShaderfVec3("worldMin", &(worldMin));
		singleton->setShaderfVec3("worldMax", &(worldMax));

		glCallList(singleton->volTris);
		singleton->unsampleFBO("volGenFBO");
		singleton->unbindFBO();
		singleton->unbindShader();
		

		curState = E_STATE_GENERATEVOLUME_END;
	}

	~GamePage() {

		if (volData) {
			delete[] volData;
		}

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
class FBOWrapper
{
public:

	uint color_tex;
	uint depth_rb;
	//uint color_buf;
	//uint depth_buf;
	uint slot;
	int width;
	int height;
	int bytesPerChannel;
	bool hasDepth;


	GLint internalFormat;

	unsigned char *pixelsChar;
	float *pixelsFloat;
	bool isFloat;

    FBOWrapper() {}
    ~FBOWrapper() {}
    int init(int _width, int _height, int _bytesPerChannel, int _slot, bool _hasDepth) {
		width = _width;
		height = _height;
		bytesPerChannel = _bytesPerChannel;
		hasDepth = _hasDepth;

		int w = width;
		int h = height;

		isFloat = false;

		pixelsChar = NULL;
		pixelsFloat = NULL;


		slot = GL_COLOR_ATTACHMENT0_EXT;

		switch (_slot) {
			case 0:
				slot = GL_COLOR_ATTACHMENT0_EXT;
			break;
			case 1:
				slot = GL_COLOR_ATTACHMENT1_EXT;
			break;
			case 2:
				slot = GL_COLOR_ATTACHMENT2_EXT;
			break;
			case 3:
				slot = GL_COLOR_ATTACHMENT3_EXT;
			break;
			case 4:
				slot = GL_COLOR_ATTACHMENT4_EXT;
			break;
			case 5:
				slot = GL_COLOR_ATTACHMENT5_EXT;
			break;
			case 6:
				slot = GL_COLOR_ATTACHMENT6_EXT;
			break;
			case 7:
				slot = GL_COLOR_ATTACHMENT7_EXT;
			break;
		}

	    

		glGenTextures(1, &color_tex);
		glBindTexture(GL_TEXTURE_2D, color_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
	    switch (bytesPerChannel) {
	    	case 1:
	    		internalFormat = GL_RGBA8;
	    		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	    		
	    	break;
	    	case 2:
	    		internalFormat = GL_RGBA16;
	            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, GL_RGBA, GL_UNSIGNED_SHORT, 0);

	    	break;
	    	case 4:
	    		internalFormat = GL_RGBA32F;
	            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, GL_RGBA, GL_FLOAT, 0);
	            isFloat = true;

	    	break;
	    }

	    TOT_GPU_MEM_USAGE += ((float)(w*h*bytesPerChannel*4))/(1024.0f*1024.0f);


	    //

	    if (hasDepth) {
	    	glGenRenderbuffersEXT(1, &depth_rb);
	    	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
	    	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, w, h);
	    	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);
	    }

	    
	    //

		
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, slot, GL_TEXTURE_2D, color_tex, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

	    return 1;

	}



	void getPixelAtF(FIVector4* fv, int x, int y) {

		if (isFloat) {


			if ( (pixelsFloat == NULL) ) {
				getPixels();
			}

			//a
			//b
			//g
			//r

			fv->setFX(pixelsFloat[ (x + y*width)*4 + 0 ]);
			fv->setFY(pixelsFloat[ (x + y*width)*4 + 1 ]);
			fv->setFZ(pixelsFloat[ (x + y*width)*4 + 2 ]);
			fv->setFW(pixelsFloat[ (x + y*width)*4 + 3 ]);

		}
		else {
			doTrace("Attempted to call getPixelAtF on char buffer.");
		}

	}

	unsigned char getPixelAtC(int x, int y, int channel) {

		if (isFloat) {
			if ( (pixelsFloat == NULL) ) {
				getPixels();
			}

			return pixelsChar[ (x + y*width)*4 + channel ];

		}
		else {
			doTrace("Attempted to call getPixelAtC on float buffer.");
		}

	}


	void getPixels() {

		glBindTexture(GL_TEXTURE_2D, color_tex);
		GLint numBytes = 0;
		
		//GLint intForm;
		//glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPONENTS, &intForm); // get internal format type of GL texture


		switch(internalFormat) // determine what type GL texture has...
		{
			case GL_RGB:
				numBytes = width * height * 3;
			break;
			case GL_RGBA:
				numBytes = width * height * 4;
			break;
			case GL_RGBA8:
				numBytes = width * height * 4;
			break;
			//case GL_RGBA16:
			//
			//break;
			case GL_RGBA32F:
				numBytes = width * height * 4 * 4;
			break;

			default:
				doTrace("Unsupported Format Type");
				return;
			break;
		}

		if (numBytes) {

			if (isFloat) {

				if (pixelsFloat == NULL) {
					pixelsFloat = (float*)malloc(numBytes);
				}
				
				glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixelsFloat);
			}
			else {

				if (pixelsChar == NULL) {
					pixelsChar = (unsigned char*)malloc(numBytes);
				}
				glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsChar);
			}

			
		}

		glBindTexture(GL_TEXTURE_2D, 0);


	}


};


class FBOSet
{
public:
	int numBufs;
	int width;
	int height;
	int bytesPerChannel;

	GLuint mFBO;

	FBOWrapper* fbos;

	bool hasDepth;

    FBOSet() {}
    ~FBOSet() {}

    FBOWrapper* getFBOWrapper(int offset) {
    	return &(fbos[offset]);
    }

    void init(int _numBufs, int _width, int _height, int _bytesPerChannel, bool _hasDepth) {
		int i;

		hasDepth = _hasDepth;

		numBufs = _numBufs;
		height = _height;
		width = _width;
		bytesPerChannel = _bytesPerChannel;

		fbos = new FBOWrapper[numBufs];


		glGenFramebuffersEXT(1, &mFBO);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO);

		for (i = 0; i < numBufs; i++) {
			fbos[i].init(width, height, bytesPerChannel, i, hasDepth);
		}

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
    void bind(int doClear) {

		//setWH(width, height);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO);

		GLenum buffers[] = {GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT, GL_COLOR_ATTACHMENT4_EXT};
		glDrawBuffers(numBufs, buffers);

		if (doClear) {
	    	glClearColor(0.0,0.0,0.0,0.0);
	    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    }

	}
	/*
    void setWH(int w, int h) {
		glViewport(0,0,w,h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
		glMatrixMode(GL_MODELVIEW);
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity ();
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
	}
	*/
    void copyFromMem(int ind, uint* dat) {

		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO);

	    glBindTexture(GL_TEXTURE_2D,fbos[ind].color_tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dat);
		//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, fbos[ind].slot, GL_TEXTURE_2D, fbos[ind].color_tex, 0);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D,0);

		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
};



/*
//RGBA8 2D texture, 24 bit depth texture, 256x256
   glGenTextures(1, &color_tex);
   glBindTexture(GL_TEXTURE_2D, color_tex);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   //NULL means reserve texture memory, but texels are undefined
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 256, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
   //-------------------------
   glGenFramebuffersEXT(1, &fb);
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
   //Attach 2D texture to this FBO
   glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, color_tex, 0);
   //-------------------------
   glGenRenderbuffersEXT(1, &depth_rb);
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
   glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 256, 256);
   //-------------------------
   //Attach depth buffer to FBO
   glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);
   //-------------------------
   //Does the GPU support current FBO configuration?
   GLenum status;
   status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
   switch(status)
   {
      case GL_FRAMEBUFFER_COMPLETE_EXT:
      cout<<"good";
   default:
      HANDLE_THE_ERROR;
   }
   //-------------------------
   //and now you can render to GL_TEXTURE_2D
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClearDepth(1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //-------------------------
   glViewport(0, 0, 256, 256);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 256.0, 0.0, 256.0, -1.0, 1.0); 
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   //-------------------------
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glEnable(GL_DEPTH_TEST);
   //-------------------------
   //**************************
   //RenderATriangle, {0.0, 0.0}, {256.0, 0.0}, {256.0, 256.0}
   //Read http://www.opengl.org/wiki/VBO_-_just_examples
   RenderATriangle();
   //-------------------------
   GLubyte pixels[4*4*4];
   glReadPixels(0, 0, 4, 4, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
   //pixels 0, 1, 2 should be white
   //pixel 4 should be black
   //----------------
   //Bind 0, which means render to back buffer
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
*/

 

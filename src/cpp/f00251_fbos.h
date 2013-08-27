class FBOWrapper
{
public:

	uint color_tex;
	//uint color_buf;
	//uint depth_buf;
	uint slot;
	int width;
	int height;
	int bytesPerChannel;

    FBOWrapper() {}
    ~FBOWrapper() {}
    int init(int _width, int _height, int _bytesPerChannel, int _slot) {
		width = _width;
		height = _height;
		bytesPerChannel = _bytesPerChannel;

		int w = width;
		int h = height;


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
	    		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	    	break;
	    	case 2:
	            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, w, h, 0, GL_RGBA, GL_UNSIGNED_SHORT, 0);
	    	break;
	    	case 4:
	            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, 0);
	    	break;
	    }
		
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, slot, GL_TEXTURE_2D, color_tex, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

	    return 1;

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

    FBOSet() {}
    ~FBOSet() {}
    void init(int _numBufs, int _width, int _height, int _bytesPerChannel) {
		int i;

		numBufs = _numBufs;
		height = _height;
		width = _width;
		bytesPerChannel = _bytesPerChannel;

		fbos = new FBOWrapper[numBufs];


		glGenFramebuffersEXT(1, &mFBO);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO);

		for (i = 0; i < numBufs; i++) {
			fbos[i].init(width, height, bytesPerChannel, i);
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
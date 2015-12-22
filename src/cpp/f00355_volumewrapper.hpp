
class VolumeWrapper {
public:
	
	FBOSet fboSet;
	
	GLuint volId;
	FIVector4 terGenDim;
	
	FIVector4 genPosMin;
	FIVector4 genPosMax;
	
	bool isFloat;
	bool isReady;
	
	
	VolumeWrapper() {
		isReady = false;
	}
	void init(int z, GLenum clampMethod, bool _isFloat, int filterType) {
		
		int tx = 0;
		int ty = 0;
		
		switch (z) {
			case 16:
				tx = 64;
				ty = 64;
			break;
			case 32:
				tx = 256;
				ty = 128;
			break;
			case 64:
				tx = 512;
				ty = 512;
			break;
			case 128:
				tx = 2048;
				ty = 1024;
			break;
			case 256:
				tx = 4096;
				ty = 4096;
			break;
			default:
				cout << "Invalid VolumeWrapper dim\n";
			break;
		}
		
		int x = tx;
		int y = ty;
		
		
		isFloat = _isFloat;
		
		
		terGenDim.setFXYZ(x,y,z); //4096.0f,4096.0f,256.0f
		
		glGenTextures(1, &(volId));
		
		glBindTexture(GL_TEXTURE_3D, volId);
		
		if (isFloat) {
			glTexImage3D(
				GL_TEXTURE_3D,
				0,
				GL_RGBA32F, //GL_RGBA, // GL_RGBA32F
				z,
				z,
				z,
				0,
				GL_RGBA,
				GL_FLOAT, // GL_UNSIGNED_BYTE, // GL_FLOAT
				0
			);
		}
		else {
			glTexImage3D(
				GL_TEXTURE_3D,
				0,
				GL_RGBA, //GL_RGBA, // GL_RGBA32F
				z,
				z,
				z,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE, // GL_UNSIGNED_BYTE, // GL_FLOAT
				0
			);
		}
		
		
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filterType);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filterType);
		//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0); // causes crash on getError()?
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, clampMethod);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, clampMethod);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, clampMethod);
		glBindTexture(GL_TEXTURE_3D, 0);
		
		if (isFloat) {
			fboSet.init(1, tx, ty, 4, false);
		}
		else {
			fboSet.init(1, tx, ty, 1, false);
		}
		
		isReady = true;
		
	}
	
	/*
		GLenum target,
	 	GLint level,
	 	GLint xoffset,
	 	GLint yoffset,
	 	GLint zoffset,
	 	GLsizei width,
	 	GLsizei height,
	 	GLsizei depth,
	 	GLenum format,
	 	GLenum type,
	 	const GLvoid * data);

	*/
	
	void copyFloatArr(float* floatArr) {
		glBindTexture(GL_TEXTURE_3D, volId);
		glTexSubImage3D(
			GL_TEXTURE_3D,
			0,

			0,
			0,
			0,

			terGenDim[2],
			terGenDim[2],
			terGenDim[2],

			GL_RGBA,
			GL_FLOAT, //GL_UNSIGNED_BYTE,

			floatArr
		);
		glBindTexture(GL_TEXTURE_3D, 0);
	}
	
	void copyCharArr(unsigned char* charArr) {
		glBindTexture(GL_TEXTURE_3D, volId);
		glTexSubImage3D(
			GL_TEXTURE_3D,
			0,

			0,
			0,
			0,

			terGenDim[2],
			terGenDim[2],
			terGenDim[2],

			GL_RGBA,
			GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE,

			charArr
		);
		glBindTexture(GL_TEXTURE_3D, 0);
	}
	
	
};

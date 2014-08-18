
const static int MAX_OG_RAD = 20;
const static int MAX_OG_DIAM = MAX_OG_RAD*2+1;
const static int MAX_OG_SIZE = MAX_OG_DIAM * MAX_OG_DIAM * MAX_OG_DIAM;
const static int OG_NULL_VAL = 9999;

class OffsetGrid {
private:
	
public:
	iVector3 offsetGrid[MAX_OG_RAD+1][MAX_OG_SIZE];
	iVector3 offsetGridCum[(MAX_OG_RAD+1)*(MAX_OG_SIZE)];

	int offsetGridCounters[MAX_OG_RAD+1];
	int offsetGridCumCounters[MAX_OG_RAD+1];

	
	
	OffsetGrid() {
		int i;
		int j;
		int k;
		int curCount;
		
		for (j = 0; j < MAX_OG_RAD+1; j++) {
			offsetGridCounters[j] = 0;
			for (i = 0; i < MAX_OG_SIZE; i++) {
				offsetGrid[j][i].x = OG_NULL_VAL;
				offsetGrid[j][i].y = OG_NULL_VAL;
				offsetGrid[j][i].z = OG_NULL_VAL;
			}
		}
		
		
		int xdis;
		int ydis;
		int zdis;
		
		int max1;
		int max2;
		int ind;

		
		
		for (i = 0; i < MAX_OG_DIAM; i++) {
			for (j = 0; j < MAX_OG_DIAM; j++) {
				for (k = 0; k < MAX_OG_DIAM; k++) {
					xdis = i-MAX_OG_RAD;
					ydis = j-MAX_OG_RAD;
					zdis = k-MAX_OG_RAD;
					
					max1 = max(abs(xdis),abs(ydis));
					max2 = max(max1,abs(zdis));
					
					ind = offsetGridCounters[max2];
					
					offsetGrid[max2][ind].x = xdis;
					offsetGrid[max2][ind].y = ydis;
					offsetGrid[max2][ind].z = zdis;
					

					offsetGridCounters[max2]++;
					
					
					
				}
			}
		}

		curCount = 0;
		

		for (j = 0; j <= MAX_OG_RAD; j++) {
			for (i = 0; i < offsetGridCounters[j]; i++) {
				offsetGridCum[curCount].x = offsetGrid[j][i].x;
				offsetGridCum[curCount].y = offsetGrid[j][i].y;
				offsetGridCum[curCount].z = offsetGrid[j][i].z;

				curCount++;
			}
			offsetGridCumCounters[j] = curCount;

		}

	}
};




class UniformBuffer {
public:

	

	UniformBuffer() {
		wasInit = false;
	}

	// GLchar* names[] =
	// {
	// 	"SurfaceColor",
	// 	"WarmColor",
	// 	"CoolColor",
	// 	"DiffuseWarm",
	// 	"DiffuseCool"
	// };

	int uniPosition;

	GLuint bufferId, uniformBlockIndex, index, progId;
	GLsizei uniformBlockSize;
	GLint singleSize, offset;
	GLfloat* uniData;

	// GLfloat colors[] = 
	// {
	// 	0.45,0.45,1,1,
	// 	0.45,0.45,1,1,
	// 	0.75,0.75,0.75,1,
	// 	0.0,0.0,1.0,1,
	// 	0.0,1.0,0.0,1
	// };


	bool wasUpdated;
	bool wasInit;

	void init(GLuint _progId, int bufNameInd)
	{

		wasInit = true;
		uniPosition = 0;
		wasUpdated = false;
		progId = _progId;
		uniData = NULL;


		//Update the uniforms using ARB_uniform_buffer_object
		glGenBuffers(1, &bufferId);


		
		//There's only one uniform block here, the 'colors0' uniform block. 
		//It contains the color info for the gooch shader.
		uniformBlockIndex = glGetUniformBlockIndex(
			progId,
			BUF_NAMES[bufNameInd]//bufName.c_str()//"colors0"
		);

		
		//We need to get the uniform block's size in order to back it with the
		//appropriate buffer
		glGetActiveUniformBlockiv(
			progId,
			uniformBlockIndex,
			GL_UNIFORM_BLOCK_DATA_SIZE,
			&uniformBlockSize
		);

		doTraceND("uniformBlockSize: ", i__s(uniformBlockSize));



		uniData = new GLfloat[uniformBlockSize/4];


		
		//Create UBO.
		glBindBuffer(
			GL_UNIFORM_BUFFER,
			bufferId
		);
		glBufferData(
			GL_UNIFORM_BUFFER,
			uniformBlockSize,
			NULL,
			GL_STATIC_DRAW//GL_DYNAMIC_DRAW
		);

		//Now we attach the buffer to UBO binding point 0...
		glBindBufferBase(
			GL_UNIFORM_BUFFER,
			0,
			bufferId
		);
		//And associate the uniform block to this binding point.
		glUniformBlockBinding(
			progId,
			uniformBlockIndex,
			0
		);


		//To update a single uniform in a uniform block, we need to get its
		//offset into the buffer.
		
		//glGetUniformIndices(progId, 1, &names[2], &index);
		//glGetActiveUniformsiv(progId, 1, &index, GL_UNIFORM_OFFSET, &offset);
		//glGetActiveUniformsiv(progId, 1, &index, GL_UNIFORM_SIZE, &singleSize);

	    
	}

	void updateUniformBlock(int numFloats) {

		int datSize = uniformBlockSize;

		if (numFloats < 0) {

		}
		else {
			datSize = numFloats*4;
		}



		if (wasUpdated) {

		}
		else {
			glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
			//We can use BufferData to upload our data to the shader,
			//since we know it's in the std140 layout
			//each float is 4 bytes
			glBufferData(GL_UNIFORM_BUFFER, datSize, uniData, GL_DYNAMIC_DRAW);
			//With a non-standard layout, we'd use BufferSubData for each uniform.
			//glBufferSubData(GL_UNIFORM_BUFFER, offset, singleSize, &uniData[8]);
			
			wasUpdated = true;
		}

		

	}

	void beginUniformBlock() {
		uniPosition = 0;
	}

	void invalidateUniformBlock() {
		wasUpdated = false;
	}
	bool wasUpdatedUniformBlock() {
		return wasUpdated;
	}

	~UniformBuffer() {

		if (wasInit) {
			if (uniData) {
				delete[] uniData;
				uniData = NULL;
			}
			glDeleteBuffers(1,&bufferId);
		}
		

	}


};



class Shader {
private:
	unsigned int shader_id;
	unsigned int shader_vp;
	unsigned int shader_fp;
public:

	int curUBIndex;

	vector<UniformBuffer> uniVec;

	
	Shader() {

		curUBIndex = 0;

		pushTrace("Shader()");
		popTrace();
	}

	Shader(const char *shaderFile) {
		pushTrace("Shader(", shaderFile, ")");
		init(shaderFile);
		popTrace();
	}


	static char* textFileRead(const char *fileName) {
		pushTrace("textFileRead(",  fileName, ")");
		
		char* text = "";
		bool failed = true;
	    
		if (fileName != NULL) {
	        FILE *file = fopen(fileName, "rt");
	        
			if (file != NULL) {
	            fseek(file, 0, SEEK_END);
	            int count = ftell(file);
	            rewind(file);
	            
				if (count > 0) {
					text = new char[(count + 1)];
					//(char*)malloc(sizeof(char) * (count + 1));
					count = fread(text, sizeof(char), count, file);
					text[count] = '\0';
					failed = false;
				}
				fclose(file);
			}
		}

		if (failed) {
			doTraceND("FAILED TO READ FILE: ", fileName);
		}
		else {
			doTrace("READ FILE: ", fileName);
		}
		
		popTrace();
		
		return text;
	}

	static void validateShader(GLuint shader, const char* file = 0) {
		pushTrace("validateShader(", file, ")");
		
		const unsigned int BUFFER_SIZE = 512;
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		GLsizei length = 0;
	    
		glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
		if (length > 0) {
			doTraceND("Shader " , i__s(shader) , " (" , (file?file:"") , ") compile error: " , buffer);
			LAST_COMPILE_ERROR = true;
		}
		popTrace();


	}

	static int validateProgram(GLuint program) {
		pushTrace("validateProgram()");
		
		const unsigned int BUFFER_SIZE = 512;
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		GLsizei length = 0;
	    
		memset(buffer, 0, BUFFER_SIZE);
		glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
		if (length > 0) {
			doTraceND( "Program " , i__s(program) , " link error: " , buffer);
			LAST_COMPILE_ERROR = true;
			popTrace();
			return 0;
		}
	    
		glValidateProgram(program);
		GLint status;
		glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
		if (status == GL_FALSE) {
			doTraceND( "Error validating shader " , i__s(program));
			LAST_COMPILE_ERROR = true;
			popTrace();
			return 0;
		}
		
		popTrace();
		return 1;
		
	}
	

	void init(const char *shaderFile) {
		pushTrace("init(", shaderFile, ")");
		shader_vp = glCreateShader(GL_VERTEX_SHADER);
		shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
	    
	    
		std::size_t found;

		int baseIndex;
		int totCount;

		bool doCont;

		int i;

		const char* allText = textFileRead(shaderFile);


		if (allText == NULL) {
			doTraceND( "Either vertex shader or fragment shader file not found." );
	    }
	    else {
	    	string allTextString(allText);

	    	baseIndex = 0;
	    	doCont = true;
	    	totCount = 0;



	    	while (doCont) {
	    		found = allTextString.find("ublock", baseIndex);
	    		if (found != std::string::npos) {
	    			baseIndex = found+1;
	    			doCont = true;
	    			totCount++;
	    		}
	    		else {
	    			doCont = false;
	    		}
	    	}
			


			vector<string> allTextStringSplit = split(allTextString, '$');

			if (allTextStringSplit.size() == 3) {

				string vertStr = allTextStringSplit[0] + allTextStringSplit[1];
				string fragStr = allTextStringSplit[0] + allTextStringSplit[2];

				const GLchar* vertCS = new char[vertStr.length() + 1];
				const GLchar* fragCS = new char[fragStr.length() + 1];

				std::strcpy((GLchar*)vertCS,vertStr.c_str());
				std::strcpy((GLchar*)fragCS,fragStr.c_str());


		    	glShaderSource(shader_vp, 1, &(vertCS), 0);
				glShaderSource(shader_fp, 1, &(fragCS), 0);
			    
				glCompileShader(shader_vp);
				validateShader(shader_vp, shaderFile);
				glCompileShader(shader_fp);
				validateShader(shader_fp, shaderFile);



				shader_id = glCreateProgram();
				glAttachShader(shader_id, shader_fp);
				glAttachShader(shader_id, shader_vp);
				glLinkProgram(shader_id);
				validateProgram(shader_id);

				delete [] vertCS;
				delete [] fragCS;


				for (i = 0; i < totCount; i++) {
					uniVec.push_back(UniformBuffer());
					uniVec.back().init(shader_id, i);
				}


			}
			else {
				LAST_COMPILE_ERROR = true;
				doTraceND( "Error: " , shaderFile , "does not contain proper amount of splits ($)\n" );
			}
			
			
			delete[] allText;
	    }
		
		
		popTrace();
		
	}


	~Shader() {

		uniVec.clear();

		pushTrace("~Shader()");
		glDetachShader(shader_id, shader_fp);
		glDetachShader(shader_id, shader_vp);
		glDeleteShader(shader_fp);
		glDeleteShader(shader_vp);
		glDeleteProgram(shader_id);
		popTrace();
	}

	unsigned int id() {
		pushTrace("id()");
		return shader_id;
		popTrace();
	}

	void bind() {
		//pushTrace("bind()");
		glUseProgram(shader_id);
		//popTrace();
	}

	void updateUniformBlock(int ubIndex, int ubDataSize) {
		uniVec[ubIndex].updateUniformBlock(ubDataSize);
	}
	void invalidateUniformBlock(int ubIndex) {
		uniVec[ubIndex].invalidateUniformBlock();
	}
	void beginUniformBlock(int ubIndex) {
		curUBIndex = ubIndex;
		uniVec[ubIndex].beginUniformBlock();
	}
	bool wasUpdatedUniformBlock(int ubIndex) {

		if (uniVec.size() > ubIndex) {
			return uniVec[ubIndex].wasUpdatedUniformBlock();
		}
		else {
			return true;
		}

		
	}

	void unbind() {
		//pushTrace("unbind()");
		glUseProgram(0);
		//popTrace();
	}

	void setTexture(const GLchar* name, int texUnit, int texId) {
		GLint baseImageLoc = glGetUniformLocation(shader_id, name);

		glUniform1i(baseImageLoc, texUnit); //Texture unit 0 is for base images.
		
		//When rendering an objectwith this program.
		glActiveTexture(GL_TEXTURE0 + texUnit);
		glBindTexture(GL_TEXTURE_2D, texId);
	}

	
	void setVec(const GLchar* name, const GLfloat* vecData, int vecSize) {
		
		GLint loc = glGetUniformLocation(shader_id, name);
		
		switch (vecSize) {
			case 0:
				doTraceND( "Error: vecSize of 0 in setVec" );
			break;
			case 1:
				glUniform1fv(loc, 1, vecData);
			break;
			case 2:
				glUniform2fv(loc, 1, vecData);
			break;
			case 3:
				glUniform3fv(loc, 1, vecData);
			break;
			case 4:
				glUniform4fv(loc, 1, vecData);
			break;
		}
	}

	void setVecString(string name, const GLfloat* vecData, int vecSize) {
		
		GLint loc = glGetUniformLocation(shader_id, name.c_str());
		
		switch (vecSize) {
			case 0:
				doTraceND( "Error: vecSize of 0 in setVec" );
			break;
			case 1:
				glUniform1fv(loc, 1, vecData);
			break;
			case 2:
				glUniform2fv(loc, 1, vecData);
			break;
			case 3:
				glUniform3fv(loc, 1, vecData);
			break;
			case 4:
				glUniform4fv(loc, 1, vecData);
			break;
		}
	}

	void setShaderArrayfVec4(string paramName, float* x, int count) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform4fv(loc, count, x);
	}

	void setShaderArrayfVec3(string paramName, float* x, int count) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform3fv(loc, count, x);
	}

	void setShaderArray(string paramName, float* x, int count) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform1fv(loc, count, x);
	}

	void setShaderFloat(string paramName, float x) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform1f(loc,x);
	}
	void setShaderVec2(string paramName, float x, float y) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform2f(loc,x,y);
	}
	void setShaderVec3(string paramName, float x, float y, float z) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform3f(loc,x,y,z);
	}
	void setShaderVec4(string paramName, float x, float y, float z, float w) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform4f(loc,x,y,z,w);
	}

	void setShaderInt(string paramName, int x) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform1i(loc,x);
	}



	void setShaderfVec2(string paramName, FIVector4* f) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform2f(loc,f->getFX(),f->getFY());
	}
	void setShaderfVec3(string paramName, FIVector4* f) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform3f(loc,f->getFX(),f->getFY(),f->getFZ());
	}
	void setShaderfVec4(string paramName, FIVector4* f) {
		GLint loc = glGetUniformLocation(shader_id, paramName.c_str());
		glUniform4f(loc,f->getFX(),f->getFY(),f->getFZ(),f->getFW());
	}

	void setShaderFloatUB(string paramName, float x) {
		int cp = uniVec[curUBIndex].uniPosition;

		uniVec[curUBIndex].uniData[cp] = x;
		uniVec[curUBIndex].uniPosition += 1;
	}
	void setShaderfVec4UB(string paramName, FIVector4* f) {

		int cp = uniVec[curUBIndex].uniPosition;

		uniVec[curUBIndex].uniData[cp+0] = f->getFX();
		uniVec[curUBIndex].uniData[cp+1] = f->getFY();
		uniVec[curUBIndex].uniData[cp+2] = f->getFZ();
		uniVec[curUBIndex].uniData[cp+3] = f->getFW();
		uniVec[curUBIndex].uniPosition += 4;
	}

	

};





 

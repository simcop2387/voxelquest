// f00307_dynbuffer.h
//

#include "f00307_dynbuffer.e"
#define LZZ_INLINE inline
DynBuffer::DynBuffer ()
                    {
				drawMode = 0;
				imageData = 0;
				
				SCREEN_WIDTH    = 400;
				SCREEN_HEIGHT   = 300;
				CAMERA_DISTANCE = 3.0f;
				TEXT_WIDTH      = 8;
				TEXT_HEIGHT     = 13;
				IMAGE_WIDTH     = 1024;
				IMAGE_HEIGHT    = 1024;
				CHANNEL_COUNT   = 4;
				DATA_SIZE       = IMAGE_WIDTH * IMAGE_HEIGHT * CHANNEL_COUNT;
				
				font = GLUT_BITMAP_8_BY_13;
				
		
		    initSharedMem();

		    //TODO: 
		    //initGL();

		    // get OpenGL info
		    
		    glInfo.getInfo();
		    //glInfo.printSelf();

		    // init 2 texture objects
		    glGenTextures(1, &textureId);
		    glBindTexture(GL_TEXTURE_2D, textureId);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, IMAGE_WIDTH, IMAGE_HEIGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)imageData);
		    glBindTexture(GL_TEXTURE_2D, 0);

		#ifdef _WIN32
		    // check PBO is supported by your video card
		    if(glInfo.isExtensionSupported("GL_ARB_pixel_buffer_object"))
		    {
		        // get pointers to GL functions
		        glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
		        glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
		        glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
		        glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
		        glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
		        glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
		        glMapBufferARB = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
		        glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");

		        // check once again PBO extension
		        if(glGenBuffersARB && glBindBufferARB && glBufferDataARB && glBufferSubDataARB &&
		           glMapBufferARB && glUnmapBufferARB && glDeleteBuffersARB && glGetBufferParameterivARB)
		        {
		            pboSupported = true;
		            pboMode = 1;    // using 1 PBO
		            std::cout << "Video card supports GL_ARB_pixel_buffer_object." << std::endl;
		        }
		        else
		        {
		            pboSupported = false;
		            pboMode = 0;    // without PBO
		            std::cout << "Video card does NOT support GL_ARB_pixel_buffer_object." << std::endl;
		        }
		    }

		    // check EXT_swap_control is supported
		    if(glInfo.isExtensionSupported("WGL_EXT_swap_control"))
		    {
		        // get pointers to WGL functions
		        wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		        wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
		        if(wglSwapIntervalEXT && wglGetSwapIntervalEXT)
		        {
		            // enable v-sync
		            wglSwapIntervalEXT(1);
		            std::cout << "Video card supports WGL_EXT_swap_control." << std::endl;
		        }
		    }


		#else // for linux, do not need to get function pointers, it is up-to-date
		    if(glInfo.isExtensionSupported("GL_ARB_pixel_buffer_object"))
		    {
		        pboSupported = true;
		        pboMode = 1;
		        std::cout << "Video card supports GL_ARB_pixel_buffer_object." << std::endl;
		    }
		    else
		    {
		        pboSupported = false;
		        pboMode = 0;
		        std::cout << "Video card does NOT support GL_ARB_pixel_buffer_object." << std::endl;
		    }
		#endif

		    if(pboSupported)
		    {
		        // create 2 pixel buffer objects, you need to delete them when program exits.
		        // glBufferDataARB with NULL pointer reserves only memory space.
		        glGenBuffersARB(2, pboIds);
		        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboIds[0]);
		        glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, DATA_SIZE, 0, GL_STREAM_DRAW_ARB);
		        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboIds[1]);
		        glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, DATA_SIZE, 0, GL_STREAM_DRAW_ARB);
		        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
		    }

		    // start timer, the elapsed time will be used for updateVertices()
		    //timer.start();

		    // the last GLUT call (LOOP)
		    // window will be shown and display callback is triggered by events
		    // NOTE: this call never return main().
		    //glutMainLoop(); /* Start GLUT event-processing loop */

		
	}
void DynBuffer::setVsync (bool enabled)
                                    {
		// check EXT_swap_control is supported
		if(glInfo.isExtensionSupported("WGL_EXT_swap_control"))
		{
		    // get pointers to WGL functions
		    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		    wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
		    if(wglSwapIntervalEXT && wglGetSwapIntervalEXT)
		    {
		    		if (enabled) {
		    			wglSwapIntervalEXT(1);
		    		}
		    		else {
		    			wglSwapIntervalEXT(0);
		    		}
		        // enable v-sync
		        
		        //std::cout << "Video card supports WGL_EXT_swap_control." << std::endl;
		    }
		}
	}
DynBuffer::~ DynBuffer ()
                     {
		clearSharedMem();
	}
bool DynBuffer::initSharedMem ()
        {
	    screenWidth = SCREEN_WIDTH;
	    screenHeight = SCREEN_HEIGHT;

	    mouseLeftDown = mouseRightDown = false;
	    mouseX = mouseY = 0;

	    cameraAngleX = cameraAngleY = 0;
	    cameraDistance = CAMERA_DISTANCE;

	    drawMode = 0; // 0:fill, 1: wireframe, 2:points

	    // allocate texture buffer
	    imageData = new GLubyte[DATA_SIZE];
	    memset(imageData, 0, DATA_SIZE);

	    return true;
	}
void DynBuffer::clearSharedMem ()
        {
	    // deallocate texture buffer
	    delete [] imageData;
	    imageData = 0;

	    // clean up texture
	    glDeleteTextures(1, &textureId);

	    // clean up PBOs
	    if(pboSupported)
	    {
	        glDeleteBuffersARB(2, pboIds);
	    }
	}
void DynBuffer::initLights ()
        {
	    // set up light colors (ambient, diffuse, specular)
	    GLfloat lightKa[] = {.2f, .2f, .2f, 1.0f};  // ambient light
	    GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
	    GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
	    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
	    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
	    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

	    // position the light
	    float lightPos[4] = {0, 0, 20, 1}; // positional light
	    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	    glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
	}
void DynBuffer::setCamera (float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
        {
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	    gluLookAt(posX, posY, posZ, targetX, targetY, targetZ, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
	}
void DynBuffer::updatePixels (GLubyte * dst, int size)
        {
	    static int color = 0;

	    if(!dst)
	        return;

	    int* ptr = (int*)dst;

	    // copy 4 bytes at once
	    for(int i = 0; i < IMAGE_HEIGHT; ++i)
	    {
	        for(int j = 0; j < IMAGE_WIDTH; ++j)
	        {
	            *ptr = color;
	            ++ptr;
	        }
	        color += 257;   // add an arbitary number (no meaning)
	    }
	    ++color;            // scroll down
	}
void DynBuffer::showTransferRate ()
        {
	    //static Timer timer;
	    //static int count = 0;
	    //static std::stringstream ss;

	    // // update fps every second
	    // ++count;
	    // double elapsedTime = timer.getElapsedTime();
	    // if(elapsedTime > 1.0)
	    // {
	    //     ss.str("");
	    //     ss << std::fixed << std::setprecision(1);
	    //     ss << "Transfer Rate: " << (count / elapsedTime) * DATA_SIZE / (1024 * 1024) << " MB" << std::ends; // update fps string
	    //     ss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	    //     count = 0;                      // reset counter
	    //     timer.start();                  // restart timer
	    // }

	    // backup current model-view matrix
	    glPushMatrix();                     // save current modelview matrix
	    glLoadIdentity();                   // reset modelview matrix

	    // set to 2D orthogonal projection
	    glMatrixMode(GL_PROJECTION);        // switch to projection matrix
	    glPushMatrix();                     // save current projection matrix
	    glLoadIdentity();                   // reset projection matrix
	    //gluOrtho2D(0, IMAGE_WIDTH, 0, IMAGE_HEIGHT); // set to orthogonal projection
	    gluOrtho2D(0, screenWidth, 0, screenHeight); // set to orthogonal projection

	    //float color[4] = {1, 1, 0, 1};
	    //drawString(ss.str().c_str(), 200, 286, color, font);

	    // restore projection matrix
	    glPopMatrix();                      // restore to previous projection matrix

	    // restore modelview matrix
	    glMatrixMode(GL_MODELVIEW);         // switch to modelview matrix
	    glPopMatrix();                      // restore to previous modelview matrix
	}
void DynBuffer::toOrtho ()
        {
	    // set viewport to be the entire window
	    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

	    // set orthographic viewing frustum
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);

	    // switch to modelview matrix in order to set scene
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	}
void DynBuffer::toPerspective ()
        {
	    // set viewport to be the entire window
	    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

	    // set perspective viewing frustum
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluPerspective(60.0f, (float)(screenWidth)/screenHeight, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip

	    // switch to modelview matrix in order to set scene
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	}
void DynBuffer::displayCB ()
        {
	    static int index = 0;
	    int nextIndex = 0;                  // pbo index used for next frame

	    if(pboMode > 0)
	    {
	        // "index" is used to copy pixels from a PBO to a texture object
	        // "nextIndex" is used to update pixels in a PBO
	        if(pboMode == 1)
	        {
	            // In single PBO mode, the index and nextIndex are set to 0
	            index = nextIndex = 0;
	        }
	        else if(pboMode == 2)
	        {
	            // In dual PBO mode, increment current index first then get the next index
	            index = (index + 1) % 2;
	            nextIndex = (index + 1) % 2;
	        }

	        // start to copy from PBO to texture object ///////
	        //t1.start();

	        // bind the texture and PBO
	        glBindTexture(GL_TEXTURE_2D, textureId);
	        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboIds[index]);

	        // copy pixels from PBO to texture object
	        // Use offset instead of ponter.
	        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, GL_BGRA, GL_UNSIGNED_BYTE, 0);

	        // measure the time copying data from PBO to texture object
	        //t1.stop();
	        //copyTime = t1.getElapsedTimeInMilliSec();
	        ///////////////////////////////////////////////////


	        // start to modify pixel values ///////////////////
	        //t1.start();

	        // bind PBO to update pixel values
	        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboIds[nextIndex]);

	        // map the buffer object into client's memory
	        // Note that glMapBufferARB() causes sync issue.
	        // If GPU is working with this buffer, glMapBufferARB() will wait(stall)
	        // for GPU to finish its job. To avoid waiting (stall), you can call
	        // first glBufferDataARB() with NULL pointer before glMapBufferARB().
	        // If you do that, the previous data in PBO will be discarded and
	        // glMapBufferARB() returns a new allocated pointer immediately
	        // even if GPU is still working with the previous data.
	        glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, DATA_SIZE, 0, GL_STREAM_DRAW_ARB);
	        GLubyte* ptr = (GLubyte*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
	        if(ptr)
	        {
	            // update data directly on the mapped buffer
	            updatePixels(ptr, DATA_SIZE);
	            glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB); // release pointer to mapping buffer
	        }

	        // measure the time modifying the mapped buffer
	        //t1.stop();
	        //updateTime = t1.getElapsedTimeInMilliSec();
	        ///////////////////////////////////////////////////

	        // it is good idea to release PBOs with ID 0 after use.
	        // Once bound with 0, all pixel operations behave normal ways.
	        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	    }
	    else
	    {
	        ///////////////////////////////////////////////////
	        // start to copy pixels from system memory to textrure object
	        //t1.start();

	        glBindTexture(GL_TEXTURE_2D, textureId);
	        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)imageData);

	        //t1.stop();
	        //copyTime = t1.getElapsedTimeInMilliSec();
	        ///////////////////////////////////////////////////


	        // start to modify pixels /////////////////////////
	        //t1.start();
	        updatePixels(imageData, DATA_SIZE);
	        //t1.stop();
	        //updateTime = t1.getElapsedTimeInMilliSec();
	        ///////////////////////////////////////////////////
	    }


	    // clear buffer
	    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	    // save the initial ModelView matrix before modifying ModelView matrix
	    // glPushMatrix();

	    // // tramsform camera
	    // glTranslatef(0, 0, -cameraDistance);
	    // glRotatef(cameraAngleX, 1, 0, 0);   // pitch
	    // glRotatef(cameraAngleY, 0, 1, 0);   // heading

	    // // draw a point with texture
	    // glBindTexture(GL_TEXTURE_2D, textureId);
	    // glColor4f(1, 1, 1, 1);
	    // glBegin(GL_QUADS);
	    // glNormal3f(0, 0, 1);
	    // glTexCoord2f(0.0f, 0.0f);   glVertex3f(-1.0f, -1.0f, 0.0f);
	    // glTexCoord2f(1.0f, 0.0f);   glVertex3f( 1.0f, -1.0f, 0.0f);
	    // glTexCoord2f(1.0f, 1.0f);   glVertex3f( 1.0f,  1.0f, 0.0f);
	    // glTexCoord2f(0.0f, 1.0f);   glVertex3f(-1.0f,  1.0f, 0.0f);
	    // glEnd();

	    // // unbind texture
	    // glBindTexture(GL_TEXTURE_2D, 0);

	    // // draw info messages
	    // showInfo();
	    // //showTransferRate();
	    // printTransferRate();

	    // glPopMatrix();

	    //glutSwapBuffers();
	}
void DynBuffer::reshapeCB (int width, int height)
        {
	    screenWidth = width;
	    screenHeight = height;
	    toPerspective();
	}
void DynBuffer::idleCB ()
        {
	    glutPostRedisplay();
	}
void DynBuffer::keyboardCB (unsigned char key, int x, int y)
        {
	    switch(key)
	    {
	    case 27: // ESCAPE
	        exit(0);
	        break;

	    case ' ':
	        if(pboSupported)
	            pboMode = ++pboMode % 3;
	        std::cout << "PBO mode: " << pboMode << std::endl;
	         break;

	    case 'd': // switch rendering modes (fill -> wire -> point)
	    case 'D':
	        drawMode = ++drawMode % 3;
	        if(drawMode == 0)        // fill mode
	        {
	            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	            glEnable(GL_DEPTH_TEST);
	            glEnable(GL_CULL_FACE);
	        }
	        else if(drawMode == 1)  // wireframe mode
	        {
	            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	            glDisable(GL_DEPTH_TEST);
	            glDisable(GL_CULL_FACE);
	        }
	        else                    // point mode
	        {
	            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	            glDisable(GL_DEPTH_TEST);
	            glDisable(GL_CULL_FACE);
	        }
	        break;

	    default:
	        ;
	    }
	}
void DynBuffer::mouseCB (int button, int state, int x, int y)
        {
	    mouseX = x;
	    mouseY = y;

	    if(button == GLUT_LEFT_BUTTON)
	    {
	        if(state == GLUT_DOWN)
	        {
	            mouseLeftDown = true;
	        }
	        else if(state == GLUT_UP)
	            mouseLeftDown = false;
	    }

	    else if(button == GLUT_RIGHT_BUTTON)
	    {
	        if(state == GLUT_DOWN)
	        {
	            mouseRightDown = true;
	        }
	        else if(state == GLUT_UP)
	            mouseRightDown = false;
	    }
	}
void DynBuffer::mouseMotionCB (int x, int y)
        {
	    if(mouseLeftDown)
	    {
	        cameraAngleY += (x - mouseX);
	        cameraAngleX += (y - mouseY);
	        mouseX = x;
	        mouseY = y;
	    }
	    if(mouseRightDown)
	    {
	        cameraDistance -= (y - mouseY) * 0.2f;
	        if(cameraDistance < 2.0f)
	            cameraDistance = 2.0f;

	        mouseY = y;
	    }
	}
#undef LZZ_INLINE
 

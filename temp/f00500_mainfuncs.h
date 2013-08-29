





















Singleton* singleton;


void processSpecialKeys(int key, int x, int y) {
    singleton->processSpecialKeys(key,x,y);
}
void processKey(unsigned char key, int x, int y, bool isPressed) {
    singleton->processKey(key,x,y,isPressed);
}
void keyboardUp(unsigned char key, int x, int y) {
    singleton->keyboardUp(key,x,y);
}
void keyboardDown(unsigned char key, int x, int y) {
    singleton->keyboardDown(key,x,y);
}
void mouseMovementWithButton(int x, int y) {
    singleton->mouseMove(x,y);
}
void mouseMovementWithoutButton(int x, int y) {
    singleton->mouseMove(x,y);
}
void mouseClick(int button, int state, int x, int y) {
    singleton->mouseClick(button,state,x,y);
}

void display(void) {
    singleton->display();
}

void reshape (int w, int h) {
    singleton->reshape(w,h);
}
void idleFunc(void) {
    singleton->display();
}

int MAX_CONSOLE_LINES = 500;

void RedirectIOToConsole()

{

    int hConHandle;

    long lStdHandle;

    CONSOLE_SCREEN_BUFFER_INFO coninfo;

    FILE *fp;

    // allocate a console for this app

    AllocConsole();

    // set the screen buffer to be big enough to let us scroll text

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),

                               &coninfo);

    coninfo.dwSize.Y = MAX_CONSOLE_LINES;

    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),

                               coninfo.dwSize);

    // redirect unbuffered STDOUT to the console

    lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);

    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    fp = _fdopen( hConHandle, "w" );

    *stdout = *fp;

    setvbuf( stdout, NULL, _IONBF, 0 );

    // redirect unbuffered STDIN to the console

    lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);

    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    fp = _fdopen( hConHandle, "r" );

    *stdin = *fp;

    setvbuf( stdin, NULL, _IONBF, 0 );

    // redirect unbuffered STDERR to the console

    lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);

    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    fp = _fdopen( hConHandle, "w" );

    *stderr = *fp;

    setvbuf( stderr, NULL, _IONBF, 0 );

    std::ios::sync_with_stdio();

}






#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024


int main(int argc, char* argv[])
{


    RedirectIOToConsole();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(400, 200);
    glutCreateWindow("VoxelQuest");

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        doTrace("There was an error with GLEW");
    }
    else {
        doTrace("GLEW_OK");
    }

    
    ////////////
    singleton = new Singleton();
    singleton->init(WINDOW_WIDTH,WINDOW_HEIGHT);
    glClearColor(0, 0, 0, 0);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, WINDOW_WIDTH-1, WINDOW_HEIGHT-1, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    ////////////



    glutDisplayFunc(display);
    glutIdleFunc(idleFunc);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(mouseMovementWithoutButton);
    glutMotionFunc(mouseMovementWithButton);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(processSpecialKeys);
    glutMainLoop();

    return 0;
}



/*
int WebSocketServer::main(const std::vector<std::string>& args)
{
    int argc = 0;
    char** argv = NULL;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Main Window");
    init(); // changed the init function to come directly before display function
    glutDisplayFunc(display);
    glutMainLoop();

    return Application::EXIT_OK;
}
*/




/*
int WebSocketServer::main(const std::vector<std::string>& args)
{

    srand ( time(NULL) );
    int winW = 1024;
    int winH = 1024;
    GLenum err;

    int argCount = 0;
    char** argStrs = NULL;

    g_RecBuffer = new char[g_MAX_FRAME_SIZE];

    //RedirectIOToConsole();

    if (_helpRequested)
    {
        displayHelp();
    }
    else
    {

        doTrace("WebSocketServer::main");


        
        unsigned short port = (unsigned short) config().getInt("WebSocketServer.port", 9980);
        ServerSocket svs(port);
        HTTPServer srv(new RequestHandlerFactory, svs, new HTTPServerParams);
        srv.start();
        //waitForTerminationRequest();
        


        glutInit(&argCount, argStrs); //&argc, argv

        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
        glutInitWindowSize(winW, winH);
        glutInitWindowPosition(300, 100);
        glutCreateWindow("Voxel Quest");
        

        glewExperimental = GL_TRUE;
        err = glewInit();
        if (err != GLEW_OK) {
            doTrace("There was an error with GLEW");
        }
        else {
            doTrace("GLEW_OK");
        }

        singleton = new Singleton();
        singleton->init(winW,winH);

        glutDisplayFunc(display);
        glutIdleFunc(idleFunc);
        glutReshapeFunc(reshape);
        glutPassiveMotionFunc(mouseMovementWithoutButton);
        glutMotionFunc(mouseMovementWithButton);
        glutMouseFunc(mouseClick);
        glutKeyboardFunc(keyboardDown);
        glutKeyboardUpFunc(keyboardUp);
        glutSpecialFunc(processSpecialKeys);

        

        
        glutMainLoop();



        srv.stop();
    }

    delete[] g_RecBuffer;

    return Application::EXIT_OK;
}
*/


//POCO_SERVER_MAIN(WebSocketServer) 

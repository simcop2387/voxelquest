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
	singleton->mouseMovementWithButton(x,y);
}
void mouseMovementWithoutButton(int x, int y) {
	singleton->mouseMovementWithoutButton(x,y);
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
	singleton->idleFunc();
}



int WebSocketServer::main(const std::vector<std::string>& args)
{

	srand ( time(NULL) );
	int winW = 1024;
	int winH = 1024;

	int argCount = 0;
	char** argStrs = NULL;

	g_RecBuffer = new char[g_MAX_FRAME_SIZE];


	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

	if (_helpRequested)
	{
		displayHelp();
	}
	else
	{
		unsigned short port = (unsigned short) config().getInt("WebSocketServer.port", 9980);
		ServerSocket svs(port);
		HTTPServer srv(new RequestHandlerFactory, svs, new HTTPServerParams);
		srv.start();
		//waitForTerminationRequest();



		glutInit(&argCount, argStrs); //&argc, argv
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	
		glutInitWindowSize(winW, winH);
		glutInitWindowPosition(100, 100);
		glutCreateWindow("Voxel Quest");
		glutDisplayFunc(display);
		glutIdleFunc(display);
		glutReshapeFunc(reshape);
		glutPassiveMotionFunc(mouseMovementWithoutButton);
		glutMotionFunc(mouseMovementWithButton);
		glutMouseFunc(mouseClick);
		glutKeyboardFunc(keyboardDown);
		glutKeyboardUpFunc(keyboardUp);
		glutSpecialFunc(processSpecialKeys);
		singleton = new Singleton();
		singleton->init(winW,winH);
		glutMainLoop();



		srv.stop();
	}

	delete[] g_RecBuffer;

	return Application::EXIT_OK;
}


POCO_SERVER_MAIN(WebSocketServer)




/*
int main(int argc, char **argv) {
	srand ( time(NULL) );
	
	int winW = 1024;
	int winH = 1024;
	
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	
	glutInitWindowSize(winW, winH);
	glutInitWindowPosition(100, 100);
    glutCreateWindow("Voxel Quest");
    glutDisplayFunc(display);
	glutIdleFunc(display);
    glutReshapeFunc(reshape);
	glutPassiveMotionFunc(mouseMovementWithoutButton);
	glutMotionFunc(mouseMovementWithButton);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(processSpecialKeys);

	singleton = new Singleton();
	singleton->init(winW,winH);
    
    glutMainLoop();
    
    return 0;
}
*/

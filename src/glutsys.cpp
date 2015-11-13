#include "glutsys.h"

static GlutSys *g_pSys = NULL;

//============================================================================
//============================================================================
GlutSys::GlutSys()
{
	m_hanWin = 0;
	m_rotX = 0.0;
	m_rotY = 0.0;
	m_transZ = -3.0;
    _exit = false;

	if (g_pSys != NULL)
	{
		LogError("Error: Glut already initialized");
		m_globalInit = false;
	}
	else
	{
		g_pSys = this;
		m_globalInit = true;
	}

	//m_moveSys.reset(new MoveSys(20.0f));
	//m_moveSys.reset(new MoveSys(0.1f));
	m_setCamera = true;

	m_dd.reset(new DrawData());

}

//============================================================================
//============================================================================
GlutSys::~GlutSys()
{
    destroy();

	if (m_globalInit) g_pSys = NULL;
}

//============================================================================
//============================================================================
DrawData* GlutSys::dataDraw()
{
    return m_dd.get();
}

//============================================================================
//============================================================================
DataMouse* GlutSys::dataMouse()
{
    return m_dd->_dm.get();
}

//============================================================================
//============================================================================
DataSel* GlutSys::dataSel()
{
    return m_dd->_ds.get();
}

//============================================================================
//============================================================================
MoveSys* GlutSys::moveSys()
{
    return m_dd->_movesys.get();
}

//============================================================================
//============================================================================
string GlutSys::getGLInfo()
{
	char *pstr;
	string info;
	info =  "OpenGL Information\n\nWho: ";
	pstr = (char *)::glGetString( GL_VENDOR );
	if (pstr) info += pstr;
	info += "\nWhich: ";
	pstr = (char *)::glGetString( GL_RENDERER );
	if (pstr) info += pstr;
	info += "\nVersion: ";
	pstr = (char *)::glGetString( GL_VERSION );
	if (pstr) info += pstr;
	info += "\nExtensions: ";
	pstr = (char *)::glGetString( GL_EXTENSIONS );
	if (pstr) info += pstr;
	return info;
}

//============================================================================
// TODO: this should only be called once, if it needs to be called more than once, then destroy needs to be called and refactored to work, but really this init should only happen once.
//============================================================================
bool GlutSys::init(QCoreApplication *app, int argc, const char** argv, int winWidth, int winHeight, PFilePaths pFilePaths)
{
	if (!m_globalInit) return false;

    //destroy();

    //m_moveSys.reset(new MoveSys(20.0f));
    //m_moveSys.reset(new MoveSys(0.1f));
    m_dd.reset(new DrawData());

    m_app = app;

    m_dd->_winW = winWidth;
    m_dd->_winH = winHeight;
	m_filePaths = pFilePaths;
    if (m_filePaths.get() == NULL)
    {
        m_filePaths.reset(new FilePaths());
        m_filePaths->Init(argv[0]);
    }

    // initialize GLUT 
    glutInit(&argc, (char**)argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH) / 2 - m_dd->_winW / 2, glutGet(GLUT_SCREEN_HEIGHT) / 2 - m_dd->_winH / 2);
    glutInitWindowSize(m_dd->_winW, m_dd->_winH);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION); // for glutLeaveMainLoop to return executation to the caller and not exit the application.
    m_hanWin = glutCreateWindow("GIS Render");
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);

    //glutHideWindow();

    // register GLUT callback functions
    glutReshapeFunc(resizeWindow);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(motionPassive);

	// initialize necessary OpenGL extensions
    //glewInit();
    //GLboolean bGLEW = glewIsSupported("GL_VERSION_2_0 GL_ARB_pixel_buffer_object");
	//if (!bGLEW)
	//{
	//	LogError("Pixel Buffer Object not supported");
	//	Cleanup(-1);
	//}

    // default initialization
    glClearColor(1.0, 1.0, 1.0, 1.0);
	//glEnable(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);

	// for smooth lines
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// also hits for smooth lines and polys
	glEnable( GL_LINE_SMOOTH );
	//glEnable( GL_POLYGON_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    glAlphaFunc(GL_GREATER, 0);
    glEnable(GL_ALPHA_TEST);
	

    // viewport
    moveSys()->m_camera->onResize(winWidth, winHeight);

    /*
    glViewport(0, 0, winWidth, winHeight);

    // projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)winWidth / (GLfloat) winHeight, 0.1, 10000.0);
	//gluPerspective(60.0, (GLfloat)winWidth / (GLfloat) winHeight, 1, 100.0);
    */

    // set view matrix
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	/*
    glTranslatef(0.0, 0.0, translate_z);
    glRotatef(rotate_x, 1.0, 0.0, 0.0);
    glRotatef(rotate_y, 0.0, 1.0, 0.0);
	*/

	glewInit();

    m_dd->init();

#ifdef _WIN32
	if (wglSwapIntervalEXT)
	{
		wglSwapIntervalEXT(1); 
	}
#endif
	/*
	int (*SwapInterval)(int);
SwapInterval = getProcAddress("glXSwapInterval");
if (!SwapInterval)
SwapInterval = getProcAddress("glXSwapIntervalEXT");
if (!SwapInterval)
SwapInterval = getProcAddress("glXSwapIntervalSGI");
if (!SwapInterval)
SwapInterval = getProcAddress("wglSwapInterval");
if (!SwapInterval)
SwapInterval = getProcAddress("wglSwapIntervalEXT");
if (!SwapInterval)
SwapInterval = getProcAddress("wglSwapIntervalSGI");

SwapInterval(1);
*/

	return true;
}

//============================================================================
//============================================================================
void GlutSys::run()
{
	// could use a try catch here an throw and exception in Exit to exit
	glutMainLoop();
}

//============================================================================
//============================================================================
void GlutSys::exit()
{
    //destroy();
    LogTrace("exiting...");
    //glutLeaveMainLoop();
    //::exit(0);
    _exit = true;
}

//============================================================================
//============================================================================
void GlutSys::destroy()
{
	if (m_hanWin)
	{
		glutDestroyWindow(m_hanWin); // this crashes when shutting down the window with the gui, but doesn't if forcing a shutdown such as windowless mode.
        m_hanWin = 0;
	}

	m_hanWin = 0;
	m_rotX = 0.0;
	m_rotY = 0.0;
	m_transZ = -3.0;

    //m_moveSys.reset();
    m_dd.reset();
}

//============================================================================
//============================================================================
void GlutSys::setCamera()
{
    if (moveSys() && m_setCamera)
	{
        moveSys()->m_camera->setView();
        //m_dd->m_frustum->CalcViewVolumePlanes();
		return;
	}

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //m_dd->m_frustum->CalcViewVolumePlanes();
}

//============================================================================
//============================================================================
void GlutSys::onResizeWindow(int width, int height)
{
    if (m_dd)
    {
        m_dd->_winW = width;
        m_dd->_winH = height;
    }

    if (moveSys())
    {
        moveSys()->m_camera->onResize(width, height);
    }
}

//============================================================================
//============================================================================
void GlutSys::onDisplay()
{
#ifdef QT
    if (m_app) m_app->processEvents();
#endif

    onPreDisplay();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    setCamera();
    displayData();

	glFlush();
	glFinish();

    // flip backbuffer to screen
    glutSwapBuffers();

    if (_exit)
    {
        glutLeaveMainLoop();
        destroy();
        return;
    }
    glutPostRedisplay();
}

//============================================================================
//============================================================================
void GlutSys::onKeyboard(unsigned char key, int x, int y)
{
    // remove warnings
    x = x;
    y = y;

	switch(key) 
    {
        case '\033': // escape quits
        //case '\015': // Enter quits    
        case 'Q':    // Q quits
        case 'q':    // q (or escape) quits
            // Cleanup up and quit
            exit();
            break;
		case 'w':
			if (moveSys()) moveSys()->MoveForward();
			break;
		case 's':
			if (moveSys()) moveSys()->MoveBackward();
			break;
		case 'a':
			if (moveSys()) moveSys()->StrafeLeft();
			break;
		case 'd':
			if (moveSys()) moveSys()->StrafeRight();
			break;
		case 'r':
			if (moveSys()) moveSys()->MoveUp();
			break;
		case 'f':
			if (moveSys()) moveSys()->MoveDown();
			break;
		case 'm':
			m_dd->m_wireframe = !m_dd->m_wireframe;
			if (m_dd->m_wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			break;
		case 'h':
			m_dd->m_shadersOn = !m_dd->m_shadersOn;
			break;
		case 'l':
			m_dd->m_lightingOn = !m_dd->m_lightingOn;
			break;
    }
}

//============================================================================
//============================================================================
void GlutSys::onKeyboardSpecial(int key, int x, int y)
{
    // remove warnings
    x = x;
    y = y;

	switch(key)
	{
		case GLUT_KEY_LEFT:
			if (moveSys()) moveSys()->LookLeft();
			break;
		case GLUT_KEY_RIGHT:
			if (moveSys()) moveSys()->LookRight();
			break;
		case GLUT_KEY_UP:
			if (moveSys()) moveSys()->LookUp();
			break;
		case GLUT_KEY_DOWN:
			if (moveSys()) moveSys()->LookDown();
			break;
	}
}

//============================================================================
//============================================================================
void GlutSys::onMouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) 
	{
        dataMouse()->btnsDown() |= 1 << button;
    } 
	else if (state == GLUT_UP) 
	{
        dataMouse()->btnsDown() = DataMouse::MOUSE_DOWN_NONE;
    }

    dataMouse()->x() = x;
    dataMouse()->y() = y;

    glutPostRedisplay();
}

//============================================================================
//============================================================================
void GlutSys::onMouseScroll(int dir, int x, int y)
{
    //const char *func = "GlutSys::onMouseScroll() - ";

    //LogTrace("%s dir: %d, x: %d, y: %d", func, dir, x, y);

    double zoomAmt = .01;

    if (dir > 0)
    {
        // zoom in
        moveSys()->m_camera->zoomInc(-zoomAmt);
    }
    else
    {
        // zoom out
        moveSys()->m_camera->zoomInc(zoomAmt);
    }

    dataMouse()->x() = x;
    dataMouse()->y() = y;

    glutPostRedisplay();
}

//============================================================================
//============================================================================
void GlutSys::onMotion(int x, int y)
{
    float dx, dy;
    dx = x - dataMouse()->x();
    dy = y - dataMouse()->y();

    // MBTN Down not being recognized
    if (dataMouse()->btnsDown() & DataMouse::MOUSE_DOWN_MBTN || dataMouse()->btnsDown() & DataMouse::MOUSE_DOWN_LBTN)
    {
        // pan
        moveSys()->m_camera->panByPixel(-dx, dy);
    }

    dataMouse()->prevx() = dataMouse()->x();
    dataMouse()->prevy() = dataMouse()->y();
    dataMouse()->x() = x;
    dataMouse()->y() = y;
    onMouseMove(); // TODO: setup view tools


    glutPostRedisplay();

    /*
      // perspective transform.. todo: add this to the camera
    if (m_mouseBtns & 1) 
	{
        m_rotX += dy * 0.2;
        m_rotY += dx * 0.2;
    } 
	else if (m_mouseBtns & 4) 
	{
        m_transZ += dy * 0.01;
    }

    m_mouseOldX = x;
    m_mouseOldY = y;

    // set view matrix
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, m_transZ);
    glRotatef(m_rotX, 1.0, 0.0, 0.0);
    glRotatef(m_rotY, 0.0, 1.0, 0.0);
    glutPostRedisplay();
    */
}

//============================================================================
//============================================================================
void GlutSys::onMotionPassive(int x, int y)
{
    onMotion(x, y);
}

//============================================================================
//============================================================================
void GlutSys::resizeWindow(int width, int height)
{
    if (g_pSys) g_pSys->onResizeWindow(width, height);
}

//============================================================================
//============================================================================
void GlutSys::display()
{
    if (g_pSys) g_pSys->onDisplay();
}

//============================================================================
//============================================================================
void GlutSys::keyboard(unsigned char key, int x, int y)
{
    if (g_pSys) g_pSys->onKeyboard(key, x, y);
}

//============================================================================
//============================================================================
void GlutSys::specialKeys(int key, int x, int y)
{
    if (g_pSys) g_pSys->onKeyboardSpecial(key, x, y);
}

//============================================================================
//============================================================================
void GlutSys::mouse(int button, int state, int x, int y)
{
    if (!g_pSys) return;

    if (button == GlutSys::MOUSE_SCROLL_UP)
        g_pSys->onMouseScroll(1, x, y);
    else if (button == GlutSys::MOUSE_SCROLL_DOWN)
        g_pSys->onMouseScroll(-1, x, y);
    else
        g_pSys->onMouse(button, state, x, y);
}

//============================================================================
//============================================================================
void GlutSys::motion(int x, int y)
{
    if (g_pSys) g_pSys->onMotion(x, y);
}

//============================================================================
//============================================================================
void GlutSys::motionPassive(int x, int y)
{
    if (g_pSys) g_pSys->onMotionPassive(x, y);
}

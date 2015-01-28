#include "glutsys.h"

static GlutSys *g_pSys = NULL;

//============================================================================
//============================================================================
GlutSys::GlutSys()
{
	m_winW = 0;
	m_winH = 0;
	m_hanWin = 0;
	m_mouseOldX = 0;
	m_mouseOldY = 0;
	m_mouseBtns = 0;
	m_rotX = 0.0;
	m_rotY = 0.0;
	m_transZ = -3.0;

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

	m_moveSys.reset(new MoveSys(20.0f));
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
//============================================================================
bool GlutSys::init(QCoreApplication *app, int argc, const char** argv, int winWidth, int winHeight, PFilePaths pFilePaths)
{
	if (!m_globalInit) return false;

    destroy();

    m_app = app;

	m_winW = winWidth;
	m_winH = winHeight;
	m_filePaths = pFilePaths;
    if (m_filePaths.get() == NULL)
    {
        m_filePaths.reset(new FilePaths());
        m_filePaths->Init(argv[0]);
    }

    // initialize GLUT 
    glutInit(&argc, (char**)argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL | GLUT_DOUBLE );
    glutInitWindowPosition (glutGet(GLUT_SCREEN_WIDTH)/2 - m_winW/2, glutGet(GLUT_SCREEN_HEIGHT)/2 - m_winH/2);
    glutInitWindowSize(m_winW, m_winH);
    m_hanWin = glutCreateWindow("GIS Render");

    //glutHideWindow();

    // register GLUT callback functions
    glutReshapeFunc(resizeWindow);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

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
    m_moveSys->m_camera->onResize(winWidth, winHeight);

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
	//wglewInit();

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
#ifdef QT
//a.processEvents();
void GlutSys::run(QCoreApplication *app)
{
  m_app = app;
  glutMainLoop();
}
#else
void GlutSys::run()
{
	// could use a try catch here an throw and exception in Exit to exit
	glutMainLoop();
}
#endif

//============================================================================
//============================================================================
void GlutSys::exit()
{
    destroy();
    LogTrace("exiting...");
    ::exit(0);
}

//============================================================================
//============================================================================
void GlutSys::destroy()
{
	if (m_hanWin)
	{
		glutDestroyWindow(m_hanWin);
	}

	m_winW = 0;
	m_winH = 0;
	m_hanWin = 0;
	m_mouseOldX = 0;
	m_mouseOldY = 0;
	m_mouseBtns = 0;
	m_rotX = 0.0;
	m_rotY = 0.0;
	m_transZ = -3.0;
}

//============================================================================
//============================================================================
void GlutSys::setCamera()
{
	if (m_moveSys && m_setCamera)
	{
        m_moveSys->m_camera->setView();
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
   m_winW = width;
   m_winH = height;
   m_moveSys->m_camera->onResize(width, height);
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
			if (m_moveSys) m_moveSys->MoveForward();
			break;
		case 's':
			if (m_moveSys) m_moveSys->MoveBackward();
			break;
		case 'a':
			if (m_moveSys) m_moveSys->StrafeLeft();
			break;
		case 'd':
			if (m_moveSys) m_moveSys->StrafeRight();
			break;
		case 'r':
			if (m_moveSys) m_moveSys->MoveUp();
			break;
		case 'f':
			if (m_moveSys) m_moveSys->MoveDown();
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
			if (m_moveSys) m_moveSys->LookLeft();
			break;
		case GLUT_KEY_RIGHT:
			if (m_moveSys) m_moveSys->LookRight();
			break;
		case GLUT_KEY_UP:
			if (m_moveSys) m_moveSys->LookUp();
			break;
		case GLUT_KEY_DOWN:
			if (m_moveSys) m_moveSys->LookDown();
			break;
	}
}

//============================================================================
//============================================================================
void GlutSys::onMouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) 
	{
        m_mouseBtns |= 1<<button;
    } 
	else if (state == GLUT_UP) 
	{
        m_mouseBtns = 0;
    }

    m_mouseOldX = x;
    m_mouseOldY = y;

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
        m_moveSys->m_camera->zoomInc(-zoomAmt);
    }
    else
    {
        // zoom out
        m_moveSys->m_camera->zoomInc(zoomAmt);
    }

    m_mouseOldX = x;
    m_mouseOldY = y;

    glutPostRedisplay();
}

//============================================================================
//============================================================================
void GlutSys::onMotion(int x, int y)
{
    float dx, dy;
    dx = x - m_mouseOldX;
    dy = y - m_mouseOldY;

    if (m_mouseBtns & GlutSys::MOUSE_MBTN_DOWN)
    {
        // pan
        m_moveSys->m_camera->panByPixel(-dx, dy);
    }

    m_mouseOldX = x;
    m_mouseOldY = y;

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

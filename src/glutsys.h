#ifndef GLUTSYS_H
#define GLUTSYS_H

#include "sysdefs.h"
#include "MoveSys.h"
#include "filepaths.h"
#include "drawdata.h"

#ifdef QT
#include <QtCore/QCoreApplication>
#endif

class GlutSys
{
public:
    enum EMouseBtn
    {
        MOUSE_LEFT_BUTTON = 0,
        MOUSE_MIDDLE_BUTTON = 1,
        MOUSE_RIGHT_BUTTON = 2,
        MOUSE_SCROLL_UP = 3,
        MOUSE_SCROLL_DOWN = 4
    };

    enum EMouseBtnDown
    {
        MOUSE_LBTN_DOWN = 1,
        MOUSE_MBTN_DOWN = 2,
        MOUSE_RBTN_DOWN = 4
    };

public:
	GlutSys();
	virtual ~GlutSys();
    virtual bool init(QCoreApplication *app, int argc, const char** argv, int winWidth, int winHeight, PFilePaths pPaths = PFilePaths());

    virtual void run();

    virtual void exit();

    virtual string getGLInfo();

protected:

    virtual void destroy();
    virtual void displayData() {};
    virtual void setCamera();

    virtual void onResizeWindow(int width, int height);
    virtual void onPreDisplay() {};
    virtual void onDisplay();
    virtual void onKeyboard(unsigned char key, int x, int y);
    virtual void onKeyboardSpecial(int key, int x, int y);
    virtual void onMouse(int button, int state, int x, int y);
    virtual void onMouseScroll(int dir, int x, int y);
    virtual void onMotion(int x, int y);

    static void resizeWindow(int width, int height);
    static void display();
    static void keyboard(unsigned char key, int x, int y);
    static void specialKeys(int key, int x, int y);
    static void mouse(int button, int state, int x, int y);
    static void motion(int x, int y);

protected:
	int m_hanWin;
	int m_winW;
	int m_winH;
	bool m_globalInit;

	// mouse controls
	int m_mouseOldX;
	int m_mouseOldY;
	int m_mouseBtns;
    bool m_mouseFirstDown;
	float m_rotX;
	float m_rotY;
	float m_transZ;

	PMoveSys m_moveSys;
	bool m_setCamera;

	PDrawData m_dd;

	PFilePaths m_filePaths;

#ifdef QT
    QCoreApplication *m_app;
#endif
};

#endif

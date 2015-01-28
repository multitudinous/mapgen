#include "MoveSys.h"
#include "utlmath.h"

MoveSys::MoveSys(float velMove, float velView)
{
	m_vel = velMove;
	m_velView = velView;
	m_maxXRot = 90;
	m_elapsedTime = 1;
	m_camera.reset(new Camera());
}

void MoveSys::MoveForward()
{
	MoveForward(m_vel);
}

void MoveSys::MoveBackward()
{
	MoveBackward(m_vel);
}

void MoveSys::StrafeLeft()
{
	StrafeLeft(m_vel);
}

void MoveSys::StrafeRight()
{
	StrafeRight(m_vel);
}

void MoveSys::MoveUp()
{
	MoveUp(m_vel);
}

void MoveSys::MoveDown()
{
	MoveDown(m_vel);
}

void MoveSys::MoveForward(float mv)
{
    m_camera->_pos.x -= (float)sin(m_camera->_rot.y*piover180) * mv * m_elapsedTime;
    m_camera->_pos.z -= (float)cos(m_camera->_rot.y*piover180) * mv * m_elapsedTime;
}

void MoveSys::MoveBackward(float mv)
{
    m_camera->_pos.x += (float)sin(m_camera->_rot.y*piover180) * mv * m_elapsedTime;
    m_camera->_pos.z += (float)cos(m_camera->_rot.y*piover180) * mv * m_elapsedTime;
}

void MoveSys::StrafeLeft(float mv)
{
    m_camera->_pos.x -= (float)sin(m_camera->_rot.y*piover180 + piover2) * mv * m_elapsedTime;
    m_camera->_pos.z -= (float)cos(m_camera->_rot.y*piover180 + piover2) * mv * m_elapsedTime;
}

void MoveSys::StrafeRight(float mv)
{
    m_camera->_pos.x += (float)sin(m_camera->_rot.y*piover180 + piover2) * mv * m_elapsedTime;
    m_camera->_pos.z += (float)cos(m_camera->_rot.y*piover180 + piover2) * mv * m_elapsedTime;
}

void MoveSys::MoveUp(float mv)
{
    m_camera->_pos.y += mv * m_elapsedTime;
}

void MoveSys::MoveDown(float mv)
{
    m_camera->_pos.y -= mv * m_elapsedTime;
}

void MoveSys::LookLeft(float fMpDis)
{
    m_camera->_rot.y += fMpDis * m_velView * m_elapsedTime;
}

void MoveSys::LookRight(float fMpDis)
{
    m_camera->_rot.y -= fMpDis * m_velView * m_elapsedTime;
}

void MoveSys::LookUp(float fMpDis)
{
    m_camera->_rot.x -= fMpDis * m_velView * m_elapsedTime;
	RestrainLookUpDown();
}
void MoveSys::LookDown(float fMpDis)
{
    m_camera->_rot.x += fMpDis * m_velView * m_elapsedTime;
	RestrainLookUpDown();
}

void MoveSys::RestrainLookUpDown()
{
    if (m_camera->_rot.x > m_maxXRot)
	{
        m_camera->_rot.x = m_maxXRot;
	}
    else if (m_camera->_rot.x < -m_maxXRot)
	{
        m_camera->_rot.x = -m_maxXRot;
	}
}

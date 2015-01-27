#ifndef MOVESYS_H
#define MOVESYS_H
#include "camera.h"


class MoveSys
{
public:
	MoveSys(float velMove=2.0f, float velView=1.0f);

	virtual void MoveForward();
	virtual void MoveBackward();
	virtual void StrafeLeft();
	virtual void StrafeRight();
	virtual void MoveUp();
	virtual void MoveDown();

	virtual void MoveForward(float mv);
	virtual void MoveBackward(float mv);
	virtual void StrafeLeft(float mv);
	virtual void StrafeRight(float mv);
	virtual void MoveUp(float mv);
	virtual void MoveDown(float mv);

	virtual void LookLeft(float fMpDis=1.0f);
	virtual void LookRight(float fMpDis=1.0f);
	virtual void LookUp(float fMpDis=1.0f);
	virtual void LookDown(float fMpDis=1.0f);

    virtual vec3f GetPos() { return m_camera->_pos; }
    virtual void  SetPos(const vec3f &pos) { m_camera->_pos = pos; }
    virtual vec3f GetView() { return m_camera->_rot; }
    virtual void  SetetView(const vec3f &view) { m_camera->_rot = view; }

protected:
	virtual void RestrainLookUpDown();
public:
	PCamera m_camera;
	float m_vel;
	float m_velView;
	float m_maxXRot;
	float m_elapsedTime;
};

typedef shared_ptr<MoveSys> PMoveSys;

#endif

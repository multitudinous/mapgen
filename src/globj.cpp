#include "globj.h"

//============================================================================
//============================================================================
GlObj::GlObj(bool draw) :
_draw(draw)
{

}

//============================================================================
//============================================================================
GlObj::GlObj(const GlObj &obj)
{
    _name = obj._name;
    _draw = obj._draw;
    for (unsigned int i=0; i<obj._childList.size(); i++)
    {
        GlObj *obj = _childList[i]->clone();
        if (obj)
        {
            _childList.push_back(PGlObj(obj));
        }
    }
}

//============================================================================
//============================================================================
void GlObj::draw(DrawData *pdd)
{
    if (!_draw) return;

    for (unsigned int i=0; i<_childList.size(); i++)
    {
        _childList[i]->draw(pdd);
    }
}

//============================================================================
// todo: if computing a bounding box based on whats displayed,
// then need to early exit here if !_draw 
//============================================================================
void GlObj::computeBox(DrawData *pdd, box3d *pbox)
{
    for (unsigned int i=0; i<_childList.size(); i++)
    {
        _childList[i]->computeBox(pdd, pbox);
    }
}

//============================================================================
//============================================================================
void GlObj::runSel(DrawData *pdd)
{
    if (!_draw) return;

    for (unsigned int i = 0; i<_childList.size(); i++)
    {
        _childList[i]->runSel(pdd);
    }
}

//============================================================================
//============================================================================
void GlObj::insertBack(shared_ptr<GlObj> obj)
{
    _childList.push_back(obj);
}

//============================================================================
//============================================================================
void GlObj::insertFront(shared_ptr<GlObj> obj)
{
    _childList.insert(_childList.begin(), obj);

    // insert at loc
   // _childList.insert( _childList.begin() + loc, obj)
}

//============================================================================
//============================================================================
void GlObj::clearChildren()
{
   _childList.clear();
}

//============================================================================
//============================================================================
void GlObj::init(DrawData *pdd)
{
    initAttr(pdd);
    initLabels(pdd);
}

//============================================================================
//============================================================================
void GlObj::initAttr(DrawData *pdd)
{
    for (unsigned int i = 0; i<_childList.size(); i++)
    {
        _childList[i]->initAttr(pdd);
    }
}

//============================================================================
//============================================================================
void GlObj::initLabels(DrawData *pdd)
{
    for (unsigned int i=0; i<_childList.size(); i++)
    {
        _childList[i]->initLabels(pdd);
    }
}


//============================================================================
//============================================================================
void GlObj::scale(double x, double y, double z)
{
    for (unsigned int i=0; i<_childList.size(); i++)
    {
        _childList[i]->scale(x, y, z);
    }
}

//============================================================================
//============================================================================
void GlObj::translate(double x, double y, double z)
{
    for (unsigned int i=0; i<_childList.size(); i++)
    {
        _childList[i]->translate(x, y, z);
    }
}

//============================================================================
//============================================================================
void GlObj::moveFromTo(const vec3d &fr, const vec3d &to)
{
    for (unsigned int i=0; i<_childList.size(); i++)
    {
        _childList[i]->moveFromTo(fr, to);
    }
}

//============================================================================
//============================================================================
GlObj* GlObj::clone() const
{
    return new GlObj(*this);
}

//============================================================================
//============================================================================
void GlObj::shallowCopy(GlObj *pobj)
{
    if (!pobj) return;

    // just copy pointers for shallow
    _childList = pobj->_childList;
}

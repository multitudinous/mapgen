#ifndef GLOBJ_H
#define GLOBJ_H

#include "drawdata.h"
#include "box3.h"

class GlObj
{
public:
    GlObj();
    GlObj(const GlObj &obj);

    virtual void draw(DrawData *pdd);

    virtual void computeBox(DrawData *pdd, box3d *pbox);

    virtual void insertBack(shared_ptr<GlObj> obj);
    virtual void insertFront(shared_ptr<GlObj> obj);
    virtual void clearChildren();

    virtual void setDrawAttr(PDrawAttr ) {}

    virtual void initLabels(DrawData *pdd);

    virtual void scale(double x, double y, double z=1.0);
    virtual void translate(double x, double y, double z=0.0);
    virtual void moveFromTo(const vec3d &fr, const vec3d &to);

    virtual GlObj* clone() const;
    virtual void shallowCopy(GlObj *pobj);

    void setName(const std::string &name) { _name = name; }
    std::string getName() { return _name; }

   vector< shared_ptr<GlObj> >& childList() { return _childList; }

public:
    std::string _name;

protected:
    vector< shared_ptr<GlObj> > _childList;
};

typedef shared_ptr<GlObj> PGlObj;
typedef vector< shared_ptr<GlObj> > GlObjList;

#endif

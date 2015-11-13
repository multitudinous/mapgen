#ifndef GLOBJ_H
#define GLOBJ_H

#include "drawdata.h"
#include "box3.h"

class DrawData;

class GlObj
{
public:
    GlObj(bool draw=true);
    GlObj(const GlObj &obj);

    virtual void draw(DrawData *pdd);

    virtual void computeBox(DrawData *pdd, box3d *pbox);

    virtual void runSel(DrawData *pdd);

    virtual void insertBack(shared_ptr<GlObj> obj);
    virtual void insertFront(shared_ptr<GlObj> obj);
    virtual void clearChildren();
    vector< shared_ptr<GlObj> >& childList() { return _childList; }

    virtual void setDrawAttr(PDrawAttr ) {}

    virtual void init(DrawData *pdd);
    virtual void initAttr(DrawData *pdd);
    virtual void initLabels(DrawData *pdd);

    virtual void scale(double x, double y, double z=1.0);
    virtual void translate(double x, double y, double z=0.0);
    virtual void moveFromTo(const vec3d &fr, const vec3d &to);

    virtual GlObj* clone() const;
    virtual void shallowCopy(GlObj *pobj);

    void setName(const std::string &name) { _name = name; }
    std::string getName() { return _name; }

    void setDraw(bool d) { _draw = d;  }
    bool getDraw() { return _draw;  }

public:
    std::string _name;
    bool _draw;

protected:
    vector<shared_ptr<GlObj>> _childList;
};

typedef shared_ptr<GlObj> PGlObj;
typedef vector< shared_ptr<GlObj> > GlObjList;

#endif

#ifndef DATASEL_H
#define DATASEL_H

#include "vector3.h"
#include "sysdefs.h"

class GlObj;

class DataSel
{
public:
    DataSel();

    void init(int mouseX, int mouseY, bool windows=true); // windows means mouse coords, y starts at the top (0)

    void setRay(const vec3d &sray, const vec3d &eray, double depth=99999999);
    const vec3d& sray() const;
    const vec3d& eray() const;
    const vec3d& nray() const;

    int mx() const;
    int my() const;

    GlObj *sel();
    bool setSel(GlObj *sel, bool useDepth = false, double depth = 0); // depth only needed for 3d selections

protected:
    int _mx;
    int _my; // in glscreen coords, so y == 0 at the bottom
    vec3d _sray; // starting ray point
    vec3d _eray; // ending ray point
    vec3d _nray; // normalized ray

    double _depth;

    GlObj *_sel;
};

typedef shared_ptr<DataSel> PDataSel;

#endif
#include "gldraw.h"
#include "vector3.h"
#include "geopoly.h"

#if WIN32
#define GLU_FUNC_CAST (void (__stdcall *)(void))
#else
#define GLU_FUNC_CAST (void (*)(void))
#endif
struct SVertInfoVN3
{
	double adV[3];
    double adN[3];
};

//=======================================================================
//=======================================================================
/*
void GlDraw::drawPolyConcaveVN(GLUtesselator *hTess, const std::vector<vec3d> &ptlist, const Vector3 &n) 
{
  if (!hTess)
  {
    assert(0);
    return;
  }

  if (!ptlist.size())
  {
    assert(0);
    return;
  }
 

  // Set up the begin callback to call glBegin directly; there's no need    
  // to do anything special in this case.  If you had a flat-shaded         
  // polygon, you could point to a special function and call glColor here. 
  gluTessProperty(hTess, GLU_TESS_BOUNDARY_ONLY, GL_FALSE);
  gluTessCallback(hTess, GLU_BEGIN, GLU_FUNC_CAST vGluCallbackBeginPoly); 
  gluTessCallback(hTess, GLU_VERTEX,GLU_FUNC_CAST vGluCallbackTessalateVertVN3F); 
  gluTessCallback(hTess, GLU_END, glEnd); 
  gluBeginPolygon(hTess); 
  gluNextContour(hTess, GLU_EXTERIOR);

  SVertInfoVN3 v;
  for (i=0; i<ptlist.size(); i++)
  {
	vec3d p = ptlist[i];
    ptlist.vCursorForward();

	v.adV[0] = p.x;
	v.adV[1] = p.y;
	v.adV[2] = p.z;
	v.adN[0] = n.x;
	v.adN[1] = n.y;
	v.adN[2] = n.z;
    gluTessVertex(hTess, v.adV, &pstVert[i]);   
  }

  gluEndPolygon(hTess); 


  // How to draw a whole
  // Draw the first 10 vertices as an outside contour
  //for (i = 0; i < 10; i++) 
    //gluTessVertex(tobj, &vertices[i].vertex[0], &vertices[i]); 
  //gluNextContour(tobj, GLU_INTERIOR); 
  // Draw the next 10 vertices as an inside contour
  //for (i = 10; i < 20; i++) 
    //gluTessVertex(tobj, &vertices[i].vertex[0], &vertices[i]); 
  //gluEndPolygon(tobj); 
} 
*/

//============================================================================
//============================================================================
void Gldraw::drawLineLoop(const ListPt2d &pts)
{
    glBegin(GL_LINE_LOOP);

    for (int i = 0; i<pts.size(); i++)
    {
        Point2d pt = pts.at(i);
        glVertex2d(pt.dX, pt.dY);
    }

    glEnd();
}

//============================================================================
//============================================================================
void Gldraw::drawMask(DrawData *pdd, const GlObjList* maskList, const ListPt2d &regionPtList)
{
    if (!maskList) return;
    if (maskList->size() <= 0) return;
    if (regionPtList.size() < 3) return;

    for (unsigned int i = 0; i<maskList->size(); i++)
    {
        PGlObj colorObj = (*maskList)[i];
        drawMask(pdd, colorObj.get(), regionPtList);
    }
}

//============================================================================
//============================================================================
void Gldraw::drawMask(DrawData *pdd, GlObj *maskColoring, const ListPt2d &regionPtList)
{
    if (!maskColoring) return;
    if (regionPtList.size() < 3) return;

    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);

    glEnable(GL_STENCIL_TEST);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    glStencilFunc(GL_ALWAYS, 1, -1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    Gldraw::drawPolyConcaveVN(pdd->m_hGluTess, regionPtList);
    glStencilFunc(GL_EQUAL, 1, -1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    maskColoring->draw(pdd);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_STENCIL_TEST);
}

//============================================================================
//============================================================================
void Gldraw::drawQuad(double l, double r, double t, double b, double z)
{
    drawQuad(GL_QUADS, l, r, t, b, z);
}

//============================================================================
//============================================================================
void Gldraw::drawQuadLine(double l, double r, double t, double b, double z)
{
    drawQuad(GL_LINE_LOOP, l, r, t, b, z);
}

//============================================================================
//============================================================================
void Gldraw::drawQuad(GLenum mode, double l, double r, double t, double b, double z)
{
    glBegin(mode);

    glTexCoord2f(0, 0);
    glVertex3d(l, b, z);

    glTexCoord2f(1, 0);
    glVertex3d(r, b, z);

    glTexCoord2f(1, 1);
    glVertex3d(r, t, z);

    glTexCoord2f(0, 1);
    glVertex3d(l, t, z);

    glEnd();
}

//============================================================================
//============================================================================
void Gldraw::drawPolyConcaveVN(GLUtesselator *hTess, const GeoPoly *poly)
{
    drawPolyConcaveVN(hTess, poly->getPts());
}

//============================================================================
//============================================================================
void Gldraw::drawPolyConcaveVN(GLUtesselator *hTess, const ListPt2d &ptlist)
{
	// Set up the begin callback to call glBegin directly; there's no need    
  // to do anything special in this case.  If you had a flat-shaded         
  // polygon, you could point to a special function and call glColor here. 
  gluTessProperty(hTess, GLU_TESS_BOUNDARY_ONLY, GL_FALSE);
  gluTessCallback(hTess, GLU_BEGIN, GLU_FUNC_CAST gluCallbackBeginPoly); 
  gluTessCallback(hTess, GLU_VERTEX,GLU_FUNC_CAST gluCallbackTessalateVertVN3F); 
  gluTessCallback(hTess, GLU_END, glEnd); 
  gluBeginPolygon(hTess); 
  gluNextContour(hTess, GLU_EXTERIOR);

  SVertInfoVN3 *v = new SVertInfoVN3[ptlist.size()];
  for (size_t i=0; i<ptlist.size(); i++)
  {
    Point2d p = ptlist[i];

	v[i].adV[0] = p.dX;
	v[i].adV[1] = p.dY;
	v[i].adV[2] = 0;
	v[i].adN[0] = 0;
	v[i].adN[1] = 0;
	v[i].adN[2] = 1;
    gluTessVertex(hTess, v[i].adV, &v[i]);   
  }

  gluEndPolygon(hTess); 

  delete [] v;
}


//=======================================================================
//=======================================================================
void Gldraw::gluCallbackTessalateVertVN3F(void *pvVertInfoVN3) 
{ 
  SVertInfoVN3 *pstVertInfo = (SVertInfoVN3 *)pvVertInfoVN3;

  glNormal3dv(pstVertInfo->adN); 
  //glTexCoord2fv(info->texCoord); 
  glVertex3dv(pstVertInfo->adV);
} 

//=======================================================================
//=======================================================================
void Gldraw::gluCallbackBeginPoly(GLenum eWhich) 
{
  switch (eWhich)
  {
    case GL_LINES:
      //TRACE0("Lines Found\n");
      break;
    case GL_LINE_LOOP:
      //TRACE0("Line Loop Found\n");
      break;
    case GL_TRIANGLE_FAN:
      //TRACE0("Triangle Fan Found\n");
      break;
    case GL_TRIANGLE_STRIP:
      //TRACE0("Triangle Strip Found\n");
      break;
  };
  glBegin(eWhich);
}



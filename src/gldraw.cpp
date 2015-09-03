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
void Gldraw::drawLine(double x1, double y1, double x2, double y2, //coordinates of the line
    float w, //width/thickness of the line in pixel
    float Cr, float Cg, float Cb) //RGB color components
{
    double t; double R; double f = w - static_cast<int>(w);
    float A;

    float Br=.5, Bg=1, Bb=1; // // THIS WAS A FUNCTION PARAM, // color of background when alphablend=false,
    bool alphablend = true; // THIS WAS A FUNCTION PARAM, but we always want blending

    if (alphablend)
        A = Br;
    else
        A = 1.0f;

    //determine parameters t,R
    /*   */if (w >= 0.0 && w<1.0) {
        t = 0.05; R = 0.48 + 0.32*f;
        if (!alphablend) {
            Cr += 0.88*(1 - f);
            Cg += 0.88*(1 - f);
            Cb += 0.88*(1 - f);
            if (Cr>1.0) Cr = 1.0;
            if (Cg>1.0) Cg = 1.0;
            if (Cb>1.0) Cb = 1.0;
        }
        else {
            A *= f;
        }
    }
    else if (w >= 1.0 && w<2.0) {
        t = 0.05 + f*0.33; R = 0.768 + 0.312*f;
    }
    else if (w >= 2.0 && w<3.0){
        t = 0.38 + f*0.58; R = 1.08;
    }
    else if (w >= 3.0 && w<4.0){
        t = 0.96 + f*0.48; R = 1.08;
    }
    else if (w >= 4.0 && w<5.0){
        t = 1.44 + f*0.46; R = 1.08;
    }
    else if (w >= 5.0 && w<6.0){
        t = 1.9 + f*0.6; R = 1.08;
    }
    else if (w >= 6.0){
        double ff = w - 6.0;
        t = 2.5 + ff*0.50; R = 1.08;
    }
    //printf( "w=%f, f=%f, C=%.4f\n", w,f,C);

    //determine angle of the line to horizontal
    double tx = 0, ty = 0; //core thinkness of a line
    double Rx = 0, Ry = 0; //fading edge of a line
    double cx = 0, cy = 0; //cap of a line
    double ALW = 0.01;
    double dx = x2 - x1;
    double dy = y2 - y1;
    if (abs(dx) < ALW) {
        //vertical
        tx = t; ty = 0;
        Rx = R; Ry = 0;
        if (w>0.0 && w<1.0)
            tx *= 8;
        else if (w == 1.0)
            tx *= 10;
    }
    else if (abs(dy) < ALW) {
        //horizontal
        tx = 0; ty = t;
        Rx = 0; Ry = R;
        if (w>0.0 && w<1.0)
            ty *= 8;
        else if (w == 1.0)
            ty *= 10;
    }
    else {
        if (w < 3) { //approximate to make things even faster
            double m = dy / dx;
            //and calculate tx,ty,Rx,Ry
            if (m>-0.4142 && m <= 0.4142) {
                // -22.5< angle <= 22.5, approximate to 0 (degree)
                tx = t*0.1; ty = t;
                Rx = R*0.6; Ry = R;
            }
            else if (m>0.4142 && m <= 2.4142) {
                // 22.5< angle <= 67.5, approximate to 45 (degree)
                tx = t*-0.7071; ty = t*0.7071;
                Rx = R*-0.7071; Ry = R*0.7071;
            }
            else if (m>2.4142 || m <= -2.4142) {
                // 67.5 < angle <=112.5, approximate to 90 (degree)
                tx = t; ty = t*0.1;
                Rx = R; Ry = R*0.6;
            }
            else if (m>-2.4142 && m<-0.4142) {
                // 112.5 < angle < 157.5, approximate to 135 (degree)
                tx = t*0.7071; ty = t*0.7071;
                Rx = R*0.7071; Ry = R*0.7071;
            }
            else {
                // error in determining angle
                //printf( "error in determining angle: m=%.4f\n",m);
            }
        }
        else { //calculate to exact
            dx = y1 - y2;
            dy = x2 - x1;
            double L = sqrt(dx*dx + dy*dy);
            dx /= L;
            dy /= L;
            cx = -0.6*dy; cy = 0.6*dx;
            tx = t*dx; ty = t*dy;
            Rx = R*dx; Ry = R*dy;
        }
    }

    //draw the line by triangle strip
    glBegin(GL_TRIANGLE_STRIP);
    if (!alphablend) { glColor3f(Br, Bg, Bb); }
    else { glColor4f(Cr, Cg, Cb, 0); }
    glVertex2f(x1 - tx - Rx, y1 - ty - Ry); //fading edge
    glVertex2f(x2 - tx - Rx, y2 - ty - Ry);

    if (!alphablend) { glColor3f(Cr, Cg, Cb); }
    else { glColor4f(Cr, Cg, Cb, A); }
    glVertex2f(x1 - tx, y1 - ty); //core
    glVertex2f(x2 - tx, y2 - ty);
    glVertex2f(x1 + tx, y1 + ty);
    glVertex2f(x2 + tx, y2 + ty);

    if ((abs(dx) < ALW || abs(dy) < ALW) && w <= 1.0) {
        //printf("skipped one fading edge\n");
    }
    else {
        if (!alphablend) { glColor3f(Br, Bg, Bb); }
        else { glColor4f(Cr, Cg, Cb, 0); }
        glVertex2f(x1 + tx + Rx, y1 + ty + Ry); //fading edge
        glVertex2f(x2 + tx + Rx, y2 + ty + Ry);
    }
    glEnd();

    //cap
    if (w < 3) {
        //do not draw cap
    }
    else {
        //draw cap
        glBegin(GL_TRIANGLE_STRIP);
        if (!alphablend) { glColor3f(Br, Bg, Bb); }
        else { glColor4f(Cr, Cg, Cb, 0); }
        glVertex2f(x1 - Rx + cx, y1 - Ry + cy);
        glVertex2f(x1 + Rx + cx, y1 + Ry + cy);
        glColor3f(Cr, Cg, Cb);
        glVertex2f(x1 - tx - Rx, y1 - ty - Ry);
        glVertex2f(x1 + tx + Rx, y1 + ty + Ry);
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
        if (!alphablend) { glColor3f(Br, Bg, Bb); }
        else { glColor4f(Cr, Cg, Cb, 0); }
        glVertex2f(x2 - Rx - cx, y2 - Ry - cy);
        glVertex2f(x2 + Rx - cx, y2 + Ry - cy);
        glColor3f(Cr, Cg, Cb);
        glVertex2f(x2 - tx - Rx, y2 - ty - Ry);
        glVertex2f(x2 + tx + Rx, y2 + ty + Ry);
        glEnd();
    }
}

//============================================================================
//============================================================================
void Gldraw::drawLineLoop(const ListPt2d &pts, float w, float r, float g, float b)
{
    const Point2d *pt1=NULL, *pt2=NULL;
    for (int i = 0; i<pts.size(); i++)
    {
        pt2 = &pts.at(i);
        if (pt1 == NULL)
        {
            pt1 = pt2;
            continue;
        }

        drawLine(pt1->dX, pt1->dY, pt2->dX, pt2->dY, w, r, g, b);
        pt1 = pt2;
    }

    if (pts.size() <= 2) return;

    pt2 = &pts.at(0); // first point
    drawLine(pt1->dX, pt1->dY, pt2->dX, pt2->dY, w, r, g, b); // draw last point to first point line

}
/*
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
*/

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



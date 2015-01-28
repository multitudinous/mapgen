#include "gltext.h"

#include <FTGL/ftgl.h>
//#include "FTInternals.h"

//============================================================================
//============================================================================
GlText::GlText()
{
}

//============================================================================
//============================================================================
GlText::~GlText()
{
}

//============================================================================
//============================================================================
bool GlText::initFontPoly(const char *name, int size)
{
	_font.reset(new FTPolygonFont(name));
	_font->FaceSize(size);

	return true;
}

//============================================================================
//============================================================================
bool GlText::initFontTexture(const char *name, int size)
{
	_font.reset(new FTTextureFont(name));
	_font->FaceSize(size);

	return true;
}

//============================================================================
//============================================================================
bool GlText::initFontExtrude(const char *name, int size, float depth)
{
	_font.reset(new FTExtrudeFont(name));
	_font->FaceSize(size);
	_font->Depth(depth);
	//_font->Outset(0, 5);
	//_font->Outset(0, 3);

	/*
Then you can use a different colour for the two render modes:
glColor3f(1.0, 1.0, 1.0); // White inside
font->Render("Hello", FTPoint(), FTPoint(), FTGL::RENDER_FRONT);
glColor3f(1.0, 0.0, 0.0); // Red outline
font->Render("Hello", FTPoint(), FTPoint(), FTGL::RENDER_SIDE);
*/

	return true;
}

//============================================================================
//============================================================================
bool GlText::initFontOutline(const char *name, int size)
{
	_font.reset(new FTOutlineFont(name));
	_font->FaceSize(size);

	return true;
}

//============================================================================
//============================================================================
void GlText::renderC(const char *str, const Point2d &c, double z)
{
   box3d box = getBBoxd(c, str);
   renderBL(str, box.GetBtmLeft().get2d(), z);
   //renderTL(str, c, z);
}

//============================================================================
// render text positioned, bl = bottom left position of the texts bounds
//============================================================================
void GlText::renderBL(const char *str, const Point2d &tl, double z)
{
    glPushMatrix();
    glTranslated(tl.dX, tl.dY, z);
    render(str);
    glPopMatrix();
}

//============================================================================
//============================================================================
void GlText::render(const char *str)
{
	render(str, FTGL::RENDER_ALL);
}

//============================================================================
//============================================================================
void GlText::render(const char *str, int renderMode)
{
	if (!_font) return;

	_font->Render(str, -1, FTPoint(), FTPoint(), renderMode);
}

//============================================================================
//============================================================================
box3f GlText::getBBoxf(const char *str)
{
	box3d boxd = GlText::getBBoxd(str);

	box3f boxf;
	boxf.UpdateBox((float)boxd.vmin.x, (float)boxd.vmin.y, (float)boxd.vmin.z);
	boxf.UpdateBox((float)boxd.vmax.x, (float)boxd.vmax.y, (float)boxd.vmax.z);
	return boxf;
}

//============================================================================
//============================================================================
box3d GlText::getBBoxd(const char *str)
{
	if (!_font) return box3d();

	FTBBox ftbox = _font->BBox(str);

	box3d box;
	box.UpdateBox(ftbox.Lower().X(), ftbox.Lower().Y(), ftbox.Lower().Z());
	box.UpdateBox(ftbox.Upper().X(), ftbox.Upper().Y(), ftbox.Upper().Z());
	return box;
}

//============================================================================
//============================================================================
box3d GlText::getBBoxd(const Point2d &center, const char *str)
{
    box3d box = getBBoxd(str);
    box3d boxnew;

    vec3d tl, br;
    tl.x = center.dX - box.GetSize().x/2.0;
    tl.y = center.dY + box.GetSize().y/2.0;
    tl.z = box.vmin.z;

    br.x = tl.x + box.GetSize().x;
    br.y = tl.y - box.GetSize().y;
    br.z = box.vmax.z;

    boxnew.UpdateBox(tl);
    boxnew.UpdateBox(br);

    return boxnew;
}


// jitter example for outlined expanded text
/*
//============================================================================
//============================================================================
#include "FTGL/ftgl.h"
void GisSys::DrawTestLabelCenter()
{
    const char *text = "My Field Boundary";
    box3d box = m_textLabel.getBBoxd(text);

    //LogTrace("text box: min: %.2f, %.2f, %.2f, max: %.2f, %.2f, %.2f", box.vmin.x, box.vmin.y, box.vmin.z, box.vmax.x, box.vmax.y, box.vmax.z);


    // center of window
    double x = m_moveSys->m_camera->_ext.l + (m_moveSys->m_camera->_ext.r - m_moveSys->m_camera->_ext.l)/2.0;
    double y = m_moveSys->m_camera->_ext.b + (m_moveSys->m_camera->_ext.t - m_moveSys->m_camera->_ext.b)/2.0;
    double z = 1;
    double jitter = .01;

    // now center text
    x -= box.GetSize().x/2;
    y += box.GetSize().y/2;

    // regular
    //glPushMatrix();
    //glColor4f(1.0, 0.0, 0.0, 1.0);
    //glTranslated(x,y,z);
    //m_textLabel.render(text);
    //glPopMatrix();


    // with outline
    for (int i=0; i<4; i++)
    {
        double jx = x + jitter;
        double jy = y + jitter;

        glPushMatrix();
        glColor3f(1,0,0);
        glTranslated(jx,jy,z);
        //m_textLabel.render(text);

        //Then you can use a different colour for the two render modes:
        glColor4f(1.0, 1.0, 1.0, .75); // White inside
        m_textLabel.render(text, FTGL::RENDER_FRONT);
        glColor4f(1.0, 0.0, 0.0, .75); // Red outline
        m_textLabel.render(text, FTGL::RENDER_SIDE);
        glPopMatrix();

        jitter += jitter;
        jitter *= -1;
    }


}
*/

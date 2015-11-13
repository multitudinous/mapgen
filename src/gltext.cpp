#include "gltext.h"
#include "sysdefs.h"

#include <FTGL/ftgl.h>

//============================================================================
//============================================================================
GlText::GlText() : 
_layout(new FTSimpleLayout())
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
    _layout->SetFont(_font.get());

	return true;
}

//============================================================================
//============================================================================
bool GlText::initFontTexture(const char *name, int size)
{
	_font.reset(new FTTextureFont(name));
	_font->FaceSize(size);
    _layout->SetFont(_font.get());

	return true;
}

//============================================================================
//============================================================================
bool GlText::initFontExtrude(const char *name, int size, float depth)
{
	_font.reset(new FTExtrudeFont(name));
	_font->FaceSize(size);
	_font->Depth(depth);
    _layout->SetFont(_font.get());

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
    _layout->SetFont(_font.get());

	return true;
}

//============================================================================
//============================================================================
void GlText::renderC(const char *str, const Point2d &c, double z)
{
   box3d box = getBBoxd(c, str);
   renderBL(str, box.getBtmLeft().get2d(), z);
   //renderTL(str, c, z);
}

//============================================================================
// render text positioned, bl = bottom left position of the texts bounds
//============================================================================
void GlText::renderBL(const char *str, const Point2d &bl, double z)
{
    glPushMatrix();
    glTranslated(bl.dX, bl.dY, z);
    render(str);
    glPopMatrix();
}

//============================================================================
// first align the text within the text box and then render it
//============================================================================
void GlText::render(const char *str, const Extents &textbox, Define::AlignH ah, Define::AlignV av, double z)
{
    box3d box = getBBoxd(str, textbox, ah, av, z);
    renderBL(str, box.getBtmLeft().get2d(), z);
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
void GlText::renderLayout(const char *str, const Point2d &bl, int align)
{
    glPushMatrix();
    glTranslated(bl.dX, bl.dY, 1);

    _layout->SetAlignment((FTGL::TextAlignment)align);
    _layout->Render(str, -1, FTPoint(), FTGL::RENDER_ALL);

    glPopMatrix();
}

//============================================================================
//============================================================================
float GlText::getLayoutLineLen()
{
    return _layout->GetLineLength();
}

//============================================================================
//============================================================================
void GlText::setLayoutLineLen(float len)
{
    _layout->SetLineLength(len);
}

//============================================================================
//============================================================================
box3f GlText::getBBoxLayoutf(const char *str, int align)
{
    box3d boxd = GlText::getBBoxLayoutd(str);

    box3f boxf;
    boxf.updateBox((float)boxd.vmin.x, (float)boxd.vmin.y, (float)boxd.vmin.z);
    boxf.updateBox((float)boxd.vmax.x, (float)boxd.vmax.y, (float)boxd.vmax.z);
    return boxf;
}

//============================================================================
//============================================================================
box3d GlText::getBBoxLayoutd(const char *str, int align)
{
    if (!_font) return box3d();
    if (!_layout) return box3d();

    _layout->SetAlignment((FTGL::TextAlignment)align);
    FTBBox ftbox = _layout->BBox(str);

    box3d box;
    box.updateBox(ftbox.Lower().X(), ftbox.Lower().Y(), ftbox.Lower().Z());
    box.updateBox(ftbox.Upper().X(), ftbox.Upper().Y(), ftbox.Upper().Z());
    return box;
}

//============================================================================
//============================================================================
box3d GlText::getBBoxLayoutd(const char *str, const Extents &ext, int alignh, double z)
{
    box3d box = getBBoxLayoutd(str, alignh);

    Extents aexts;
    
    // todo: deal with align value
    aexts.l = ext.l;
    aexts.r = aexts.l + box.getWidth();
    aexts.b = ext.b;
    aexts.t = aexts.b + box.getHeight();

    box3d boxnew;
    boxnew.updateBox(aexts.l, aexts.t, z);
    boxnew.updateBox(aexts.r, aexts.b, z);

    return boxnew; // the aligned bounding box of the tex
}

//============================================================================
//============================================================================
box3f GlText::getBBoxf(const char *str)
{
	box3d boxd = GlText::getBBoxd(str);

	box3f boxf;
	boxf.updateBox((float)boxd.vmin.x, (float)boxd.vmin.y, (float)boxd.vmin.z);
	boxf.updateBox((float)boxd.vmax.x, (float)boxd.vmax.y, (float)boxd.vmax.z);
	return boxf;
}

//============================================================================
//============================================================================
box3d GlText::getBBoxd(const char *str)
{
	if (!_font) return box3d();

	FTBBox ftbox = _font->BBox(str);

	box3d box;
	box.updateBox(ftbox.Lower().X(), ftbox.Lower().Y(), ftbox.Lower().Z());
	box.updateBox(ftbox.Upper().X(), ftbox.Upper().Y(), ftbox.Upper().Z());
	return box;
}

//============================================================================
//============================================================================
box3d GlText::getBBoxd(const Point2d &center, const char *str)
{
    box3d box = getBBoxd(str);
    box3d boxnew;

    vec3d tl, br;
    tl.x = center.dX - box.getSize().x/2.0;
    tl.y = center.dY + box.getSize().y/2.0;
    tl.z = box.vmin.z;

    br.x = tl.x + box.getSize().x;
    br.y = tl.y - box.getSize().y;
    br.z = box.vmax.z;

    boxnew.updateBox(tl);
    boxnew.updateBox(br);

    return boxnew;
}

//============================================================================
// compute actual bounding box from an extents box and alighment specificiations
//============================================================================
box3d GlText::getBBoxd(const char *str, const Extents &ext, Define::AlignH ah, Define::AlignV av, double z)
{
    // this gets the bounding box but its aligned with the baseline, need to modifiy FTGL to run through the render code without drawing anything to compute proper bounding box
    box3d box = getBBoxd(str);
    Extents aexts;

    // horizontal
    if (ah == Define::align_h_lft)
    {
        aexts.l = ext.l;
        aexts.r = aexts.l + box.getWidth();
    }
    else if (ah == Define::align_h_rht)
    {
        aexts.r = ext.r;
        aexts.l = aexts.r - box.getWidth();
    }
    else
    {
        aexts.l = ext.cx() - box.getWidth() / 2.0;
        aexts.r = aexts.l + box.getWidth();
    }

    // cartesian vertical
    if (av == Define::align_v_top)
    {
        aexts.t = ext.t;
        aexts.b = aexts.t - box.getHeight();
    }
    else if (av == Define::align_v_btm)
    {
        aexts.b = ext.b;
        aexts.t = aexts.b + box.getHeight();
    }
    else
    {
        aexts.t = ext.cy() + box.getHeight() / 2.0;
        aexts.b = aexts.t - box.getHeight();
    }

    box3d boxnew;
    boxnew.updateBox(aexts.l, aexts.t, z);
    boxnew.updateBox(aexts.r, aexts.b, z);

    return boxnew; // the aligned bounding box of the text
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

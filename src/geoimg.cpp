#include "geoimg.h"

//============================================================================
//============================================================================
GeoImg::GeoImg()
{

}

//============================================================================
//============================================================================
GeoImg::GeoImg(PTexture tx, const Extents &ext)
{
    init(tx, ext);
}

//============================================================================
//============================================================================
void GeoImg::draw(DrawData *pdd)
{
    GeoObj::draw(pdd);

    if (!_tx) return;

    float a = DrawAttr::alpha(pdd->_drawAttr, _drawAttr);
    drawImg(a);
}

//============================================================================
//============================================================================
void GeoImg::drawImg(float alpha)
{
    if (!_tx) return;

    glEnable(GL_BLEND);     // Turn Blending On
    glDisable(GL_DEPTH_TEST);   // Turn Depth Testing Off

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _tx->getId());
    glColor4f(1, 1, 1, alpha);
    glBegin(GL_QUADS);

        // meed tp flip image vertically
        glTexCoord2d(0,1);
        glVertex2d(_ext.l, _ext.b);

        glTexCoord2d(1,1);
        glVertex2d(_ext.r, _ext.b);

        glTexCoord2d(1,0);
        glVertex2d(_ext.r, _ext.t);

        glTexCoord2d(0,0);
        glVertex2d(_ext.l, _ext.t);
    glEnd();


    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}


//============================================================================
//============================================================================
void GeoImg::computeBox(DrawData *pdd, box3d *pbox)
{
    if (!_tx) return;

    GeoObj::computeBox(pdd, pbox);
    pbox->updateBox(_ext.l, _ext.t, 0);
    pbox->updateBox(_ext.r, _ext.b, 0);
}

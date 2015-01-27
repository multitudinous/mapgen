#ifndef GLTEXT_H
#define GLTEXT_H

#include "box3.h"
class FTFont;

class GlText
{
public:
	GlText();
	virtual ~GlText();

	bool initFontTexture(const char *name, int size);


	bool initFontPoly(const char *name, int size);
	bool initFontExtrude(const char *name, int size, float depth=0);
	bool initFontOutline(const char *name, int size);

    void renderC(const char *str, const Point2d &c, double z=0); // center of text
    void renderBL(const char *str, const Point2d &tl, double z=0); // top left point
	void render(const char *str);
	void render(const char *str, int renderMode); // renderMode= FTGL::RENDER_ALL
	box3f getBBoxf(const char *str);
	box3d getBBoxd(const char *str);
    box3d getBBoxd(const Point2d &center, const char *str);




protected:

	shared_ptr<FTFont> _font;
};

#endif

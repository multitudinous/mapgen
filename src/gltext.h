#ifndef GLTEXT_H
#define GLTEXT_H

#include "box3.h"
#include "extents.h"
#include "define.h"
#include <memory>

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
    void renderBL(const char *str, const Point2d &bl, double z=0); // btm left point
    void render(const char *str, const Extents &textbox, Define::AlignH ah, Define::AlignV av, double z = 0); // first align the text within the text box and then render it
	void render(const char *str);
	void render(const char *str, int renderMode); // renderMode= FTGL::RENDER_ALL

	box3f getBBoxf(const char *str);
	box3d getBBoxd(const char *str);
    box3d getBBoxd(const Point2d &center, const char *str);
    box3d getBBoxd(const char *str, const Extents &ext, Define::AlignH ah, Define::AlignV av, double z = 0);



protected:

	std::shared_ptr<FTFont> _font;
};

#endif

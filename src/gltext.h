#ifndef GLTEXT_H
#define GLTEXT_H

#include "box3.h"
#include "extents.h"
#include "define.h"
#include <memory>

class FTFont;
class FTSimpleLayout;

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

    // for multiline.. could just use these routines instead of above, but seperate for now until refactor
    void renderLayout(const char *str, const Point2d &bl, int align = 0); // typedef enum { ALIGN_LEFT = 0, ALIGN_CENTER = 1, ALIGN_RIGHT = 2, ALIGN_JUSTIFY = 3 } TextAlignment;
    float getLayoutLineLen();
    void setLayoutLineLen(float len);
    box3f getBBoxLayoutf(const char *str, int align = 0);
    box3d getBBoxLayoutd(const char *str, int align = 0);
    box3d getBBoxLayoutd(const char *str, const Extents &ext, int alignh = 0, double z = 0);



protected:

	std::shared_ptr<FTFont> _font;
    std::shared_ptr<FTSimpleLayout> _layout;
};

#endif

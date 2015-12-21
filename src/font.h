#ifndef FONT_H
#define FONT_H

#include <string>
#include <qcolor.h>
#include <memory>

class Font
{
public:
	enum SizeFormat
	{
		Unknown = 0,
		Points = 1,
		Pixels = 2
	};

public:
	Font();
	Font(const char *face, int size, const QColor &c = QColor(0,0,0), SizeFormat szfrmt=Points, const char *name="");

	std::string name() const;
	void name(const char *name);
	void name(const std::string &name);

	std::string face() const;
	void face(const char *fontface);
	void face(const std::string &fontface);

	int size() const;
	void size(int sz, SizeFormat format=Points);
	int size(const std::string &sz); // this will check for the format pt, or px

	int getPixelSize() const;

	SizeFormat sizeFormat() const;
	void sizeFormat(SizeFormat format);

	QColor color() const;
	void color(const QColor &c);

protected:
	std::string _name;
	std::string _face;
	int _size;
	SizeFormat _szFormat;
	QColor _color;

};

typedef std::shared_ptr< Font > PFont;

#endif
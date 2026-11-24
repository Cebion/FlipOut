#ifndef MSP_GLTK_TEXT_H_
#define MSP_GLTK_TEXT_H_

#include <string>
#include <vector>

namespace Msp {
namespace GLtk {

class Geometry;
class Part;
class Style;

/**
Stores and renders text.  Supports multiline text.
*/
class Text
{
private:
	struct Line
	{
		unsigned start;
		unsigned length;
		unsigned width;
	};

	const Style *const &style;
	std::string text;
	std::vector<Line> lines;

public:
	Text(const Style *const &);
	Text(const Style *const &, const std::string &);

	unsigned get_width() const;
	unsigned get_height() const;

	void set(const std::string &);
	void erase(unsigned, unsigned);
	void insert(unsigned, const std::string &);
	const std::string &get() const { return text; }
	unsigned size() const { return text.size(); }

	void render(const Part &, const Geometry &) const;

	Text &operator=(const std::string &);
	//operator const std::string &() const { return text; }
};

} // namespace GLtk
} // namespace Msp

#endif

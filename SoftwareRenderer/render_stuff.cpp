
struct Render_State {
	int height, width;
	uint32_t* memory;

	BITMAPINFO bitmap_info;
};

Render_State surface;


struct Color
{
	union
	{
		struct { uint8_t g, b, r, a; };
		uint8_t raw[4];
		uint32_t whole;
	};

	inline Color() = default;
	inline Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}

	Color operator *(float f)
	{
		return Color(r * f, g * f, b * f);
	}
};
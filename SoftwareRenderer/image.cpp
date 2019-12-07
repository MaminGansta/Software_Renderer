

struct Image
{
	Color* texture;
	int w;
	int h;
	// just return height - i in operator[]
	int h_rotate_{ 0 };

	inline Image() = default;

	inline Image(std::string filename)
	{
		int channels;
		uint8_t* pixmap = stbi_load(filename.c_str(), &w, &h, &channels, 0);

		if (channels != 4 && channels != 3) {
			stbi_image_free(pixmap);
		}

		texture = new Color[w * h];

		if (channels == 4)
		{
			for (int j = 0; j < h; j++)
			{
				for (int i = 0; i < w; i++)
				{
					uint8_t r = pixmap[(i + j * w) * 4 + 0];
					uint8_t g = pixmap[(i + j * w) * 4 + 1];
					uint8_t b = pixmap[(i + j * w) * 4 + 2];
					uint8_t a = pixmap[(i + j * w) * 4 + 3];
					texture[i + j * w] = Color(r, g, b, a);
				}
			}
		}
		else if (channels == 3)
		{
			for (int j = 0; j < h; j++)
			{
				for (int i = 0; i < w; i++)
				{
					uint8_t r = pixmap[(i + j * w) * 3 + 0];
					uint8_t g = pixmap[(i + j * w) * 3 + 1];
					uint8_t b = pixmap[(i + j * w) * 3 + 2];
					texture[i + j * w] = Color(r, b, g);
				}
			}
		}
		stbi_image_free(pixmap);
	}

	// move semantic
	Image(Image&& img) noexcept
	{
		texture = img.texture;
		w = img.w;
		h = img.h;
		img.texture = nullptr;
	}

	Image& operator =(Image&& img) noexcept
	{
		texture = img.texture;
		w = img.w;
		h = img.h;
		img.texture = nullptr;
		return *this;
	}

	inline ~Image() { delete[] texture; }

	inline void h_rotate()
	{
		h_rotate_ = h_rotate_ > 0 ? 0 : h;
	}

	inline Color get_pixel(Vec2i& uv)
	{
		return texture[abs(h_rotate_ - uv.y) * w + uv.x];
	}


	inline Color& operator [] (int i)
	{
		return texture[i];
	}

};

struct Model
{
	
	std::vector<Vec3f> verts_;
	std::vector<std::vector<Vec3i>> faces_; // this means vertex/uv/normal 
	std::vector<Vec3f> norms_;
	std::vector<Vec2f> uv_;
	Image diffusemap_;

	inline Model(std::string filename)
	{
		    std::ifstream in;
		    in.open (filename + ".obj", std::ifstream::in);
		    if (in.fail()) return;
		    std::string line;
		    while (!in.eof()) {
		        std::getline(in, line);
		        std::istringstream iss(line.c_str());
		        char trash;
		        if (!line.compare(0, 2, "v ")) {
		            iss >> trash;
		            Vec3f v;
		            for (int i=0;i<3;i++) iss >> v[i];
		            verts_.push_back(v);
		        } else if (!line.compare(0, 3, "vn ")) {
		            iss >> trash >> trash;
		            Vec3f n;
		            for (int i=0;i<3;i++) iss >> n[i];
		            norms_.push_back(n);
		        } else if (!line.compare(0, 3, "vt ")) {
		            iss >> trash >> trash;
		            Vec2f uv;
		            for (int i=0;i<2;i++) iss >> uv[i];
		            uv_.push_back(uv);
		        }  else if (!line.compare(0, 2, "f ")) {
		            std::vector<Vec3i> f;
		            Vec3i tmp;
		            iss >> trash;
		            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
		                for (int i=0; i<3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
							f.push_back(tmp);
		            }
		            faces_.push_back(f);
		        }
		    }

			diffusemap_ = std::move(Image((std::string(filename) + "_diffuse.tga")));
			diffusemap_.h_rotate();
	}

	inline ~Model() {}

	int verts()
	{
		return (int)verts_.size();
	}

	int faces()
	{
		return (int)faces_.size();
	}

	std::vector<int> face(int idx)
	{
		std::vector<int> face;
		for (int i = 0; i < (int)faces_[idx].size(); i++) face.push_back(faces_[idx][i][0]);
		return face;
	}

	Vec3f vert(int i)
	{
		return verts_[i];
	}

	Vec3f norm(int i)
	{
		return norms_[i];
	}

	Color diffuse(Vec2i& uv)
	{
		return diffusemap_.get_pixel(uv);
	}

	Vec2i uv(int iface, int nvert)
	{
		int idx = faces_[iface][nvert][1];
		return Vec2i(uv_[idx].x * diffusemap_.w, uv_[idx].y * diffusemap_.h);
	}

	Vec2i uv(std::vector<Vec3i>& face, int nvert)
	{
		int idx = face[nvert][1];
		return Vec2i(uv_[idx].x * diffusemap_.w, uv_[idx].y * diffusemap_.h);
	}
};
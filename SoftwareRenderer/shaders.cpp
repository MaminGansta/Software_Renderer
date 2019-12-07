struct IShader {
    virtual ~IShader() {};
    virtual void vertex(std::vector<Vec3i>& face, Matrix44f transformation, Model& model, Vec3f light_dir, Vec3f* screen_coords) = 0;
    virtual bool fragment(Vec3f bar, Color& color) = 0;
};

struct GouraudShader : public IShader {
    Vec3f varying_intensity; // written by vertex shader, read by fragment shader

    virtual void vertex(std::vector<Vec3i>& face, Matrix44f transformation, Model& model, Vec3f light_dir, Vec3f* screen_coords)
    {
        // iteration for 3 triangle vertices
        for (int i = 0; i < 3; i++)
        {
            varying_intensity[i] = MAX(0.1f, dproduct(model.norm(face[i][2]), light_dir)); // get diffuse lighting intensity
            screen_coords[i] = (transformation *  Matrix44f(model.vert(face[i][0]))).toVec3();
        }
    }

    virtual bool fragment(Vec3f bar, Color& color)
    {
        float intensity = dproduct(varying_intensity, bar);   // interpolate intensity for the current pixel
        color = color * intensity;
        return false;                              // no, we do not discard this pixel
    }
};
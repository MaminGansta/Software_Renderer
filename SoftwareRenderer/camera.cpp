
struct Camera
{
	Vec3f pos;
	Vec3f forward;
	Vec3f up;
	Vec3f right;
	float pitch = 0;
	float yaw = 0;


	inline void basis(Vec3f fake_up = Vec3f(0, 1, 0))
	{
		forward.normalizeYourself();
		right = cross(fake_up.normalize(), forward);
		up = cross(forward, right);
	}

	inline void direction()
	{
		forward.x = cosf(pitch) * cosf(yaw);
		forward.y = sinf(pitch);
		forward.z = cosf(pitch) * sinf(yaw);
	}

	inline Matrix44f lookAt()
	{
		// instead multiplication by position matrix
		Vec3f center = pos + forward;

		Matrix44f res = Matrix44f(identity);
		for (int i = 0; i < 3; i++) {
			res[i][0] = right[i];
			res[i][1] = up[i];
			res[i][2] = forward[i];
			res[i][3] = -pos[i];
		}
		return res;
	}
};



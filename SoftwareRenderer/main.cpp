#include <windows.h>
#include <Winuser.h>

#include <stdint.h>
#include <cstring>
#include <math.h>
#include <limits>

#undef min
#undef max

#define MAX(a, b) (a > b? a: b)
#define MIN(a, b) (a < b? a: b)


// STL
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// Global varibals
bool running = true;

const int width = 800;
const int height = 800;
const int depth = 255;

// headers

// Unity build
#include "render_stuff.cpp"
#include "geometry.cpp"
#include "image.cpp"
#include "model.cpp"
#include "shaders.cpp"
#include "draw.cpp"
#include "input.cpp"
#include "timer.cpp"
#include "camera.cpp"




LRESULT CALLBACK win_callback(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lParam)
{
	LRESULT res = 0;

	switch (uMsg)
	{
		case WM_CLOSE:
		case WM_DESTROY:
		{
			running = false;
		} break;

		case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			surface.width = rect.right - rect.left;
			surface.height = rect.bottom - rect.top;
		
			int size = surface.width * surface.height * sizeof(unsigned int);
		
			if (surface.memory) VirtualFree(surface.memory, 0 , MEM_RELEASE);
			surface.memory = (uint32_t*)VirtualAlloc(0, size * sizeof(uint32_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		
			surface.bitmap_info.bmiHeader.biSize = sizeof(surface.bitmap_info.bmiHeader);
			surface.bitmap_info.bmiHeader.biWidth = surface.width;
			surface.bitmap_info.bmiHeader.biHeight = surface.height;
			surface.bitmap_info.bmiHeader.biPlanes = 1;
			surface.bitmap_info.bmiHeader.biBitCount = 32;
			surface.bitmap_info.bmiHeader.biCompression = BI_RGB;
		
		} break;

		default:
		{
			res = DefWindowProc(hwnd, uMsg, wparam, lParam);
		}
	}
	return res;
}



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPiv, LPSTR args, int someshit)
{
	// create window class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = "Game";
	window_class.lpfnWndProc = win_callback;

	// reg window
	RegisterClass(&window_class);

	// create window
	HWND window = CreateWindow(window_class.lpszClassName, "Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInst, 0);
	HDC hdc = GetDC(window);

	// Model
	//Model model("obj/african_head");
	Model model("obj/diablo3_pose/diablo3_pose");

	// zBuffer
	float* zbuffer = new float[surface.width * surface.height];


	// variables camera
	Camera camera{ {1, 1, 3}, {0, 0 , 0} };


	// input
	Key_input Kinput;
	Mouse_input mouse;

	Timer timer;

	while (running)
	{
		// Input
		MSG msg;
		while (PeekMessage(&msg, window, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
				case WM_KEYUP:
				case WM_KEYDOWN:
				{
					uint32_t vk_code = (uint32_t)msg.wParam;
					bool is_down = ((msg.lParam & (1 << 31)) == 0);

					switch (vk_code)
					{
						case VK_LEFT:
						{
							Kinput.buttons[BUTTON_LROTATE].changed = true;// input.buttons[BUTTON_LROTATE].is_down != is_down;
							Kinput.buttons[BUTTON_LROTATE].is_down = is_down;
						}break;
						case VK_RIGHT:
						{
							Kinput.buttons[BUTTON_RROTATE].changed = true;// input.buttons[BUTTON_RROTATE].is_down != is_down;
							Kinput.buttons[BUTTON_RROTATE].is_down = is_down;
						}break;
						case VK_UP:
						{
							Kinput.buttons[BUTTON_UROTATE].changed = true;// input.buttons[BUTTON_LROTATE].is_down != is_down;
							Kinput.buttons[BUTTON_UROTATE].is_down = is_down;
						}break;
						case VK_DOWN:
						{
							Kinput.buttons[BUTTON_DROTATE].changed = true;// input.buttons[BUTTON_RROTATE].is_down != is_down;
							Kinput.buttons[BUTTON_DROTATE].is_down = is_down;
						}break;
						case VK_W:
						{
							Kinput.buttons[BUTTON_UP].changed = is_down;// input.buttons[BUTTON_UP].is_down != is_down;
							Kinput.buttons[BUTTON_UP].is_down = is_down;
						
						}break;
						case VK_S:
						{
							Kinput.buttons[BUTTON_DOWN].changed = true;// input.buttons[BUTTON_DOWN].is_down != is_down;
							Kinput.buttons[BUTTON_DOWN].is_down = is_down;
						
						}break;
						case VK_A:
						{
							Kinput.buttons[BUTTON_LEFT].changed = true;// input.buttons[BUTTON_LEFT].is_down != is_down;
							Kinput.buttons[BUTTON_LEFT].is_down = is_down;
						
						}break;
						case VK_D:
						{
							Kinput.buttons[BUTTON_RIGHT].changed = true;// input.buttons[BUTTON_RIGHT].is_down != is_down;
							Kinput.buttons[BUTTON_RIGHT].is_down = is_down;
						
						}break;
					}
				}break;
				case WM_MOUSEMOVE:
				{
					mouse.x = int(LOWORD(msg.lParam));
					mouse.y = surface.height - int(HIWORD(msg.lParam));
				}break;
				case WM_LBUTTONDOWN:
				{
					mouse.buttons[M_LBUTTON].changed = !mouse.buttons[M_LBUTTON].is_down;
					mouse.buttons[M_LBUTTON].is_down = true;
				}break;
				case WM_LBUTTONUP:
				{
					mouse.buttons[M_LBUTTON].changed = mouse.buttons[M_LBUTTON].is_down;
					mouse.buttons[M_LBUTTON].is_down = false;
				}break;
				default:
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}break;
			}
		}

		// Simulate

		// update z buffer
		for (int i = surface.width * surface.height; i--;)
			zbuffer[i] = -std::numeric_limits<float>::max();


		// camera movement
		if (Kinput.buttons[BUTTON_UP].is_down)
			camera.pos += camera.forward * timer.elapsed;

		if (Kinput.buttons[BUTTON_DOWN].is_down)
			camera.pos -= camera.forward * timer.elapsed;
		
		if (Kinput.buttons[BUTTON_LEFT].is_down)
			camera.pos -= camera.right * timer.elapsed;
		
		if (Kinput.buttons[BUTTON_RIGHT].is_down)
			camera.pos += camera.right * timer.elapsed;


		// camera rotation
		if (Kinput.buttons[BUTTON_UROTATE].is_down)
			camera.pitch += 2 * timer.elapsed;

		if (Kinput.buttons[BUTTON_DROTATE].is_down)
			camera.pitch -= 2 * timer.elapsed;
		
		
		if (Kinput.buttons[BUTTON_RROTATE].is_down)
			camera.yaw += 2 * timer.elapsed;

		if (Kinput.buttons[BUTTON_LROTATE].is_down)
			camera.yaw -= 2 * timer.elapsed;


		// Calculat the matrixes
		Vec3f light_dir = Vec3f(1, 2, 4).normalize();
		
		camera.direction();
		camera.basis();
		Matrix44f ModelView = lookAt( camera.pos + camera.forward, camera.pos);//camera.lookAt();
		Matrix44f Projection(identity);
		Matrix44f ViewPort = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
		Projection[3][2] = -1.f / camera.pos.norm();




		// Draw ------------------------------------------------------

		// clear screen
		for (int i = 0; i < surface.height; i++)
			for (int j = 0; j < surface.width; j++)
				surface.memory[i * surface.width + j] = Color(0, 0, 0).whole;


		Matrix44f transformation = ViewPort * Projection * ModelView;
		GouraudShader shader;
		
		// draw model
		for (auto face : model.faces_)
		{
			Vec3f screen_coords[3];
			shader.vertex(face, transformation, model, light_dir, screen_coords);

			Vec2i uv[3];
			for (int k = 0; k < 3; k++)
				uv[k] = model.uv(face, k);

			triangle(screen_coords, uv, zbuffer, model, &shader);
		}

		// Render
		StretchDIBits(hdc, 0, 0, surface.width, surface.height, 0, 0, surface.width, surface.height, surface.memory, &surface.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		// Timer
		timer.update();
		
		// Log
		char log[128];
		sprintf_s(log, "fps: %d ftime: %.3f sec   forward: %.2f %.2f %.2f:  pos: %.2f %.2f %.2f\n",
			timer.FPS, timer.elapsed, camera.forward.x, camera.forward.y, camera.forward.z, camera.pos.x, camera.pos.y, camera.pos.z);
		OutputDebugString(log);
	}
		
	return 0;
}
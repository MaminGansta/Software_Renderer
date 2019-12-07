

struct Button_state
{
	bool is_down{false};
	bool changed{false};
};



enum Mouse_buttons
{
	M_LBUTTON,
	M_RBUTTON,

	MBUTTONS_COUNT
};

struct Mouse_input
{
	uint16_t x, y;
	Button_state buttons[MBUTTONS_COUNT];
};




// Keyboard input 
#define VK_W 0x57
#define VK_S 0x53
#define VK_A 0x41
#define VK_D 0x44

enum Keyboard_buttons
{
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_LROTATE,
	BUTTON_RROTATE,
	BUTTON_UROTATE,
	BUTTON_DROTATE,

	BUTTON_COUNT
};

struct Key_input
{
	Button_state buttons[BUTTON_COUNT];
};

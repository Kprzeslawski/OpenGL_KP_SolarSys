#include "libndef.h"

class simInput
{
public:

	//simulation parameters
	//all simulation simScale
	DATA_PREC simScale = 1;

	//camera move
	DATA_PREC camdx = 0;
	DATA_PREC camdy = 0;

	// display move vector
	DATA_PREC dx = 0;
	DATA_PREC dy = 0;
	DATA_PREC dz = 0;
	//planet scale
	DATA_PREC ps = 2000;
	//sun scale
	DATA_PREC ss = 10;
	// time scale 1 for 1day/1sec
	DATA_PREC ts = 10; 
	DATA_PREC pausetimebuffer = 0;
	DATA_PREC time = 0;
	DATA_PREC lastframe = 0;

	DATA_PREC initialx=0, initialy=0;
	DATA_PREC xpos=0, ypos=0;

	static std::string cons_inp;
	static std::string cons_com;
	static std::string cons_com2;

	GLFWwindow* win;
	HANDLE s;


	//console x y
	int nScreenWidth = 120;			
	int nScreenHeight = 30;

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];

	static DATA_PREC buffer;
	static int action;

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_callback(GLFWwindow* window, int button, int act, int mods);
	static void key_callback(GLFWwindow* window, int key, int scancode, int act,int mods);
	static void focus_callback(GLFWwindow* window, int focused);

	simInput(GLFWwindow *w);

	void inputEvent();

	void timeEvent();

	void changeSimParam();

	void displaySP();

	void writeString(int row, int col,std::wstring s);

};



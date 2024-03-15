#include "simInput.h"

void simInput::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	buffer = (10 + yoffset) * 0.1f;
	action = 1;
	return;
}

void simInput::mouse_callback(GLFWwindow* window, int button, int act, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && act == GLFW_PRESS) {
		action = 2;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && act == GLFW_RELEASE) {
		action = 0;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	return;
}

void simInput::key_callback(GLFWwindow* window, int key, int scancode, int act, int mods) {
	if (act != GLFW_PRESS) return;

	if (key == GLFW_KEY_P)
		action = 4;

	if ((key >= '0' && key <= '9') || key == ' ' || key == '.') {
			cons_inp += key;
	}

	if (key == GLFW_KEY_ENTER)
		action = 3;

}

simInput::simInput(GLFWwindow* w): win(w){
	
	glfwSetScrollCallback(win, scroll_callback);
	glfwSetMouseButtonCallback(win, mouse_callback);
	glfwSetKeyCallback(win, key_callback); 
	
	std::fill_n(screen, nScreenHeight * nScreenWidth, L' ');

	s = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(s);
}

void simInput::inputEvent()
{
	switch (action) {
		case 1:
			simScale *= buffer;
			action = 0;
			break;
		case 2:
			// new x y pos
			glfwGetCursorPos(win, &xpos, &ypos);
			// return cursor to initial pos
			glfwSetCursorPos(win, initialx, initialy);
			//calculate move
			camdx -= (initialx - xpos) * 0.001f;
			camdy += (initialy - ypos) * 0.001f;
			break;
		case 3:
			action = 0;
			changeSimParam();
			break;
		case 4:
			DATA_PREC x;
			x = ts;
			ts = pausetimebuffer;
			pausetimebuffer = x;
			action = 0;
			break;
		
		default:
			//actualize curret mouse position in case of click
			glfwGetCursorPos(win, &initialx, &initialy);
	}
}


void simInput::timeEvent()
{
	time += (std::clock() - lastframe) * ts *0.001f;
	lastframe = std::clock();
}

void simInput::changeSimParam()
{
	
	std::string t[6];
	for(int i=0;i<6;i++)
		t[i]="";


	int j = 0;
	for (int i = 0; i < cons_inp.size(); i++)
		if (cons_inp[i] != ' ')
			t[j] += cons_inp[i];
		else
			j++;

	if (t[5] == "") {
		cons_com = "Invalid input try again!";
		cons_inp = "";
		return;
	}

	simScale = std::stod(t[0]);
	dx = std::stod(t[1]);
	dy = std::stod(t[2]);
	ps = std::stod(t[3]);
	ss = std::stod(t[4]);
	ts = std::stod(t[5]);

	cons_com = "";
	cons_inp = "";
}


//Displaying simulation parameters
void simInput::displaySP() {

	std::fill_n(screen, nScreenHeight * nScreenWidth, L' ');
	writeString( 0, 0, L"Simulation scale: " + std::to_wstring(simScale));
	writeString( 1, 0, L"Center move vector: " + std::to_wstring(dx) + L" " + std::to_wstring(dy));
	writeString( 2, 0, L"Planet scale : " + std::to_wstring(ps));
	writeString( 3, 0, L"Sun scale: " + std::to_wstring(ss));
	writeString( 4, 0, L"time ratio: " + std::to_wstring(ts) + L" days/s");
	writeString( 5, 0,
		L"Input new parameters [sim scale] [xmov] [ymov] [plan scale] [sun scale] [time ratio] and press ENTER");
	writeString(6, 0, std::wstring(cons_inp.begin(), cons_inp.end()));
	writeString(7, 0, std::wstring(cons_com.begin(), cons_com.end()));
	writeString(8, 0, std::wstring(cons_com2.begin(), cons_com2.end()));
	
	WriteConsole(s, screen, nScreenWidth * nScreenHeight, NULL, NULL);
	
}

void simInput::writeString(int row, int col,std::wstring s)
{
	for (int i = 0; i < s.size(); i++)
		screen[row * nScreenWidth + col + i] = s[i];
}

DATA_PREC simInput::buffer = 0;
int simInput::action = 0;
std::string simInput::cons_inp = "";
std::string simInput::cons_com = "";
std::string simInput::cons_com2 = "Input data in simulation window not in the console or it will pause!";


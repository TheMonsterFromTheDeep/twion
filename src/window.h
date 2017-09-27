#include <GLFW/glfw3.h>

#include <vector>

#include "vector.h"
#include "control.h"

class Window : public EventSource {
public:
	Window(int,int,const char*);
	
	Vec getMouse();

	void open();
	
	void attach(Control*);
private:
	GLFWwindow *glfwWin;
	void calculate_viewport(int, int);
	
	std::vector<Control*> children;
	void draw(const Control&);
    
    Vec previous_mouse;
	
	static void callback_framebuffer_size(GLFWwindow*, int, int);
	static void callback_cursor_position(GLFWwindow*,double,double);
	static void callback_mouse_button(GLFWwindow*,int,int,int);
	static void callback_scroll(GLFWwindow*,double,double);
	
	int viewport_right;
	int viewport_top;
	
	static int wincount;
};
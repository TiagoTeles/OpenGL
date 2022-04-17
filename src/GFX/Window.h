#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Keyboard {
    bool keys[GLFW_KEY_LAST];
};

struct Mouse {
    bool buttons[GLFW_MOUSE_BUTTON_LAST];
    double x, y;
    double dx, dy;
};

class Window {
   public:
    GLFWwindow* window;
    Keyboard keyboard;
    Mouse mouse;

   public:
    Window(int width, int height, const char* name);
    void Delete();

    // Update input devices
    void UpdateKeyboard();
    void UpdateMouse();
};

#endif
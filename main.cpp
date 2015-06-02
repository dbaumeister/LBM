#include "GUI/GUI.h"

int main(void)
{

    GUI gui("GraphicsApp");

    //TODO open  in a new thread -> should not stop others from executing
    gui.display();

    return 0;
}
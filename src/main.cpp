#include "Application.h"

int main(int argc, char* argv[])
{
    Application app;

    app.initialize();
    app.run();
    app.destroy();

	return 0;
}


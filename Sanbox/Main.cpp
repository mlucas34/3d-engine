#include "MyGame.h"


int main() {
	
	try {
		MyGame app;
		app.Run();
	}
	catch (...) 
	{
		return -1;
	}

	return 0;
}
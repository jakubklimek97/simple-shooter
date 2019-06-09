#include <string>


//zwraca 1 jezeli serwer, 0 w przeciwnym wypadku
int parseParams(int argc, char** argv) {
	for (int i = 1; i < argc; ++i) {
		std::string param(argv[i]);
		if (param == "s")
			return 1;
	}
	return 0;
}
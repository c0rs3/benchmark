#include <fstream>

class Logger {
private:
	std::string file_name;
	std::ofstream stream;
public:
	Logger(std::string file_name, std::_Ios_Openmode mode);

	~Logger();

	void log(const std::string& arg);
	
	void log(const double& arg);


	void clear();
};
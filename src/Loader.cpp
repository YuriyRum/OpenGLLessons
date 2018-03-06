#include "Loader.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

std::string Loader::Load(const std::string fileName)
{
	std::fstream os;
	std::string result{""};

	try
	{
		os.open(fileName, std::ios::in);
		if (!os.fail())
		{
			std::stringstream str;
			str << os.rdbuf();
			result = str.str();
		}
	}
	catch (std::exception e)
	{
		std::cerr << e.what() << "\n";
	}
	os.close();
	return result;
}
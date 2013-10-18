#include <string>
#include <iostream>
#include <fstream>
using std::string;

#define VERSION "3.1"

string HSLog;

void UpdateLog(string text, bool isError)
{
	HSLog = HSLog + "\n";

	if(isError)
	{
		HSLog = HSLog + "ERROR: ";
	}

	HSLog = HSLog + text;
	
	string versionText = "Homestrife v.";
	string logText = versionText + VERSION + "\n\nLog:\n" + HSLog;

	std::ofstream file;
	file.open("log.txt");
	file << logText;
	file.close();
}
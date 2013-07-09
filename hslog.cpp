#include <string>
using std::string;

string HSLog;

void UpdateLog(string text, bool isError)
{
	HSLog = HSLog + "\n";

	if(isError)
	{
		HSLog = HSLog + "ERROR: ";
	}

	HSLog = HSLog + text;
}
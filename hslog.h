#ifndef __HSLOG_H_
#define __HSLOG_H_

#include <string>
using std::string;

extern string HSLog;

extern void UpdateLog(string newEntry, bool isError);

#endif
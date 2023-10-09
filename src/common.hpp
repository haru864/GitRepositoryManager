#ifndef _COMMON_H_
#define _COMMON_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <libgen.h>
#include <regex>

using namespace std;

extern int BUFFER_SIZE;
extern vector<string> AVAILABLE_OPTION;
extern string scanResultFileAbsPath;
extern string checkResultFileAbsPath;
extern string manualFileAbsPath;

void initGlobalVariables();

#endif

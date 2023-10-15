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
#include <variant>
#include "functions.hpp"

using namespace std;

typedef variant<std::function<void()>, std::function<void(std::string)>> FunctionVariant;

extern int BUFFER_SIZE;
extern vector<string> AVAILABLE_OPTION;
extern string scanResultFileAbsPath;
extern string checkResultFileAbsPath;
extern string manualFileAbsPath;
extern unordered_map<string, FunctionVariant> commands;

void initGlobalVariables();

#endif

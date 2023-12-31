#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <string>
#include <vector>

using namespace std;

void help() noexcept;
void scan(const string) noexcept;
void check() noexcept;
void list() noexcept;
vector<string> splitString(string, string);
bool isRepogitoryNumber(string);

#endif

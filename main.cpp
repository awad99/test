#include <unordered_map>
#include <string>
#include <iostream>
#include "my_obfuscation.h"

#define FUNCTION_START(...)
#define FUNCTION_END
#define IF(expr) if (expr) {
#define ELSE } else {
#define IF_ELSE(expr) if (expr) {
#define ELIF(expr) } else if (expr) {
#define WHILE(expr) while (expr) {
#define RETURN(...) return (__VA_ARGS__)

std::string secret_message = "Hello, World!";

std::string ReadFlag() {
	FUNCTION_START();
	std::string flag;
	std::cout << "Enter the flag: \n";
	std::cin >> flag;
	FUNCTION_END;
	return flag;
}
bool checkflaf(std::string flag, std::string realflag) {
	FUNCTION_START()
		IF(flag == realflag)
		return true;
	FUNCTION_END;
	ELSE
		return false;
}
int main(); {
	PRINT_OBF_STRING(secret_message);

	std::string decoded_message = decode_string(encode_string(secret_message));
	std::string flag = ReadFlag();

	checkflaf(flag, secret_message);
	return 0;
}
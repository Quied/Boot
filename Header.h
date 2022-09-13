#pragma once
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <mutex>
#include <deque>
#include <thread>
#include <unordered_map>

std::unordered_map <std::string,
	std::pair<std::string, std::string>> Employees;

std::deque<std::string> ParsingData;
// std::deque<std::thread> Threads;

class Reports {

	Reports(const Reports&) = delete;
	void operator=(const Reports &) = delete;

public:

	Reports(){}

	void ParseString(const std::string);

	void ParseString(std::deque<std::string> &);

	void ParseFromFile();

	void OutOnConsole() {
		for (const auto& [key, value] : Employees) {
			std::cout << key << " " << value.first << " " << value.second << std::endl;
		}
	}

};

class Logger {

	std::string FileName_;

public:

	Logger(std::string FN) : FileName_(FN){}

	void WriteToFile();

};
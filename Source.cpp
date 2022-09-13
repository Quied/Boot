#include <iostream>
#include "Header.h"

std::mutex mtx;

void Reports::ParseFromFile() {
	std::ifstream ReadStream("Employees.csv");
	if (!ReadStream) return;

	// Take all lines from file
	std::string TempLine;
	while (!ReadStream.eof()) {
		std::getline(ReadStream, TempLine);
		ParsingData.push_back(TempLine);
	}
	ReadStream.close();

}

void Reports::ParseString(const std::string str) {

	std::string Name = "";
	std::string Month = "";
	uint8_t Hourse = 0;

	uint8_t Byte = 0;
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == ';') { ++Byte; }
		if (Byte == 0) { Name += str[i]; }
		if (Byte == 6 && str[i] != ';') { Month += str[i]; }
		if (Byte == 7 && str[i] != ';') { Hourse = str[i]; }
	}

	std::lock_guard<std::mutex> lock(mtx);
	Employees[Name].first = Month;
	Employees[Name].second = Hourse;
	
}

// For one thread
void Reports::ParseString(std::deque<std::string> &deq) {
	uint8_t Byte = 0;
	for (int t = 0; t < deq.size(); t++) {
		std::string Name = "";
		std::string Month = "";
		std::string Hourse = "";
		Byte = 0;
		for (int i = 0; i < deq[t].size(); i++) {
			if (deq[t][i] == ';') { ++Byte; }
			if (Byte == 0) { Name += deq[t][i]; }
			if (Byte == 6 && deq[t][i] != ';') { Month += deq[t][i]; }
			if (Byte == 7 && deq[t][i] != ';') { Hourse += deq[t][i]; }
		}
		Employees[Name].first = Month;
		Employees[Name].second = Hourse;
	}
}



void Logger::WriteToFile() {

	std::ofstream Log(Logger::FileName_, std::fstream::app);
	
	for (const auto& [key, value] : Employees) {
		Log << key << ';' << value.first << ';' << value.second << std::endl;
	}

	Log.close();
}

int main() {
		auto begin = std::chrono::steady_clock::now();	
		Reports report;
		Logger loger("Log.csv");

		// Parsing file
		report.ParseFromFile();

		size_t Threads = std::thread::hardware_concurrency();

		for (int i = 0; i < Threads; i++) {
			ThreadsDelay.emplace_back([&, i]() {
				for (int j = i; j < ParsingData.size(); j += Threads) {
					report.ParseString(ParsingData[j]);
				}
			});
		}

		for (auto &el : ThreadsDelay) {
			el.join(); }


	 loger.WriteToFile();
	 auto end = std::chrono::steady_clock::now();
	 auto TimeOut = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	 std::cout << "Milliseconds: " << TimeOut.count() << std::endl;
}
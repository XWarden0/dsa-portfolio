// ProjectTwo.cpp
// ABCU Advising Assistance Program (CS 300 - Project Two)
// Single-file program; standard library only.

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Course {
	std::string id;
	std::string title;
	std::vector<std::string> prereqs;
};

// Forward declarations
static std::string trim(std::string s);
static void toUpperInPlace(std::string& s);
static std::vector<std::string> splitcsv(const std::string& line);

static size_t loadCourses(const std::string& filename, std::unordered_map<std::string, Course>& courses, std::vector<std::string>& messages);

static void printCourseList(const std::unordered_map<std::string, Course>& courses);
static void printSingleCourse(const std::unordered_map<std::string, Course>& courses, std::string key);
static bool readIntChoice(const std::string& prompt, int& out);

int main() {
	std::unordered_map<std::string, Course> courses;
	bool loaded = false;

	std::cout << "Welcome to the course planner.\n\n";

	while (true) {
		std::cout << "1. Load Data Structure.\n"
			"2. Print Course List.\n"
			"3. Print Course.\n"
			"9. Exit\n\n";

		int choice = 0;
		if (!readIntChoice("What would you like to do? ", choice)) {
			std::cout << "\nInvalid input. Please enter a number.\n\n";
			continue;
		}

		if (choice == 9) {
			std::cout << "\nThank you for using the course planner!\n";
			break;
		}

		switch (choice) {
		case 1: {
			std::cout << "\nEnter the file name to load (e.g., courses.csv): ";
			std::string filename;
			std::getline(std::cin, filename);
			filename = trim(filename);
			if (filename.empty()) {
				std::cout << "No file provided. Please try again.\n\n";
				break;
			}
			std::vector<std::string> messages;
			size_t count = loadCourses(filename, courses, messages);
			for (const auto& m : messages) std::cout << m << "\n";
			if (count > 0) {
				loaded = true;
				std::cout << "Loaded " << count << " course(s) from '" << filename << "'.\n\n";
			} else {
				loaded = false;
				std::cout << "No courses loaded.\n\n";
			}
			break;
		}
		case 2: {
			if (!loaded) {
				std::cout << "\nPlease load the data first (Option 1).\n\n";
				break;
			}
			printCourseList(courses);
			break;
		}
		case 3: {
			if (!loaded) {
				std::cout << "\nPlease load the data first (Option 1).\n\n";
				break;
			}
			std::cout << "\nWhat course do you want to know about? ";
			std::string key;
			std::getline(std::cin, key);
			key = trim(key);
			if (key.empty()) {
				std::cout << "No course entered.\n\n";
				break;
			}
			printSingleCourse(courses, key);
			break;
		}
		default: {
			std::cout << "\n" << choice << " is not a valid option.\n\n";
			break;
		    }
		}
	}
	return 0;
}
// --- trim helpers ---
static inline void ltrimInPlace(std::string& s) {
	size_t i = 0;
	while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
	if (i) s.erase(0, i);
}
static inline void rtrimInPlace(std::string& s) {
	if (s.empty()) return;
	size_t i = s.size();
	while (i > 0 && std::isspace(static_cast<unsigned char>(s[i - 1]))) --i;
	if (i < s.size()) s.erase(i);
}
static std::string trim(std::string s) {
	ltrimInPlace(s);
	rtrimInPlace(s);
	return s;
}

// --- uppercase in-place ---
static void toUpperInPlace(std::string& s) {
	for (char& ch : s) ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
}

// --- simple CSV split (no quoted fields for this assignment) ---
static std::vector<std::string> splitcsv(const std::string& line) {
	std::vector<std::string> out;
	std::string field;
	std::stringstream ss(line);
	while (std::getline(ss, field, ',')) {
		out.push_back(trim(field));
	}
	return out;
}

// Reads a full line choice from stdin and attempts to parse an int; returns false if not an int.
static bool readIntChoice(const std::string& prompt, int& out) {
	std::cout << "\n" << prompt;
	std::string line;
	if (!std::getline(std::cin, line)) return false;
	std::stringstream ss(line);
	if ((ss >> out) && ss.eof()) return true;
	return false;
}
static size_t loadCourses(const std::string& filename,
	std::unordered_map<std::string, Course>& courses,
	std::vector<std::string>& messages) {
	courses.clear();
	messages.clear();

	std::ifstream in(filename);
	if (!in) {
		messages.push_back("ERROR: Could not open file: " + filename);
		return 0;
	}

	std::vector<std::vector<std::string>> rawTokens;
	rawTokens.reserve(256);

	std::string line;
	size_t lineNo = 0;
	while (std::getline(in, line)) {
		++lineNo;
		std::string t = trim(line);
		if (t.empty()) continue;
		auto tokens = splitcsv(t);
		if (tokens.size() < 2) {
			std::stringstream os;
			os << "ERROR (line " << lineNo << "): Expected at least 2 fields (courseNumber, courseTitle).";
			messages.push_back(os.str());
			continue; // skip malformed line
		}
		rawTokens.push_back(std::move(tokens));
	}
	in.close();

	// Pass 1: build course map (warn on duplicates; replace old)
	for (size_t i = 0; i < rawTokens.size(); ++i) {
		const auto& tokens = rawTokens[i];
		std::string id = tokens[0];
		std::string title = tokens[1];
		toUpperInPlace(id);
		title = trim(title);

		if (id.empty() || title.empty()) {
			std::stringstream os;
			os << "ERROR (logical line #" << (i + 1) << "): Missing course number or title.";
			messages.push_back(os.str());
			continue;
		}

		Course c;
		c.id = id;
		c.title = title;

		for (size_t k = 2; k < tokens.size(); ++k) {
			std::string p = tokens[k];
			toUpperInPlace(p);
			p = trim(p);
			if (!p.empty()) c.prereqs.push_back(p);
		}

		if (courses.find(id) != courses.end()) {
			messages.push_back("WARN: Duplicate course ID '" + id + "' encountered. Replacing previous entry.");
		}
		courses[id] = std::move(c);
	}

	// Pass 2: validate that each prerequisite exists
	for (const auto& kv : courses) {
		const Course& c = kv.second;
		for (const auto& p : c.prereqs) {
			if (courses.find(p) == courses.end()) {
				messages.push_back("WARN: Course '" + c.id + "' has missing prerequisite '" + p + "'.");
			}
		}
	}

	return courses.size();
}
static void printCourseList(const std::unordered_map<std::string, Course>& courses) {
	if (courses.empty()) {
		std::cout << "No course data loaded. Please load the data first.\n";
		return;
	}
	std::vector<std::string> ids;
	ids.reserve(courses.size());
	for (const auto& kv : courses) ids.push_back(kv.first);
	std::sort(ids.begin(), ids.end()); // alphanumeric by course ID

	std::cout << "\nHere is a sample schedule:\n\n";
	for (const auto& id : ids) {
		const Course& c = courses.at(id);
		std::cout << c.id << ", " << c.title << "\n\n";
	}
}

static void printSingleCourse(const std::unordered_map<std::string, Course>& courses, std::string key) {
	if (courses.empty()) {
		std::cout << "No course data loaded. Please load the data first.\n";
		return;
	}
	toUpperInPlace(key);
	auto it = courses.find(key);
	if (it == courses.end()) {
		std::cout << "Course not found: " << key << "\n";
		return;
	}
	const Course& c = it->second;
	std::cout << "\n" << c.id << ", " << c.title << "\n";

	if (c.prereqs.empty()) {
		std::cout << "Prerequisites: None\n\n";
		return;
	}

	std::cout << "Prerequisites: ";
	for (size_t i = 0; i < c.prereqs.size(); ++i) {
		const std::string& pid = c.prereqs[i];
		auto pit = courses.find(pid);
		if (pit != courses.end()) {
			const Course& pc = pit->second;
			std::cout << pc.id << ", " << pc.title;
		}
		else {
			std::cout << pid; // if missing from map, print the code we saw
		}
		if (i + 1 < c.prereqs.size()) std::cout << " | ";
	}
	std::cout << "\n\n";
}

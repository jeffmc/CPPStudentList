// Jeff McMillan 10-6-2022 CPP Student List

#include <iostream>
#include <vector>
#include <limits>
#include <cstring>

#define FNAMELEN 8
#define LNAMELEN 12
#define IDLEN 6
#define GPALEN 4

struct Student {
  char firstName[FNAMELEN + 1],
      lastName[LNAMELEN + 1]; // allow for null terminating char
  int stuid;
  float gpa;
};

void printUsingWidth(const char cstr[], int width) {
  std::cout.width(width);
  std::cout << cstr;
}
void printUsingWidth(const int &val, int width) {
  std::cout.width(width);
  std::cout << val;
}
void printUsingWidth(const float &val, int width) {
  std::cout.width(width);
  std::cout << val;
}

void printHeader() {
  std::cout.fill(' ');
  printUsingWidth("ID", IDLEN);
  std::cout << ' ';
  printUsingWidth("FIRST", FNAMELEN);
  std::cout << ' ';
  printUsingWidth("LAST", LNAMELEN);
  std::cout << ' ';
  printUsingWidth("GPA", GPALEN);
  std::cout << ' ' << std::endl;
}

void printStudent(const Student &s) {
  std::cout.fill(' ');
  printUsingWidth(s.stuid, IDLEN);
  std::cout << ' ';
  printUsingWidth(s.firstName, FNAMELEN);
  std::cout << ' ';
  printUsingWidth(s.lastName, LNAMELEN);
  std::cout << ' ';
  std::cout.precision(GPALEN - 1);
  printUsingWidth(s.gpa, GPALEN);
  std::cout << ' ' << std::endl;
}

void takeCommand(char str[], const int &max) {
	while (true) { // Run until valid input is receieved.
    std::cout << " > "; 
    std::cin.get(str, max); 
    std::cin.clear(); // Clear the flags
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore until the next delimiting char.
	  return;
  }
}

int main() {
  std::vector<Student> stus = {
      {"Jeff", "McMillan", 395752, (24.0 / 7.0)},
      {"Gerald", "Jones", 12353523, (40.0 / 9.0)},
      {"Jorge", "Masvidal", 245793, (13.0 / 7.0)},
      {"Bobby", "Coulter", 431257, (25.0 / 6.0)},
      {"Ahmed", "Abdelmoneum", 367485, (40.0 / 9.0)},
      {"Connor", "MacGregor", 213553, (2.0)},
      {"Portersh", "Bradfordfast", 123456, (32.0 / 11.0)}};
  printHeader();

  for (std::vector<Student>::iterator it = stus.begin(); it != stus.end();
       ++it) {
    printStudent(*it);
  }
  const int buflen = 128;
  char buf[buflen];
  while (true) {
    takeCommand(buf,buflen);
    if (strcmp(buf,"exit") == 0) return 0;
  }
}

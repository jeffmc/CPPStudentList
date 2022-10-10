// Jeff McMillan 10-6-2022 CPP Student List

#include <iostream>
#include <vector>
#include <limits>
#include <cstring>
#include <cstdlib>

#define FNAMELEN 8
#define LNAMELEN 12
#define IDLEN 6
#define GPALEN 4

struct Student {
  public:
    char firstName[FNAMELEN + 1],
          lastName[LNAMELEN + 1]; // allow for null terminating char
    int stuid;
    float gpa;
    Student(const char * fn, const char * ln, const int &stuid, const float &gpa) {
      memcpy(this->firstName, fn, FNAMELEN);
      this->firstName[FNAMELEN] = '\0';
      memcpy(this->lastName, ln, LNAMELEN);
      this->lastName[LNAMELEN] = '\0';
      this->stuid = stuid;
      this->gpa = gpa;
    }
    Student() {}
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

void printStudents(const std::vector<Student> &stus) {
  printHeader();
  for (std::vector<Student>::const_iterator it = stus.cbegin(); it != stus.cend(); ++it) {
    printStudent(*it);
  }

}

#define resetcin() std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n') // Ignore until the next delimiting char.
bool addStudent(std::vector<Student> &stus) {
  const int buflen = 128;
  char buf[buflen];

  Student temp;
  memset(&temp,'\0',sizeof(Student)); 
  
  std::cout << "First: ";
  std::cin.get(buf,FNAMELEN+1);
  memcpy(&temp.firstName,buf,FNAMELEN); 
  resetcin();
  std::cout << "Last: ";
  std::cin.get(buf,LNAMELEN+1);
  memcpy(&temp.lastName,buf,LNAMELEN); 
  resetcin();
 
  std::cout << "Student ID: ";
  std::cin.get(buf,buflen);
  temp.stuid = atoi(buf); 
  resetcin();

  std::cout << "GPA: ";
  std::cin.get(buf,buflen);
  temp.gpa = strtof(buf,nullptr); 
  resetcin();
  
  stus.push_back(temp);
  return true;
}

void printStats(std::vector<Student> &stus) {
  std::cout << "Size: " << stus.size() << std::endl;
  std::cout << "Capacity: " << stus.capacity() << std::endl;
}

void printHelp() {
  std::cout << "Commands:" << std::endl <<
    "help - this" << std::endl <<
    "exit - end program" << std::endl <<
    "add - create new student" << std::endl <<
    "dbg - print vector stats" << std::endl <<
    "ls - list students" << std::endl <<
    "rm [ID] - remove student by id #" << std::endl;
}

bool removeStudent() {}
void takeCommand(char str[], const int &max, std::vector<char*>& tokens) {
	while (true) { // Run until valid input is receieved.
    std::cout << " > "; 
    std::cin.get(str, max);
    resetcin();
    tokens.clear();
    const char* const whitespace = " \f\n\r\t\v";
    char* tkn = strtok(str,whitespace);
    while (tkn != NULL) {
      tokens.push_back(tkn);
      tkn = strtok(NULL,whitespace);
    };
    //for (std::vector<char*>::const_iterator it = tokens.cbegin(); it != tokens.cend(); ++it) {
    //  std::cout << *it << std::endl;
    //}
	  return;
  }
}
int main() {
  std::vector<Student> stus;
  const int buflen = 128;
  std::vector<char*> tokens;
  char buf[buflen];
  const char* cmd;
  printHelp();
  while (true) {
    takeCommand(buf,buflen,tokens);
    cmd = tokens[0];
    if (strcmp(cmd,"help") == 0) {
      printHelp();
      continue;
    }
    if (strcmp(cmd,"exit") == 0) return 0;
    if (strcmp(cmd,"add") == 0) {
      addStudent(stus);
      continue;
    }
    if (strcmp(cmd,"dbg") == 0) {
      printStats(stus);
      continue;
    }
    if (strcmp(cmd,"ls") == 0) {
      printStudents(stus);
    }
    if (strcmp(cmd,"rm") == 0) {
      removeStudent();
    }
  }
}

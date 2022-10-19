// Jeff McMillan 10-6-2022 
// CPP Student List
// This application allows you to manage a list of students. Operations include adding, removing, and printing a list to the console.

#include <iostream> // Console input
#include <vector> // Storage of students
#include <limits> // For maxes of ints
#include <cstring> // CString manipulation
#include <cstdlib> // Random and math
#include <ctime> // Rand seed
#include <cstdio> // File in/out
#include <cctype> // Char tolower() for command case-insensitive

#include "names.h" // Random names

#define FNAMELEN 10
#define LNAMELEN 14
#define SPACING 2
#define IDLEN 6
#define GPALEN 4

struct Student {
  enum Vars {
    FIRSTNAME,LASTNAME,ID,GPA
  };
  public:
    static int cmp(const Student::Vars &v, const Student* a, const Student* b);
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
    Student() {
      memset(this,'\0',sizeof(Student)); 
    }
};

int Student::cmp(const Student::Vars &v, const Student* a, const Student* b) {
  switch (v) {
  case FIRSTNAME:
    return strncmp(a->firstName,b->firstName,FNAMELEN);
  case LASTNAME:
    return strncmp(a->lastName,b->lastName,LNAMELEN);    
  case ID:
    return a->stuid - b->stuid;
  case GPA:
    float c = a->gpa - b->gpa;
    if (c < 0) return -1;
    if (c > 0) return 1;
    return 0;
  }
  return 0;
}

void printUsingWidth(const char cstr[], int width) {
  std::cout.width(width);
  std::cout << cstr;
}
void printUsingWidth(const char &c, int width) {
  std::cout.width(width);
  std::cout << c;
}
void printUsingWidth(const int &val, int width) {
  std::cout.width(width);
  std::cout << val;
}
void printGPAUsingWidth(const float &val, int width) { 
  std::cout.width(width);
  char* buf = new char[width+1];
  snprintf(buf,width+1,"%.2f",val);
  std::cout << buf;
}

void printHeader() {
  std::cout.fill(' ');
  printUsingWidth("ID", IDLEN);
  printUsingWidth(' ', SPACING);
  printUsingWidth("FIRST", FNAMELEN);
  printUsingWidth(' ', SPACING);
  printUsingWidth("LAST", LNAMELEN);
  printUsingWidth(' ', SPACING);
  printUsingWidth("GPA", GPALEN);
  std::cout << ' ' << std::endl;
}

void printStudent(const Student* const s) {
  std::cout.fill(' ');
  printUsingWidth(s->stuid, IDLEN);
  printUsingWidth(' ', SPACING);
  printUsingWidth(s->firstName, FNAMELEN);
  printUsingWidth(' ', SPACING);
  printUsingWidth(s->lastName, LNAMELEN);
  printUsingWidth(' ', SPACING);
  printGPAUsingWidth(s->gpa, GPALEN);
  std::cout << std::endl;
}

void printStudents(const std::vector<Student*> &stus) {
  printHeader();
  if (stus.size() == 0) {
    std::cout << "  No students." << std::endl;
    return;
  }
  for (std::vector<Student*>::const_iterator it = stus.cbegin(); it != stus.cend(); ++it) {
    printStudent(*it);
  }

}

#define resetcin() std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n') // Ignore until the next delimiting char.
bool addStudent(std::vector<Student*> &stus) {
  const int buflen = 128;
  char buf[buflen];

  Student* const temp = new Student();
  
  std::cout << "First: ";
  std::cin.get(buf,FNAMELEN+1);
  memcpy(&temp->firstName,buf,FNAMELEN); 
  resetcin();
  std::cout << "Last: ";
  std::cin.get(buf,LNAMELEN+1);
  memcpy(&temp->lastName,buf,LNAMELEN); 
  resetcin();
 
  std::cout << "Student ID: ";
  std::cin.get(buf,buflen);
  temp->stuid = atoi(buf); 
  resetcin();

  std::cout << "GPA: ";
  std::cin.get(buf,buflen);
  temp->gpa = strtof(buf,nullptr); 
  resetcin();
  
  stus.push_back(temp);
  return true;
}

inline int absrand3() {
  return abs(rand() * rand() * rand());
}

#define RANDOMCT_DEFAULT 10
void addRandoms(std::vector<Student*> &stus, const std::vector<char*> tokens) {
  
  int count = (tokens.size()>1) ? atoi(tokens[1]) : RANDOMCT_DEFAULT;
  Student* s;
  for (int i=0;i<count;i++) {
    s = new Student(randomFirst(), randomLast(), (absrand3()%900000)+100000, (float)(rand()%361)/90.0f);
    stus.push_back(s);
  }
}

void printStats(std::vector<Student*> &stus) {
  std::cout << "Size: " << stus.size() << std::endl;
  std::cout << "Capacity: " << stus.capacity() << std::endl;
}

typedef const char* const cstr_const;

cstr_const CMD_HELP = "help", CMD_EXIT = "exit", CMD_ADD = "add", CMD_DEBUG = "debug", CMD_LIST = "ls", CMD_REMOVE = "rm", CMD_SORT = "sort", CMD_RANDOM = "random", CMD_REVERSE = "reverse", CMD_SAVE = "save", CMD_LOAD = "load", SORT_VAR_FIRSTNAME = "first", SORT_VAR_LASTNAME = "last", SORT_VAR_GPA = "gpa", SORT_VAR_ID = "id";

void printHelp() {
  std::cout << "Commands:" << std::endl <<
    CMD_HELP << " - this" << std::endl <<
    CMD_EXIT << " - end program" << std::endl <<
    CMD_ADD << " - create new student" << std::endl <<
    CMD_DEBUG << " - print vector stats" << std::endl <<
    CMD_LIST << " - list students" << std::endl <<
    CMD_REMOVE << " [ID] - remove student by id (*, wildcard removes all)" << std::endl <<
    CMD_SORT << " [VAR] - sort students by specified variable (" << SORT_VAR_FIRSTNAME << '/' << SORT_VAR_LASTNAME << '/' << SORT_VAR_GPA << '/' << SORT_VAR_ID << ") " << std::endl <<
    CMD_RANDOM << " [COUNT] - add the specified number of randomly generated students (default: " << RANDOMCT_DEFAULT << ")" << std::endl <<
    CMD_REVERSE << " - reverse the order of the student list." << std::endl <<
    CMD_SAVE << " [FORMAT: bin/txt] - save the student list to output.txt file." << std::endl <<
    CMD_LOAD << " [FILENAME, default: \"output.bin\" ] - load students from savefile, only works on binary files." << std::endl;
}

bool removeStudent(std::vector<Student*> &stus, const std::vector<char*> tokens) {
  if (tokens.size() < 2) {
    std::cout << "Need to specify student ID!" << std::endl;
    return false;
  }
  if (strcmp(tokens[1], "*") == 0) {
    std::cout << "Removing with global wildcard!" << std::endl;
    for (std::vector<Student*>::const_iterator it = stus.cbegin(); it != stus.cend(); ++it) {
      delete *it;
    }

    stus.clear();
    return true;
  }
  const int target = atoi(tokens[1]);
  for (std::vector<Student*>::iterator it = stus.begin();it!=stus.end();it++) {
    if (target == (*it)->stuid) {
      std::cout << "Removed " << (*it)->firstName << ' ' << (*it)->lastName << std::endl;
      stus.erase(it);
      return true;
    }
  }
  std::cout << "Couldn't find student with ID: " << target << std::endl;
  return false;
}

// Swap two indices within the referenced vector.
void vecswap(std::vector<Student*> &stus, const int &a, const int &b) {
  Student* t = stus[a];
  stus[a] = stus[b];
  stus[b] = t;
}

// Simple bubble sort implemenation based on https://en.wikipedia.org/wiki/Bubble_sort
void sort(std::vector<Student*> &stus, const std::vector<char*>& tokens) {
  if (tokens.size() < 2) {
    std::cout << "Need to specify sort variable!" << std::endl;
    return;
  }
  const char* vtkn = tokens[1];
  Student::Vars var;
  if (strcmp(vtkn,SORT_VAR_FIRSTNAME)==0) {
    var = Student::FIRSTNAME;
  } else if (strcmp(vtkn,SORT_VAR_LASTNAME)==0) {
    var = Student::LASTNAME;
  } else if (strcmp(vtkn,SORT_VAR_GPA)==0) {
    var = Student::GPA;
  } else if (strcmp(vtkn,SORT_VAR_ID)==0) {
    var = Student::ID;
  } else {
    std::cout << "Unknown student variable: " << vtkn << std::endl;
    return;
  }

  int n = stus.size();
  bool swapped = true;
  int swaps = 0;
  while (swapped) {
    swapped = false;
    for (int i=1;i<n;i++) {
      if (Student::cmp(var, stus[i-1], stus[i])>0) {
        vecswap(stus,i-1,i);
        swapped = true;
        swaps++;
      }
    }
  }
  std::cout << "Using " << swaps << " swaps, sorted by: " << vtkn << std::endl;
}

// Reverse the contents of referenced vector.
void vecreverse(std::vector<Student*> &stus) {
  const int n = stus.size(), hn = n / 2;
  for (int i=0;i<hn;i++) {
    vecswap(stus, i, n-1-i);
  }
}

// Take command input into char buffer from console.
// Tokenize char buf into vector of char ptrs.
void takeCommand(char str[], const int &max, std::vector<char*>& tokens) {
  std::cout << ">"; 
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
}
void save(std::vector<Student*> &stus, std::vector<char*>& tokens) {
  if (tokens.size() < 2) {
    std::cout << "Must specify save format!" << std::endl;
    return;
  }
  FILE* file = nullptr;
  if (strcmp(tokens[1],"txt") == 0) {
    file = fopen("output.txt","w");
    fputs("ID FIRST LAST GPA\n", file);
    const int linelimit = 256;
    char* line = new char[linelimit];
    for (std::vector<Student*>::const_iterator it = stus.cbegin(); it != stus.cend(); ++it) { 
      snprintf(line,linelimit,"%i %s %s %.2f\n",(*it)->stuid,(*it)->firstName,(*it)->lastName,(*it)->gpa);
      fputs(line,file); 
    }
    delete[] line;
  } else if (strcmp(tokens[1],"bin") == 0) {
    file = fopen("output.bin","w");
    for (std::vector<Student*>::const_iterator it = stus.cbegin(); it != stus.cend(); ++it) { 
      fwrite(*it,sizeof(Student),1,file); 
    } 
  } else {
    std::cout << '\"' << tokens[1] << "\" not a valid save format!" << std::endl;
  }
  fclose(file);
}

void load(std::vector<Student*> &stus, std::vector<char*>& tokens) {
  const char* filename = "output.bin";
  if (tokens.size() > 1) {
    filename = tokens[1];
  }
  FILE* file = fopen(filename,"r");
  if (file == nullptr) {
    std::cout << "Couldn't open file for reading: \"" << filename << "\"" << std::endl;
    perror(nullptr);
    return;
  }
  while (true) {
    Student* ptr = new Student();
    fread(ptr,sizeof(Student),1,file);
    if (ferror(file) || feof(file)) {
      if (ferror(file)) perror("Encountered error loading savefile: "); 
      delete ptr;
      break;
    }
    int flen = strlen(ptr->firstName), llen = strlen(ptr->lastName);
    if (flen < 1 || flen > FNAMELEN || llen < 1 || llen > LNAMELEN || ptr->stuid < 0 || ptr->gpa < 0) {
      std::cout << "Read invalid student data!" << std::endl;
      delete ptr;
      continue;
    }
    stus.push_back(ptr);
    std::cout << "Added \"" << ptr->firstName << ' ' << ptr->lastName << "\'" << std::endl;
  }
  fclose(file);
}

int main() {
  srand(time(nullptr));

  std::vector<Student*>* stus = new std::vector<Student*>();
  const int buflen = 128;
  std::vector<char*> tokens;
  char buf[buflen];
  char* cmd = nullptr;
  printHelp();
  while (true) {
    takeCommand(buf,buflen,tokens);
    if (tokens.size()<1) {
      cmd = nullptr;
      continue;
    }
    if (cmd != nullptr) delete[] cmd;
    int cmd_len = strlen(tokens[0]);
    cmd = new char[cmd_len+1];
    strcpy(cmd, tokens[0]);
    for (int i=0;i<cmd_len;i++) {
      cmd[i] = tolower(cmd[i]); // Set lower
    }
    if (strcmp(cmd,CMD_HELP) == 0) {
      printHelp();
      continue;
    }
    if (strcmp(cmd,CMD_EXIT) == 0) break;
    if (strcmp(cmd,CMD_ADD) == 0) {
      addStudent(*stus);
      continue;
    }
    if (strcmp(cmd,CMD_DEBUG) == 0) {
      printStats(*stus);
      continue;
    }
    if (strcmp(cmd,CMD_LIST) == 0) {
      printStudents(*stus);
      continue;
    }
    if (strcmp(cmd,CMD_REMOVE) == 0) {
      removeStudent(*stus, tokens);
      continue;
    }
    if (strcmp(cmd,CMD_RANDOM) == 0) {
      addRandoms(*stus, tokens);
      continue;
    }
    if (strcmp(cmd,CMD_SORT) == 0) {
      sort(*stus, tokens);
      continue;
    }
    if (strcmp(cmd,CMD_REVERSE) == 0) {
      vecreverse(*stus);
      continue;
    }
    if (strcmp(cmd,CMD_SAVE) == 0) {
      save(*stus, tokens);
      continue;
    }
    if (strcmp(cmd,CMD_LOAD) == 0) {
      load(*stus, tokens);
      continue;
    }

    std::cout << "Unknown command: " << buf << std::endl;
  }
  for (std::vector<Student*>::const_iterator it = stus->cbegin(); it != stus->cend(); ++it) {
    delete *it;
  }
  delete stus;
  return 0;
}

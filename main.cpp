// Jeff McMillan 10-6-2022 CPP Student List

#include <iostream> // Console input
#include <vector> // Storage of students
#include <limits> // For maxes of ints
#include <cstring> // CString manipulation
#include <cstdlib> // Random and math
#include <ctime> // Rand seed
#include <cstdio> // File in/out

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
void printUsingWidth(const float &val, int width) { 
  std::cout.precision(width - ((val < 1 && val > -1) ? 2 : 1));
  std::cout.width(width);
  std::cout << val;
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
  printUsingWidth(s->gpa, GPALEN);
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

void printHelp() {
  std::cout << "Commands:" << std::endl <<
    "help - this" << std::endl <<
    "exit - end program" << std::endl <<
    "add - create new student" << std::endl <<
    "dbg - print vector stats" << std::endl <<
    "ls - list students" << std::endl <<
    "rm [ID] - remove student by id (*, wildcard removes all)" << std::endl <<
    "sort [VAR] - sort students by specified variable (first/last/gpa/id) " << std::endl <<
    "rand [COUNT] - add the specified number of randomly generated students (default: " << RANDOMCT_DEFAULT << ")" << std::endl <<
    "rev - reverse the order of the student list." << std::endl <<
    "save [FORMAT: bin/txt] - save the student list to output.txt file." << std::endl <<
    "load [FILENAME, default: \"output.bin\" ] - load students from savefile, only works on binary files." << std::endl;
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
  if (strcmp(vtkn,"first")==0) {
    var = Student::FIRSTNAME;
  } else if (strcmp(vtkn,"last")==0) {
    var = Student::LASTNAME;
  } else if (strcmp(vtkn,"gpa")==0) {
    var = Student::GPA;
  } else if (strcmp(vtkn,"id")==0) {
    var = Student::ID;
  } else {
    std::cout << "Unknown student variable: " << vtkn << std::endl;
    return;
  }

  int n = stus.size();
  bool swapped = true;
  while (swapped) {
    swapped = false;
    for (int i=1;i<n;i++) {
      if (Student::cmp(var, stus[i-1], stus[i])>0) {
        vecswap(stus,i-1,i);
        swapped = true;
      }
    }
  }
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
    for (std::vector<Student*>::const_iterator it = stus.cbegin(); it != stus.cend(); ++it) { 
      // TODO: Cannot properly load the txt savefiles.
      //fputs(s.stuid,file);
      //fputs(" ",file);
      fputs((*it)->firstName,file);
      fputs(" ",file);
      fputs((*it)->lastName,file);
      fputs("\n",file);
      //fputs(" ",file);
      //fputs(s.gpa,file);
    }
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
  const char* cmd;
  printHelp();
  while (true) {
    takeCommand(buf,buflen,tokens);
    cmd = tokens[0];
    if (strcmp(cmd,"help") == 0) {
      printHelp();
      continue;
    }
    if (strcmp(cmd,"exit") == 0) break;
    if (strcmp(cmd,"add") == 0) {
      addStudent(*stus);
      continue;
    }
    if (strcmp(cmd,"dbg") == 0) {
      printStats(*stus);
      continue;
    }
    if (strcmp(cmd,"ls") == 0) {
      printStudents(*stus);
      continue;
    }
    if (strcmp(cmd,"rm") == 0) {
      removeStudent(*stus, tokens);
      continue;
    }
    if (strcmp(cmd,"rand") == 0) {
      addRandoms(*stus, tokens);
      continue;
    }
    if (strcmp(cmd,"sort") == 0) {
      sort(*stus, tokens);
      continue;
    }
    if (strcmp(cmd,"rev") == 0) {
      vecreverse(*stus);
      continue;
    }
    if (strcmp(cmd,"save") == 0) {
      save(*stus, tokens);
      continue;
    }
    if (strcmp(cmd,"load") == 0) {
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

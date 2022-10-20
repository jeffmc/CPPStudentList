// Jeff McMillan 10-6-2022 
// CPP Student List
// This application allows you to manage a list of students. 
// Operations include adding, removing, and printing a list to the console.
// 
// Other features:
// Sorting by different variables, alphabetically or numerically
// Saving in text or binary, loading binary only.
// Generating random students in mass quantities.
// Reversing order of students in list

#include <iostream> // Console input
#include <vector> // Storage of students
#include <limits> // For maxes of ints
#include <cstring> // CString manipulation
#include <cstdlib> // Random, math, string to number conversion
#include <ctime> // Rand seed
#include <cstdio> // File in/out
#include <cctype> // Char tolower() for command case-insensitive

#include "names.h" // Random names

// Student variable column widths / data size limits.
#define FNAMELEN 10 // Hard limit
#define LNAMELEN 14 // Hard limit
#define IDLEN 6 // Preferred print width
#define GPALEN 4 // Truncated print width
#define SPACING 2 // For columns

// Student list will be made up of pointers to instances of this class
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
      INSTANCES++;
      memcpy(this->firstName, fn, FNAMELEN);
      this->firstName[FNAMELEN] = '\0';
      memcpy(this->lastName, ln, LNAMELEN);
      this->lastName[LNAMELEN] = '\0';
      this->stuid = stuid;
      this->gpa = gpa;
    }
    static int INSTANCES;
    Student() {
      INSTANCES++; // For memory leak checking, debug purposes
      memset(this,'\0',sizeof(Student)); 
    }
    ~Student() {
      INSTANCES--;
    }
};

int Student::INSTANCES = 0; // Constructors ran - Destructors ran

// Student comparison based on given Student::Vars enum, [A-Z] or [least-greatest].
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

// TODO: Templates?
// Print different types at specified widths
inline void printUsingWidth(const char cstr[], int width) {
  std::cout.width(width);
  std::cout << cstr;
}
inline void printUsingWidth(const char &c, int width) {
  std::cout.width(width);
  std::cout << c;
}
inline void printUsingWidth(const int &val, int width) {
  std::cout.width(width);
  std::cout << val;
}
// Print to intermediery buffer, then print buffer to console
void printGPAUsingWidth(const float &val, int width) { 
  char* buf = new char[width+1];
  snprintf(buf,width+1,"%.2f",val);
  std::cout << buf;
}

// Print student list header
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
// Print all student vars
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

// Print header and student list body
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

// Macro to ignore until the next delimiting char.
#define resetcin() std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n') 

// Add student using inline vars
bool addStudentUsingTokens(std::vector<Student*> &stus, std::vector<char*> &token) {
  Student* const temp = new Student();
  int templen;
 
  // ID
  int id = atoi(token[1]);
  if (id < 0) {
    std::cout << "ID cannot be negative: \"" << token[1] << '"' << std::endl;
    delete temp;
    return false;
  }
  temp->stuid = id;

  // First name
  templen = strlen(token[2]);
  if (templen>FNAMELEN||templen<1) {
    std::cout << "First name invalid length(" << templen << "): \"" << token[2] << '"' << std::endl;
    delete temp;
    return false;
  }
  strcpy(temp->firstName, token[2]);

  // Last name
  templen = strlen(token[3]);
  if (templen>LNAMELEN||templen<1) {
    std::cout << "Last name invalid length(" << templen << "): \"" << token[3] << '"' << std::endl;
    delete temp;
    return false;
  }
  strcpy(temp->lastName, token[3]);

  // GPA 
  float gpa = strtof(token[4],nullptr);
  if (gpa<0) {
    std::cout << "GPA cannot be negative: \"" << token[4] << '"' << std::endl;
    delete temp;
    return false;
  }
  temp->gpa = gpa;

  std::cout << "Added \"" << temp->firstName << ' ' << temp->lastName << '\"' << std::endl;
  stus.push_back(temp);
  return true;
}

// Add student, by prompt or by inline vars
bool addStudent(std::vector<Student*> &stus, std::vector<char*> &tokens) {
  const int buflen = 128;
  char buf[buflen];

  if (tokens.size()>1) {
    if (tokens.size()!=5) {
      std::cout << "Dont include arguments if not intended to be inline student addition!" << std::endl;
      return false;
    }
    return addStudentUsingTokens(stus,tokens);
  }

  Student* const temp = new Student();
 
  // Prompt for student vars and check validity, if invalid delete temp Student and return false. 
  std::cout << "Student ID: ";
  std::cin.get(buf,buflen);
  temp->stuid = atoi(buf); 
  resetcin();
  if (temp->stuid < 0) {
    std::cout << "New student's ID cannot be negative!" << std::endl;
    delete temp;
    return false;
  }

  std::cout << "First: ";
  std::cin.get(buf,FNAMELEN+1);
  memcpy(&temp->firstName,buf,FNAMELEN); 
  resetcin();
  if (strlen(temp->firstName) == 0) {
    std::cout << "New student's first name cannot be empty!" << std::endl;
    delete temp;
    return false;
  }
  
  std::cout << "Last: ";
  std::cin.get(buf,LNAMELEN+1);
  memcpy(&temp->lastName,buf,LNAMELEN); 
  resetcin();
  if (strlen(temp->lastName) == 0) {
    std::cout << "New student's last name cannot be empty!" << std::endl;
    delete temp;
    return false;
  }

  std::cout << "GPA: ";
  std::cin.get(buf,buflen);
  temp->gpa = strtof(buf,nullptr); 
  resetcin(); 
  if (temp->gpa < 0) {
    std::cout << "New student's GPA cannot be negative!" << std::endl;
    delete temp;
    return false;
  }

  // If made it through validation, add temp to vector.
  std::cout << "Added \"" << temp->firstName << ' ' << temp->lastName << '\"' << std::endl;
  stus.push_back(temp);
  return true;
}

// TODO: Fix this
// Should be a positive large random number 
inline int absrand3() {
  return abs(rand() * rand() * rand());
}

// Use random names from header file to generate a new student(s).
#define RANDOMCT_DEFAULT 10
void addRandoms(std::vector<Student*> &stus, const std::vector<char*> tokens) { 
  int count = (tokens.size()>1) ? atoi(tokens[1]) : RANDOMCT_DEFAULT;
  Student* s;
  for (int i=0;i<count;i++) {
    s = new Student(randomFirst(), randomLast(), (absrand3()%900000)+100000, (float)(rand()%361)/90.0f);
    stus.push_back(s);
  }
  std::cout << "Added " << count << " random " << (count==1?"student.":"students.") << std::endl;
}

// Print behind-the-scenes values to console
void printStats(std::vector<Student*> &stus) {
  std::cout << "Student size: " << sizeof(Student) << " bytes" << std::endl <<
               "Vector stats:" << std::endl <<
               "  Size: " << stus.size() << " / " << stus.size()*sizeof(Student) << " bytes" << std::endl <<
               "  Capacity (students): " << stus.capacity() << " / " << stus.capacity()*sizeof(Student) << " bytes" << std::endl <<
               "Student instances: " << Student::INSTANCES << std::endl;
}

// Help / Command word def / Sort var word def
typedef const char* const cstr_const;
cstr_const CMD_HELP = "help", CMD_EXIT = "quit", CMD_ADD = "add", CMD_DEBUG = "debug", CMD_LIST = "print", CMD_REMOVE = "delete", CMD_SORT = "sort", CMD_RANDOM = "random", CMD_REVERSE = "reverse", CMD_SAVE = "save", CMD_LOAD = "load", SORT_VAR_FIRSTNAME = "first", SORT_VAR_LASTNAME = "last", SORT_VAR_GPA = "gpa", SORT_VAR_ID = "id";

inline void printHelp() {
  cstr_const INDENT = "  ";
  std::cout << "ESSENTIAL COMMANDS:" << std::endl <<
    INDENT << CMD_ADD << " - create new student using prompt" << std::endl <<
    INDENT << CMD_LIST << " - list students" << std::endl <<
    INDENT << CMD_REMOVE << " - prompt user for id of student to be removed. (*, wildcard removes all)" << std::endl <<
    INDENT << CMD_EXIT << " - end program" << std::endl << std::endl <<
    "MISCELLANEOUS COMMANDS:" << std::endl <<
    INDENT << CMD_ADD << " [ID] [FIRSTNAME] [LASTNAME] [GPA] - create new student using parameters" << std::endl <<
    INDENT << CMD_REMOVE << " [ID] - remove student by id (*, wildcard removes all)" << std::endl <<
    INDENT << CMD_HELP << " - this" << std::endl <<
    INDENT << CMD_DEBUG << " - print vector stats" << std::endl <<
    INDENT << CMD_SORT << " [VAR] - sort students by specified variable (" << SORT_VAR_FIRSTNAME << '/' << SORT_VAR_LASTNAME << '/' << SORT_VAR_GPA << '/' << SORT_VAR_ID << ") " << std::endl <<
    INDENT << CMD_RANDOM << " [COUNT] - add the specified number of randomly generated students (default: " << RANDOMCT_DEFAULT << ")" << std::endl <<
    INDENT << CMD_REVERSE << " - reverse the order of the student list." << std::endl <<
    INDENT << CMD_SAVE << " [FORMAT: bin/txt] - save the student list to output.txt file." << std::endl <<
    INDENT << CMD_LOAD << " [FILENAME, default: \"output.bin\" ] - load students from savefile, only works on binary files." << std::endl << std::endl;
}
// Remove any students that match given ID token
bool removeStudent(std::vector<Student*> &stus, const std::vector<char*> &tokens) {
  if (tokens.size() < 2) {
    std::cout << "Need to specify student ID!" << std::endl;
    return false;
  }
  int removed = 0;
  if (strcmp(tokens[1], "*") == 0) {
    std::cout << "Removing with global wildcard!" << std::endl;
    for (std::vector<Student*>::const_iterator it = stus.cbegin(); it != stus.cend(); ++it) {
      delete *it;
      removed++;
    }
    stus.clear();
  } else {
    const int target = atoi(tokens[1]);
    for (std::vector<Student*>::iterator it = stus.begin();it!=stus.end();) {
      if (target == (*it)->stuid) {
        std::cout << "Removed " << (*it)->firstName << ' ' << (*it)->lastName << std::endl;
        delete *it;
        it = stus.erase(it);
        removed++;
      } else {
        it++;
      }
    }
  }
  if (removed > 0) { 
    std::cout << "Removed " << removed << (removed==1?" student.":" students.") << std::endl;
  } else {
    std::cout << "Removed no students, could not find ID: " << tokens[1] << std::endl;
  }
  return false;
}
// Prompt user if tokens aren't passed via cmd
bool removeStudentPrompt(std::vector<Student*> &stus, std::vector<char*> &tokens) {
  if (tokens.size()>1) return removeStudent(stus,tokens);
  std::cout << "ID to remove: ";
  
  const int buflen = 32;
  char buf[buflen];
  std::cin.get(buf,buflen);
  resetcin();

  tokens.push_back(buf);
  return removeStudent(stus,tokens); // after this function is popped from stack, tokens vector is invalidated
}

// Swap two indices within the referenced vector.
void vecswap(std::vector<Student*> &stus, const int &a, const int &b) {
  Student* t = stus[a];
  stus[a] = stus[b];
  stus[b] = t;
}

// TODO: https://en.wikipedia.org/wiki/Block_sort implement this
// Simple bubble sort implemenation based on https://en.wikipedia.org/wiki/Bubble_sort
void sort(std::vector<Student*> &stus, const std::vector<char*>& tokens) {
  if (tokens.size() < 2) {
    std::cout << "Need to specify sort variable!" << std::endl;
    return;
  }
  const char* vtkn = tokens[1];
  Student::Vars var; // Parse cstr into Student::Vars enum
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

  // Bubble sort impl
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

// Save into a txt/bin file called output
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

// Load from file into student list
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
  // Set random seed for random command
  srand(time(nullptr));

  // Vector of Students, the "Student List"
  std::vector<Student*>* stus = new std::vector<Student*>();
  const int buflen = 128; // Max command length, including arguments
  std::vector<char*> tokens; // Vector of char* to tokens within the command, seperated by whitespace
  char buf[buflen]; // Cmd buffer, will be seperated by more than one null-terminating char once tokenized.
  char* cmd = nullptr; // Lowercase version of first token from command input 
  printHelp();
  while (true) {
    takeCommand(buf,buflen,tokens);
    if (cmd != nullptr) delete[] cmd;
    if (tokens.size()<1) {
      cmd = nullptr;
      continue;
    }/* else { // Print the tokenized version of the command typed by user.
      std::cout << (void*)buf << ": Received:" << std::endl;
      for (std::vector<char*>::const_iterator it = tokens.cbegin(); it != tokens.cend(); ++it) {
        std::cout << "  " << (void*)*it << " Tokenized: \"" << *it << '"' << std::endl;      
      }
      std::cout << std::endl;
    }*/
    // Allocate correctly sized cstring ptr, copy string, change to lowercase
    int cmd_len = strlen(tokens[0]);
    cmd = new char[cmd_len+1];
    strcpy(cmd, tokens[0]);
    for (int i=0;i<cmd_len;i++) {
      cmd[i] = tolower(cmd[i]); // Set lower
    }

    // Check lowercase 1st token against command words
    if (strcmp(cmd,CMD_HELP) == 0) {
      printHelp();
      continue;
    }
    if (strcmp(cmd,CMD_EXIT) == 0) break;
    if (strcmp(cmd,CMD_ADD) == 0) {
      addStudent(*stus, tokens);
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
      removeStudentPrompt(*stus, tokens);
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

    std::cout << "Unknown command: \"" << buf << '"' << std::endl;
  }

  // Cleanup student list, deallocate all heap memory
  for (std::vector<Student*>::const_iterator it = stus->cbegin(); it != stus->cend(); ++it) {
    delete *it;
  }
  delete stus;
  return 0;
}

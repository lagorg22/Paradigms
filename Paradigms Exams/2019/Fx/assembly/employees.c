typedef bool(*IsLower)(char);

typedef struct {
  int salary;  
  char* name;
} Employee;

int EmployeeCmp(const Employee* a, const Employee* b) {
  if (a->salary != b->salary) {
    return a->salary - b->salary;
  }
  return strcmp(a->name, b->name);
}

char* EmployeWithHighestSalary(int num_employees, Employee* employees, IsLower is_lower) {
  qsort(employees, num_employees, sizeof(Employee), EmployeeCmp);
  int len = strlen(employees[num_employees - 1].name);
  char* upper = malloc(len * sizeof(char) + 1);
  for (int i = 0; i < len; ++i) {
    char ch = employees[num_employees - 1].name[i];
    if (is_lower(ch)) {
      upper[i] = 'A' + ch - 'a';
    }
  }
  upper[len] = '\0';
  return upper;
}

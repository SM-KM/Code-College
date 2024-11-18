

#include <iostream>
#include <string>

class Student {
private:
  std::string name;
  int age;
  std::string id;

public:
  // Constructor
  Student(const std::string &name, int age, const std::string &id)
      : name(name), age(age), id(id) {}

  // Getters
  std::string getName() const { return name; }
  int getAge() const { return age; }
  std::string getId() const { return id; }

  // Setters
  void setName(const std::string &newName) { name = newName; }
  void setAge(int newAge) { age = newAge; }
  void setId(const std::string &newId) { id = newId; }

  // printStudent
  void printStudent() const {
    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl;
    std::cout << "ID: " << id << std::endl;
  }

  // birthday
  void birthday() { age++; }
};

int main() {
  // Create an instance of Student
  Student student1("John Doe", 20, "A00123456");

  // Test getters
  std::cout << "Using Getters:" << std::endl;
  std::cout << "Name: " << student1.getName() << std::endl;
  std::cout << "Age: " << student1.getAge() << std::endl;
  std::cout << "ID: " << student1.getId() << std::endl;

  // Test printStudent
  std::cout << std::endl << "Using printStudent method:" << std::endl;
  student1.printStudent();

  // Test birthday method
  student1.birthday();
  std::cout << std::endl << "After birthday:" << std::endl;
  student1.printStudent();

  // Modify attributes using setters
  student1.setName("James Smith");
  student1.setAge(22);
  student1.setId("A00765432");

  // Print updated information
  std::cout << std::endl << "Updated Information:" << std::endl;
  student1.printStudent();

  // Verify updates with getters
  std::cout << std::endl << "Using Getters after Updates:" << std::endl;
  std::cout << "Name: " << student1.getName() << std::endl;
  std::cout << "Age: " << student1.getAge() << std::endl;
  std::cout << "ID: " << student1.getId() << std::endl;

  return 0;
}

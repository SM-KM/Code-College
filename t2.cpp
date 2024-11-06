// Santiago Vargas Rugeles
// Arturo Pedraza

#include <cmath>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <ios>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

typedef struct {
  int x;
  int y;
} nums;

// Console Utils
void ClearConsole() {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ShowInvalidInput() {
  std::cout << "Invalid Input, Please try again" << std::endl;
}

void NewLines(int linesAmount) {
  for (int i = 0; i < linesAmount; i++) {
    std::cout << '\n';
  }
}

template <typename FuncType>
void addMenuOption(
    std::vector<std::pair<std::string, std::function<void()>>> &menu,
    const std::string &name, FuncType func) {
  menu.emplace_back(name, [func]() { func(); });
}

template <typename T>
bool getValidInput(T &value, const std::string &inputText) {
  std::cout << inputText;
  std::cin >> value;

  if (std::cin.fail() || (std::is_integral<T>::value && value < 0)) {
    ClearConsole();
    ShowInvalidInput();
    return false;
  }

  return true;
}

// Function to handle input for multiple types, using a vector of references to
// variables
template <typename T>
void getInputs(std::vector<std::reference_wrapper<T>> values,
               std::vector<std::string> prompts) {
  bool validInput = false;

  // Loop through the provided values and prompts and get valid input for each
  while (!validInput) {
    validInput = true;
    for (size_t i = 0; i < values.size(); ++i) {
      if (!getValidInput(values[i].get(), prompts[i])) {
        validInput = false;
        break; // If any input is invalid, restart the loop
      }
    }
  }
}

// Utils

// Menu Utils
void pairAndAddToMenu(
    std::vector<std::string> labels,
    std::vector<std::function<void()>> functions,
    std::vector<std::pair<std::string, std::function<void()>>> &menu) {
  for (size_t i = 0; i < labels.size(); i++) {
    addMenuOption(menu, labels[i], [functions, i]() { functions[i](); });
  }
}

// Math Utils
int sum(nums nums) { return nums.x + nums.y; }
int difference(nums nums) { return nums.x - nums.y; }
int mult(nums nums) { return nums.x * nums.y; }

nums grabbingInputs() {
  nums nums = {.x = 0, .y = 0};

  std::cout << "Introduce the two numbers" << std::endl;
  while (true) {
    if (!getValidInput(nums.x, "First number: "))
      continue;

    break;
  }

  while (true) {
    if (!getValidInput(nums.y, "Second number: "))
      continue;

    break;
  }

  return nums;
};

void loadMethods(nums nums) {
  std::cout << "Sum: " << sum(nums) << std::endl;
  std::cout << "Difference: " << difference(nums) << std::endl;
  std::cout << "Product: " << mult(nums) << std::endl;
}

int getRandomNum(int min, int max) { return rand() % (max - min + 1) + min; }

void act_1() {
  nums nums = grabbingInputs();
  loadMethods(nums);
}

void act_2() {
  int num;
  while (true) {
    if (!getValidInput(num, "Number to validate: "))
      continue;

    if (!(num % 2 == 0)) {
      std::cout << "Odd number!" << std::endl;
      break;
    }

    std::cout << num << " is an even number, try again!" << std::endl;
    continue;
  }
}

void act_3() {
  int sum;
  // for (int i = 0; i <= 100; i += 2) {
  //   sum = sum + i;
  // }

  // Further optimization without using loops, since
  // sum = n * (n + 1)
  // n = 100 / 2, n == max even number

  sum = 50 * (50 + 1);
  std::cout << "Final result: " << sum << std::endl;
}

void act_4() {
  int numsAmount;
  int res = 0;

  while (true) {
    if (!getValidInput(numsAmount, "Amount of numbers: "))
      continue;

    int pos = 1;
    while (pos <= numsAmount) {
      int currentValue;

      if (!getValidInput(currentValue, "Num " + std::to_string(pos) + ": "))
        continue;

      res += currentValue;
      pos++;
    }

    std::cout << "Result: " << res / numsAmount;
    break;
  }
}

void act_5() {
  int min = 1, max = 100;
  int ranNum = getRandomNum(min, max);
  int tries = 0;

  while (true) {
    int currentGuessedNum;
    if (!getValidInput(currentGuessedNum, "Guess: "))
      continue;

    if (currentGuessedNum == ranNum) {
      std::cout << "Guessed Correctly the num was: " << ranNum
                << " , you guessed in " << tries << " tries" << std::endl;
      break;
    }
    currentGuessedNum > ranNum ? std::cout << "Lower" << std::endl
                               : std::cout << "Higher" << std::endl;
    tries++;
  }
}

void act_6() {
  double celsius, fahrenheit;
  int numConversions, increment;

  std::vector<std::reference_wrapper<double>> inputsDouble = {celsius};
  std::vector<std::string> promptsDouble = {"Initial celsius value: "};

  getInputs(inputsDouble, promptsDouble);

  std::vector<std::reference_wrapper<int>> inputsInt = {numConversions,
                                                        increment};
  std::vector<std::string> promptsInt = {
      "Number of conversions: ", "Increment between the celsius values: "};

  getInputs(inputsInt, promptsInt);

  std::cout << "\nConversions from Celsius to Fahrenheit\n";
  std::cout << std::setw(15) << std::left << "CELSIUS" << std::setw(15)
            << std::left << "FARENHEIT" << std::endl;
  std::cout << "------------------------------------------" << std::endl;

  for (int i = 0; i < numConversions; i++) {
    fahrenheit = (celsius * 9 / 5) + 32;

    std::cout << std::setw(15) << std::left << std::fixed
              << std::setprecision(2) << celsius << std::setw(15) << std::left
              << std::fixed << std::setprecision(2) << fahrenheit << std::endl;

    celsius += increment;
  }
}

void arithmeticSeries(int a, int d, int n) {
  int sumaTotal = 0;

  for (int i = 0; i < n; i++) {
    int termino = a + i * d;
    std::cout << "Term " << (i + 1) << ": " << termino << std::endl;
    sumaTotal += termino;
  }

  std::cout << "Valor total de la serie: " << sumaTotal << std::endl;
}

void act_7() {
  int a = 1, d = 3, n = 25;
  arithmeticSeries(a, d, n);
}

void harmonicMean(std::vector<int> nums) {
  int amount = nums.size();
  double sum = 0.0;

  for (int i = 0; i < amount; i++) {
    sum += 1.0 / nums[i];
  }

  std::cout << "Harmonic mean: " << amount / sum << std::endl;
}

void geometricMean(std::vector<int> nums) {
  int amount = nums.size();
  double sum = 1.0;

  for (int i = 0; i < amount; i++) {
    sum *= nums[i];
  }

  std::cout << "Geometric mean: " << pow(sum, 1.0 / amount) << std::endl;
}

void act_8() {
  std::vector<int> nums;
  int currentNum;

  std::cout << "Start typing numbers and finish by typing a 0 \n" << std::endl;

  while (currentNum != 0) {
    if (!getValidInput(currentNum, "Num: "))
      continue;

    if (currentNum == 0)
      break;
    nums.push_back(currentNum);
  }

  if (!nums.empty()) {
    geometricMean(nums);
    harmonicMean(nums);
    return;
  }

  std::cout << "No valid numbers entered." << std::endl;
}

// Menu
// Default menu options

void displayMenu(
    const std::vector<std::pair<std::string, std::function<void()>>> &options,
    std::string menuType) {

  std::string menuTitle = menuType + ":";
  NewLines(1);
  std::cout << menuTitle << std::endl;
  for (size_t i = 0; i < options.size(); i++) {
    std::cout << i + 1 << ". " << options[i].first << std::endl;
  }

  std::cout << options.size() + 1 << ". Exit" << "\n" << std::endl;
  std::cout << "Choice an option: " << std::endl;
}

void Menu() {
  // Menu to load submenus if need that accepts any type of function
  std::vector<std::pair<std::string, std::function<void()>>> menu;
  std::vector<std::string> optionLabels = {
      "Num operations",
      "Validate is odd number",
      "Sum all the even numbers between 0 - 100",
      "Get the media of specific range of numbers",
      "Guess the random number between 0 - 100",
      "Convert celcius to fahrenheit",
      "Aritmetic Series",
      "Geometric and Harmonic mean",
  };

  std::vector<std::function<void()>> optionFunctions = {
      []() { act_1(); }, []() { act_2(); }, []() { act_3(); },
      []() { act_4(); }, []() { act_5(); }, []() { act_6(); },
      []() { act_7(); }, []() { act_8(); }};

  pairAndAddToMenu(optionLabels, optionFunctions, menu);

  std::string menuType = "Menu";
  int choice;
  int exitOption = optionLabels.size() + 1;

  do {
    displayMenu(menu, menuType);
    std::cin >> choice;

    if (std::cin.fail()) {
      ClearConsole();
      ShowInvalidInput();
      continue;
    }

    if (choice > 0 && choice <= menu.size()) {
      NewLines(1);
      menu[choice - 1].second();
    }

  } while (choice != exitOption);
}

// Main entry point
int main(int argc, char *argv[]) {
  Menu();
  return 0;
}

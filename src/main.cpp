#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include <string>

void home(float num);
void error(String line1);
float stringToNum(String str);
String formatFloat(float number);
float getInputNum();
void printMenu(String menuEntries[], int index);

void addition(float n1);
void subtraction(float n1);
void multiplication(float n1);
void division(float n1);

void fnMenu();
void sqrt();
void pow();

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

byte fullChar[8] = {0b11111, 0b11111, 0b11111, 0b11111,
                    0b11111, 0b11111, 0b11111, 0b11111};
byte upArrow[8] = {0b00100, 0b01110, 0b10101, 0b00100,
                   0b00100, 0b00100, 0b00100, 0b00000};
byte downArrow[8] = {0b00000, 0b00100, 0b00100, 0b00100,
                     0b00100, 0b10101, 0b01110, 0b00100};

class Button {
 public:
  int pin;
  bool buttonPressed = false;

  Button(int pin) {
    this->pin = pin;
    pinMode(pin, INPUT_PULLDOWN);
  }

  bool isPressed() {
    int buttonState = digitalRead(pin);

    if (buttonState == HIGH && !buttonPressed) {
      buttonPressed = true;
      return true;
    } else if (buttonState == LOW) {
      buttonPressed = false;
    }

    return false;
  }
};

Button enterButton = Button(4);
Button clearButton = Button(35);
Button dotButton = Button(14);
Button plusButton = Button(23);
Button minusButton = Button(2);
Button multiplyButton = Button(18);
Button divideButton = Button(19);
Button fnButton = Button(33);
Button upButton = Button(5);
Button downButton = Button(13);

Button numberButtons[10] = {
    Button(26),  // 0
    Button(32),  // 1
    Button(13),  // 2
    Button(34),  // 3
    Button(25),  // 4
    Button(5),   // 5
    Button(16),  // 6
    Button(15),  // 7
    Button(17),  // 8
    Button(27),  // 9
};

void error(String line1) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print("Press Enter");

  while (true) {
    if (clearButton.isPressed() || enterButton.isPressed()) {
      home(0);
    }
    delay(10);
  }
}

float stringToNum(String str) {
  Serial.println(str);
  try {
    return str.toFloat();
  } catch (...) {
    error("Invalid number!");
  }
}

// only returns decimal places if number has some.
String formatFloat(float number) {
  int intPart = static_cast<int>(number);
  if (number == intPart) {
    return String(intPart);
  } else {
    return String(number, 4);
  }
}

float getInputNum() {
  String inputStr = "0";
  while (true) {
    for (int i = 0; i < 10; i++) {
      if (numberButtons[i].isPressed()) {
        lcd.print(i);
        inputStr += String(i);
      }
    }

    if (dotButton.isPressed()) {
      lcd.print(".");
      inputStr += ".";
    }

    if (minusButton.isPressed()) {
      if (inputStr == "0") {
        inputStr = "-0";
        lcd.print("-");
      }
    }

    if (enterButton.isPressed()) {
      if (inputStr == "-0") lcd.print("0");
      return stringToNum(inputStr);
    }

    if (clearButton.isPressed()) {
      lcd.clear();
      lcd.setCursor(0, 0);
      home(0);
    }

    delay(20);
  }
}

void home(float num) {
  // set initial number if available
  String inputStr;
  if (num != 0) {
    inputStr = String(formatFloat(num));
    lcd.print(formatFloat(num));
  } else {
    inputStr = "0";
  }

  // wait until button is pressed and trigger spicific action
  while (true) {
    for (int i = 0; i < 10; i++) {
      if (numberButtons[i].isPressed()) {
        lcd.print(i);
        inputStr += String(i);
      }
    }

    if (dotButton.isPressed()) {
      lcd.print(".");
      inputStr += ".";
    }

    if (clearButton.isPressed()) {
      lcd.clear();
      lcd.setCursor(0, 0);
      home(0);
    }

    if (fnButton.isPressed()) {
      Serial.print("fn menu opened.");
      fnMenu();
    }

    if (plusButton.isPressed()) {
      if (inputStr == "-0") lcd.print("0");
      addition(stringToNum(inputStr));
    }

    if (minusButton.isPressed()) {
      if (inputStr == "0") {
        inputStr = "-0";
        lcd.print("-");
      } else {
        if (inputStr == "-0") lcd.print("0");
        subtraction(stringToNum(inputStr));
      }
    }

    if (multiplyButton.isPressed()) {
      if (inputStr == "-0") lcd.print("0");
      multiplication(stringToNum(inputStr));
    }

    if (divideButton.isPressed()) {
      if (inputStr == "-0") lcd.print("0");
      division(stringToNum(inputStr));
    }

    delay(20);
  }
}

void addition(float n1) {
  lcd.print(" + ");
  float n2 = getInputNum();

  lcd.clear();
  lcd.setCursor(0, 0);

  float result = n1 + n2;
  if (result == 0) lcd.print(0);
  home(result);
}

void subtraction(float n1) {
  lcd.print(" - ");
  float n2 = getInputNum();

  lcd.clear();
  lcd.setCursor(0, 0);

  float result = n1 - n2;
  if (result == 0) lcd.print(0);
  home(result);
}

void multiplication(float n1) {
  lcd.print(" * ");
  float n2 = getInputNum();

  lcd.clear();
  lcd.setCursor(0, 0);

  float result = n1 * n2;
  if (result == 0) lcd.print(0);
  home(result);
}

void division(float n1) {
  lcd.print(" / ");
  float n2 = getInputNum();

  lcd.clear();
  lcd.setCursor(0, 0);

  float result = n1 / n2;
  if (result == 0) lcd.print(0);
  home(result);
}

void printMenu(String menuEntries[], int index) {
  lcd.clear();
      lcd.setCursor(3, 1);
      lcd.write(1);
      lcd.print(" Select ");
      lcd.write(2);
      lcd.setCursor(0, 0);
      lcd.print(menuEntries[index]);
}

void fnMenu() {
  int menuEntriesLength = 2;
  String menuEntries[menuEntriesLength] = {"sqrt(x)", "pow(x, y)"};
  int index = 0;

  printMenu(menuEntries, index);

  while (true) {
    if (upButton.isPressed()) {
      if (index == 0) {
        index = menuEntriesLength - 1;
      } else {
        index--;
      }
      printMenu(menuEntries, index);
    }

    if (downButton.isPressed()) {
      if (index == menuEntriesLength - 1) {
        index = 0;
      } else {
        index++;
      }
      printMenu(menuEntries, index);
    }

    if (clearButton.isPressed()) {
      lcd.clear();
      lcd.setCursor(0, 0);
      home(0);
    }

    delay(20);
  }
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.createChar(0, fullChar);
  lcd.createChar(1, upArrow);
  lcd.createChar(2, downArrow);

  // display start animation
  lcd.setCursor(0, 0);
  for (int i = 0; i <= lcdColumns; i++) {
    lcd.setCursor(i, 0);
    lcd.write(0);
    lcd.setCursor(lcdColumns - i, 1);
    lcd.write(0);
    delay(50);
  }
  lcd.setCursor(0, 0);
  for (int i = 0; i <= lcdColumns; i++) {
    lcd.setCursor(i, 0);
    lcd.write(' ');
    lcd.setCursor(lcdColumns - i, 1);
    lcd.write(' ');
    delay(50);
  }

  lcd.clear();

  home(0);
}

void loop() {}
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include <string>

void home(int num);
void error(String line1);
float stringToNum(String str);

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

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
Button plusButton = Button(24);
Button minusButton = Button(2);
Button multiplyButton = Button(18);
Button divideButton = Button(19);
Button fnButton = Button(33);

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

float stringToNum(std::string str) {
  try {
    return std::stof(str);
  } catch (...) {
    error("Invalid number!");
  }
}

void home(int num) {
  lcd.clear();
  lcd.setCursor(0, 0);

  // set initial number if available
  std::string inputStr
  if (num != 0) {
    inputStr = std::to_string(num);
    lcd.print(num);

  } else {
    inputStr = "0";
  }

  while (true) {
    for (int i = 0; i < 10; i++) {
      if (numberButtons[i].isPressed()) {
        lcd.print(i);
        inputStr += std::to_string(i);
      }
    }

    if (dotButton.isPressed()) {
      lcd.print(".");
      inputStr += ".";
    }

    if (clearButton.isPressed()) {
      home(0);
    }

    if (plusButton.isPressed()) {
    }

    delay(10);
  }
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  home(0);
}

void loop() {}
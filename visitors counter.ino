
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define IR_IN 2
#define IR_OUT 3

int lastStatusIrIn =  -1;
int lastStatusIrOut =  -1;

long walkOutTime;
long walkInTime;

int counter_in = 0;
int counter_out = 0;

bool isWalkIn = false;
bool isWalkOut = false;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("IN  = ");
  lcd.setCursor(0, 1);
  lcd.print("OUT = ");
  pinMode(IR_IN, INPUT);
  pinMode(IR_OUT, INPUT);
}

void loop() {
  checkWalkIn();
  checkWalkOut();

  if (isWalkIn && isWalkOut) {
    lcdUpdate();
    isWalkIn = false;
    isWalkOut = false;
  }
}

void checkWalkIn() {

  if (lastStatusIrIn != digitalRead(IR_IN)) {
    lastStatusIrIn = digitalRead(IR_IN);

    if (digitalRead(IR_IN) == LOW && isWalkIn == false && digitalRead(IR_OUT) == HIGH) {
      walkInTime = millis();
      isWalkIn = true;
    }
  }

}

void checkWalkOut() {

  if (lastStatusIrOut != digitalRead(IR_OUT)) {
    lastStatusIrOut = digitalRead(IR_OUT);

    if (digitalRead(IR_OUT) == LOW && isWalkOut == false && digitalRead(IR_IN) == HIGH) {
      walkOutTime = millis();
      isWalkOut = true;
    }
  }

}

void lcdUpdate() {
  if (walkInTime - walkOutTime < 0) {
    counter_in++;
    lcd.setCursor(6, 0);
    lcd.print(counter_in);
  }
  if (walkOutTime - walkInTime < 0) {
    counter_out++;
    lcd.setCursor(6, 1);
    lcd.print(counter_out);
  }
}

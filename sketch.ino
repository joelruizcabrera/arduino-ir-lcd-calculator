/*
 * ARDUINO TASCHENRECHNER
 * --------------------------
 * Ein Projekt von
 *   -  Tobias Bumann
 *   -  Jannik Bauer
 *   -  Joel Schwegmann
 *
 * GIT REPO
 *   - https://github.com/joelruizcabrera/arduino-ir-lcd-calculator
 *
 * © 2022 - Joel Schwegmann
 */


#include <IRremote.h>
#include <LiquidCrystal.h>
#include <math.h>

int Contrast=100;

// PIN Konfiguration
const int
        CR = 6, 
        RS = 12,
        EN = 11,
        D4 = 10,
        D5 = 9,
        D6 = 8,
        D7 = 7,
        REC = 13;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

//Deklarierung des IR Pins
IRrecv irrecv(REC);
decode_results results;

String
        num1 = "0",
        num2 = "0",
        optr = "=",
        spaceStr = "                ";

void setup() {                      // SETUP
    Serial.begin(9600);
    analogWrite(CR,Contrast);
    lcd.begin(16, 2);
    irrecv.enableIRIn();
}

/*
Switch/Case um die Eingabe ID's der Infrarot Schnittstelle zu übersetzen
*/

void inputWatchEvent(long charValue) {
    switch(charValue) {
        case -217252096:            // Zahlen Kombo
            mergeStr("1");
            break;
        case -417792256:
            mergeStr("2");
            break;
        case -1587609856:
            mergeStr("3");
            break;
        case -150405376:
            mergeStr("4");
            break;
        case -484638976:
            mergeStr("5");
            break;
        case -1520763136:
            mergeStr("6");
            break;
        case -1119682816:
            mergeStr("7");
            break;
        case -1387069696:
            mergeStr("8");
            break;
        case -1253376256:
            mergeStr("9");
            break;
        case -384368896:
            mergeStr("0");
            break;
        case -434503936:
            mergeStr(".");
            break;
        case -1169817856:            // Eingabe löschen
            num1 = "0";
            num2 = "0";
            optr = "=";
            break;
        case -1186529536:             // Operatoren
            operatorEvent("+");
            break;
        case -367657216:
            operatorEvent("-");
            break;
        case -1136394496:
            operatorEvent("/");
            break;
        case -1153106176:
            operatorEvent("x");
            break;
        case -1086259456:
            if(optr != "=")
                determineFormula("=");
            break;
        case -233963776:
            delLast();
            break;
        default:                    // Eingabe nicht valide => Keine funktion
            Serial.println("EINGABE NICHT VALIDE");
    }
}

void loop() {
    if (irrecv.decode()) {
        long result = irrecv.decodedIRData.decodedRawData;
        String val = String(result);
        inputWatchEvent(val.toInt());
        Serial.println(val);
        irrecv.resume();
    }
    // LCD Handler
    lcd.setCursor(0,0);
    lcd.print(optr + "  " + spaceStr.substring(num1.length() + 3) + num1);
    lcd.setCursor(0,1);
    lcd.print(spaceStr.substring(num2.length()) + num2);
}

void determineFormula(String op) {
    double no1 = num1.toDouble();
    double no2 = num2.toDouble();
    double calcVal = 0.0;

    // Rechenformeln des Taschenrechners

    if(optr == "+")
        calcVal = (no1 + no2);
    else if(optr == "-")
        calcVal = (no1 - no2);
    else if(optr == "x")
        calcVal = (no1 * no2);
    else if(optr == "/")
        calcVal = (no1 / no2);

    num1 = toString(calcVal);
    num2 = "0";
    optr = op;
}

String toString(double num) {
    return String(num);
}

void operatorEvent(String e) {
    if(num1 != "0" && num2 != "0") {
        determineFormula(e);
    }
    else if(num1 == "0") {
        num1 = num2;
        num2 = "0";
    }
    optr = e;
}

void mergeStr(String num) {
    if(optr == "=")
        num1 = "0";
    if(num != "."){
        if(num2.length() == 1 && num2 == "0")
            num2 = num;
        else
            num2 += num;
    }
    else {
        if(num2.charAt(num2.length()-1) != '.' && num2.indexOf('.') == -1)
            num2 += num;
    }
}

void delLast() {
    num2 = num2.substring(0, num2.length() - 1);
    if(num2 == "")
        num2 = "0";
}

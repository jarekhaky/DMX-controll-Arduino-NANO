/*
 * Ovládání 2 PAR RGBW světel na podcast
 * Součásti systému
Potenciometry:

Potenciometr A0: Ovládá celkový jas (master brightness) obou PAR světel.
Potenciometr A1: Ovládá RGB mix, tedy barvu světla.
Potenciometr A2: Ovládá vyvážení mezi RGB mixem a bílou LED.
Přepínače:

Přepínač 1 a 2: Jsou typu ON-OFF-ON a slouží k výběru ovládaného světla (PAR 1, PAR 2 nebo obě světla současně).
Startovací DMX kanály

PAR 1:
Ch1: Master Brightness
Ch2: Red
Ch3: Green
Ch4: Blue
Ch5: White

PAR 2:
Ch10: Master Brightness
Ch11: Red
Ch12: Green
Ch13: Blue
Ch14: White

Ovládání systému
Výběr ovládaného světla:

Obě světla současně: Přepínač 1 a přepínač 2 jsou v pozici OFF.
PAR 1: Přepínač 1 je v pozici ON, přepínač 2 je v pozici OFF.
PAR 2: Přepínač 1 je v pozici OFF, přepínač 2 je v pozici ON.
Nastavení jasu (Potenciometr A0):

Otočením potenciometru A0 nastavíte celkový jas vybraného světla nebo obou světel současně.
Otočte potenciometr doprava pro zvýšení jasu.
Otočte potenciometr doleva pro snížení jasu.
Nastavení barvy (Potenciometr A1):

Otočením potenciometru A1 nastavíte barvu světla pomocí RGB mixu.
Barva procházející celým spektrem (červená, zelená, modrá a jejich kombinace).
Nastavení vyvážení (Potenciometr A2):

Otočením potenciometru A2 nastavíte vyvážení mezi RGB mixem a bílou LED.
Pro hodnoty A2 ≤ 512: Bílá LED je na plné intenzitě a RGB mix roste od 0 do 255.
Pro hodnoty A2 > 512: RGB mix je na plné intenzitě a bílá LED klesá od 255 do 0.
Příklad použití
Nastavení jasu pro obě světla:

Ujistěte se, že oba přepínače jsou v pozici OFF.
Otočte potenciometr A0 na požadovanou úroveň jasu.
Nastavení rozdílných barev pro každé světlo:

Nastavte přepínač 1 do pozice ON a přepínač 2 do pozice OFF.
Otočte potenciometr A1 na požadovanou barvu pro PAR 1.
Nastavte přepínač 1 do pozice OFF a přepínač 2 do pozice ON.
Otočte potenciometr A1 na požadovanou barvu pro PAR 2.
Nastavení vyvážení pro obě světla:

Ujistěte se, že oba přepínače jsou v pozici OFF.
Otočte potenciometr A2 na požadovanou úroveň vyvážení mezi RGB mixem a bílou LED.
 */

#include <DMXSerial.h>

// Potenciometry a přepínače
const int potChannel1 = A0; // Master brightness (kanál 1 nebo 10)
const int potHue = A1;      // RGB mix (kanály 2, 3, 4 nebo 11, 12, 13)
const int potBalance = A2;  // Balance mezi RGB a bílou LED (kanál 5 nebo 14)
const int switchPin1 = 2;   // Poloha 1
const int switchPin2 = 3;   // Poloha 2
const int dePin = 9;        // DE pin pro SN75176

// Startovací DMX kanály pro PAR světla
const int PAR1Start = 1;
const int PAR2Start = 10;

// Proměnné pro aktuální stav
int switchState = 0; // 0 = oba PAR, 1 = PAR1, 2 = PAR2
int lastPotValues[3] = {0, 0, 0}; // Poslední hodnoty potenciometrů

// Uchovávání individuálních hodnot pro každý PAR
int par1Hue = 0, par2Hue = 0;
int par1Balance = 512, par2Balance = 512;

// Nastavení citlivosti (hysteréze) potenciometrů
const int potThreshold = 15; // Citlivost

bool dmxUpdated[3] = {false, false, false}; // Flag pro změnu potenciometrů

// Převod HUE na RGB
void HUEtoRGB(float hue, int &r, int &g, int &b) {
    float h = fmod(hue, 360.0) / 60.0;
    float c = 255;
    float x = (1 - abs(fmod(h, 2) - 1)) * c;

    if (h < 1) { r = c; g = x; b = 0; }
    else if (h < 2) { r = x; g = c; b = 0; }
    else if (h < 3) { r = 0; g = c; b = x; }
    else if (h < 4) { r = 0; g = x; b = c; }
    else if (h < 5) { r = x; g = 0; b = c; }
    else { r = c; g = 0; b = x; }
}

void setup() {
    DMXSerial.init(DMXController);
    pinMode(switchPin1, INPUT_PULLUP);
    pinMode(switchPin2, INPUT_PULLUP);
    pinMode(dePin, OUTPUT);
    digitalWrite(dePin, LOW);
}

void readInputs() {
    int currentPotValues[3];
    currentPotValues[0] = analogRead(potChannel1);
    currentPotValues[1] = analogRead(potHue);
    currentPotValues[2] = analogRead(potBalance);

    for (int i = 0; i < 3; i++) {
        if (abs(currentPotValues[i] - lastPotValues[i]) > potThreshold) {
            dmxUpdated[i] = true;
            lastPotValues[i] = currentPotValues[i];
        }
    }

    bool pin1State = digitalRead(switchPin1);
    bool pin2State = digitalRead(switchPin2);

    if (pin1State == HIGH && pin2State == LOW) {
        switchState = 1;
    } else if (pin1State == LOW && pin2State == HIGH) {
        switchState = 2;
    } else {
        switchState = 0;
    }
}

void sendDMX() {
    digitalWrite(dePin, HIGH); // Aktivace dePin před odesláním DMX

    int startChannel1 = (switchState == 2) ? 0 : PAR1Start;
    int startChannel2 = (switchState == 1) ? 0 : PAR2Start;

    for (int i = 0; i < 3; i++) {
        if (!dmxUpdated[i]) continue;
        dmxUpdated[i] = false;

        if (i == 0) { // Master brightness
            int brightness = map(lastPotValues[0], 0, 1023, 0, 255);
            if (startChannel1 > 0) DMXSerial.write(startChannel1, brightness);
            if (startChannel2 > 0) DMXSerial.write(startChannel2, brightness);
        } else if (i == 1) { // RGB mix
            float hue = map(lastPotValues[1], 0, 1023, 0, 360);
            if (switchState == 0 || switchState == 1) par1Hue = hue;
            if (switchState == 0 || switchState == 2) par2Hue = hue;

            int r1, g1, b1, r2, g2, b2;
            HUEtoRGB(par1Hue, r1, g1, b1);
            HUEtoRGB(par2Hue, r2, g2, b2);

            if (switchState == 0 || switchState == 1) {
                int balance = par1Balance;
                int whiteValue, rgbValue;
                if (balance <= 512) {
                    whiteValue = 255;
                    rgbValue = map(balance, 0, 512, 0, 255);
                } else {
                    whiteValue = map(balance, 512, 1023, 255, 0);
                    rgbValue = 255;
                }
                r1 = r1 * rgbValue / 255;
                g1 = g1 * rgbValue / 255;
                b1 = b1 * rgbValue / 255;
                DMXSerial.write(startChannel1 + 1, r1);
                DMXSerial.write(startChannel1 + 2, g1);
                DMXSerial.write(startChannel1 + 3, b1);
                DMXSerial.write(startChannel1 + 4, whiteValue);
            }
            if (switchState == 0 || switchState == 2) {
                int balance = par2Balance;
                int whiteValue, rgbValue;
                if (balance <= 512) {
                    whiteValue = 255;
                    rgbValue = map(balance, 0, 512, 0, 255);
                } else {
                    whiteValue = map(balance, 512, 1023, 255, 0);
                    rgbValue = 255;
                }
                r2 = r2 * rgbValue / 255;
                g2 = g2 * rgbValue / 255;
                b2 = b2 * rgbValue / 255;
                DMXSerial.write(startChannel2 + 1, r2);
                DMXSerial.write(startChannel2 + 2, g2);
                DMXSerial.write(startChannel2 + 3, b2);
                DMXSerial.write(startChannel2 + 4, whiteValue);
            }
        } else if (i == 2) { // Balance
            int balance = lastPotValues[2];
            if (switchState == 0 || switchState == 1) par1Balance = balance;
            if (switchState == 0 || switchState == 2) par2Balance = balance;

            int whiteValue1, rgbValue1, whiteValue2, rgbValue2;
            if (par1Balance <= 512) {
                whiteValue1 = 255;
                rgbValue1 = map(par1Balance, 0, 512, 0, 255);
            } else {
                whiteValue1 = map(par1Balance, 512, 1023, 255, 0);
                rgbValue1 = 255;
            }
            if (par2Balance <= 512) {
                whiteValue2 = 255;
                rgbValue2 = map(par2Balance, 0, 512, 0, 255);
            } else {
                whiteValue2 = map(par2Balance, 512, 1023, 255, 0);
                rgbValue2 = 255;
            }

            int r1, g1, b1, r2, g2, b2;
            HUEtoRGB(par1Hue, r1, g1, b1);
            HUEtoRGB(par2Hue, r2, g2, b2);
            r1 = r1 * rgbValue1 / 255;
            g1 = g1 * rgbValue1 / 255;
            b1 = b1 * rgbValue1 / 255;
            r2 = r2 * rgbValue2 / 255;
            g2 = g2 * rgbValue2 / 255;
            b2 = b2 * rgbValue2 / 255;

            if (switchState == 0 || switchState == 1) {
                DMXSerial.write(startChannel1 + 1, r1);
                DMXSerial.write(startChannel1 + 2, g1);
                DMXSerial.write(startChannel1 + 3, b1);
                DMXSerial.write(startChannel1 + 4, whiteValue1);
            }
            if (switchState == 0 || switchState == 2) {
                DMXSerial.write(startChannel2 + 1, r2);
                DMXSerial.write(startChannel2 + 2, g2);
                DMXSerial.write(startChannel2 + 3, b2);
                DMXSerial.write(startChannel2 + 4, whiteValue2);
            }
        }
    }

    delay(100);//Krátké zpoždění po odeslání DMX
    digitalWrite(dePin, LOW); // Deaktivace dePin po odeslání DMX
}

void loop() {
    readInputs();
    sendDMX();
}

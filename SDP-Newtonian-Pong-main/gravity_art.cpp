#include <stdio.h>
#include <math.h>
#include <string.h>
#include <Windows.h>
#include <iostream>
#include <cstring>
#include <FEHLCD.h>
#include <FEHImages.h>
#include <FEHUtility.h>
#include <FEHRandom.h>
#include <FEHSD.h>
//void gamerun();
void gravity_art() {
    LCD.WriteLine("Hello World!");
    LCD.SetFontColor(RED);
    LCD.FillCircle(140,140,20);
    while (1) {
        LCD.Update();
        // Never end
    }
}
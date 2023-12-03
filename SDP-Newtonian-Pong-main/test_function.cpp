#include <stdio.h>
#include <math.h>
#include <string.h>
#include <Windows.h>
#include <iostream>
#include <cstring>
#include <FEHLCD.h>
//#include <FEHImage.h>
#include <FEHUtility.h>
#include <FEHRandom.h>
#include <FEHSD.h>

void test_print() {
    LCD.WriteLine("Hello World!, :(");
    while (1) {
        LCD.Update();
        // Never end
    }
}
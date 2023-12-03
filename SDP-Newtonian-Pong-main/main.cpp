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
#include <fstream>
extern void gamerun(int num_planets, bool pvp, int dif, bool aivai);
extern bool settings(int* num_planets, bool* pvp, int* dif, bool* aivai);
extern bool run();
int main() {
    int num_p;
    bool pvp;
    bool aivai = false;
    int difficulty; //ranges from 1-3
    while (run()) {
        while (settings(&num_p, &pvp, &difficulty, &aivai)) {
            gamerun(num_p, pvp, difficulty, aivai);
        }
    }
    return 0;
}

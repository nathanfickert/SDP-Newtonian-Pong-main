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
#define scrn_h 240
#define scrn_w 320
class ClickButton { //for standertising buttons
    private:
        int xpos;
        int ypos;
        int width;
        int height;
        unsigned int color;
        unsigned int altcolor;
        unsigned int offcolor;
        float clicktime = 1;
        float timer = 0;
    public:
        bool maxhit = false;
        ClickButton(int xpos, int ypos, int width, int height, unsigned int color, unsigned int altcolor, unsigned int offcolor);
        bool Click(float x, float y);
        void Draw();
        void DrawArrow(int direction);
};
class ToggleButton { //for standertising buttons
    private:
        int xpos;
        int ypos;
        int width;
        int height;
        unsigned int color;
        unsigned int oncolor;
    public:
        bool toggled;
        bool clicked;
        ToggleButton(int xpos, int ypos, int width, int height, bool toggled, unsigned int color, unsigned int oncolor);
        bool Click(float x, float y);
        void Draw();
};
bool settings(int* num_planets, bool* pvptrue, int* dif, bool* aivai) { //settings.cpp created by Artem Vovchenko
    ClickButton up((scrn_w - 120), 40, 60, 30, WHITE, LIGHTBLUE, SLATEGRAY); //this creates the buttons
    ClickButton down((scrn_w - 120), 115, 60, 30, WHITE, LIGHTBLUE, SLATEGRAY);
    ClickButton play((scrn_w - 140), (scrn_h - 60), 90, 30, GREEN, LIGHTGREEN, DARKGREEN);
    ClickButton back(50, (scrn_h - 60), 90, 30, RED, MAGENTA, DARKRED);
    ToggleButton easydif(45, 100, 20, 20, true, WHITE, SLATEGRAY);
    ToggleButton normaldif(45, 125, 20, 20, false, WHITE, SLATEGRAY);
    ToggleButton harddif(45, 150, 20, 20, false, WHITE, SLATEGRAY);
    ToggleButton pvp(70, 25, 60, 60, true, WHITE, SLATEGRAY);
    ToggleButton cpu(70, 105, 60, 60, false, WHITE, SLATEGRAY);
    ToggleButton ai1(135, 112, 20, 20, true, WHITE, SLATEGRAY);
    ToggleButton ai2(135, 137, 20, 20, false, WHITE, SLATEGRAY);
    //create pictures
    FEHImage easy;
    FEHImage normal;
    FEHImage hard;
    FEHImage pvpon;
    FEHImage cpuon;
    FEHImage pvpoff;
    FEHImage cpuoff;
    FEHImage oneai;
    FEHImage aionly;
    easy.Open("files/easyFEH.pic");
    normal.Open("files/normalFEH.pic");
    hard.Open("files/hardFEH.pic");
    pvpon.Open("files/PVPoffFEH.pic");
    cpuon.Open("files/AIoffFEH.pic");
    pvpoff.Open("files/PVPFEH.pic");
    cpuoff.Open("files/AIFEH.pic");
    oneai.Open("files/1vaiFEH.pic");
    aionly.Open("files/aionlyFEH.pic");
    *num_planets = 1;
    *pvptrue = true;
    *dif = 3;
    float fps = 480;
    float x = 0, y = 0;
    LCD.Clear();
    while(true) {
        LCD.SetFontColor(BLACK); //draw the screen
        LCD.FillRectangle((scrn_w - 110), 85, 50, 20);
        LCD.SetFontColor(WHITE);
        LCD.WriteAt("# of Planets", (scrn_w - 160), 150);
        if (*num_planets > 9) { //write number of planets
            LCD.WriteAt((*num_planets), (scrn_w - 101), 85);
        }
        else {
            LCD.WriteAt((*num_planets), (scrn_w - 97), 85);
        }
        if (*num_planets == 0) {
            down.maxhit = true;
            up.maxhit = false;
        }
        else if (*num_planets == 10) {
            up.maxhit = true;
            down.maxhit = false;
        }
        else {
            up.maxhit = false;
            down.maxhit = false;
        }
        up.Draw(); // draw the buttons
        up.DrawArrow(1);
        down.Draw();
        down.DrawArrow(-1);
        pvp.Draw();
        cpu.Draw();
        back.Draw();
        play.Draw();
        easydif.Draw();
        normaldif.Draw();
        harddif.Draw();
        ai1.Draw();
        ai2.Draw();
        easy.Draw(45, 100);
        normal.Draw(45, 125);
        hard.Draw(45, 150);
        oneai.Draw(135, 112);
        aionly.Draw(135, 137);
        if (pvp.toggled) {
            pvpon.Draw(70, 25);
            cpuoff.Draw(70, 105);
        }
        else {
            pvpoff.Draw(70, 25);
            cpuon.Draw(70, 105);
        }
        LCD.SetFontColor(GREEN);
        LCD.WriteAt("Play", (scrn_w - 120), (scrn_h - 53));
        LCD.SetFontColor(RED);
        LCD.WriteAt("Back", 70, (scrn_h - 53));
        while (LCD.Touch(&x,&y)); // make sure to wait before clicking again
        while(!LCD.Touch(&x,&y)); //wait for click
        if (pvp.Click(x, y)) { //check for click
            cpu.toggled = false;
        }
        if (cpu.Click(x, y)) {
            pvp.toggled = false;
        }
        if (ai1.Click(x, y)) {
            ai2.toggled = false;
        }
        if (ai2.Click(x, y)) {
            ai1.toggled = false;
        }
        if (easydif.Click(x, y)) {
            normaldif.toggled = false;
            harddif.toggled = false;
        }
        if (normaldif.Click(x, y)) {
            easydif.toggled = false;
            harddif.toggled = false;
        }
        if (harddif.Click(x, y)) {
            easydif.toggled = false;
            normaldif.toggled = false;
        }
        if (up.Click(x, y)) {
            (*num_planets)++;
        }
        if (down.Click(x, y)) {
            (*num_planets)--;
        }
        if (back.Click(x, y)) {
            return false;
        }
        if (play.Click(x ,y)) {
            return true;
        }
        if (pvp.toggled) { // check the status
            *pvptrue = true;
            *aivai = false;
        }
        else {
            *pvptrue = false;
            if (ai1.toggled) {
                *aivai = false;
            }
            else if (ai2.toggled) {
                *aivai = true;
            }
        }
        if (easydif.toggled) {
            *dif = 1;
        }
        else if (normaldif.toggled) {
            *dif = 2;
        }
        else if (harddif.toggled) {
            *dif = 3;
        }
        LCD.Update();
    }
}
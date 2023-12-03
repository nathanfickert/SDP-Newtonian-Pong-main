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

class Paddle { //Entire gamerun.cpp is made by Artem Vovchenko
    public:
        int height = 40;
        int width = 5;
        int dist;
        float mvmt = 2;
        float pos = 120;
        int vel = 0; //for transfering speed to ball
        Paddle(int d);
        void Move(int direction); // 1 is up, 0 is down
};
class Stats { // tracks stats
    public:
        int p1w = 0;
        int p2w = 0;
        int p1wtotal = 0;
        int p2wtotal = 0;
        float tottime = 0;
        float sessiontime = 0;
        void ReadFile(char* filename);
        void SaveFile(char* filename);
        void ClearData(char* filename);
};
class Planet { //the planets
    public:
        float xpos;
        float ypos;
        float mass;
        float radius;
        float G = 6.67408; // a = Gm/R^2
        //Planet(float x, float y, float m, float r);
};
class Ball { // the ball
    public:
        int radius = 2;
        float xpos = 160;
        float ypos = 120;
        float xvel;
        float yvel;
        float xaccl =  0;
        float yaccl =  0;
        bool hit = false;
        bool score = false;
        Ball(float xv, float yv, float tick);
        void CalcAccl(Planet plnt, float tick);
        void CalcPos(float tick);
        void CheckCol(Paddle pddl, float tick);
        void CheckBounds(float tick);
        void CheckWin(Stats* stats, float tick);
        void Reset(float xv, float yv, float tick);
};


extern void calcpos(float xv, float yv, float* x, float* y, float tick);
extern float randvelx();
extern float randvely();
extern void generatePlanets(Planet* planets, int num);
extern int AImove(Paddle AIpddl, Ball ball, int dif);

#define MAXPLANETS 10

void gamerun(int num_planets, bool pvp, int dif, bool aivai) { // the input is from game settings :)
    std::cout << "gamerun started with " << num_planets << " planets" << std::endl;
    float fps = 480 * 4; //set up variables
    Paddle lpdl(5);
    Paddle rpdl(320 - (5 + lpdl.width));
    Ball ball(randvelx(), randvely(), fps);
    Stats stats;
    stats.ReadFile("files/save.txt");
    std::cout << "preplanet data made" << std::endl;
    Planet planet[MAXPLANETS];
    generatePlanets((Planet*)&planet, num_planets);
    unsigned int rpdlcol;
    unsigned int lpdlcol;
    float timestart = TimeNow();
    if (pvp) {
        rpdlcol = WHITE;
    }
    else {
        rpdlcol = RED;
    }
    if (aivai) {
        lpdlcol = RED;
    }
    else {
        lpdlcol = WHITE;
    }
    while (true) {
        //draw each frame
        LCD.Clear();
        //LCD.SetBackgroundColor(BLACK);
        LCD.SetFontColor(WHITE);
        LCD.WriteAt(stats.p1w, (scrn_w / 2 - 30), 40);
        LCD.WriteAt(stats.p2w, (scrn_w / 2 + 30), 40);
        LCD.SetFontColor(lpdlcol);
        LCD.FillRectangle(lpdl.dist, lpdl.pos, lpdl.width, lpdl.height);
        LCD.SetFontColor(rpdlcol);
        LCD.FillRectangle(rpdl.dist, rpdl.pos, rpdl.width, rpdl.height);
        LCD.SetFontColor(WHITE);
        LCD.FillCircle(ball.xpos, ball.ypos, ball.radius);
        for (int i = 0; i < num_planets; i++) {
            LCD.DrawCircle(planet[i].xpos,planet[i].ypos,planet[i].radius);
        }
        //check for input
        SHORT rustate = GetKeyState(79); // o
        SHORT rdstate = GetKeyState(76); // l
        SHORT lustate = GetKeyState(87); // w
        SHORT ldstate = GetKeyState(83); // s
        SHORT esc = GetKeyState(VK_ESCAPE); // esc key
        bool rupress = false, rdpress = false, lupress = false, ldpress = false;
        float timest = TimeNow();
        for (int i = 0; i < num_planets; i++) { //calculate the accelloration
            ball.CalcAccl(planet[i], fps);
        }
        //LCD.DrawLine(scrn_w/2,scrn_h/2,((scrn_w/2)-(50*pow(fps,2)*(-ball.xaccl))),((scrn_h/2)-(50*pow(fps,2)*(-ball.yaccl))));//accell vector
        Sleep(1 / fps); //to control the tick rate
        if (pvp) {
            if(rustate & 0x8000 && rpdl.pos > 0) { //check if buttons are pressed and stops if going beyond screen
                rpdl.Move(1);
            }
            if(rdstate & 0x8000 && rpdl.pos < (scrn_h - rpdl.height)) {
                rpdl.Move(0);
            }
        }
        else { //ai movement
            int dir = AImove(rpdl, ball, dif);
            if (dir == 1) {
                rpdl.Move(1);
            }
            else if (dir == -1) {
                rpdl.Move(0);
            }
        }
        if (aivai) {
            int ldir = AImove(lpdl, ball, dif);
            if (ldir == 1) {
                lpdl.Move(1);
            }
            else if (ldir == -1) {
                lpdl.Move(0);
            }
        }
        else {
            if(lustate & 0x8000 && lpdl.pos > 0) {
                lpdl.Move(1);
            }
            if(ldstate & 0x8000 && lpdl.pos < (scrn_h - lpdl.height)) {
                lpdl.Move(0);
            }
        }
        if(esc & 0x8000) { //exit game
            stats.SaveFile("files/save.txt");
            return;
        }
        stats.sessiontime = TimeNow() - timestart;
        ball.CheckCol(lpdl, fps); //ball calculations
        ball.CheckCol(rpdl, fps);
        ball.CheckBounds(fps);
        ball.CheckWin(&stats, fps);
        ball.CalcPos(fps);
        rpdl.vel = 0, lpdl.vel = 0;
        if (ball.score) {
            ball.Reset(randvelx(), randvely(), fps); //resetting the ball
            Sleep(0.5);
        }
        LCD.Update();
    }
}
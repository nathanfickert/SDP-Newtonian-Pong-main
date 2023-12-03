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
class Paddle { //Entire objects.cpp is made by Artem Vovchenko
    public:
        int height = 40;
        int width = 5;
        int dist;
        float mvmt = 2;
        float pos = 120;
        int vel = 0; //for transfering speed to ball
        Paddle(int d);
        void Move(int direction);
};

Paddle::Paddle(int d) {
    dist = d;
}

void Paddle::Move(int d) { // 1 is up, 0 is down
    if (d == 1) {
        pos -= mvmt;
        vel = -2;
    }
    else if (d == 0) {
        pos += mvmt;
        vel = 2;
    }
}

class Stats { //stats
    public:
        int p1w = 0;
        int p2w = 0;
        int p1wtotal = 0;
        int p2wtotal = 0;
        float tottime = 0; // in minutes
        float sessiontime = 0;
        void ReadFile(char* filename);
        void SaveFile(char* filename);
        void ClearData(char* filename);
};

void Stats::ReadFile(char* filename) { //read stats data
    std::ifstream data;
    data.open(filename);
    data >> p1wtotal >> p2wtotal >> tottime;
}

void Stats::SaveFile(char* filename) { //save stats data
    p1wtotal += p1w;
    p2wtotal += p2w;
    tottime += sessiontime / 60;
    std::ofstream data;
    data.open(filename);
    data << p1wtotal << std::endl << p2wtotal << std::endl << tottime;
}

void Stats::ClearData(char* filename) { //clear data
    p1wtotal = 0;
    p2wtotal = 0;
    tottime = 0;
    std::ofstream data;
    data.open(filename);
    data << p1wtotal << std::endl << p2wtotal << std::endl << tottime;
}

class Planet { //the planets
    public:
        float xpos;
        float ypos;
        float mass;
        float radius;
        float G = 6.67408; // a = Gm/R^2
        //Planet(float x, float y, float m, float r);
};
/*Planet::Planet(float x, float y, float m, float r) {
    xpos = x;
    ypos = y;
    mass = m;
    radius = r;
}*/

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

Ball::Ball(float xv, float yv, float tick) { //set up the ball
    xvel = xv / tick;
    yvel = yv / tick;
}

void Ball::Reset(float xv, float yv, float tick) { //reset the ball
    hit = false;
    score = false;
    xpos = 160;
    ypos = 120;
    xvel = xv / tick;
    yvel = yv / tick;
}

void Ball::CalcAccl(Planet plnt, float tick) {//calculate acceloration from a given planet
    float R = sqrt(pow((plnt.xpos - xpos),2) + pow((plnt.ypos - ypos),2)); //calc for distance from planet center;
    float scaleR = pow(R,0.7);//scale it
    float cosratio = (plnt.xpos - xpos) / R; //calculation ratios
    float sinratio = (plnt.ypos - ypos) / R;
    float accel = plnt.G * plnt.mass / pow(scaleR,2); //calc accelloration
    float maxval = plnt.mass * 0.5;
    if (R > maxval) {
        xaccl += (accel * cosratio) / pow(tick,2.0);
        yaccl += (accel * sinratio) / pow(tick,2.0);
    }
}

void Ball::CalcPos(float tick) { //calculate position of ball for next tick
    if (xpos < 120 && xpos > 2 && xvel < 0) { //add a little nudge to the endzones
        xaccl += -1 / (xpos * pow(tick,2.0));
    }
    else if (xpos > scrn_w - 120 && xpos < scrn_w - 2 && xvel > 0) { //add a little nudge to the endzones
        xaccl += 1 / ((scrn_w - xpos) * pow(tick,2.0));
    }
    if (!hit) {
        //xpos = xpos + xvel * tick; for no planet calc
        //ypos = ypos + yvel * tick;
        xpos = xpos + xvel * tick + (0.5 * xaccl * pow(tick,2));
        ypos = ypos + yvel * tick + (0.5 * yaccl * pow(tick,2));
    }
    hit = false;
    xvel += xaccl * tick;
    yvel += yaccl * tick;
    xaccl = 0;
    yaccl = 0;
}

void Ball::CheckCol(Paddle pddl, float tick) { //check for collisions with the paddle
    for (int i = 0; i < tick; i++) {
        float tempxpos = xpos + xvel * i + (0.5 * xaccl * pow(i,2.0)); //check for all mini ticks to make sure no spots are missed
        float tempypos = ypos + yvel * i + (0.5 * yaccl * pow(i,2.0));
        if (tempxpos <= (pddl.dist + pddl.width + radius) && tempxpos >= (pddl.dist - radius)) { //check if hits the paddle
            if (tempypos >= pddl.pos && tempypos <= (pddl.pos + pddl.height)) {
                if (xvel < 0) {
                    xvel = (-xvel) + (0.02 / tick);
                }
                else if (xvel > 0) {
                    xvel = (-xvel) - (0.02 / tick);
                }
                float tempvel = yvel;
                yvel = yvel + (((pddl.mvmt / 4.0) * pddl.vel) / tick);
                hit = true;
                xpos = xpos + (-xvel * i) + (xvel * (tick - i)) + (0.5 * xaccl * pow(i,2.0));
                ypos = ypos + (tempvel * i) + (yvel * (tick - i)) + (0.5 * yaccl * pow(i,2.0));
                break;
            }
        }
    }
}

void Ball::CheckBounds(float tick) { //check if the ball hits bounds
    for (int i = 0; i < tick; i++) {
        float tempxpos = xpos + xvel * i + (0.5 * xaccl * pow(i,2.0)); //check for all mini ticks to make sure no spots are missed
        float tempypos = ypos + yvel * i + (0.5 * yaccl * pow(i,2.0));
        if ((tempypos - radius - 1) < 0) { //check if hits the wall
            yvel = -yvel;
            hit = true;
            ypos = ypos + (-yvel * i) + (yvel * (tick - i)) + (0.5 * yaccl * pow(i,2.0));
            xpos = xpos + xvel * tick;
            break;
        }
        else if ((tempypos + radius + 1) > scrn_h) {
            yvel = -yvel;
            hit = true;
            ypos = ypos + (-yvel * i) + (yvel * (tick - i)) + (0.5 * yaccl * pow(i,2.0));
            xpos = xpos + xvel * tick;
            break;
        }
    }
}

void Ball::CheckWin(Stats* stats, float tick) { //check if any player scored
    for (int i = 0; i < tick; i++) {
        if ((xpos + (xvel * i) - radius - 1) < 0) { //check if hits the wall
            hit = true;
            score = true;
            stats->p2w = stats->p2w + 1;
            break;
        }
        else if ((xpos + (xvel * i) + radius + 1) > scrn_w) {
            hit = true;
            score = true;
            stats->p1w = stats->p1w + 1;
            break;
        }
    }
}

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

ClickButton::ClickButton(int x, int y, int w, int h, unsigned int incolor, unsigned int inaltcolor, unsigned int inoffcolor) {
    xpos = x;
    ypos = y;
    width = w;
    height = h;
    color = incolor;
    altcolor = inaltcolor;
    offcolor = inoffcolor;
}

bool ClickButton::Click(float x, float y) { //check if button is clicked
    bool xhit = x <= (xpos + width) && x >= xpos;
    bool yhit = y <= (ypos + height) && y >= ypos;
    if (xhit && yhit && !maxhit) {
        return true;
        timer = TimeNow();
    }
    else {
        return false;
    }
}

void ClickButton::Draw() {
    if (maxhit) {
        LCD.SetFontColor(offcolor);
    }
    else if (10 > TimeNow() - timer) {
        LCD.SetFontColor(altcolor);
    }
    else {
        LCD.SetFontColor(color);
    }
    LCD.DrawRectangle(xpos, ypos, width, height);
}

void ClickButton::DrawArrow(int direction) {
    float cursorx, cursory;
    if (maxhit) {
        LCD.SetFontColor(offcolor);
    }
    else if (10 > TimeNow() - timer) {
        LCD.SetFontColor(altcolor);
    }
    else {
        LCD.SetFontColor(color);
    }
    int linepos[3][2];
    if (direction == 1) {
        linepos[0][0] = xpos + (width / 3.0);
        linepos[0][1] = ypos + (height - height / 3.0);
        linepos[1][0] = xpos + (width / 2.0) - 1;
        linepos[1][1] = ypos + (height / 3.0) + 1;
        linepos[2][0] = xpos + (width - width / 3.0) - 1;
        linepos[2][1] = ypos + (height - height / 3.0) + 1;
        LCD.DrawLine(linepos[0][0], linepos[0][1], linepos[1][0], linepos[1][1]);
        LCD.DrawLine(linepos[1][0], linepos[1][1], linepos[2][0], linepos[2][1]);
    }
    else if (direction == -1) {
        linepos[0][0] = xpos + (width / 3.0);
        linepos[0][1] = ypos + (height / 3.0) + 2;
        linepos[1][0] = xpos + (width / 2.0) - 1;
        linepos[1][1] = ypos + (height - height / 3.0) + 1;
        linepos[2][0] = xpos + (width - width / 3.0) - 1;
        linepos[2][1] = ypos + (height / 3.0) + 1;
        LCD.DrawLine(linepos[0][0], linepos[0][1], linepos[1][0], linepos[1][1]);
        LCD.DrawLine(linepos[1][0], linepos[1][1], linepos[2][0], linepos[2][1]);
    }
}

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

ToggleButton::ToggleButton(int x, int y, int w, int h, bool t, unsigned int incolor, unsigned int inoncolor) {
    xpos = x;
    ypos = y;
    width = w;
    height = h;
    toggled = t;
    color = incolor;
    oncolor = inoncolor;
}

bool ToggleButton::Click(float x, float y) { //check if button is clicked
    bool xhit = x <= (xpos + width) && x >= xpos;
    bool yhit = y <= (ypos + height) && y >= ypos;
    if (xhit && yhit && !toggled) {
        toggled = !toggled;
        return true;
    }
    else {
        return false;
    }
}
void ToggleButton::Draw() {
    if (toggled) {
        LCD.SetFontColor(oncolor);
    }
    else {
        LCD.SetFontColor(color);
    }
    LCD.DrawRectangle(xpos, ypos, width, height);
}

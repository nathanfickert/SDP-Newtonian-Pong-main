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
#define scrn_h 240
#define scrn_w 320
//entire calcpos.cpp is created by Artem Vovchenko
void calcpos(float xv, float yv, float* x, float* y, float tick) {
    *x = *x + xv * tick;
    *y = *y + yv * tick;
}
void check_col_L(float xv, float yv, float xball, float yball, float paddlepos, int hpaddle, float tick) {
    for (int i = 0; i < tick; i++) {
        if ((xball + (xv * i)) <= 10) { // 5 is standard dist + 5 paddle width
            if ((yball + (yv * i)) >= paddlepos && (yball + (yv * i)) <= (paddlepos - hpaddle)) {

            }
        }
    }
}
float randvelx() {//random velocity x
    float vels;
    if ((rand() % 2) == 1) {
        vels = 2.2;
    }
    else {
        vels = -2.2;
    }
    return vels;
}
float randvely() {//random velocity y
    float vels;
    vels = ((rand() % 100) - 50) / 50.0;
    return vels;
}

class Planet { //the planets
    public:
        float xpos;
        float ypos;
        float mass;
        float radius;
        float G = 6.67408; // a = Gm/R^2
};

void generatePlanets(Planet* planetsdata, int num) {
    bool goodnumber = false; // make sure we get a good number
    for (int i = 0; i < num; i++) {
        float x, y;
        
        while (!goodnumber) {
            x = (rand() % (scrn_w - 139)) + 70;
            y = (rand() % (scrn_h - 119)) + 60;
            //not closer than 10 pixels to each other
            if (i == 0) {//dont wanna break the first planet
                goodnumber = true;
                std::cout << "goodnumber = true" << std::endl;
            }
            else {
                bool anybad = false;
                for(int j = 0; j < i; j++) {
                    float r = sqrt(pow((planetsdata[j].xpos - x),2) + pow((planetsdata[j].ypos - y), 2));
                    std::cout << "r is " << r << std::endl;
                    if (r < 10) {
                        anybad = true;
                        std::cout << "anybad = true" << std::endl;
                    }
                }
                if (!anybad) {
                    goodnumber = true;
                    std::cout << "goodnumber = true" << std::endl;
                }
            }
        }
        
        float m = ((rand() % (200)) + 50) / 100.0;
        planetsdata[i].xpos = x;
        planetsdata[i].ypos = y;
        planetsdata[i].mass = m;
        planetsdata[i].radius = m * 10;
        goodnumber = false;
        std::cout << "x = " << x << std::endl;
        std::cout << "y = " << y << std::endl;
        std::cout << "m = " << m << std::endl;
    }
    std::cout << "values generated" << std::endl;
}

class Paddle {
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
        //void CalcAccl(Planet plnt, float tick);
        void CalcPos(float tick);
        void CheckCol(Paddle pddl, float tick);
        void CheckBounds(float tick);
        //void CheckWin(Stats* stats, float tick);
        void Reset(float xv, float yv, float tick);
};


int AImove(Paddle AIpddl, Ball ball, int dif) { // -1, 0, or 1 output, This is the code for the ai
    float ytarget;
    float ytargetaccounted;
    bool rightpdl = true;
    if (AIpddl.dist > scrn_w / 2) {
        rightpdl = true;
    }
    else if (AIpddl.dist < scrn_w / 2) {
        rightpdl = false;
    }
    if (((ball.xpos > scrn_w / 2 && !rightpdl) || (ball.xpos < scrn_w / 2 && rightpdl)) && dif == 1) {
        return 0; //make the ai only try to catch the ball only when it is in its half of the screen, this makes the ai easier for easy difficulty
    }
    if ((ball.xvel > 0 && rightpdl) || (ball.xvel < 0 && !rightpdl)) { // meet the ball to its impact point
        float timehit = (AIpddl.dist - ball.xpos) / ball.xvel;
        ytarget = ball.ypos + (ball.yvel * timehit); //calculate point of impact
        if (ytarget < 0) {
            ytarget = -ytarget;
        }
        else if (ytarget > scrn_h) {
            ytarget = scrn_h - (ytarget - scrn_h);
        }
        ytargetaccounted = ytarget - (AIpddl.height / 2);
        bool closeenough = false;
        if (abs(ytargetaccounted - AIpddl.pos) < 1) {
            closeenough = true; // this is to prevent it from going up and down continously
        }
        //LCD.SetFontColor(RED);// used for looking at what AI is thinking
        //LCD.FillCircle(AIpddl.dist, ytarget, 2);
        if (ytargetaccounted > AIpddl.pos && AIpddl.pos < (scrn_h - AIpddl.height) && !closeenough) {
            return -1;
        }
        else if (ytargetaccounted < AIpddl.pos && AIpddl.pos > 0 && !closeenough) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else if ((ball.xvel > 0 && !rightpdl) || (ball.xvel < 0 && rightpdl)) { // if ball is not moving towards the ai
        if (dif == 3) { // it only does this for the hard difficulty 
            ytarget = ball.ypos;
            ytargetaccounted = ytarget - (AIpddl.height / 2);
            bool closeenough = false;
            if (abs(ytargetaccounted - AIpddl.pos) < 2) {
                closeenough = true;
            }
            if (ytargetaccounted > AIpddl.pos && AIpddl.pos < (scrn_h - AIpddl.height) && !closeenough) {
                return -1;
            }
            else if (ytargetaccounted < AIpddl.pos && AIpddl.pos > 0 && !closeenough) {
                return 1;
            }
            else {
                return 0;
            }
        }
        else {
            return 0; // doesnt do this for normal or easy difficulty
        }
    }
}
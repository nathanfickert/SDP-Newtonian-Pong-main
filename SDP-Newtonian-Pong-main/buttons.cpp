#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHRandom.h"
#include "FEHImages.h"
// #include "FEHBuzzer.h"

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

//File created and coded by Nathan Fickert

bool run() {
    Sleep(0.1);
    //Loop through everything
    while (true){

    //Declare variables
    int x1,y1,pw1 = 1, pw2 = 4;
    float time = 24;

    //Create Start Button
    LCD.Clear();
    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(125,25,70,25);

    //Create Display Stats Button
    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(125,75,70,25);

    //Create Display Intructions Button
    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(83,125,154,25);
    
    //Create a View Credits Button
    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(113,175,94,25);

    //Add Words to Buttons
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Start",130,30);
    LCD.WriteAt("Stats",130,80);
    LCD.WriteAt("Instructions",88,130);
    LCD.WriteAt("Credits",118,180);
    LCD.SetFontColor(RED);
    LCD.DrawRectangle(1, 214, 60, 25);
    LCD.WriteAt("Quit", 5, 220);

    //Add stars to background of home screen, the background randomly generates differently each time
    for (int i = 0; i < 100; i++){
        int xrand,yrand;
        xrand = Random.RandInt();
        yrand = Random.RandInt();

        //Draw a colored pixel at the center of the screen
        LCD.SetFontColor(WHITE);
        LCD.DrawPixel(xrand,yrand);
    }

    //Add Planet Earth
    LCD.SetFontColor(BLUE);
    LCD.FillCircle(30,103,15);

    LCD.SetFontColor(GREEN);

    LCD.FillCircle(28,93,5);
    LCD.FillCircle(35,109,5);

    LCD.FillRectangle(20,105,10,6);
    LCD.DrawPixel(18,107);
    LCD.DrawPixel(18,106);
    LCD.DrawPixel(18,105);
    LCD.DrawPixel(18,104);
    LCD.DrawPixel(18,103);
    LCD.DrawPixel(18,102);
    LCD.DrawPixel(19,107);
    LCD.DrawPixel(19,106);
    LCD.DrawPixel(19,105);
    LCD.DrawPixel(19,104);
    LCD.DrawPixel(19,103);
    LCD.DrawPixel(19,102);
    LCD.DrawPixel(20,104);
    LCD.DrawPixel(20,103);
    LCD.DrawPixel(21,104);
    LCD.DrawPixel(22,104);
    LCD.DrawPixel(23,104);

    LCD.FillRectangle(31,94,4,6);
    LCD.DrawPixel(32,99);
    LCD.DrawPixel(32,100);
    LCD.DrawPixel(33,99);
    LCD.DrawPixel(33,100);
    LCD.DrawPixel(34,99);
    LCD.DrawPixel(34,100);
    LCD.DrawPixel(34,101);
    LCD.DrawPixel(34,102);
    LCD.DrawPixel(35,99);
    LCD.DrawPixel(35,100);
    LCD.DrawPixel(35,101);
    LCD.DrawPixel(35,102);
    LCD.DrawPixel(35,103);
    LCD.DrawPixel(36,99);
    LCD.DrawPixel(36,99);
    LCD.DrawPixel(36,100);
    LCD.DrawPixel(36,101);
    LCD.DrawPixel(36,102);
    LCD.DrawPixel(36,103);

    LCD.SetFontColor(WHITE);
    LCD.FillRectangle(20,105,5,2);
    LCD.FillRectangle(34,113,5,2);
    LCD.FillRectangle(29,94,5,2);
    LCD.FillRectangle(35,101,4,3);   

    LCD.Update();
    
    //Set values to false
    bool play = false;
    bool stat = false;
    bool instr = false;
    bool cred = false;

    //Wait for touch
    while(!LCD.Touch(&x1,&y1));

    //Record where the user touched
    while(LCD.Touch(&x1,&y1));

    //Set value equal to true if the button was touched
    if ((x1 < 200 && x1 > 100) && (y1 < 55 && y1 > 25)){
        play = true;
    }
    if ((x1 < 200 && x1 > 100) && (y1 < 105 && y1 > 75)){
        stat = true;
    }
    if ((x1 < 240 && x1 > 65) && (y1 < 155 && y1 > 125)){
        instr = true;
    }
    if ((x1 < 200 && x1 > 100) && (y1 < 205 && y1 > 175)){
        cred = true;
    }
    if(x1 >= 1 && x1 <= 61 && y1 >= 214 && y1 <= 239) { // back and quit buttons added by Artem, modified by Nate
        return false;
    }
    
    //Run the game
    if (play) {
        LCD.Clear();
        /*LCD.SetFontColor(WHITE);
        LCD.WriteAt("Play Game Here",80,50);

        //Add stars to the background, randomly generated, different each time
        for (int i = 0; i < 100; i++){
            int xrand,yrand;
            xrand = Random.RandInt();
            yrand = Random.RandInt();

            //Draw a colored pixel at the center of the screen
            LCD.SetFontColor(WHITE);
            LCD.DrawPixel(xrand,yrand);
        }

        while(!LCD.Touch(&x1,&y1));
        while(LCD.Touch(&x1,&y1));*/
        return true;
    }

    //Display Stats Screen
    if (stat){
        x1 = 0, y1 = 0;
        LCD.Clear();
        Stats stats;
        stats.ReadFile("files/save.txt");
        //Display Message
        LCD.SetFontColor(WHITE);
        LCD.WriteAt("Player One Wins: ",52,77);
        LCD.WriteAt(stats.p1wtotal,252,77);
        LCD.WriteAt("Player Two Wins: ",52,107);
        LCD.WriteAt(stats.p2wtotal,252,107);
        LCD.WriteAt("Time Played (min): ",22,137);
        LCD.WriteAt(stats.tottime,246,137);
        LCD.SetFontColor(RED);
        LCD.DrawRectangle(50, 185, 90, 30);
        LCD.WriteAt("Back", 70, 192);
        LCD.SetFontColor(BLUE);
        LCD.DrawRectangle(190, 185, 90, 30);
        LCD.WriteAt("Clear", 205, 192);

        //Add stars to the background, randomly generated, different each time
        for (int i = 0; i < 100; i++){
            int xrand,yrand;
            xrand = Random.RandInt();
            yrand = Random.RandInt();

            //Draw a colored pixel at the center of the screen
            LCD.SetFontColor(WHITE);
            LCD.DrawPixel(xrand,yrand);
        }

        //Wait for touch
        while(true) {
            while(!LCD.Touch(&x1,&y1));
            if(x1 >= 50 && x1 <= 140 && y1 >= 185 && y1 <= 215) {
                break;
            }
            if(x1 >= 190 && x1 <= 280 && y1 >= 185 && y1 <= 215) {
                stats.ClearData("files/save.txt");
                break;
            }
        }
        //Continue and record where the touch occured
        while(LCD.Touch(&x1,&y1));
    }

    //Display Instructions
    if (instr) {

        // Buzzer.Beep();

        x1 = 0, y1 = 0;
        LCD.Clear();
        
        /*LCD.SetFontColor(WHITE);
        LCD.WriteAt("Newtonian Pong!!!",30,10);
        LCD.WriteAt("Pong with a twist!",30,40);
        LCD.WriteAt("Gravitational Bodies are",0,70);
        LCD.WriteAt("added to the playing",10,100);
        LCD.WriteAt("field. Player one uses w",0,130);
        LCD.WriteAt("and s keys. Player two",5,160);
        LCD.WriteAt("uses top and bottom arrows",0,190);
        LCD.WriteAt("Goodluck!",70,220);*/

        FEHImage instructions;
        instructions.Open("files/newtPongInstrFEH.pic");
        instructions.Draw(0,0);
        LCD.SetFontColor(RED);
        LCD.DrawRectangle(1, 214, 60, 25);
        LCD.WriteAt("Back", 5, 220);
        //Add stars to the background, randomly generated, different each time
        for (int i = 0; i < 100; i++){
            int xrand,yrand;
            xrand = Random.RandInt();
            yrand = Random.RandInt();

            //Draw a colored pixel at the center of the screen
            LCD.SetFontColor(WHITE);
            LCD.DrawPixel(xrand,yrand);
        }

        while(true) {
            while(!LCD.Touch(&x1,&y1));
            if(x1 >= 1 && x1 <= 60 && y1 >= 214 && y1 <= 239) {
                break;
            }
        }
        while(LCD.Touch(&x1,&y1));

    }

    //Display Credits
    if (cred){
        x1 = 0, y1 = 0;
        LCD.Clear();
        LCD.SetFontColor(WHITE);
        LCD.WriteAt("Created by Artem Vovchenko",0,80);
        LCD.WriteAt("and Nathan Fickert",50,110);
        LCD.SetFontColor(GRAY);
        LCD.DrawRectangle(1,70,315,70);
        LCD.SetFontColor(RED);
        LCD.DrawRectangle(1, 214, 60, 25);
        LCD.WriteAt("Back", 5, 220);

        //Add stars to the background, randomly generated, different each time
        for (int i = 0; i < 100; i++){
            int xrand,yrand;
            xrand = Random.RandInt();
            yrand = Random.RandInt();

            //Draw a colored pixel at the center of the screen
            LCD.SetFontColor(WHITE);
            LCD.DrawPixel(xrand,yrand);
        }

        while(true) {
            while(!LCD.Touch(&x1,&y1));
            if(x1 >= 1 && x1 <= 60 && y1 >= 214 && y1 <= 239) {
                break;
            }
        }
        while(LCD.Touch(&x1,&y1));
    }

    //Update
    LCD.Update();
    }

}
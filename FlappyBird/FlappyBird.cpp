// FlappyBird.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include<io.h>
#include<stdio.h>
#include<fcntl.h>

using namespace std;

#include "olcConsoleGameEngineGL.h"

class FlappyBird : public olcConsoleGameEngine {

public:
    FlappyBird() {
        m_sAppName = L"Flappy Bird";
    }

private:

    //bird physics variables
    float birdPos = 0.0f;
    float birdVel = 0.0f;
    float birdAcc = 0.0f;
    float gravity = 100.0f;

    //world variables
    float sectionWidth;
    list<int> sections;
    float levelPos = 0.0f;

    //collision
    bool hasCollided = false;

    //game reset
    bool resetGame = false;

    //scoring
    int attempts = 0;
    int flapCount = 0;
    int maxFlaps = 0;

protected:
    virtual bool OnUserCreate() {

        sections = { 0, 0, 0, 0 };
        resetGame = true;
        sectionWidth = (float)ScreenWidth() / (float)(sections.size() - 1);

        return true;
    }
    //virtual bool OnUserDestroy() { return true; };
    virtual bool OnUserUpdate(float fElapsedTime) {
        
        if (resetGame) {

            hasCollided = false;
            resetGame = false;
            sections = { 0, 0, 0, 0 };
            birdAcc = 0.0f;
            birdVel = 0.0f;
            birdPos = ScreenHeight() / 2.0f;

            flapCount = 0;
            attempts++;
        }

        if (hasCollided) {

            if (m_keys[VK_SPACE].bReleased)
                resetGame = true;
        }
        else {

            //bird physics
            if (m_keys[VK_SPACE].bPressed && birdVel >= gravity / 10.0f) {

                birdAcc = 0.0f;
                birdVel = -gravity / 4.0f;
                flapCount++;
                if (flapCount > maxFlaps) maxFlaps = flapCount;
            }
            else {
                birdAcc += gravity * fElapsedTime;
            }

            if (birdAcc >= gravity) birdAcc = gravity;

            birdVel += birdAcc * fElapsedTime;
            birdPos += birdVel * fElapsedTime;

            //the world
            levelPos += 14.0f * fElapsedTime;

            if (levelPos > sectionWidth) {
                levelPos -= sectionWidth;
                sections.pop_front();
                int i = rand() % (ScreenHeight() - 20);
                if (i <= 10) i = 0;
                sections.push_back(i);
            }

            //world sketch
            Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');

            int currSection = 0;
            for (auto section : sections) {

                if (section != 0) {

                    //bottom section
                    Fill(currSection * sectionWidth + 10 - levelPos, ScreenHeight() - section,
                        currSection * sectionWidth + 15 - levelPos, ScreenHeight(),
                        PIXEL_SOLID, FG_GREEN);

                    //top section
                    Fill(currSection * sectionWidth + 10 - levelPos, 0,
                        currSection * sectionWidth + 15 - levelPos, ScreenHeight() - section - 15,
                        PIXEL_SOLID, FG_GREEN);
                }
                currSection++;
            }

            int birdX = (int)(ScreenWidth() / 3.0f); //bird position

            //Collision Detection
           //hasCollided = birdPos < 2 || birdPos > ScreenHeight() - 2;
           hasCollided = birdPos < 2 || birdPos > ScreenHeight() - 2 ||
                m_bufScreen[(int)(birdPos + 0) * ScreenWidth() + birdX].Char.UnicodeChar != L' ' ||
                m_bufScreen[(int)(birdPos + 1) * ScreenWidth() + birdX].Char.UnicodeChar != L' ' ||
                m_bufScreen[(int)(birdPos + 0) * ScreenWidth() + birdX + 6].Char.UnicodeChar != L' ' ||
                m_bufScreen[(int)(birdPos + 1) * ScreenWidth() + birdX + 6].Char.UnicodeChar != L' ';

            /*hasCollided = birdPos < 2 || birdPos > ScreenHeight() - 2 ||
                m_bufScreen[0][(int)(birdPos + 0) * ScreenWidth() + birdX].glyph != L' ' ||
                m_bufScreen[0][(int)(birdPos + 1) * ScreenWidth() + birdX].glyph != L' ' ||
                m_bufScreen[0][(int)(birdPos + 0) * ScreenWidth() + birdX + 6].glyph != L' ' ||
                m_bufScreen[0][(int)(birdPos + 1) * ScreenWidth() + birdX + 6].glyph != L' ';*/
            //bird position and sketch

            if (birdVel > 0) {

                DrawString(birdX, birdPos + 0, L"\\\\\\");
                DrawString(birdX, birdPos + 1, L"\\\\\\=Q");
            }
            else {

                DrawString(birdX, birdPos + 0, L"///=Q");
                DrawString(birdX, birdPos + 1, L"///");

            }

            DrawString(1, 1, L"Attemp: " + to_wstring(attempts) +
                L" Score: " + to_wstring(flapCount) +
                L" Highest Score: " + to_wstring(maxFlaps));

            return true;
        }
    }
};


int main() {
    //_setmode(_fileno(stdout), _O_U16TEXT);
    FlappyBird game;
    game.ConstructConsole(80, 48, 16, 16);
    game.Start();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

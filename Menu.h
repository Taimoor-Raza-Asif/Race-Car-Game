#pragma once
#include <windows.h>
#include <iostream>
#include<fstream>
#include <conio.h>
#include <iomanip>
#include<string>
using namespace std;

void setConsoleColor(int text, int background) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, text | background);
}


void drawmap() {
    cout << "\t\t\t\t\t\t\to   o---o---o---o" << endl;;
    cout << "\t\t\t\t\t\t\t|           |   |" << endl;
    cout << "\t\t\t\t\t\t\to---";
    setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
    cout << "C";
    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
        cout<<"---o---o   o" << endl;
    cout << "\t\t\t\t\t\t\t                |" << endl;
    cout << "\t\t\t\t\t\t\to---o---o   o---o" << endl;
    cout << "\t\t\t\t\t\t\t|           |   |" << endl;
    cout << "\t\t\t\t\t\t\to---o   o   o   o" << endl;
    cout << "\t\t\t\t\t\t\t    |   |   |   |" << endl;
    cout << "\t\t\t\t\t\t\to---o   o---o---o" << endl;

}

void displayWelcomeScreen() {
    system("cls");
    setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
    cout << "\n\n\n\t\t\t\t\t'''*****|Welcome To Racing Car Game!|*****'''\n\n\n";
    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
    drawmap();
    cout << "\n\n\n";
    setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
    cout << " Created by \n";
    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED, FOREGROUND_INTENSITY);
    cout << "  - Taimoor Raza Asif\n  - Hamza Aftab\n";
    Sleep(2500);  // Sleep for 3 seconds (3000 milliseconds)
    system("cls");
}

string Name;
string getUserName() {
    setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
    cout << "\n\n\n\t\t\t\t\t'''*****|Welcome To Racing Car Game!|*****'''\n\n\n";
    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
   /* string username;
    cout << "  Enter your user name: ";
    getline(cin, username);*/
    string username;
    cout << "Enter your user name (without Spaces) : ";
    getline(cin, username);
    Name = username;
    cout << "\n\n\n\t\t\t\t\t\t WELCOME! ";
    setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
    cout << username;
    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
    cout << " TO RACING CAR GAME ";
    cout << endl;
    Sleep(2500);
    // Save the username to profile.txt
    ofstream profileFile("profile.txt");
    if (profileFile.is_open()) {
        profileFile << username;
        profileFile.close();
    }
    else {
        cout << "Error opening profile.txt for writing." << endl;
    }

    return username;
}

void playSound(int frequency, int duration) {
    Beep(frequency, duration);
}

void printMenuOption(const char* option, int width, bool isSelected) {
    if (isSelected) {
        setConsoleColor(FOREGROUND_RED, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
    }
    else {
        setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
    }

    cout << "  |--------------------";
    cout << "||       ";
    cout << setw(width) << left << option;
    cout << "  ||";
    cout << "--------------------|";
    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, 0);
}

void printMenu(const char* menuOptions[], int numOptions, int selectedOption, bool beepSoundEnabled, int difficultyLevel) {
    system("cls");

    setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
    cout << "\n                      '***|RACING CAR GAME|***'               " << endl;
    cout << "\n";
    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
    cout << "  ==================================================================" << endl;
    setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
    cout << "  |                          GAME MENU                             |" << endl;
    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
    cout << "  ==================================================================" << endl;

    for (int i = 0; i < numOptions; ++i) {
        cout << endl;
        printMenuOption(menuOptions[i], 10, i == selectedOption);
        cout << endl;
    }

    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);

    cout << "\n  ==================================================================" << endl;

    cout << "\n Sound: ";
    setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
    cout << (beepSoundEnabled ? "ON" : "OFF") << "                                          " << endl;
    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
    cout << "\n Difficulty: ";
    setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
    switch (difficultyLevel) {
    case 1:
        cout << "Easy";
        break;
    case 2:
        cout << "Medium";
        break;
    case 3:
        cout << "Hard";
        break;
    default:
        cout << "Easy";
        break;
    }
    cout << endl;
    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
}

void HelpFile() {
    system("cls");
    setConsoleColor(FOREGROUND_INTENSITY, 0);
    ifstream file("help.txt"); // Open the file in read mode

    if (file.is_open()) {
        char c;
        while (file.get(c)) {
            cout << c;
        }

        file.close(); // Close the file
    }
    else {
        cout << "Error opening the file for reading." << endl;
    }
}

int difficulty = 1; // Default difficulty is Easy
int difficultySelection() {
    system("cls");
    cout << "Difficulty Level\n";
    setConsoleColor(FOREGROUND_GREEN , FOREGROUND_INTENSITY);
    cout << "\n\n   1. Easy\n";
    setConsoleColor(FOREGROUND_BLUE, FOREGROUND_INTENSITY);
    cout << "\n   2. Medium\n";
    setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
    cout << "\n   3. Hard\n";
    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
    cout << "\nEnter 1 , 2 , or 3 as your choice: ";
    cin >> difficulty;

    return difficulty;
}










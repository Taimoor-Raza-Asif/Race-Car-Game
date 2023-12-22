#include<iostream>
#include"Menu.h"
#include"Graph.h"
#include"windows.h"
using namespace std;


int main() {

    srand(time(nullptr));

    CarGame game;
    displayWelcomeScreen();
    // Get the username and store it in profile.txt
    string username = getUserName();
    Map t;
    int scr = t.getScore();
    const int numOptions = 6;
    const char* menuOptions[numOptions] = { "Start", "High Score", "Options", "Help", "Sounds", "Quit" };

    int selectedOption = 0;
    bool flag = true;
    int difficultyLevel = difficulty; // Default difficulty is Easy

    while (flag) {
        printMenu(menuOptions, numOptions, selectedOption, beepSoundEnabled, difficultyLevel);

        int key = _getch();

        if (key == 224) { // Arrow key was pressed
            key = _getch(); // Get the extended key code

            switch (key) {
            case 72: // Up arrow
                playSound(beepSoundEnabled ? 500 : 0, 100); // Example sound for navigation
                selectedOption = (selectedOption - 1 + numOptions) % numOptions;
                break;
            case 80: // Down arrow
                playSound(beepSoundEnabled ? 500 : 0, 100); // Example sound for navigation
                selectedOption = (selectedOption + 1) % numOptions;
                break;
            }
        }
        else if (key == 13) { // Enter key
            playSound(beepSoundEnabled ? 1000 : 0, 200); // Example sound for selection

            // Handle the selected option
            switch (selectedOption + 1) {
            case 1:
                system("cls");
                cout << "\nStarting the game..." << endl;
                game.selectGameMode();
                Sleep(1000); // Sleep for 2 seconds (2000 milliseconds)
                break;
            case 2:
                system("cls");
                cout << "\nDisplaying high scores..." << endl;
                game.displayTopPlayers("highscores.txt");
                Sleep(1000);
                break;
            case 3:
                system("cls");
                cout << "\nDisplay Options..." << endl;
                Sleep(1000);
                difficultyLevel = difficultySelection();
                break;
            case 4:
                cout << "\nDisplaying help..." << endl;
                Sleep(1000);
                HelpFile();
                break;
            case 5:
                system("cls");
                cout << "\nPlease select the sound option:\n -Press 'o' for ON\n -Press 'f' for OFF\n -Press 'b' for back\n\n";
                // Setting option, switch sound
                char soundOption;
                cin >> soundOption;
                switch (soundOption) {
                case 'o':
                case 'O':
                    beepSoundEnabled = true;
                    cout << "\nSound is ON\n\n";
                    break;
                case 'f':
                case 'F':
                    beepSoundEnabled = false;
                    cout << "\nSound is OFF\n\n";
                    break;
                case 'b':
                case 'B':
                    break;
                default:
                    cout << "\nInvalid option\n\n";
                    break;
                }
                break;

            case 6:
                // Quit option, ask whether to go back to the menu or quit
                setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
                cout << "\nAre you sure to Quit Program\n -Press 'y' for YES\n -Press 'n' for NO : ";
                char response;
                cin >> response;
                if (response == 'y' || response == 'Y') {
                    exit(0);
                    flag = false; // Exit the program
                }
                break;
            }

            if (selectedOption != 5) { // Check if it's not the Quit option
                // Ask whether to go back to the menu or quit
                setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
                cout << "Do you want to go back to the menu?\n -Press 'b' for Navigation to Main Menu\n -Press 'q' for Exiting Program : ";
                char response;
                cin >> response;
                if (response == 'q' || response == 'Q') {
                    exit(0);
                    flag = false; // Exit the program
                }
            }

            // Reset selectedOption after handling the selection
            selectedOption = 0;
        }
    }

    return 0;
}
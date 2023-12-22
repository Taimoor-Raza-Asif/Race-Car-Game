#pragma once
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include<algorithm>
#include<sstream>
#include<climits>
#include<string>
#include <ctime>
#include <cstdlib>
#include"Queue.h"
#include"PowerUpsList.h"
using namespace std;

int Level = 1;
int AutoLevel = 1;
int score = 0;
bool beepSoundEnabled = true;

class Graph {    // graph class which will print the the map or graph
public:
    Graph(int rows, int cols);
    void addEdge(int source, int dest);   // for adding edges
    bool hasEdge(int source, int dest) const;   // checking if there is edge or not
    int getVertex(int row, int col) const;
    int printSpace(int row, int col) const;
    void printMap(int carPosition, int finishPosition, int* obstacles, PowerUpList& powerUps, int* links) const;    // this will print map for manual game play
    void printMapAuto(int carPosition, int finishPosition, const int* path, int pathLength, const int* totalDistance) const;    // this will print map for automatic game play 
    int rows;
    int cols;
    int** edge_Weights;  // 2D array for assigning weights to adjacency matrix
private:
    int** adjacency_Matrix;   // adjecency matrux array

};

Graph::Graph(int rows, int cols) : rows(rows), cols(cols) {

    adjacency_Matrix = new int* [rows * cols];
    edge_Weights = new int* [rows * cols];  // Allocate memory for edge weights

    for (int i = 0; i < rows * cols; ++i) {
        adjacency_Matrix[i] = new int[rows * cols];
        edge_Weights[i] = new int[rows * cols];  // Allocate memory for edge weights

        fill(adjacency_Matrix[i], adjacency_Matrix[i] + rows * cols, 0);

        // Assign random weights 1 to 15 to edges
        for (int j = 0; j < rows * cols; ++j) {
            edge_Weights[i][j] = rand() % 15 + 1;
        }
    }


    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int currentVertex = getVertex(i, j);

            if (i > 0)    addEdge(currentVertex, getVertex(i - 1, j));  // move up
            if (i < rows - 1) addEdge(currentVertex, getVertex(i + 1, j));  //  move Down
            if (j > 0)    addEdge(currentVertex, getVertex(i, j - 1));  // move  Left
            if (j < cols - 1) addEdge(currentVertex, getVertex(i, j + 1));  // move Right
        }
    }
}

void Graph::addEdge(int source, int dest) {        // defining add edge function
    adjacency_Matrix[source][dest] = 1;
    adjacency_Matrix[dest][source] = 1;
}

bool Graph::hasEdge(int source, int dest) const {
    return adjacency_Matrix[source][dest] == 1;
}

int Graph::getVertex(int row, int col) const {
    return row * cols + col;
}

int Graph::printSpace(int row, int col) const {
    return row * col;
}



void Graph::printMap(int carPosition, int finishPosition, int* obstacles, PowerUpList& powerUps, int* links) const {
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // get the console handle

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int currentVertex = getVertex(i, j);
            if (currentVertex == carPosition) {
                SetConsoleTextAttribute(hConsole, 244); // set text color 
                cout << "C";
                SetConsoleTextAttribute(hConsole, 15); // set text color to white
            }
            else if (currentVertex == finishPosition) {
                SetConsoleTextAttribute(hConsole, 10); // set text color 
                cout << "F";
                SetConsoleTextAttribute(hConsole, 15); // set text color to white
            }
            else if (find(obstacles, obstacles + (rows * cols), currentVertex) != obstacles + (rows * cols)) {
                SetConsoleTextAttribute(hConsole, 12); // set text color 
                cout << "#";
                SetConsoleTextAttribute(hConsole, 15); // set text color to white
            }
            else if (find(links, links + (rows * cols), currentVertex) != links + (rows * cols)) {
                cout << " ";
            }
            else if (powerUps.hasPowerUp(currentVertex)) {
                SetConsoleTextAttribute(hConsole, 14); // set text color to yellow
                cout << "@";
                SetConsoleTextAttribute(hConsole, 15); // set text color to white
            }
            else {
                SetConsoleTextAttribute(hConsole, 31); // set text color
                cout << "+";
                SetConsoleTextAttribute(hConsole, 15); // set text color to white
            }
            if (j + 1 == cols)
                continue;
            else {
                // check if there is space in front or behind of vertices
                int nextVertex = getVertex(i, j + 1);
                if ((find(links, links + (rows * cols), currentVertex) != links + (rows * cols) ||
                    find(links, links + (rows * cols), nextVertex) != links + (rows * cols)) &&
                    (currentVertex != carPosition || i != 0 || j != 0) && currentVertex != finishPosition && nextVertex != carPosition && nextVertex != finishPosition) {
                    cout << "  ";
                }
                else {
                    SetConsoleTextAttribute(hConsole, 3); // set text color to blue
                    cout << "--";
                    SetConsoleTextAttribute(hConsole, 15); // set text color to white
                }
            }

        }
        cout << endl;
        if (i + 1 == rows)
            continue;
        else
            for (int j = 0; j < cols; ++j) {
                // check if there is space above or below of vertices
                int upperVertex = getVertex(i, j);
                int lowerVertex = getVertex(i + 1, j);
                if ((find(links, links + (rows * cols), upperVertex) != links + (rows * cols) ||
                    find(links, links + (rows * cols), lowerVertex) != links + (rows * cols)) &&
                    (upperVertex != carPosition || i != 0 || j != 0) && upperVertex != finishPosition && lowerVertex != carPosition && lowerVertex != finishPosition) {
                    cout << "   ";
                }
                else {
                    SetConsoleTextAttribute(hConsole, 3); // set text color to blue
                    cout << "|  ";
                    SetConsoleTextAttribute(hConsole, 15); // reset text color to white
                }
            }
        cout << endl;
    }

}



// this function will print maze without adding hurdles or powerups

void Graph::printMapAuto(int carPosition, int finishPosition, const int* path, int pathLength, const int* totalDistance) const {
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int currentVertex = getVertex(i, j);
            if (currentVertex == carPosition) {
                SetConsoleTextAttribute(hConsole, 244);
                cout << "C";
                SetConsoleTextAttribute(hConsole, 15);
            }
            else if (currentVertex == finishPosition) {
                SetConsoleTextAttribute(hConsole, 10);
                cout << "F";
                SetConsoleTextAttribute(hConsole, 15);
            }
            else {
                bool isOnPath = false;
                for (int k = 0; k < pathLength; ++k) {
                    if (path[k] == currentVertex) {
                        isOnPath = true;
                        break;
                    }
                }

                if (isOnPath) {
                    SetConsoleTextAttribute(hConsole, 215);
                    cout << "+";
                    SetConsoleTextAttribute(hConsole, 15);
                }
                else {
                    SetConsoleTextAttribute(hConsole, 31);
                    cout << "+";
                    SetConsoleTextAttribute(hConsole, 15);
                }
            }

            if (j + 1 == cols)
                continue;
            else
                SetConsoleTextAttribute(hConsole, 3);
            cout << "--";
            SetConsoleTextAttribute(hConsole, 15);
        }
        cout << endl;

        if (i + 1 == rows)
            continue;
        else
            for (int j = 0; j < cols; ++j) {
                SetConsoleTextAttribute(hConsole, 3);
                cout << "|  ";
                SetConsoleTextAttribute(hConsole, 15);
            }
        cout << endl;
    }
}


class Map {
public:
    Map();
    Map(int rows, int cols);
    void generateObstacles(int numObstacles);
    void removeLinks(int numLinks);
    bool isFinish(int vertex) const;
    bool moveCar(char direction);
    int getObstaclesCount() const { return obstacles_Counter; }
    void printMap();
    int getCarPosition() const;
    int getHurdlesHit() const {
        return hit_Hurdles;
    }
    int* total_Distance_Covered;
    int getScore() const {
        return score;
    }
    bool isValidMoveAutomatically(int nextVertex);
    void DijkstraAlgo(int* distance, int* parent, int* totalDistance) const;
    void moveCarAutomatically();
    void printMapAuto() {
        graph.printMapAuto(car_Position, finish_Position, path, path_Length, total_Distance_Covered);
    }
    PowerUpList powerUps;

    const int* path;
    int path_Length;
    Graph graph;
    Queue<int> obstaclesQueue;
    int car_Position;
    int finish_Position;
    int* obstacle;
    int obstacles_Counter;
    int hit_Hurdles;
    int* links;
    int linksCount;
};



Map::Map() : graph(0, 0), car_Position(0), finish_Position(0),
obstacle(nullptr), obstacles_Counter(0), hit_Hurdles(0), links(nullptr), linksCount(0), path(nullptr), path_Length(0), total_Distance_Covered(nullptr) {}

Map::Map(int rows, int cols) : graph(rows, cols), car_Position(0), finish_Position(rows* cols - 1),
obstacle(nullptr), obstacles_Counter(0), hit_Hurdles(0), links(nullptr), linksCount(0), path(nullptr), path_Length(0), total_Distance_Covered(nullptr) {
    obstacle = new int[rows * cols];
    obstacle[obstacles_Counter++] = car_Position;
    links = new  int[rows * cols];
    links[linksCount++] = car_Position;
    total_Distance_Covered = new int[rows * cols];  // Allocate memory for total distance covered
    fill(total_Distance_Covered, total_Distance_Covered + rows * cols, 0);  // Initialize to 0
}


void Map::removeLinks(int numLinks) {
    srand(time(nullptr));
    for (int i = 0; i < numLinks; ++i) {
        int randomVertex;
        do {
            randomVertex = rand() % (graph.rows * graph.cols - 6);
            int carRow = car_Position / graph.cols;
            int finishRow = finish_Position / graph.cols;
            int randomRow = randomVertex / graph.cols;
        } while (randomVertex == car_Position ||
            find(links, links + linksCount, randomVertex) != links + linksCount);

        links[linksCount++] = randomVertex;  // Add the link to the links array
    }
}


int Map::getCarPosition() const {
    return car_Position;
}


void Map::generateObstacles(int numObstacles) {
    srand(time(nullptr));
    while (numObstacles > 0) {
        int randomVertex = rand() % (graph.rows * graph.cols);

        if (randomVertex != car_Position &&
            randomVertex != finish_Position &&  // Ensure it's not the finish line
            find(obstacle, obstacle + obstacles_Counter, randomVertex) == obstacle + obstacles_Counter) {

            // Check if the randomVertex is not already in the queue
            bool isAlreadyInQueue = false;
            Queue<int>::Node* tempNode = obstaclesQueue.front;
            while (tempNode != nullptr) {
                if (tempNode->data == randomVertex) {
                    isAlreadyInQueue = true;
                    break;
                }
                tempNode = tempNode->next;
            }

            if (!isAlreadyInQueue) {
                obstaclesQueue.enqueue(randomVertex);
                numObstacles--;
            }
        }
    }

    // Move obstacles from queue to array
    while (!obstaclesQueue.isEmpty()) {
        obstacle[obstacles_Counter++] = obstaclesQueue.getFront();
        obstaclesQueue.dequeue();
    }
}

bool Map::isFinish(int vertex) const {
    return vertex == finish_Position;
}

void Map::DijkstraAlgo(int* distance, int* parent, int* totalDistance) const {
    int source = car_Position;
    int destination = finish_Position;

    fill(distance, distance + graph.rows * graph.cols, INT_MAX);
    fill(parent, parent + graph.rows * graph.cols, -1);
    fill(totalDistance, totalDistance + graph.rows * graph.cols, 0);
    bool* visited = new bool[graph.rows * graph.cols] {false};

    distance[source] = 0;

    for (int count = 0; count < graph.rows * graph.cols - 1; ++count) {
        int minDistance = INT_MAX;
        int minVertex = -1;

        // Find the vertex with the minimum distance
        for (int v = 0; v < graph.rows * graph.cols; ++v) {
            if (!visited[v] && distance[v] < minDistance) {
                minDistance = distance[v];
                minVertex = v;
            }
        }

        if (minVertex == -1 || minVertex == destination) {
            break;
        }

        visited[minVertex] = true;

        // Update distances of adjacent vertices
        for (int v = 0; v < graph.rows * graph.cols; ++v) {
            if (!visited[v] && graph.hasEdge(minVertex, v) && (distance[minVertex] + graph.edge_Weights[minVertex][v]) < distance[v]) {
                distance[v] = distance[minVertex] + graph.edge_Weights[minVertex][v];
                parent[v] = minVertex;
                totalDistance[v] = totalDistance[minVertex] + graph.edge_Weights[minVertex][v];
            }
        }
    }

    delete[] visited;
}

void Map::moveCarAutomatically() {
    int* distance = new int[graph.rows * graph.cols];
    int* parent = new int[graph.rows * graph.cols];
    int* totalDistance = new int[graph.rows * graph.cols];
    DijkstraAlgo(distance, parent, totalDistance);

    int source = car_Position;
    int destination = finish_Position;

    // Count the number of vertices in the path
    int pathLength = 0;
    int tempDestination = destination;
    while (parent[tempDestination] != -1) {
        tempDestination = parent[tempDestination];
        pathLength++;
    }

    // Create an array to store the path
    int* path = new int[pathLength];
    tempDestination = destination;
    for (int i = pathLength - 1; i >= 0; --i) {
        path[i] = tempDestination;
        tempDestination = parent[tempDestination];
    }

    // Move the car along the path
    for (int i = 1; i < pathLength; ++i) {
        // Calculate the current and next vertices in the path
        int currentVertex = path[i - 1];
        int nextVertex = path[i];

        // Check for valid move
        if (isValidMoveAutomatically(nextVertex)) {
            // Simulate the movement
            system("cls");
            graph.printMapAuto(car_Position, finish_Position, path, pathLength, totalDistance);

            // Pause for a short duration to visualize the movement
            Sleep(700);
            total_Distance_Covered += totalDistance[nextVertex];
            // Move to the next vertex
            car_Position = nextVertex;
        }
        else {
            // Invalid move, change the path
            DijkstraAlgo(distance, parent, totalDistance);

            // Reset the hurdles hit count
            hit_Hurdles = 0;
            break;  // Exit the loop if an invalid move is encountered
        }
    }

    delete[] distance;
    delete[] parent;
    delete[] path;
    delete[] totalDistance;
}

bool Map::isValidMoveAutomatically(int nextVertex) {
    // Check if the next vertex is within bounds
    int nextRow = nextVertex / graph.cols;
    int nextCol = nextVertex % graph.cols;
    if (nextRow < 0 || nextRow >= graph.rows || nextCol < 0 || nextCol >= graph.cols) {
        cout << "Invalid move! Car went out of bounds." << endl;
        Sleep(300);
        return false;
    }
    return true;  // Valid move
}


void Map::printMap() {
    graph.printMap(car_Position, finish_Position, obstacle, powerUps, links);
    //cout << " Hit Hurdles in Level  " << Level << " : " << hit_Hurdles << endl << endl;
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE , FOREGROUND_INTENSITY);
    cout << "\n\n Score: ";
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN , FOREGROUND_INTENSITY);
    cout << score;
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, FOREGROUND_INTENSITY);
    cout << "                Hit Hurdles in Level  ";
    setConsoleColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN , FOREGROUND_INTENSITY);
    cout << Level;
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, FOREGROUND_INTENSITY);
    cout << " : ";
    setConsoleColor(FOREGROUND_RED, 0);

    cout << hit_Hurdles;
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, FOREGROUND_INTENSITY);
    cout << "                Game End At Hurdle Hit  : "; 
    //setConsoleColor(FOREGROUND_BLUE, FOREGROUND_INTENSITY);
    if (difficulty == 1) {
        cout << 3 << endl << endl;
    }
    else if (difficulty == 2) {
        cout << 2 << endl << endl;
    }
    else if (difficulty == 3){
        cout << 1 << endl << endl;
    }
}

class CarGame {
public:
    CarGame();
    bool isManualMode() const;
    void startGame();
    void playLevelAutomatic();
    void startGameAutomatic();
    void selectGameMode();
    int displayPauseMenu();
    void handlePauseMenu();
    void writeScoreToFile(const std::string& fileName);
    void displayTopPlayers(const string& fileName);
private:
    Map map;
    int level;
    int total_Hurdles_Hit;
    void initializeGame();
    void autoInitialize();
    void playLevel();
    bool automaticMode;
    int total_Distance_Covered;
};

CarGame::CarGame() : level(1), total_Hurdles_Hit(0), total_Distance_Covered(0) {}

bool CarGame::isManualMode() const {
    return automaticMode == '1';
}

void CarGame::writeScoreToFile(const std::string& fileName) {
    std::ofstream outputFile(fileName, std::ios::app);

    if (outputFile.is_open()) {
        outputFile << "Player: " << Name << "\tScore: " << score << "\tLevel: " << Level << "\n";
        outputFile.close();
        std::cout << "Score has been written to the file.\n";
    }
    else {
        std::cerr << "Unable to open the file.\n";
    }
}

void CarGame::initializeGame() {
    const int rows = 5 + level;
    const int cols = 10 + level * 2;
    map = Map(rows, cols);
    if (difficulty == 1) {
        const int numObstacles = level * 5;
        const int numLinks = level * 6;
        map.generateObstacles(numObstacles);
        map.removeLinks(numLinks);
    }
    else if (difficulty == 2) {
        const int numObstacles = level * 7;
        const int numLinks = level * 5;
        map.generateObstacles(numObstacles);
        map.removeLinks(numLinks);
    }
    else if (difficulty == 3) {
        const int numObstacles = level * 9;
        const int numLinks = level * 4;
        map.generateObstacles(numObstacles);
        map.removeLinks(numLinks);
    }
    const int numPowerUps = level * 3;
    int remainingPowerUps = numPowerUps;

    while (remainingPowerUps > 0) {
        if (map.graph.rows > 0 && map.graph.cols > 0) {
            int randomVertex = rand() % (map.graph.rows * map.graph.cols);

            if (randomVertex != map.getCarPosition() &&
                randomVertex != map.graph.getVertex(rows - 1, cols - 1) &&  // Ensure it's not the finish line
                !map.powerUps.hasPowerUp(randomVertex) &&
                find(map.obstacle, map.obstacle + map.obstacles_Counter, randomVertex) == map.obstacle + map.obstacles_Counter) {

                map.powerUps.addPowerUp(randomVertex);
                remainingPowerUps--;
            }
        }
    }
}

void CarGame::autoInitialize() {
    const int rows = 5 + AutoLevel;
    const int cols = 10 + AutoLevel * 2;
    map = Map(rows, cols);
}



void CarGame::playLevelAutomatic() {
    while (!map.isFinish(map.getCarPosition())) {

        system("cls");
        map.printMapAuto();
        char move;
        map.moveCarAutomatically();
        move = 'a';

        total_Distance_Covered += map.graph.edge_Weights[map.getCarPosition()][map.getCarPosition() - 1];


    }

    system("cls");
    map.printMapAuto();
    cout << "Total Distance Covered: " << total_Distance_Covered * 3 << endl;
    cout << "Congratulations! You reached the finish line." << endl;
    map.hit_Hurdles += map.getHurdlesHit(); // Update the total hurdles hit count        

    AutoLevel++;
}

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
void CarGame::playLevel() {
  
    while (!map.isFinish(map.getCarPosition())) {


        system("cls");
        map.printMap();
        setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN , FOREGROUND_INTENSITY);
        cout << " Game Level ";
        setConsoleColor(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
        cout << Level;
        setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
        cout << "            Total Hurdles: ";
        setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
        cout << map.getObstaclesCount();
        setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
        cout << "                          Difficulty Level: ";
        if (difficulty == 1) {
            setConsoleColor(FOREGROUND_GREEN, FOREGROUND_INTENSITY);
            cout << "Easy";
        }
        else if (difficulty == 2) {
            setConsoleColor(FOREGROUND_BLUE, FOREGROUND_INTENSITY);
            cout << "Medium";
        }
        else if (difficulty == 3) {
            setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
            cout << "Hard";
        }
        cout << endl << endl;
      

        setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
        cout << " Hurdles     : ";
        setConsoleColor(FOREGROUND_RED , FOREGROUND_INTENSITY);
        cout << "#\n";
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
        cout << " PowerUps    : ";
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
        cout << "@\n";
        setConsoleColor(FOREGROUND_GREEN, FOREGROUND_INTENSITY);
        cout << " Finish Line : ";
        setConsoleColor(FOREGROUND_GREEN, FOREGROUND_INTENSITY);
        cout << "F\n";
        setConsoleColor(FOREGROUND_GREEN | FOREGROUND_BLUE , FOREGROUND_INTENSITY);
        cout << "\n Enter Moves in Small alphabets \n D ---> Forward      >\n A ---> Backward     <\n W ---> Upward       ^\n S ---> Downward     v\n";
        SetConsoleTextAttribute(hConsole, 13);
        cout << "\n Press 'p for Pause the Game \n";
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, FOREGROUND_INTENSITY);
        char move;
        move = _getch();

        if (!map.moveCar(move)) {
            cout << "Invalid move! Use w/a/s/d." << endl;
        }
    }

    system("cls");
    cout << "Game Level " << Level << " - Total Obstacles: " << map.getObstaclesCount() << endl;
    map.printMap();
    if (difficulty == 1 && map.getHurdlesHit() == 3) {
       
            cout << "Game Over! You hit 3 hurdles." << endl;
            exit(0);  // Terminate the program
       
    }
    else if (difficulty == 2 && map.getHurdlesHit() == 2) {
     
            cout << "Game Over! You hit 2 hurdles." << endl;
            exit(0);  // Terminate the program
 
    }
    else  if (difficulty == 3 && map.getHurdlesHit() == 1) {
   
            cout << "Game Over! You hit 1 hurdles." << endl;
            exit(0);  // Terminate the program
    }
    else {
        if(beepSoundEnabled)
        Beep(1000, 500);
        cout << "Congratulations! You reached the finish line." << endl;
        total_Hurdles_Hit += map.getHurdlesHit();  // Update the total hurdles hit count
    }

    level++;
}



int CarGame::displayPauseMenu() {
    int numOptions = 4;  // Number of options in the pause menu
    const char* menuOptions[] = { "Resume        ", "Manual Play   ", "Automatic Play", "Exit           " };
    int selectedOption = 0;  // Index of the currently selected option
    bool resumeMenu = beepSoundEnabled;
    while (true) {
        system("cls");

        // Set colors for the title
        setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
        cout << "\n                      ***| PAUSE MENU |***               " << endl;

        // Set colors for the separator line
        setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
        cout << "  =====================================================================" << endl;

        // Set colors for menu options
        for (int i = 0; i < numOptions; ++i) {
            cout << endl;
            printMenuOption(menuOptions[i], 10, i == selectedOption);
            cout << endl;
        }

        // Reset colors after options
        setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
        cout << "  =====================================================================" << endl;

        // Use _getch() to get a single character without waiting for Enter
        char key = _getch();

        // Beep when arrow keys are pressed
        if (key == 72 || key == 80) {
            if (resumeMenu)
            Beep(700, 100);  // Beep at 700 Hz for 100 milliseconds
        }

        switch (key) {
        case 72:  // Arrow Up
            selectedOption = (selectedOption - 1 + numOptions) % numOptions;
            break;
        case 80:  // Arrow Down
            selectedOption = (selectedOption + 1) % numOptions;
            break;
        case 13:  // Enter key
            if (resumeMenu)
            Beep(1000, 200);  // Beep at 1000 Hz for 200 milliseconds
            return selectedOption;  // Return the selected option index
            break;
        default:
            break;
        }
    }
}

void CarGame::handlePauseMenu() {
    int selectedOption = displayPauseMenu(); // Get the selected option index from displayPauseMenu

    // Perform action based on the selected option
    switch (selectedOption) {
    case 0:
        // Resume the game
        return;
    case 1:
        score = 0;
        Level = 1;
        // Manual play
        startGame();
        break;
    case 2:
       
        // Automatic play
        startGameAutomatic();
        Level = 1;
        break;
    case 3:
        // Exit the game (navigate to main menu)
        selectGameMode();
        Level = 1;
        score = 0;
        break;
    default:
        break;
    }
}


void CarGame::startGame() {

    while (true) {
        initializeGame();
        playLevel();

        cout << "Do You want to Play Next Level (y/n): ";
        char choice;
        cin >> choice;
       
        if (choice != 'y') {
            writeScoreToFile("highscores.txt");
            Level = 1;
            level = 1;
            score = 0;
            break;
        }
        else {
            Level++;
        }
    }
}



void CarGame::startGameAutomatic() {
    while (true) {
        autoInitialize();
        playLevelAutomatic();

        cout << "Do You want to Play Next Level (y/n): ";
        char choice;
        cin >> choice;
      
        if (choice != 'y') {
            AutoLevel = 1;
           level = 1;
            break;
        }
        else {
            AutoLevel++;
        }
    }
}

bool Map::moveCar(char direction) {
    CarGame pause;
    CarGame fileHandling;
    int newRow, newCol;
    int currentRow = car_Position / graph.cols;
    int currentCol = car_Position % graph.cols;
    int newVertex;

    switch (direction) {
    case 'w':
        newRow = currentRow - 1;
        newCol = currentCol;
        break;
    case 'a':
        newRow = currentRow;
        newCol = currentCol - 1;
        break;
    case 's':
        newRow = currentRow + 1;
        newCol = currentCol;
        break;
    case 'd':
        newRow = currentRow;
        newCol = currentCol + 1;
        break;
    case 'p':
        pause.handlePauseMenu();
    default:
        return false;  // Invalid direction
    }

    if (newRow >= 0 && newRow < graph.rows && newCol >= 0 && newCol < graph.cols) {
        newVertex = graph.getVertex(newRow, newCol);

        if (powerUps.hasPowerUp(newVertex)) {
            // Apply the effect of the power-up
            score += 47;
            powerUps.removePowerUp(newVertex);
            cout << "You found a power-up! Score is increased by 50." << endl;
            if(beepSoundEnabled)
            Beep(1000, 200);
            Sleep(700);
        }

        if (find(obstacle, obstacle + obstacles_Counter, newVertex) != obstacle + obstacles_Counter) {
            hit_Hurdles++;  // Increment the hurdles hit count
            cout << "Invalid move! You hit an obstacle." << endl;
            if (beepSoundEnabled)
            Beep(500, 300);
            Sleep(700);

            // Update the score by dividing it by 3 and taking the remainder
            if (score > 5) {
                score = (score - 5);
            }
            else {
                score = 0;
            }

            if (difficulty == 1) {

                if (hit_Hurdles >= 3) {
                    cout << "Game Over! You hit 3 hurdles." << endl;
                    if (beepSoundEnabled)
                    Beep(200, 500);
                    fileHandling.writeScoreToFile("highscores.txt");
                    exit(0);  // Terminate the program
                }

            }
            else if (difficulty == 2) {

                if (hit_Hurdles >= 2) {
                    cout << "Game Over! You hit 2 hurdles." << endl;
                    if (beepSoundEnabled)
                    Beep(200, 500);
                    fileHandling.writeScoreToFile("highscores.txt");
                    exit(0);  // Terminate the program
                }

            }
            else  if (difficulty == 3) {

                if (hit_Hurdles >= 1) {
                    cout << "Game Over! You hit 1 hurdle." << endl;
                    if (beepSoundEnabled)
                    Beep(200, 500);
                    fileHandling.writeScoreToFile("highscores.txt");
                    exit(0);  // Terminate the program
                }
            }
            return false;
        }

        if (find(links, links + linksCount, newVertex) != links + linksCount) {   // Linking part
            cout << "Invalid move! There is no link " << endl;
            if (beepSoundEnabled)
            Beep(300, 200);
            Sleep(700);
            return false;

        }

        if (powerUps.hasPowerUp(newVertex)) {
            // Apply the effect of the power-up
            score += 47;
            powerUps.removePowerUp(newVertex);
            cout << "You found a power-up! Score is increased by 50." << endl;
            Sleep(700);
        }

        // Update the score by adding 3 for a valid move
        score += 3;

        car_Position = newVertex;
        return true;
    }

    cout << "Invalid move! You went out of bounds." << endl;
    if (beepSoundEnabled)
    Beep(300, 200);
    Sleep(300);

    return false;
}



void CarGame::selectGameMode() {
    char modeChoice = ' ';  // Initialize to a space character
    bool gameModeSound = beepSoundEnabled;
    int numOptions = 3;  // Update to include the "Back" option
    const char* menuOptions[] = { "Manual Mode   ", "Automatic Mode", "Back          " };  // Add "Back" option
    int selectedOption = 0;

    while (true) {
        system("cls");

        // Set colors for the title
        setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
        cout << "\n                      '***|RACING CAR GAME|***'               " << endl;

        // Set colors for the separator line
        setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
        cout << "  =====================================================================" << endl;

        // Set colors for the menu header
        setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
        cout << "  |                             Start Game                             |" << endl;

        // Set colors for the separator line
        setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
        cout << "  =====================================================================" << endl;

        // Set colors for menu options
        for (int i = 0; i < numOptions; ++i) {
            cout << endl;
            printMenuOption(menuOptions[i], 10, i == selectedOption);
            cout << endl;
        }
        setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
        // Print the line below the selected option
        cout << "\n  =====================================================================" << endl;

        // Use _getch() to get a single character without waiting for Enter
        char key = _getch();

        // Beep when arrow keys are pressed
        if (key == 72 || key == 80) {
            if(gameModeSound)
            Beep(700, 100);  // Beep at 700 Hz for 100 milliseconds
        }

        switch (key) {
        case 72:  // Arrow Up
            selectedOption = (selectedOption - 1 + numOptions) % numOptions;
            break;
        case 80:  // Arrow Down
            selectedOption = (selectedOption + 1) % numOptions;
            break;
        case 13:  // Enter key
            if (gameModeSound)
            Beep(1000, 200);  // Beep at 1000 Hz for 200 milliseconds
            if (selectedOption == numOptions - 1) {
                // "Back" option selected
                /*return;  */// Exit the function, you can also use break to go back to the previous menu

                //string username = getUserName();
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
                            selectGameMode();
                            Sleep(1000); // Sleep for 2 seconds (2000 milliseconds)
                            break;
                        case 2:
                            system("cls");
                            cout << "\nDisplaying high scores..." << endl;
                            displayTopPlayers("highscores.txt");
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
                    Level = 1;
                    score = 0;
                }
            }
            else {
                modeChoice = selectedOption + '1';  // Convert index to character '1' or '2'
            }
            break;
        default:
            break;
        }

        if (modeChoice != ' ') {
            break;
        }
    }
    automaticMode = modeChoice;

    if (modeChoice == '1') {
        startGame();
    }
    else if (modeChoice == '2') {
        AutoLevel = 1;
        startGameAutomatic();
    }
    else {
        cout << "Invalid choice. Please enter 1 for Manual Mode or 2 for Automatic Mode.\n";
    }
}

void CarGame::displayTopPlayers(const string& fileName) {
    const int maxPlayers = 50;
    pair<string, int> playerScores[maxPlayers];

    // Read scores from the file
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error opening file: " << fileName << endl;
        return;
    }

    int count = 0;
    string line;
    while (count < maxPlayers && getline(file, line)) {
        // Create a stringstream object from the line
        stringstream ss(line);

        // Skip "Player:" and read the player name
        string playerName;
        ss >> playerName;

        // Check if the line is valid and contains "Score:" and "Level:"
        if (playerName == "Player:") {
            // Extract the player name and score
            ss >> playerName; // Read the player name
            int score;
            string scoreLabel;
            ss >> scoreLabel >> score; // Read "Score:" and the score

            // Check if the scoreLabel is "Score:" and the extraction was successful
            if (scoreLabel == "Score:") {
                playerScores[count].first = playerName;
                playerScores[count].second = score;
                ++count;
            }
        }
    }

    file.close();

    // Sort playerScores in descending order based on score
    sort(playerScores, playerScores + count, [](const auto& a, const auto& b) {
        return a.second > b.second;
        });

    // Display the top players
    if (count > 0) {
        setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
        cout << "=================================\n";
        setConsoleColor(FOREGROUND_RED, FOREGROUND_INTENSITY);
        cout << "|           Top Scorers         |\n";
        setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
        cout << "=================================\n\n";
        setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
        for (int i = 0; i < count; ++i) {
            if (i <= 9) {
                cout << "Rank" << " 0" << i + 1 << "  ";
                SetConsoleTextAttribute(hConsole, 13);
                cout << playerScores[i].first;
                setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
                cout << "  - Scores : ";
                setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
                cout << playerScores[i].second << endl;
                setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
            }
            else {
                setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
                cout << "Rank" << " " << i + 1 << "  ";
                SetConsoleTextAttribute(hConsole, 13);
                cout << playerScores[i].first; 
                setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
                cout << "  - Scores : ";
                setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
                cout << playerScores[i].second << endl;
                setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
            }
        }
    }
    else {
        cout << "No player scores found in the file.\n";
    }
    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
    cout << "\n=================================\n\n";
    cout << endl;
    setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN, FOREGROUND_INTENSITY);
}


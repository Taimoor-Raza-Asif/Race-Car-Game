#pragma once
#include<iostream>
using namespace std;

class PowerUp {   
public:
    PowerUp(int position) : position_(position) {}
    int getPosition() const { return position_; }  // it will return the position of powerups

private:
    int position_;
};

class PowerUpList {    // This is linked for managing powerups @ in the game 
public:
    PowerUpList() : head_(nullptr) {}
    ~PowerUpList();

    void addPowerUp(int position);   // insertion of powerups
    bool hasPowerUp(int position) const;        // checking if there is powerup
    void removePowerUp(int position);    // deleteion 

private:
    struct Node {   // node for powerup list
        PowerUp* data;
        Node* next;
    };

    Node* head_;
};

PowerUpList::~PowerUpList() {   // destructor
    Node* current = head_;
    while (current) {
        Node* next = current->next;
        delete current->data;
        delete current;
        current = next;
    }
}

void PowerUpList::addPowerUp(int position) {      // defining insertion
    PowerUp* newPowerUp = new PowerUp(position);
    Node* newNode = new Node{ newPowerUp, head_ };
    head_ = newNode;
}

bool PowerUpList::hasPowerUp(int position) const {
    Node* current = head_;
    while (current) {
        if (current->data->getPosition() == position) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void PowerUpList::removePowerUp(int position) {   // defining deletion
    Node* current = head_;
    Node* prev = nullptr;

    while (current) {
        if (current->data->getPosition() == position) {
            if (prev) {
                prev->next = current->next;
            }
            else {
                head_ = current->next;
            }
            delete current->data;
            delete current;
            break;
        }
        prev = current;
        current = current->next;
    }
}


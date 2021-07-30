#include <iostream>
#include <cstdlib>
#include <utility>
#include <string>
#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;

void digitalWrite(pair<string, int> &pins[4], string &logic){

    for(size_t i=0; i<&pins.size(); i++)
    cout << "digitalWrite(" << &pins[i].second << ", " << &logic << ")" << endl;
}

int main (int argc, char** argv){

    system("color 3e");

    pair<string, int> pinos[4];
    pinos[0].push_back("motorA1", 10);
    pinos[1].push_back("motorA2", 11);
    pinos[2].push_back("motorB1", 12);
    pinos[3].push_back("motorB2", 13);

    string True{"HIGH"};
    string False{"LOW"};
    steady_clock::time_point interval;
    seconds sec(2);
    int counter{0};

    while(counter < 14){
        steady_clock::time_point now = steady_clock::now();
        if (now >= interval){
                for(size_t i=0; i<pinos.size(); i++){
                    digitalWrite(pinos[i].second, True);
                    digitalWrite(pinos[i+1].second, False);
                }
            interval = now + sec;
            counter++;
        }
        else{
            for(size_t i=0; i<pinos.size(); i++){
                digitalWrite(pinos[i].second, False);
                digitalWrite(pinos[i+1].second, True);

            }
           counter++;
    }

    system("pause");
    return 0;
}

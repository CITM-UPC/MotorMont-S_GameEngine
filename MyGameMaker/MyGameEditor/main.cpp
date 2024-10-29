#include <iostream>
#include "MyGameEngine/GameObject.h"
using namespace std;
#include "MyGameEngine/App.h"

int main(int argc, char* argv[]) {
    App app;
    if (!app.Init()) {
        return -1;
    }
    app.Run();
    return 0;
}
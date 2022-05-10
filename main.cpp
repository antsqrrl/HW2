#include <iostream>
#include "hw2.h"

using namespace std;

int main() {
    auto imProc = new ImageProcess();
    imProc->loadImgFromFile("test.txt");
    imProc->updateSrcImg();
    imProc->saveImgToFile("testout.txt");

    return 0;
}

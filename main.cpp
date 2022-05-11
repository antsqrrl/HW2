#include <iostream>
#include "hw2.h"

#define MASK_WIDTH 3
#define MASK_HEIGHT 3
#define MASK_BASE_WIDTH 1
#define MASK_BASE_HEIGHT 1

using namespace std;

const int test_mask[MASK_WIDTH][MASK_HEIGHT] = {
        { 1, 0, 1 },
        { 0, 1, 0 },
        { 1, 0, 1 }};

int main() {
    auto imProc = new ImageProcess();
    auto mask = new Img((const int*)&test_mask, MASK_WIDTH, MASK_HEIGHT);
    imProc->updateMask(*mask, MASK_BASE_WIDTH, MASK_BASE_HEIGHT);
    imProc->loadImgFromFile("test.txt");
    imProc->dilotation();
    //imProc->erosion();
    imProc->saveImgToFile("testout.txt");

    return 0;
}

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
    mask->x_c = MASK_BASE_WIDTH;
    mask->y_c = MASK_BASE_HEIGHT;
    imProc->updateMask(*mask);
    imProc->loadImgFromFile("test3.txt");
    imProc->dilatation();
    //imProc->erosion(0);
    //auto pairs = imProc->getListContours();
    imProc->saveImgToFile("testout.txt");

    return 0;
}

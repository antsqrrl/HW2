//
// Created by belki on 10.05.2022.
//

#include <cstring>
#include <cstdio>
#include <fstream>

#include "hw2.h"

#define MASK_HEIGHT 3
#define MASK_WIDTH 3

using namespace std;

Img::Img()
{

}
Img::Img(int w, int h):Img()
{
    srcImg = new int[w*h];
    width = w;
    height = h;
}
Img::Img(const int *src, int w, int h): Img(w, h)
{
    srcImg = (int*)src;
}
Img::~Img()
{

}

ImageProcess::ImageProcess()
{
    mask = new Img(MASK_WIDTH, MASK_HEIGHT);
}
ImageProcess::ImageProcess(int w, int h): ImageProcess()
{
    srcImg = new Img(w, h);
    processedImg = new Img(w, h);
}
ImageProcess::ImageProcess(const Img *img): ImageProcess(img->width, img->height)
{
    memcpy(srcImg, img, sizeof(int)*((img->width*img->height)+2)); // copy struct
}
ImageProcess::ImageProcess(const char *fileName): ImageProcess()
{
    loadImgFromFile(fileName);
}

ImageProcess::~ImageProcess()
{
    delete mask;
    delete srcImg;
    delete processedImg;
}

int ImageProcess::updateMask(const Img &mask)
{
    memcpy(this->mask, &mask, sizeof(int)*((mask.width*mask.height)+2));
}
int ImageProcess::updateSrcImg()
{
    memcpy(srcImg->srcImg, processedImg->srcImg, sizeof(int)*((srcImg->width*srcImg->height)+2));
    //memcpy(processedImg->srcImg, srcImg->srcImg, sizeof(int)*((srcImg->width*srcImg->height)+2));
}

int ImageProcess::loadImgFromFile(const char *fileName, int format)
{
    FILE* in = fopen(fileName, "r");

    int fwidth = 0, fheight = 0;
    fscanf(in, "%d\t%d\n", &fwidth, &fheight);

    this->srcImg = new Img(fwidth, fheight);
    this->processedImg = new Img(fwidth, fheight);

    int counter = 0;
    char c;

    while(fscanf(in, "%c", &c) != EOF)
    {
        if(c != '\n' && (counter < fwidth * fheight))
        {
            srcImg->srcImg[counter] = c == '0' ? 0 : 1;
            counter++;
        }
    }

    fclose(in);
    return 0;
}
int ImageProcess::saveImgToFile(const char *fileName, int format)
{
    ofstream fout;
    fout.open(fileName);

    if (!fout.is_open())
    {
        return -1;
    }
    fout << processedImg->width << "\t" << processedImg->height << "\n";
    for (int i = 0; i < processedImg->width*processedImg->height; i++)
    {
        fout << processedImg->srcImg[i] ? "1" : "0";
        if ((format == 1) && ((i+1)%processedImg->width == 0))
        {
            fout << "\n";
        }
    }

    fout.close();
}
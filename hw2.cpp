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
    memcpy(srcImg, img, sizeof(int)*((img->width*img->height)+4)); // copy struct
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

int ImageProcess::updateMask(const Img &mask, int maskBaseX, int maskBaseY)
{
    memcpy(this->mask, &mask, sizeof(int)*((mask.width*mask.height)+4));
    this->mask->maskBaseX = maskBaseX;
    this->mask->maskBaseY = maskBaseY;
}
int ImageProcess::updateSrcImg()
{
    memcpy(srcImg->srcImg, processedImg->srcImg, sizeof(int)*((srcImg->width*srcImg->height)+4));
    //memcpy(processedImg->srcImg, srcImg->srcImg, sizeof(int)*((srcImg->width*srcImg->height)+4));
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
    fout.open(fileName, fstream::out);

    //subtract();

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

void ImageProcess::applyDilatationMask(int x, int y)
{
    for (int h = 0; h < mask->height; h++)
    {
        for (int w = 0; w < mask->width; w++)
        {
            int index = h*mask->width + w;
            if (mask->srcImg[index] != 1)
            {
                continue;
            }
            int dX = mask->maskBaseX-w;
            int dY = mask->maskBaseY-h;
            int pX = x-dX;
            int pY = y-dY;
            if ((pX < 0) || (pX >= processedImg->width) || (pY < 0) || (pY >= processedImg->height))
            {
                continue;
            }
            int pIndex = pY*processedImg->width + pX;
//            if (pIndex < 0)
//            {
//                continue;
//            }
            if (processedImg->srcImg[pIndex] == 0)
            {
                processedImg->srcImg[pIndex] = 2;
            }

        }
    }
}
void ImageProcess::applyErosionMask(int x, int y) // забыл что эррозия применяется если полное совпадение с маской
{
    for (int h = 0; h < mask->height; h++)
    {
        for (int w = 0; w < mask->width; w++)
        {
            int index = h*mask->width + w;
            if (mask->srcImg[index] != 1)
            {
                continue;
            }
            int dX = mask->maskBaseX-w;
            int dY = mask->maskBaseY-h;
            int pX = x-dX;
            int pY = y-dY;
            if ((pX < 0) || (pX >= processedImg->width) || (pY < 0) || (pY >= processedImg->height))
            {
                continue;
            }
            int pIndex = pY*processedImg->width + pX;
//            if (pIndex < 0)
//            {
//                continue;
//            }
            if (processedImg->srcImg[pIndex] == 1)
            {
                processedImg->srcImg[pIndex] = 0;
            }

        }
    }
}

int ImageProcess::dilotation()
{
    memcpy(processedImg->srcImg, srcImg->srcImg, sizeof(int)*((srcImg->width*srcImg->height)+4));
    for (int h = 0; h < processedImg->height; h++)
    {
        for (int w = 0; w < processedImg->width; w++)
        {
            int index = h*processedImg->width + w;
            if (processedImg->srcImg[index] != 1)
            {
                continue;
            }
            applyDilatationMask(w, h);
        }
    }
    for (int i = 0;i < processedImg->width*processedImg->height; i++)
    {
        processedImg->srcImg[i] = processedImg->srcImg[i] == 0 ? 0 : 1;
    }
//    subtract();
}
int ImageProcess::erosion()
{
    memcpy(processedImg->srcImg, srcImg->srcImg, sizeof(int)*((srcImg->width*srcImg->height)+4));
    for (int h = 0; h < processedImg->height; h++)
    {
        for (int w = 0; w < processedImg->width; w++)
        {
            int index = h*processedImg->width + w;
            if (processedImg->srcImg[index] != 1)
            {
                continue;
            }
            applyErosionMask(w, h);
        }
    }
}
void ImageProcess::subtract()
{
    for (int i = 0;i < processedImg->width*processedImg->height; i++)
    {
        if (srcImg->srcImg[i] == 1)
        {
            processedImg->srcImg[i] = 0;
        }
    }
}
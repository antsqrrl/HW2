//
// Created by belki on 10.05.2022.
//

#include <cstring>
#include <cstdio>
#include <fstream>
#include <algorithm>

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

int ImageProcess::updateMask(const Img &mask)
{
    memcpy(this->mask, &mask, sizeof(int)*((mask.width*mask.height)+4));
}
int ImageProcess::updateSrcImg()
{
    memcpy(srcImg->srcImg, processedImg->srcImg, sizeof(int)*((srcImg->width*srcImg->height)+4));
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
            int dX = w - mask->x_c;
            int dY = h - mask->y_c;
            int pX = x+dX;
            int pY = y+dY;
            if ((pX < 0) || (pX >= processedImg->width) || (pY < 0) || (pY >= processedImg->height))
            {
                continue;
            }
            int pIndex = pY*processedImg->width + pX;
            if (processedImg->srcImg[pIndex] == 0)
            {
                processedImg->srcImg[pIndex] = 2;
            }

        }
    }
}
void ImageProcess::checkAndApplyErosionMask(int x, int y)
{
    bool isApplyMask = false;
    for (int h = 0; h < mask->height; h++)
    {
        for (int w = 0; w < mask->width; w++)
        {
            int index = h*mask->width + w;
            if (mask->srcImg[index] != 1)
            {
                continue;
            }
            int dX = w - mask->x_c;
            int dY = h - mask->y_c;
            int pX = x+dX;
            int pY = y+dY;
            if ((pX < 0) || (pX >= processedImg->width) || (pY < 0) || (pY >= processedImg->height))
            {
                continue;
            }
            int pIndex = pY*processedImg->width + pX;
            if (processedImg->srcImg[pIndex] != 1)
            {
                isApplyMask = true;
            }
        }
    }
    if (isApplyMask)
    {
        int pIndex = y*processedImg->width + x;
        processedImg->srcImg[pIndex] = 5;
    }
    ////////
//    if (isApplyMask)
//    {
//        for (int h = 0; h < mask->height; h++)
//        {
//            for (int w = 0; w < mask->width; w++)
//            {
//                int index = h * mask->width + w;
//                if (mask->srcImg[index] != 1)
//                {
//                    continue;
//                }
//                int dX = w - mask->x_c;
//                int dY = h - mask->y_c;
//                int pX = x + dX;
//                int pY = y + dY;
//                if ((pX < 0) || (pX >= processedImg->width) || (pY < 0) || (pY >= processedImg->height))
//                {
//                    continue;
//                }
//                if ((pX == 0) || (pY == 0))
//                {
//                    continue;
//                }
//                int pIndex = pY * processedImg->width + pX;
//                if (processedImg->srcImg[pIndex] == 1)
//                {
//                    processedImg->srcImg[pIndex] = 5;
//                }
//            }
//        }
//    }

}

int ImageProcess::dilatation(int srcImg)
{
    if (srcImg == 1)
    {
        memcpy(processedImg->srcImg, this->srcImg->srcImg, sizeof(int)*((this->srcImg->width*this->srcImg->height)+4));
    }
    //return 0;
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
    subtract();
}
int ImageProcess::erosion(int srcImg)
{
    if (srcImg == 1)
    {
        memcpy(processedImg->srcImg, this->srcImg->srcImg, sizeof(int)*((this->srcImg->width*this->srcImg->height)+4));
    }
    for (int h = 0; h < processedImg->height; h++)
    {
        for (int w = 0; w < processedImg->width; w++)
        {
            int index = h*processedImg->width + w;
            if (processedImg->srcImg[index] == 0)
            {
                continue;
            }
            checkAndApplyErosionMask(w, h);
        }
    }
//    for (int i = 0;i < processedImg->width*processedImg->height; i++)
//    {
//        processedImg->srcImg[i] = processedImg->srcImg[i] == 1 ? 1 : 0;
//    }
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

std::list<std::list<std::pair<int, int>>> ImageProcess::getListContours()
{
    buildContours();
    list<std::list<std::pair<int, int>>> pContours;
    for (int i = 0; i < contours.size(); i++)
    {
        list<pair<int, int>> pContour;
        auto contour = contours[i];
        for (int j = 0; j < contour->size(); j++)
        {
            pContour.push_back(getCoordsByIndex((*contour)[j]));
        }
        pContours.push_back(pContour);
    }
    return pContours;
}

void ImageProcess::buildContours()
{
    this->contours.clear();
    for (int i = 0; i < processedImg->width*processedImg->height; i++)
    {
        if (processedImg->srcImg[i] == 1)
        {
            auto contour = isIndexInList(i);
            if (contour != nullptr)
            {
                continue;
            }
            bool isAdded = false;
            vector<vector<int>*> contoursToMerge;
            auto coords = getCoordsByIndex(i);
            for (int y = -1; y <= 1; y++)
            {
                for (int x = -1; x <= 1; x++)
                {
                    auto nIdx =  (y+coords.second)*processedImg->width + x + coords.first;
                    if ((x == 0) && (y == 0))
                    {
                        continue;
                    }
                    auto cont = isIndexInList(nIdx);
                    if (cont != nullptr)
                    {
                        if (!isAdded)
                        {
                            cont->push_back(i);
                            isAdded = true;
                        }
                        if (!(std::find(contoursToMerge.begin(), contoursToMerge.end(), cont) != contoursToMerge.end()))
                        {
                            contoursToMerge.push_back(cont);
                        }
                    }
                }
            }
            if (!isAdded)
            {
                //vector<int> cntr;
                auto cntr = new vector<int>;
                cntr->push_back(i);
                contours.push_back(cntr);
            }
            else
            {
                if (contoursToMerge.size() > 1)
                {
                    mergeContours(contoursToMerge);
                }
            }
        }
    }
}

std::vector<int> *ImageProcess::isIndexInList(int index)
{
    for (int i = 0; i < contours.size(); i++)
    {
        std::vector<int> contour = *(contours[i]);
        if ((std::find(contour.begin(), contour.end(), index) != contour.end()))
        {
            return contours[i];
        }
    }
    return nullptr;
}

std::pair<int, int> ImageProcess::getCoordsByIndex(int index)
{
    int y = index / processedImg->width;
    int x = index - y*processedImg->width;
    return make_pair(x, y);
}

void ImageProcess::mergeContours(std::vector<std::vector<int> *> conts)
{
    for (int i = 1; i < conts.size(); i++)
    {
        for (int j = 0; j < conts[i]->size(); j++)
        {
            conts[0]->push_back((*conts[i])[j]);
        }
        contours.erase(std::remove(contours.begin(), contours.end(), conts[i]), contours.end());
    }
}
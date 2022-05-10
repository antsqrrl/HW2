//
// Created by belki on 10.05.2022.
//

#ifndef HW2_HW2_H
#define HW2_HW2_H

struct Img {
        int* srcImg;
        int width;
        int height;

        Img();
        Img(int w, int h);
        Img( const int* src, int w, int h);
        ~Img();
};

class ImageProcess {
private :
    Img* srcImg;
    Img* processedImg;
    Img* mask;
    //здесь можно объявлять другие переменные и функции
public :
    //здесь нельзя объявлять функции и переменные
    /**/
    ImageProcess();
    /* выделяется память для картинки размера w*h */
    ImageProcess(int w, int h);
    /* выделяется память для картинки размера w*h и копируется картинка*/
    ImageProcess(const Img* img);
    /* выделяется память для картинки копируется картинка из файла*/
    ImageProcess(const char* fileName);
    ~ImageProcess();

    /*задание маски*/
    int updateMask(const Img& mask);
    /*перезаписать исходную картинку картинкой, которая была получена в результате дилотации и/или эрозии*/
    int updateSrcImg();
    /*дилотация картинки, результат записать в processedImg*/
    int dilotation();
    /*эрозия картинки, результат записать в processedImg*/
    int erosion();
    /*
        загрузить/сохранить из файла с имененм fileName картинку
        input		:	fileName - имя файла
                        format - формат файла 	0 - последовательность 0 и 1, без перехода на новую строку
                                                1 - последовательность 0 и 1, с переходом на новую строку
    */
    int loadImgFromFile(const char* fileName, int format = 1);
    int saveImgToFile(const char* fileName, int format = 1);
};

#endif //HW2_HW2_H

#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    float tg = (y1-y0)/(float)(x1-x0);
    bool steep = false;
    if (std::abs(tg)>=1){
        std::swap(x0, y0);
        std::swap(x1, y1);
        tg = 1/tg;
        steep = true;
    }
    if (x0>x1) { // make it left−to−right 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    for (int x = x0; x<=x1; x++) {
        int y = (x-x0)*tg+y0;
        if (steep)
            image.set(y, x, color);
        else
            image.set(x, y, color);
    }
}

void curve(int x0, int y0, int x1, int y1,
           int v0x, int v0y, int v1x, int v1y,
           TGAImage &image, TGAColor color){
    // endpoints for vectors B for p0, C for p1
    float Bx = x0 + v0x;
    float By = y0 + v0y;
    float Cx = x1 + v1x;
    float Cy = y1 + v1y;

    for(float t = 0.0; t < 1.0 ; t += 0.01){
        // Target Coordinates
        float x = x0*pow(1-t, 3)+3*Bx*pow(1-t, 2)*t + 3*Cx*(1-t)*pow(t, 2) + x1*pow(t, 3);
        float y = y0*pow(1-t, 3)+3*By*pow(1-t, 2)*t + 3*Cy*(1-t)*pow(t, 2) + y1*pow(t, 3);
        image.set(x, y, color);
    }
}

int ipart(int x){
    return std::floor(x);
}

int round(int x){
    return ipart(x+0.5);
}

float fpart(float x){
    return (x-std::floor(x));
}

float rfpart(float x){
    return (1-fpart(x));
}
   
void wuline(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color){
    bool steep = std::abs(y1-y0) > std::abs(x1-x0);
    if (steep){
        std::swap(x0,y0);
        std::swap(x1,y1);
    }
    if (x0>x1){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    
    int dx = x1-x0;
    int dy = y1-y0;
    float gradient = (float)dy/(float)dx;
    if (dx == 0.0)
        gradient = 1.0;
    
    //first endpoint
    float xend = std::floor(x0+0.5);
    float yend = y0+gradient*(xend-x0);
    float xgap = rfpart(x0+0.5);
    float xpxl1 = xend;
    float ypxl1 = ipart(yend);
    if (steep)
    {
        int colr = rfpart(yend) * xgap * 255;
        if (colr != 0)
            image.set(ypxl1, xpxl1, TGAColor(colr, colr, colr, 255));
        int colr2 = fpart(yend) * xgap * 255;
        if (colr != 0)
            image.set(ypxl1+1, xpxl1, TGAColor(colr2, colr2, colr2, 255));
    }
    else {
        int colr = rfpart(yend) * xgap * 255;
        if (colr != 0)
            image.set(xpxl1, ypxl1, TGAColor(colr, colr, colr, 255));
        int colr2 = fpart(yend) * xgap * 255;
        if (colr != 0)
            image.set(xpxl1, ypxl1+1, TGAColor(colr2, colr2, colr2, 255));
    }
    float intery = yend + gradient;
    
    //second endpoint
    xend = round(x1);
    yend = y1+gradient*(xend-x1);
    xgap = fpart(x1+0.5);
    float xpxl2 = xend;
    float ypxl2 = ipart(yend);
    if (steep)
    {
        int colr = rfpart(yend) * xgap * 255;
        if (colr != 0)
            image.set(ypxl2, xpxl2, TGAColor(colr, colr, colr, 255));
        int colr2 = fpart(yend) * xgap * 255;
        if (colr != 0)
            image.set(ypxl2+1, xpxl2, TGAColor(colr2, colr2, colr2, 255));
    }
    else {
        int colr = rfpart(yend) * xgap * 255;
        if (colr != 0)
            image.set(xpxl2, ypxl2, TGAColor(colr, colr, colr, 255));
        int colr2 = fpart(yend) * xgap * 255;
        if (colr != 0)
            image.set(xpxl2, ypxl2+1, TGAColor(colr2, colr2, colr2, 255));
    }
    
    //main loop
    if (steep)
    {
        for(int x=xpxl1+1; x<xpxl2;x++){
            int colr = rfpart(intery)*255;
            if (colr != 0)
                image.set(ipart(intery), x, TGAColor(colr, colr, colr, 255));
            int colr2 = fpart(intery)*255;
            if (colr2 != 0)
                image.set(ipart(intery)+1, x, TGAColor(colr2, colr2, colr2, 255));
            intery = intery+gradient;
        }
    }
    else
    {
        for(int x=xpxl1+1; x<xpxl2;x++){
            int colr = rfpart(intery)*255;
            if (colr != 0)
                image.set(x, ipart(intery), TGAColor(colr, colr, colr, 255));
            int colr2 = fpart(intery)*255;
            if (colr2 != 0)
                image.set(x, ipart(intery)+1, TGAColor(colr2, colr2, colr2, 255));
            intery = intery+gradient;
        }
    }
}

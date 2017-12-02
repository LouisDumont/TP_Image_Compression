/******************************************************
* Compression of an image using QuadTree structures and Imagine++ library
* Author: Louis Dumont
* Last Modified: 02/12/2017
*******************************************************/

#include <iostream>
#include "quadtree.h"
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>

using namespace Imagine;
using namespace std;

// Global variables
QuadLeaf<byte> whiteLeaf = QuadLeaf<byte>(0);
QuadLeaf<byte> blackLeaf = QuadLeaf<byte>(1);

const int test=3;


/*---------------------------------------------------------*
 * Creating a quadtree representing the Image Img
 *---------------------------------------------------------*/

// Recursive function creating the QuadTree of a part of the
// Image Img delimited by xMin, xMax, yMin, yMax
template<typename T> QuadTree<T>* buildQuadTree
        (Image<T> Img, int xMin, int xMax, int yMin, int yMax){

    QuadTree<T>* res;
    //Case of a pixel
    if (xMin==xMax && yMin==yMax){
        //Stocking the data from the pixel into the leaf
        res = new QuadLeaf<T>(Img[xMax,yMax]);
    }

        //Case of a bigger space
    else {
        int xMid = (xMin + xMax) / 2;
        int yMid = (yMin + yMax) / 2;
        //Construction of Sons
        QuadTree<T>* fNW = buildQuadTree(Img, xMin, xMid, yMin, yMid);
        QuadTree<T>* fNE = buildQuadTree(Img, xMin, xMid, yMid, yMax);
        QuadTree<T>* fSW = buildQuadTree(Img, xMid, xMax, yMin, yMid);
        QuadTree<T>* fSE = buildQuadTree(Img, xMid, xMax, yMid, yMax);
        //A leaf is constructed if all the sons are leafs with the same value
        bool condNorth = ((fNW->isLeaf() && fNE->isLeaf()) && (fNW->value() == fNE->value()));
        bool condSouth = ((fSW->isLeaf() && fSE->isLeaf()) && (fSW->value() == fSE->value()));
        if ((condNorth && condSouth) && (fNE->value() == fSE->value())) {
            res = new QuadLeaf<T>(fNW->value());
            delete [] fNW;
            delete [] fNE;
            delete [] fSE;
            delete [] fSW;
        }
            //Otherwise it's a node
        else {
            res = new QuadNode<T>(fNW, fNE, fSE, fSW);
        }
    }

    return res;
}

//Fancy function to be called by the user
template <typename T> QuadTree<T>* imgToQuadTree(Image<T> Img){
    //Calling the recursive function on the whole Image
    int Iwidth  = Img.width();
    int Iheight = Img.height();
    return buildQuadTree(Img, 0, Iwidth, 0, Iheight);
}

//Second and better less memory-consuming recursive function
//for black and white Images
template<typename T> QuadTree<T>* buildQuadTree2
        (Image<T> img, int xMin, int xMax, int yMin, int yMax){
    QuadTree<T>* res;

    //Case of a pixel
    if (xMin==xMax && yMin==yMax){
        if (img[xMax,yMax]==0){res = &whiteLeaf;}
        else res = &blackLeaf;
    }

    //Case of a bigger space
    else{
        int xMid = (xMin + xMax) / 2;
        int yMid = (yMin + yMax) / 2;
        //Construction of Sons
        QuadTree<T>* fNW = buildQuadTree2(img, xMin, xMid, yMin, yMid);
        QuadTree<T>* fNE = buildQuadTree2(img, xMin, xMid, yMid, yMax);
        QuadTree<T>* fSW = buildQuadTree2(img, xMid, xMax, yMin, yMid);
        QuadTree<T>* fSE = buildQuadTree2(img, xMid, xMax, yMid, yMax);
        //A leaf is constructed if all the sons are leafs with the same value
        bool condNorth = ((fNW->isLeaf() && fNE->isLeaf()) && (fNW->value() == fNE->value()));
        bool condSouth = ((fSW->isLeaf() && fSE->isLeaf()) && (fSW->value() == fSE->value()));
        if ((condNorth && condSouth) && (fNE->value() == fSE->value())) {
            if(fNE->value()==0){res = &whiteLeaf;}
            else res = &blackLeaf;
            delete [] fNW;
            delete [] fNE;
            delete [] fSE;
            delete [] fSW;
        }
            //Otherwise it's a node
        else {
            res = new QuadNode<T>(fNW, fNE, fSE, fSW);
        }
    }

    return res;
}




/*-----------------------------------------------------------*
 * Getting back the image from the QuadTree representing it
 *-----------------------------------------------------------*/

// Unfinished
template <typename T> Image<T> imgFromQuadTree(QuadTree<T> tree){
    Image<T> ImRes;
    return ImRes;
}


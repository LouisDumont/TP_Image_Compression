//An image compression program using the Imagine++ library
//author: LouisDumont
//last modified: 29/11/2017

#include <iostream>
#include "quadtree.h"
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>

using namespace Imagine;
using namespace std;




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






int main() {
    //Loading and displaying the image
    Image<byte> I1;
    const char* fic1 = srcPath("running-horse-square.png");
    if(! load(I1, fic1)) {
        cout << "Probleme dans le chargement d'images" << endl;
        return 1;
    }
    for (int i=1; i<I1.width()-10;i++){
        I1[0,i]=1;
    }
    I1[I1.width()/2,I1.height()-2]=0;
    Window W1 = openWindow(I1.width(), I1.height());
    display(I1);
    endGraphics();

    //Tests on the new functions
    for (int i=1; i<100; i++) {
        QuadTree<byte> *Compr1 = imgToQuadTree(I1);
        delete[] Compr1;
    }

    //Tests on the quadtree structure
    std::cout << "Hello, World!" << std::endl;
    // Leaf and node creation
    QuadTree<int> *ql, *qn;
    ql = new QuadLeaf<int>(1);                          // ql = 1
    qn = new QuadNode<int>(0,0,new QuadLeaf<int>(2),ql);// qn = [_,_,2,1]
    // Leaf value modifications
    ql->value() = 3;                                   // qn = [_,_,2,3]
    ql->value()++;                                     // qn = [_,_,2,4]
    // Node modifications
    qn->son(NW) = new QuadLeaf<int>(6);                // qn = [6,_,2,3]
    qn->son(NW) = new QuadNode<int>(0,0,0,qn->son(NW));// qn = [[_,_,_,6],_,2,3]
    qn->son(NE) = new QuadLeaf<int>(7);                // qn = [[_,_,_,6],7,2,4]
    qn->son(SE) = new QuadNode<int>(0,0,qn->son(SE),0);
    // qn = [[_,_,_,6],7,[_,_,2,_],4]
    // Modification with (sub)typing test: increment leaves at depth 1
    for (int d = 0; d < nQuadDir; d++) {
        QuadTree<int> *q = qn->son(d);
        if (q && q->isLeaf()) q->value()++;    // qn = [[_,_,_,6],8,[_,_,2,_],5]
    }
    qn->son(NW) = new QuadNode<int>(0,0,qn->son(NW),0);
    // qn = [[_,_,_,[_,_,6,_]],8,[_,_,2,_],5]
    /*
       This corresponds to the following quadtree:

         +---------------+---------------+
         |       |       |               |
         |       |       |               |
         |       |       |               |
         +-------+-------+       8       |
         |       |   |   |               |
         |       +---+---+               |
         |       | 6 |   |               |
         +-------+-------+-------+-------+
         |               |       |       |
         |               |       |       |
         |               |       |       |
         |       5       +-------+-------+
         |               |       |       |
         |               |       |   2   |
         |               |       |       |
         +---------------+-------+-------+
    */
    // Display quadtree, e.g. for debugging
    display(qn);
    // Don't forget to delete it when not needed anymore
    delete qn;
    return 0;
}
/*************************************************************
 * An image compression program using the Imagine++ library
 * Author: Louis Dumont
 * Last Modified: 02/12/2017
 ************************************************************/


#include <iostream>
#include "quadtree.h"
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
#include "image_quadtree.h"

using namespace Imagine;
using namespace std;



int main() {
    cout<<test<<endl;
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
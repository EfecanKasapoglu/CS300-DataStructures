//
// Created by Efecan Kasapoğlu on 7.11.2024.
//

#ifndef CS300_TH2_QUADTREE_H
#define CS300_TH2_QUADTREE_H
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Quadtree{ //Quadtree class
public:

    Quadtree(); //constructor
    Quadtree(const Quadtree & rhs);//copy constructor
    ~Quadtree();//deconstructor
    void insert(const string & CityName,int xCoordinate, int yCoordinate);//adds city
    void search(int xQuery, int yQuery, int radius, vector<string>& foundCities, vector<string>& visitedCities) const; //searchs
    void printTree() const;//prints tree
    bool isEmpty() const; //checks if the tree is empty or not
private:
    struct QuadTreeNode{ //node
        string CityName; //name of the city
        int xCoordinate,yCoordinate;// coordinates
        QuadTreeNode* NorthWest;
        QuadTreeNode* NorthEast;
        QuadTreeNode* SouthWest;
        QuadTreeNode* SouthEast;

        QuadTreeNode(string CityName, int xCoordinate, int yCoordinate);
    };
    QuadTreeNode* Root; // root of tree
    QuadTreeNode* insert(const string & cityName, int xCoordinate, int yCoordinate, QuadTreeNode*& t); // inserts node
    void search(int xQuery, int yQuery, int radius, QuadTreeNode* t, vector<string>& foundCities, vector<string>& visitedCities) const; //searchs the tree
    QuadTreeNode* printTree(QuadTreeNode* t) const; //prints the tree
    void makeEmpty(QuadTreeNode*& t); //clears the tree
    QuadTreeNode* clone(QuadTreeNode* t) const;//clones the tree
};
Quadtree::Quadtree(){//constructor
    Root= nullptr;
}
Quadtree::Quadtree(const Quadtree & rhs){//copy constructor
    Root=nullptr;
    if(rhs.Root!=nullptr){
        *this = rhs;
    }
}
Quadtree::~Quadtree(){//deconstructor
    makeEmpty(Root);//clears the tree
}
Quadtree::QuadTreeNode::QuadTreeNode(string cityNameGiven, int xCoordinateGiven, int yCoordinateGiven){ //constructor for quadtreenode
    CityName=cityNameGiven;
    xCoordinate=xCoordinateGiven;
    yCoordinate=yCoordinateGiven;
    NorthEast=nullptr;
    NorthWest=nullptr;
    SouthEast=nullptr;
    SouthWest=nullptr;
}

void Quadtree::insert(const string & cityName, int xCoordinate, int yCoordinate){//public insert
    Root=insert(cityName, xCoordinate, yCoordinate, Root);//private insert method
}

Quadtree::QuadTreeNode* Quadtree::insert(const string & cityName, int xCoordinate, int yCoordinate, QuadTreeNode*& t){//private insert, add new node
    if(t==nullptr){
       return t=new QuadTreeNode(cityName,xCoordinate,yCoordinate);//creates new node
    }
    else if(t->xCoordinate>xCoordinate && t->yCoordinate!=yCoordinate){
        if(t->yCoordinate>yCoordinate){
            insert(cityName,xCoordinate,yCoordinate,t->SouthWest); //adds to SW
        }
        else if(t->yCoordinate<yCoordinate){
            insert(cityName,xCoordinate,yCoordinate,t->NorthWest);//adds to NW
        }

    }
    else if(t->xCoordinate==xCoordinate && t->yCoordinate!=yCoordinate){
        if(t->yCoordinate>yCoordinate){
            insert(cityName,xCoordinate,yCoordinate,t->SouthEast);//adds to SE
        }
        else if(t->yCoordinate<yCoordinate){
            insert(cityName,xCoordinate,yCoordinate,t->NorthEast);//adds to NE
        }

    }
    else if(t->xCoordinate!=xCoordinate && t->yCoordinate==yCoordinate){
        if(t->xCoordinate>xCoordinate){
            insert(cityName,xCoordinate,yCoordinate,t->NorthWest);//adds to NW
        }
        else if(t->xCoordinate<xCoordinate){
            insert(cityName,xCoordinate,yCoordinate,t->NorthEast);//adds to NE
        }
    }
    else if(t->xCoordinate<xCoordinate && t->yCoordinate!=yCoordinate){
        if(t->yCoordinate>yCoordinate){
            insert(cityName,xCoordinate,yCoordinate,t->SouthEast);//adds to SE
        }
        else if(t->yCoordinate<yCoordinate){
            insert(cityName,xCoordinate,yCoordinate,t->NorthEast);//adds to NE
        }
    }
    return t;
}

void Quadtree::search(int xQuery,int yQuery,int radius,vector<string>& foundCities,vector<string>& visitedCities) const { // public search method
    search(xQuery,yQuery,radius,Root,foundCities,visitedCities);//private search method
}
void Quadtree::printTree() const {//prints the tree, public method
    if(isEmpty()){
        cout<<"Tree is empty"<<endl;
    }
    else{
        printTree(Root);//private method
    }
}
bool Quadtree::isEmpty() const{//checks if the tree is empty
    return Root==nullptr;
}
void Quadtree::search(int xQuery,int yQuery,int radius,QuadTreeNode* t,vector<string>& foundCities,vector<string>& visitedCities) const { //recursive search
    if(t==nullptr){
        return; // if it is empty stops searching
    }
    int RadiusSquare=radius*radius; //radius square
    int XDifferenceSquare=(xQuery-t->xCoordinate)*(xQuery-t->xCoordinate); //Square of x difference
    int YDifferenceSquare=(yQuery-t->yCoordinate)*(yQuery-t->yCoordinate);//Square of y difference
    visitedCities.push_back((t->CityName));//pushbacks the city which is visited
    if(XDifferenceSquare+YDifferenceSquare<=RadiusSquare){// this checks if it is in the circle
        foundCities.push_back(t->CityName);//if it is than adds to foundcities vector
    }

   if(t->yCoordinate>yQuery+radius && t->xCoordinate<xQuery-radius){//Zone 1
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
   }
   else if(t->yCoordinate>yQuery+radius && t->xCoordinate>xQuery-radius && t->xCoordinate<xQuery+radius){ //zone 2
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->SouthWest,foundCities,visitedCities);
   }
   else if(t->yCoordinate>yQuery+radius && t->xCoordinate>xQuery+radius){ //zone3
       search(xQuery,yQuery,radius,t->SouthWest,foundCities,visitedCities);
   }
   else if(t->yCoordinate<yQuery+radius && t->yCoordinate>yQuery-radius && t->xCoordinate<xQuery-radius){ //zone 4
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
   }
   else if(t->yCoordinate<yQuery+radius && t->yCoordinate>yQuery-radius && t->xCoordinate>xQuery+radius){ //zone 5
       search(xQuery,yQuery,radius,t->SouthWest,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   else if(t->yCoordinate<yQuery-radius && t->xCoordinate< xQuery-radius){ //zone 6
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
   }
   else if(t->yCoordinate<yQuery-radius && t->xCoordinate<xQuery+radius && t->xCoordinate> xQuery-radius){ //zone 7
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   else if(t->yCoordinate<yQuery-radius &&t->xCoordinate>xQuery+radius){ //zone 8
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   else if(t->yCoordinate>yQuery && t->yCoordinate<yQuery+radius && t->xCoordinate<xQuery && t->xCoordinate>xQuery-radius && XDifferenceSquare+YDifferenceSquare>RadiusSquare){ // zone 9
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->SouthWest,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
   }
   else if(t->yCoordinate>yQuery && t->yCoordinate<yQuery+radius && t->xCoordinate>xQuery && t->xCoordinate<xQuery+radius && XDifferenceSquare+YDifferenceSquare>RadiusSquare){ //zone 10
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->SouthWest,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   else if(t->yCoordinate<yQuery && t->yCoordinate>yQuery-radius && t->xCoordinate<xQuery && t->xCoordinate>xQuery-radius && XDifferenceSquare+YDifferenceSquare>RadiusSquare){ //zone 11
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   else if(t->yCoordinate<yQuery && t->yCoordinate>yQuery-radius && t->xCoordinate>xQuery && t->xCoordinate<xQuery+radius && XDifferenceSquare+YDifferenceSquare>RadiusSquare){ //zone 12
       search(xQuery,yQuery,radius,t->SouthWest,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   //Corner Cases
   else if(t->yCoordinate==yQuery+radius && t->xCoordinate<xQuery && t->xCoordinate>xQuery-radius){ //Case A, given in the additional file
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->SouthWest,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
   }

   else if(t->yCoordinate==yQuery+radius && t->xCoordinate>xQuery && t->xCoordinate<xQuery+radius){ //Case B
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->SouthWest,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }

   else if (t->xCoordinate==xQuery+radius && t->yCoordinate>yQuery && t->yCoordinate<yQuery+radius){ //Case C
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->SouthWest,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   else if(t->xCoordinate==xQuery+radius && t->yCoordinate<yQuery && t->yCoordinate>yQuery-radius){ //Case D
       search(xQuery,yQuery,radius,t->SouthWest,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   else if(t->yCoordinate==yQuery-radius && t->xCoordinate>xQuery && t->xCoordinate<xQuery+radius){ //Case E
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   else if(t->yCoordinate==yQuery-radius && t->xCoordinate<xQuery && t->xCoordinate>xQuery-radius){ //Case F
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   else if(t->xCoordinate==xQuery-radius && t->yCoordinate<yQuery && t->yCoordinate>yQuery-radius){ //Case G
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
   }
   else if(t->xCoordinate==xQuery-radius && t->yCoordinate>yQuery && t->yCoordinate<yQuery+radius){ //Case I
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
   }
   else if(t->yCoordinate==yQuery+radius && t->xCoordinate>xQuery+radius){ //Case P
       search(xQuery,yQuery,radius,t->SouthWest,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   else if(t->yCoordinate==yQuery-radius && t->xCoordinate>xQuery+radius){ //Case R
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   else if(t->xCoordinate==xQuery+radius && t->yCoordinate<yQuery-radius){ // Case J
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   else if(t->xCoordinate==xQuery-radius && t->yCoordinate<yQuery-radius){ // Case K
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
   }
   else if(t->yCoordinate==yQuery-radius && t->xCoordinate<xQuery-radius){ // Case L
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   else if(t->yCoordinate==yQuery+radius && t->xCoordinate<xQuery-radius){ // Case M
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   else if(t->xCoordinate==xQuery-radius && t->yCoordinate>yQuery+radius){ // Case N
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
   }
   else if(t->xCoordinate==xQuery+radius && t->yCoordinate>yQuery+radius){ // Case O
       search(xQuery,yQuery,radius,t->SouthWest,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
   }
   else if(t->yCoordinate==yQuery+radius && t->xCoordinate==xQuery-radius){ // Case X
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->SouthWest,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
   }
   else if(t->yCoordinate==yQuery+radius && t->xCoordinate==xQuery+radius){ // Case Y
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->SouthWest,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   else if(t->yCoordinate==yQuery-radius && t->xCoordinate==xQuery+radius){ // Case Z
       search(xQuery,yQuery,radius,t->SouthWest,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
   else if(t->yCoordinate==yQuery-radius && t->xCoordinate==xQuery-radius){ // Case W
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }

   else{ // Cases in the circle
       search(xQuery,yQuery,radius,t->SouthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->SouthWest,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthEast,foundCities,visitedCities);
       search(xQuery,yQuery,radius,t->NorthWest,foundCities,visitedCities);
   }
}


Quadtree::QuadTreeNode* Quadtree::printTree(QuadTreeNode* t) const {//prints the tree, recursive
    if(t!=nullptr){
        cout<<t->CityName<<endl;
        printTree(t->SouthEast);
        printTree(t->SouthWest);
        printTree(t->NorthEast);
        printTree(t->NorthWest);
    }
    return t;
}

void Quadtree::makeEmpty(QuadTreeNode*& t){//makes empty the tree
    if(t!=nullptr){//checks the current node
        makeEmpty(t->NorthWest);
        makeEmpty(t->NorthEast);
        makeEmpty(t->SouthWest);
        makeEmpty(t->SouthEast);
        delete t;//deletes node
    }
    t=nullptr;
}

Quadtree::QuadTreeNode* Quadtree::clone(QuadTreeNode* t)const{
    if(t==nullptr){//checks the current node
        return nullptr;
    }
    else{//creates a new node and copy the tree
        QuadTreeNode* newNode =new QuadTreeNode(t->CityName,t->xCoordinate,t->yCoordinate);
        newNode->NorthWest = clone(t->NorthWest);
        newNode->NorthEast = clone(t->NorthEast);
        newNode->SouthWest = clone(t->SouthWest);
        newNode->SouthEast = clone(t->SouthEast);
        return newNode;
    }
}


#endif //CS300_TH2_QUADTREE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Quadtree.h"

using namespace std;

int main() {
    Quadtree Quadtree; // creates Quadtree
    string line,CityName;
    int MaxX,MaxY,Xcoordinate,Ycoordinate,Xquery,Yquery,Radius;
    ifstream CitiesInput("cities.txt"); // opens cities.txt
    if(!CitiesInput){ // if it does not open than gives error
        cout<<"Can not open Cities.txt";
    }
    CitiesInput>>MaxX>>MaxY; // takes maximum X and Y from the first row
    CitiesInput.ignore(); // skips the end of line
    while(getline(CitiesInput,line)){ //starts to read the line
        if(line.empty()){ // this is for empty rows
            continue;
        }
        stringstream ss(line); // separates the line
        ss>>CityName>>Xcoordinate>>Ycoordinate;
        if(ss){
            Quadtree.insert(CityName,Xcoordinate,Ycoordinate); // inserts to the tree
        }
    }
    Quadtree.printTree(); //prints the tree
    cout<<endl;
    CitiesInput.close(); // closes cities.txt
    ifstream QueriesInput("queries0.txt"); //opens queries0.txt
    if(!QueriesInput){ // if it is a problem with the file
        cout<<"Can not open Queries0.txt";
    }
    while(getline(QueriesInput,line)){//starts to read the line
        if(line.empty()){// this is for empty rows
            continue;
        }
        size_t CommaFinder1 = line.find(","); // finds the first ,
        size_t CommaFinder2 = line.find(",", CommaFinder1 + 1);// finds the second ,
        if (CommaFinder1 != string::npos && CommaFinder2 != string::npos) { // if both commas are found
            string XqueryStr = line.substr(0, CommaFinder1); //implents Xquery
            string YqueryStr = line.substr(CommaFinder1 + 1, CommaFinder2 - CommaFinder1 - 1);//implents Yquery
            string RadiusStr = line.substr(CommaFinder2 + 1);//implents radius
            Xquery = stoi(XqueryStr); //converts string
            Yquery = stoi(YqueryStr); //converts string
            Radius = stoi(RadiusStr); //converts string
            vector<string> foundCities; // creates vector
            vector<string> visitedCities;// creates vector
            Quadtree.search(Xquery, Yquery, Radius, foundCities, visitedCities); //searches
            if(!foundCities.empty()){ // if there is a city in foundcities
                for(size_t i=0;i<foundCities.size(); i++){
                    cout<<foundCities[i]; //print city
                    if(i<foundCities.size()-1){
                        cout<<", ";
                    }
                }
                cout<<endl;
            }
            else{
                cout<<"<None>"<<endl; // if the vector is empty
            }
            if(!visitedCities.empty()){ // if it visited a city
                for(size_t i=0;i<visitedCities.size();i++){
                    cout<<visitedCities[i]; //cout the city
                    if(i<visitedCities.size()-1)cout<< ", ";
                }
                cout<<endl;
            }
            cout<<endl;
        }
    }
    QueriesInput.close(); //closes queries0.txt
    return 0;
}

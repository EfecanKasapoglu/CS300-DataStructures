//
// Created by Efecan Kasapoğlu on 22.10.2024.
//
#include <iostream>
#include <vector>
#include <fstream>
#include "stack.h"
#include <sstream>

using namespace std;

bool ChecksQueensSafety(int row, int col, const vector<int> &QueenPosition){
    int rowdifference,columndifference;
    for(int x=0; x<row;x++){// if there is another queen in the same column then it returns false
        if(QueenPosition[x]==col){
            return false;
        }
        if(row>x){ // if-else statement is for finding the row difference
            rowdifference=row-x;
        }
        else{
            rowdifference=x-row;
        }
        if (col>QueenPosition[x]){ // if-else statement is for finding the column difference
            columndifference=col-QueenPosition[x];
        }
        else{
            columndifference=QueenPosition[x]-col;
        }
        if(columndifference==rowdifference){//if there is a queen diagonally than it returns false
            return false;
        }
    }
    return true; // if there is no queens in the same column or diagonally than it goes false
}

void updatingSafeMatrix(int row, int col, int Nvalue, vector<vector<int>> &safeMatrix, int currentpoint){//this function is for updating the safematrix when queen is removed or placed
    for(int x=0; x<Nvalue;x++){ // updates for the same column
        safeMatrix[x][col]+=currentpoint;
    }
    int x=row;
    int y=col;
    while(x>=0 && y>=0){ // updates left up diagonal
        safeMatrix[x][y]+=currentpoint;
        x--;
        y--;
    }
    x=row;
    y=col;
    while(x<Nvalue && y<Nvalue){// updates right down diagonal
        safeMatrix[x][y]+=currentpoint;
        x++;
        y++;
    }
    x=row;
    y=col;
    while(x>=0 && y<Nvalue){// updates right up diagonal
        safeMatrix[x][y]+=currentpoint;
        x--;
        y++;
    }
    x=row;
    y=col;
    while(x<Nvalue && y>=0){// updates left down diagonal
        safeMatrix[x][y]+=currentpoint;
        x++;
        y--;
    }
}

int main() {
    int Nvalue;
    int solutions=0;

    cout<<"Enter the value of N for the N-Queens problem: "; //takes the N value from the user
    cin>>Nvalue;

    if(Nvalue<=0){
        cout<<"Invalid input. Please enter a positive integer."<<endl; //returns 0 if N value is invalid
        return 0;
    }

    string fileName = to_string(Nvalue) + "queens_solutions.txt"; //address of the txt file
    ofstream outputFile(fileName); //opens file

    vector<int> QueenPosition(Nvalue); //creates a vector for queen position
    for(int x=0; x<Nvalue; x++){ // initializing the vector with -1
        QueenPosition[x]=-1;

    }

    vector<vector<int>> safeMatrix; //creates safe matrix
    vector<int> rowvector; //creating row vector
    for(int x=0; x<Nvalue;x++){ // initializing the safe matrix with 0
        for(int y=0; y<Nvalue;y++){
            rowvector.push_back(0);
        }
        safeMatrix.push_back(rowvector);
    }

    stack<int> rowstack; // creates a row vectore to manage rows
    stack<int> columnstack;// creates a column vectore to manage rows
    int *solutionArray=new int[Nvalue*100]; // creates array to store all solutions

    rowstack.push(0); // pushing 0 to start from the first row
    columnstack.push(0);// pushing 0 to start from the first column

    while(!rowstack.isEmpty()){
        int CurrentRow=rowstack.top(); //gets the current row
        int column=columnstack.top(); // gets the current column
        rowstack.pop(); // deletes the current row from the stack
        columnstack.pop(); // deletes the current column from the stack

        if(QueenPosition[CurrentRow]!=-1){ // checks if the queen is already placed
            updatingSafeMatrix(CurrentRow,QueenPosition[CurrentRow],Nvalue,safeMatrix,-1); // updates the safe matrix
            QueenPosition[CurrentRow]=-1; // resets the queen's position
        }

        for(;column<Nvalue;column++){ //checks the columns to find safe position
            if(ChecksQueensSafety(CurrentRow,column,QueenPosition)&&safeMatrix[CurrentRow][column]==0){ // checks if the position is safe
                QueenPosition[CurrentRow]=column; // places the queen
                updatingSafeMatrix(CurrentRow,column,Nvalue,safeMatrix,1); // updates the safe matrix
                if(CurrentRow+1==Nvalue){ // checks if the solution is found
                    for(int i=0;i<Nvalue;i++){ // stores the solution to the array
                        solutionArray[solutions*Nvalue+i]=QueenPosition[i];
                    }
                    solutions++; // increments the number of solution
                    updatingSafeMatrix(CurrentRow,column,Nvalue,safeMatrix,-1); // updates the safe matrix
                    QueenPosition[CurrentRow]=-1; //removes the queen
                }
                else{
                    rowstack.push(CurrentRow); //pushes the current row
                    columnstack.push(column+1); //moves to the next column
                    rowstack.push(CurrentRow+1);// moves to the next row
                    columnstack.push(0);//resets the column
                }
                break;
            }
        }
    }
    //this part is for writing the solutions to the txt
    outputFile<<"Solutions have been saved to ’"<<Nvalue<<"queens_solutions.txt’"<<endl<<endl;
    outputFile<<"Content of "<<Nvalue<<"queens_solutions.txt:"<<endl<<endl;
    outputFile<<"Total solutions for N="<<Nvalue<<": "<<solutions<<endl<<endl;
    for(int i=0;i<solutions;i++){
        outputFile<<"Solution "<<i+1<<": [";
        for(int j=0;j<Nvalue;j++){
            outputFile<<solutionArray[i*Nvalue+j];
            if(j+1<Nvalue){
                outputFile<< ", ";
            }
        }
        outputFile<<"]"<<endl;
    }
    outputFile.close(); //closes the file
    delete[] solutionArray; //deletes the array
    return 0;
}

#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <vector>
using namespace std;

bool CheckingGraph(int Vertices, vector<vector<int>> Adjacency){ //checks if the graph is bipartite or not
    queue<int> queue; //creates queue
    vector<int> colorVector(Vertices); //to store colors
    for(int x=0;x<Vertices;x++){//implements vector as uncolored
        colorVector[x]=-1;
    }
    for(int x=1;x<Vertices;x++){//checks vertex to see if it is bipartite
        if(colorVector[x]==-1){//unvisited vertex
            queue.push(x);
            colorVector[x]=0;//assigns color
            while(!queue.empty()){
                int topelement=queue.front();
                queue.pop();
                for(int y=0;y<Adjacency[topelement].size();y++){//checks adjacent vertices
                    int neighbor=Adjacency[topelement][y];
                    if(neighbor >= Vertices){//skips invalids
                        continue;
                    }
                    if(colorVector[neighbor]==-1){//neighbor is uncolored then assigns opposite color
                        colorVector[neighbor]=1-colorVector[topelement];
                        queue.push(neighbor);
                    }
                    else if(colorVector[neighbor]==colorVector[topelement]){//neighbor has same color
                        return false;//graph is not bipartite
                    }
                }
            }
        }
    }
    return true;//graph is bipartite
}

bool BipartiteMatching(int currentNode, vector<vector<int>>& Adjacency, vector<bool>& visitedVector, vector<int>& NumberOfMatch){//bipartite matching
    for(int x=0;x<Adjacency[currentNode].size();x++){//iterates adjacent nodes
        int y=Adjacency[currentNode][x];
        if(y>=Adjacency.size()||visitedVector[y]){//skips visited or invalid nodes
            continue;
        }
        visitedVector[y]=true;//makes visited
        if ((NumberOfMatch[y]==-1||BipartiteMatching(NumberOfMatch[y],Adjacency,visitedVector,NumberOfMatch))){//assigns new match or finds augmenting path
            NumberOfMatch[y]=currentNode;//assigned match
            return true;//matching found
        }
    }
    return false;//no matching found
}

int MaximumMatching(int Vertices, vector<vector<int>> Adjacency){//finds max matching
    vector<int> NumberOfMatch(Vertices);//this is for tracking matches
    int MaximumMatch=0;//count for maximum match
    for(int x=0;x<NumberOfMatch.size();x++){//implements matching as -1
        NumberOfMatch[x]=-1;
    }
    for(int x=1;x<Vertices/2;x++){//try to find matches
        vector<bool> visited(Vertices);//visited vector
        for(int y=0;y<Vertices;y++){//implements as unvisited
            visited[y]=false;
        }
        if(BipartiteMatching(x,Adjacency,visited,NumberOfMatch)){//check if a match is found
            MaximumMatch++;//increments the max match counter
        }
    }
    return MaximumMatch;//return to the max match size
}

int main(){
    string input;//input for file name
    int Vertices,Edges,Vertex1,Vertex2;
    while(true){
        cout<<"Enter the graph name (e.g., 'graph1' or 'exit' to terminate): ";
        cin>>input;//takes from the user
        if(input=="exit"){//program exits if input is exit
            break;
        }
        string fileName=input+".txt";
        ifstream file(fileName);//opens input file
        file>>Vertices>>Edges; //reads vertices and edges from the file
        vector<vector<int>> Adjacency(Vertices);//initialize adjacency
        while(file>>Vertex1>>Vertex2){//reads edges
            if(Vertex1==0||Vertex2==0){//ignores invalids
                continue;
            }
            Vertex1--;
            Vertex2--;
            if(Vertex1>=0&&Vertex2>=0&&Vertex1<Vertices&&Vertex2<Vertices&&Vertex1!=Vertex2){//adds if it is valid
                Adjacency[Vertex1].push_back(Vertex2);
                Adjacency[Vertex2].push_back(Vertex1);
            }
        }
        if(CheckingGraph(Vertices, Adjacency)){//checks the graph is bipartite
            cout<<"The graph is bipartite."<<endl;
            cout<<"Maximum matching size: "<<MaximumMatching(Vertices,Adjacency)<<endl;
        }
        else{
            cout<<"The graph is not bipartite."<<endl;
        }
    }
    return 0;
}

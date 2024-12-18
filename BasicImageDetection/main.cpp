#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

template <class HashedObj>
class HashTable{ //implementing HashTable class
public:
    explicit HashTable(const HashedObj& notFound,int size=101) //start with the size
            : ITEM_NOT_FOUND(notFound),array(nextPrime(size)){
        makeEmpty();//implements all the elements as empty
    }
    void makeEmpty(){//cleaning the table
        for(int x=0;x<array.size();x++){
            array[x].info=EMPTY;//implements all elements as empty
        }
        currentSize=0;
    }
    enum EntryType {ACTIVE,EMPTY,DELETED};//shows the status

    const HashedObj& find(const HashedObj& x) const{ //finds the element
        int currentPos=findPos(x);
        if (isActive(currentPos)){
            return array[currentPos].element;//returns to the element
        }
        return ITEM_NOT_FOUND; // returns if it is not found
    }
    void insert(const HashedObj& x){//method for adding an element
        int currentPos=findPos(x);
        if (isActive(currentPos)){
            return;//returns if element is already added
        }
        array[currentPos]=HashEntry(x,ACTIVE);//adds new element an updates as active
        if (++currentSize>array.size()/2){//if number of elements is larger than the half of the array size
            rehash();//it rehashes the table
        }
    }

private:
    struct HashEntry{//creating the structure
        HashedObj element;
        EntryType info;

        HashEntry(const HashedObj& e=HashedObj(),EntryType i=EMPTY)
                :element(e),info(i){}//constructor
    };
    vector<HashEntry> array;
    int currentSize;//number of added elements
    const HashedObj ITEM_NOT_FOUND;

    int findPos(const HashedObj& x) const{//finding the element
        int i=0;
        int p=hash<HashedObj>()(x) % array.size();//find the position with the function
        while(array[p].info!=EMPTY && array[p].element!=x){
            p+=1;//if the position is already full then it increases the number
            if(p>=array.size()){
                p-=array.size();//if position is larger than the array size
            }
        }
        return p;
    }
    int nextPrime(int n)const{//find the next prime number
        if(n%2==0){//checks if it is divisible with 2
            n++;
        }
        while(!isPrime(n)){
            n+=2;
        }
        return n;
    }

    bool isPrime(int n) const{// checks if the number is prime
        if(n==2||n==3){
            return true;
        }
        if(n<=1||n%2==0){
            return false;
        }
        for(int x=3;x*x<=n;x+=2){
            if(n%x==0){
                return false;
            }
        }
        return true;
    }
    bool isActive(int currentPos)const{//checks if the position is active
        return array[currentPos].info==ACTIVE;
    }
    void rehash(){//it rehashes the table and reorders
        vector<HashEntry> oldArray=array; //saves the old table
        array.resize(nextPrime(2*oldArray.size()));//new size
        for(int x=0;x<array.size();x++){
            array[x].info=EMPTY;//updates as empty to new table
        }
        currentSize=0;
        for(int x=0;x<oldArray.size();x++){
            if (oldArray[x].info==ACTIVE){
                insert(oldArray[x].element);//adds the old number
            }
        }
    }
};

string RunLengthEncoding(const string& flattenString){//RLE algorithm
    string RLEstring="";//empty string
    int counter=1;//sets as counter as 1
    char Current=flattenString[0];
    for(int x=1; x<flattenString.size();x++){
        if(Current==flattenString[x]){//if it is the same character then it increments the counter
            counter++;
        }
        else{
            if(Current=='0'){//for 0 it adds B
                RLEstring+=to_string(counter)+"B";
            }
            else{//for 1 it adds W
                RLEstring+=to_string(counter)+"W";
            }
            Current=flattenString[x];
            counter=1;//resets the counter
        }
    }
    if(Current=='0'){
        RLEstring+=to_string(counter)+"B";
    }
    else{
        RLEstring+=to_string(counter)+"W";
    }
    return RLEstring;
}
void printingFlattenString(const string& flattenString){//printing the string
    int maksrowlength=28;//maks length of the row
    int x=0;
    while(x<flattenString.size()){
        cout<<flattenString.substr(x,maksrowlength)<<endl;
        x+=maksrowlength;//goes to the next row
    }
}

int main(){//my main program starts here
    string input1,input2;
    string ITEM_NOT_FOUND="NOT_FOUND";
    HashTable<string> hashTable(ITEM_NOT_FOUND);//creates hash table
    while(true){//adding elements in the table
        string line;
        string flattenString="";
        cout<<"Enter image number to insert into the hash table (or 'query' to continue): \n";//takes the image folder name
        cin>>input1;
        if(input1=="query"){//if user types query
            break;
        }
        string fileName="image"+input1+".txt";//creates the txt file
        ifstream file(fileName);//opens
        if (!file){//if there is an error
            cout<<"Error: File "<<fileName<<" not found or could not be opened."<<endl;
            continue;
        }
        while(getline(file,line)){//reads the line
            if(!line.empty()&&line[line.length()-1]=='\r'){//removes '\r'
                line=line.substr(0,line.length()-1);
            }
            flattenString+=line; //adds to the flatten string
        }
        file.close();//closes the file
        string RLE=RunLengthEncoding(flattenString);//creates the RLE string
        hashTable.insert(RLE);//inserts to the table
        cout<<"Image "<<input1<<" inserted into the hash table."<<endl;
    }
    while(true){
        string line;
        string flattenString="";
        cout<<"Enter image number to query (or 'exit' to quit): \n";//takes the query txt number from the user
        cin>>input2;
        if(input2=="exit"){//if user types exit
            break;
        }
        string fileName="query"+input2+".txt";//creates the query
        ifstream file(fileName);//opens the query
        if (!file){//if there is an error
            cout<<"Error: File "<<fileName<<" not found or could not be opened."<<endl;
            continue;
        }
        while(getline(file,line)){//reads the lines
            if(!line.empty()&&line[line.length()-1]=='\r'){//removes '\r'
                line=line.substr(0,line.length()-1);
            }
            flattenString+=line;//adds to the flatten string
        }
        file.close();//closes the file
        string RLE=RunLengthEncoding(flattenString);//creates teh RLE string
        string RLEfinder=hashTable.find(RLE);//searches in the table
        if(RLEfinder==ITEM_NOT_FOUND){//if it is empty
            cout<<"No match for the image with encoding: ";
            cout<<RLE<<endl;
        }
        else{
            cout<<"RLE String for query"<<input2<<".txt found in hash table."<<endl;
            printingFlattenString(flattenString);//prints the string
        }
    }
    cout<<"Exiting the program!";
    return 0;
}

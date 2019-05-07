#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

struct hashNode
{
    int index;
    hashNode *next;
};

int getdir (string dir, vector<string> &files);

void readFile (string dir, vector<string> &text, vector<string> files, int index);

void chunkFile (vector<string> &text, vector<string> files, int index, int chunkSize, vector<hashNode*> &table, int hashRows);

void hashChunk(vector<string> chunk, vector<hashNode*> &table, int index, int hashRows);

int hashFunction(vector<string> chunk, int hashRows);

int main(int argc, char *argv[])
{
//Initialization of command-line arguments
    int chunk = atoi(argv[2]);
    int threshold = atoi(argv[3]);
    string dir = string(argv[1]);
    vector<string> files;

//Retrieves directory of the folder
    getdir(dir,files);

//Deletes current and parent directory
    files.erase(files.begin(),files.begin() + 2);
    int MAX_ROWS = 1000003;

//Initialization of hash table
    vector<hashNode*> hashTable;
    hashTable.resize(MAX_ROWS);

    for(int i = 0;i < MAX_ROWS;i++){
        hashTable[i] = NULL;
    }

//Initialization of collision table
    vector<int> v(files.size());
    vector<vector<int> > collision(files.size(),v);

    for(int i = 0;i<files.size();i++){
        for(int j = 0;j<files.size();j++){
            collision[i][j] = 0;
        }
    }

//Creates a vector for each text file that contains its words.
//Then chunks up the vector and puts it into the hash table
    for(int i = 0;i < files.size();i++){
        vector<string> text;
        readFile(dir, text, files, i);
        chunkFile(text, files, i, chunk, hashTable, MAX_ROWS);
    }

//Traverses the vector of linked lists and increments the collision table
    for(int i = 0;i < MAX_ROWS;i++){
        if((hashTable[i] != NULL)&&(hashTable[i]->next != NULL)){

            int index = i;
            hashNode* root = hashTable[i];

            while(root->next != NULL){
                hashNode* traversal = root->next;

                while(traversal != NULL){

//Ensures that the correct coordinates are used so that the table is only filled in above the middle diagonal
                    if(root->index > traversal->index){
                        collision[traversal->index][root->index]++;
                    }

                    else{
                        collision[root->index][traversal->index]++;
                    }
                    traversal = traversal->next;
                }
                root = root->next;
            }
        }
    }
 
//Prints out the collisions and file names that are above the threshold       
    for(int i = 0;i < files.size();i++){
        for(int j = i+1;j < files.size();j++){
            if(collision[i][j]>threshold){
                cout << collision[i][j] << ": " << files[i] << ", " << files[j] << endl;
            }
        }
    }
}

int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp = opendir(dir.c_str())) == NULL){
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return(0);
}

void readFile (string dir, vector<string> &text, vector<string> files, int index)
{
//Creates the path for the file needed
    string filename = "./" + dir + "/" + files[index];

    ifstream inFile;
    inFile.open(filename.c_str());
    
    string temp;

//The text vector composes of the entire contents of a text document
    while(inFile >> temp) {

        text.push_back(temp);

//Erases any special characters
        for(int i = 0; i < text.back().size(); i++){
            if(!isalnum(text.back()[i])){
                text.back().erase(i,1);
                i--;
            }
        }  
    }
}

void chunkFile (vector<string> &text, vector<string> files, int index, int chunkSize, vector<hashNode*> &table, int hashRows)
{
    vector<string> chunk;

    while(text.end() != (text.begin() + chunkSize - 1)){

//Creates the chunks and ensures each character is upper case
        for(int i = 0; i < chunkSize; i++){ 
            chunk.push_back(text[i]);

            for(int j = 0; j < chunk.back().size(); j++){
                chunk.back()[j] = toupper(chunk.back()[j]);
            }
        }
//Runs each chunk through the hashChunk() function 
        hashChunk(chunk, table, index, hashRows);
        text.erase(text.begin());
        chunk.clear();
    }
}


void hashChunk(vector<string> chunk, vector<hashNode*> &table, int index, int hashRows)
{

//Key is the returned value from the hashFunction()
    int key = hashFunction(chunk, hashRows);

//Case (1): linked list is empty
    if(table[key] == NULL){
        table[key] = new hashNode;
        table[key]->index = index;
        table[key]->next = NULL;

    }

//Case (2): linked list is not empty, new node is placed at head
    else{
        hashNode* temp = table[key];
        table[key] = new hashNode;
        table[key]->index = index;
        table[key]->next = temp;
    }       
}

//hashFunction = sum(character * (placement in chunk)^2)
int hashFunction(vector<string> chunk, int hashRows)
{
    int result;
    int increment = 1;

    int test = 0;
    for(int i = 0;i < chunk.size();i++){
        for(int j = 0;j < chunk[i].size();j++){
            result+= chunk[i][j] * increment * increment;
            increment++;
        }
    }

    return result%hashRows;
}

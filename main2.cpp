/*
 * File:   main.cpp
 * Author: kianoosh
 *
 * Created on 1 September 2010, 10:55 AM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <sstream>


using namespace std;

/*
 *
 */
static int MAXLINENUMBER = 200;

struct jobs {
    string name;
    int startTime;
};

class FileTransfer {
public:

    void ReadFile(string filename, string content[]) {

        ifstream inFile;
        int lineNumber = 0;
        string temp;
        cout << "content of the file is:\n------------\n";


        inFile.open(filename.c_str());
        if (!inFile) {
            cout << "Unable to open file";
        } else {
            while (!inFile.eof()) {
                getline(inFile, temp);
                cout << lineNumber << " : " << temp << "\n";
                content[lineNumber] = temp;
                lineNumber++;
            }

            inFile.close();
            cout << "File has been read, will be analysed!\n";
        }
    }
public:

    void WriteFile(string filename, string *content, int length) {
        ofstream output(filename.c_str());
        if (output.is_open()) {
            for (int i = 0; i < length; i++) {
                output << *(content + i) << "\n";
            }
            output.close();
        } else
            cout << "Unable to open output file";
    }
};

class Analysis {
public:
    int numberOfLines;
public:
 Analysis() {
   
    }
public:

    int StringToDigit(string number) {
        numberOfLines= atoi(number.c_str());
        return numberOfLines;
    }


 
};

int main(int argc, char** argv) {
    string temp;
    char *carray;
    int cnt = 0;
    queue<jobs> q;    
    FileTransfer filetransfer;
    Analysis analysis;
    string files[MAXLINENUMBER];
    string result[MAXLINENUMBER];
    string output[MAXLINENUMBER];
    if(argv[2]="") argv[2]="in.file";
    filetransfer.ReadFile(argv[2], files);
    while (files[cnt] != "") {
        temp = files[cnt];        
        filetransfer.ReadFile(temp, result);
        jobs job;
        job.name = files[cnt];        
        job.startTime = analysis.StringToDigit(result[0]);
        q.push(job);
        cnt++;
    }
    stringstream out;
    cnt = 0;
    while (!q.empty()) {
        out.clear();
        out.str("");
        jobs job;
        job = q.front();
        out << job.startTime;
        output[cnt] = job.name + "," + out.str();
        cout << output << "\n";
        q.pop();
        cnt++;


    }
    filetransfer.WriteFile("write", output, cnt);
    return 0;
}
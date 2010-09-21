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
static int MAXNUMBEROFVARIABLES = 100;

struct LinePosition{
    int frameNumber;
    int pageNumber;
    int lineNumber;
    void initialize(){
        frameNumber=-1;
        pageNumber=-1;
        lineNumber=-1;
    }
};
struct Page {
    string lines[1];
};

struct Frame {
    Page pages[2];
};

struct MainMemory {
    Frame frames[8];
public:

    void setMemory(string content[], int position, int numberOfLines) {
        int lineCnt = 0;
        int pageCnt = 0;
        int p = position;
        int frameCnt = 0;

        for (int j = 0; j < numberOfLines; j++) {
            frames[frameCnt].pages[pageCnt].lines[lineCnt] = content[p];
            lineCnt++;
            if (lineCnt == 2) {
                lineCnt = 0;
                pageCnt++;
                if (pageCnt == 1) {
                    pageCnt = 0;
                    frameCnt++;
                    if (frameCnt > 8) {
                        cout << "could not fit all of them in memory!";
                        break;
                    }

                }
            }
            p--;
        }

    }
    LinePosition findLine(string line){
        LinePosition linePosition;
        linePosition.initialize();
        for(int i=0;i<8;i++){
            for(int j=0;j<2;j++){
                for(int k=0;k<2;k++){
                    if(line==frames[i].pages[j].lines[k]){
                        linePosition.frameNumber=i;
                        linePosition.pageNumber=j;
                        linePosition.lineNumber=k;
                    }
                }
            }
        }
        return linePosition;
    }
};

struct CacheMemory {
    Page pages[2];

    void setMemory(string content[], int position, int numberOfLines) {
        int lineCnt = 0;
        int pageCnt = 0;
        int p = position;

        for (int j = 0; j < numberOfLines; j++) {
            pages[pageCnt].lines[lineCnt] = content[p];
            lineCnt++;
            if (lineCnt == 2) {
                lineCnt = 0;
                pageCnt++;
                if (pageCnt > 2) {
                    cout << "could not fit all of them in chache memory!";
                    break;

                }
            }
            p--;
        }

    }
public:

    LinePosition indexOf(string line) {
        LinePosition linePosition;
        linePosition.initialize();
        for (int i = 0; i < 4; i++){
            for(int j =0;j<1;j++)
                if (line == pages[i].lines[j]){
                    linePosition.lineNumber=j;
                    linePosition.pageNumber=i;
                    return linePosition;
                }
        }
        return linePosition;
    }
};

struct Jobs {
    string name;
    int startTime;
    int duration;
    int numberOfLines;
    int memoryNeed;
    string output;
};

class General {
public:

    void Sort(Jobs alljobs[], int length) {
        int flag = 1;
        for (int i = 0; i < length && flag; i++) {
            for (int j = 0; j < length; j++) {
                flag = 0;
                if (alljobs[j + 1].startTime > alljobs[j].startTime) {
                    Jobs temp;
                    temp = alljobs[j];
                    alljobs[j] = alljobs[j + 1];
                    alljobs[j + 1] = temp;
                    flag = 1;
                }

            }
        }
    }
};

class FileTransfer {
public:

    int ReadFile(string filename, string content[]) {

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
        return lineNumber;
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

class Analysis : public General {
public:

    Analysis() {

    }
public:

    int StringToDigit(string number) {
        int temp;
        temp = atoi(number.c_str());
        return temp;
    }
public:

    bool isCharacter(char c) {
        if ((c > 'a' && c < 'z') || (c > 'A' && c < 'Z'))
            return true;
        else
            return false;
    }
public:

    string findVariableName(string fullString) {
        int cnt = 2;
        string result = "";
        while (!isCharacter(fullString[cnt])) cnt++;
        while (isCharacter(fullString[cnt])) {
            result += fullString[cnt];
            cnt++;
        }
        return result;
    }
public:

    int findVariableValue(string fullString) {
        int cnt = 0;
        string result = "";
        for (cnt = 2; cnt < fullString.length(); cnt++) {
            if (fullString[cnt] == '<') {
                cnt++;
                break;
            }
        }
        while (fullString[cnt] == ' ') cnt++;
        while (isdigit(fullString[cnt])) {
            result += fullString[cnt];
            cnt++;
        }
        return StringToDigit(result);
    }
public:

    int findLine(string fullString) {
        int cnt = 0;
        string result = "";
        for (cnt = 2; cnt < fullString.length(); cnt++) {
            if (fullString[cnt] == 'g' && fullString[cnt + 1] == 'o' && fullString[cnt + 2] == 't'
                    && fullString[cnt + 3] == 'o') {
                cnt += 4;
                break;
            }
        }
        while (fullString[cnt] == ' ') cnt++;
        while (isdigit(fullString[cnt])) {
            result += fullString[cnt];
            cnt++;
        }
        return StringToDigit(result);
    }
public:

    int variableIndex(string variables[], string variable) {
        int index = -1;
        for (int i = 0; i < MAXNUMBEROFVARIABLES; i++) {
            if (variables[i] == "") break;
            if (strcasecmp(variables[i].c_str(), variable.c_str()) == 0)
                return i;
        }
        return index;
    }
public:

    void initialize(int *variable) {
        for (int i = 0; i < MAXNUMBEROFVARIABLES; i++)
            variable[i] = 0;
    }
public:

    int CalculateTime(string fileName) {
        string variableName[MAXNUMBEROFVARIABLES];
        string name;
        MainMemory mainMemory;
        int variableValue[MAXNUMBEROFVARIABLES];
        int cnt = 0;
        int time = 0;
        int index = 0;
        int j = 0;
        int value;
        string content[MAXLINENUMBER];
        initialize(variableValue);
        FileTransfer file;
        file.ReadFile(fileName, content);
        MainMemory memory;
        memory.setMemory(content, 4, 4);
        while (content[cnt] != "") {

            if (content[cnt][0] == 'i' && content[cnt][1] == 'f') {
                name = findVariableName(content[cnt]);
                value = findVariableValue(content[cnt]);
                index = variableIndex(variableName, name);
                if (index == -1) {
                    variableName[j] = name;
                    variableValue[j]++;
                    index = j;
                    j++;
                } else {
                    variableValue[index]++;
                }
                if (variableValue[index] < value)
                    cnt = findLine(content[cnt]);
                else {
                    variableValue[index] = -1;
                }
            }

            time++;
            cnt++;
        }
        return time;
    }
public:

    int calculateMemoryNeed(int numberOfLines) {
        int temp = numberOfLines % 2;
        if (temp == 0)
            return numberOfLines / 2;
        else
            return (numberOfLines / 2) + 1;
    }
public:

    void FCFS(Jobs *alljobs, string *output, int numberOfJobs) {
        queue<Jobs> q;
        General general;
        int cnt;
        general.Sort(alljobs, numberOfJobs);
        for (int i = 0; i < numberOfJobs; i++) {
            q.push(alljobs[i]);
        }
        stringstream out;
        cnt = 0;
        while (!q.empty()) {
            out.clear();
            out.str("");
            Jobs job;
            job = q.front();
            out << job.duration;
            output[cnt] = job.name + "," + out.str();
            cout << output << "\n";
            q.pop();
            cnt++;
        }
    }

public:

    void RR(Jobs *alljobs, string *output, int numberOfJobs, int quantom) {
        queue<Jobs> q;
        int totalTime;
        General general;
        int cnt = 0;
        general.Sort(alljobs, numberOfJobs);
        for (int i = numberOfJobs - 1; i >= 0; i--) {
            q.push(alljobs[i]);
        }
        totalTime = alljobs[numberOfJobs - 1].startTime;
        stringstream out;
        cnt = 0;
        while (!q.empty()) {
            out.clear();
            out.str("");
            Jobs job;
            job = q.front();
            if (job.startTime <= totalTime) {
                if (job.duration > 0) {
                    totalTime += quantom;
                    job.duration -= 2;
                    out << totalTime;
                    job.output = job.output + " " + out.str();
                }
            }
            q.pop();
            if (job.duration > 0)
                q.push(job);
            else {
                out.clear();
                out.str("");
                out << job.startTime;
                output[cnt] = job.name + " " + out.str() + " " + job.output;
                cnt++;
            }

        }
    }

};

int main(int argc, char** argv) {
    string temp;  
    int numberOfJobs = 0;
    int cnt = 0;
    int quantum = 0;
    argv[1] = "RR";
    argv[2] = "2";
    argv[3] = "in.file";

    Jobs alljobs[400];
    FileTransfer filetransfer;
    Analysis analysis;
    string files[MAXLINENUMBER];
    string result[MAXLINENUMBER];
    string output[MAXLINENUMBER];
    if (argv[2] == "") {
        argv[2] = "in.file";
        argv[1] = "FCFS";
    }

    if (argv[1] == "FCFS") {
        filetransfer.ReadFile(argv[2], files);
    } else if (argv[1] == "RR") {
        quantum = analysis.StringToDigit(argv[2]);
        filetransfer.ReadFile(argv[3], files);
    }
    while (files[cnt] != "") {
        temp = files[cnt];
        Jobs job;
        job.numberOfLines = filetransfer.ReadFile(temp, result);
        job.name = files[cnt];
        job.startTime = analysis.StringToDigit(result[0]);
        job.duration = analysis.CalculateTime(files[cnt]);
        job.memoryNeed = analysis.calculateMemoryNeed(job.numberOfLines);
        alljobs[cnt] = job;
        cnt++;
        numberOfJobs++;
    }
    if (argv[1] == "FCFS")
        analysis.FCFS(alljobs, output, numberOfJobs);
    else if (argv[1] == "RR")
        analysis.RR(alljobs, output, numberOfJobs, quantum);
    filetransfer.WriteFile("write", output, cnt);
    return 0;
}
//============================================================================
// Name        : os.cpp
// Author      : Kianoush Akhavan Karbasi, Ashley Ramsey
// Version     :
// Copyright   : You can copy this as many as you want
// Description : Hello World in C++, Ansi-style
//============================================================================

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
static const int MAXLINENUMBER = 200;
static const int MAXNUMBEROFVARIABLES = 100;
static const int MAXJOBSNUMBER = 400;

struct LinePosition {
	int frameNumber;
	int pageNumber;
	int lineNumber;

	void initialize() {
		frameNumber = -1;
		pageNumber = -1;
		lineNumber = -1;
	}
};

struct Page {
	string lines[2];
};

struct Frame {
	Page pages[2];
	int usedTime;
	string jobName;
};

struct MemoryJobs {
	string name;
	int startTime;
	int numberOfLines;
	string content[MAXLINENUMBER];
	string output;
	string variableName[MAXNUMBEROFVARIABLES];
	int variableValue[MAXNUMBEROFVARIABLES];
	int currentLine;
	int numberOfVariables;
	int lastUsedTime;
	bool pagefault;
public:

	void initializer() {
		startTime = 0;
		name = "";
		output = "";
		currentLine = 1;
		numberOfVariables = 0;
		numberOfLines = 0;
		lastUsedTime = 0;

	}
};

struct MainMemory {
	Frame frames[8];

public:
	void initialize() {
		for (int i = 0; i < 8; i++) {
			frames[i].usedTime = -1;
		}
	}
public:

	int findEmptyFrame() {
		int index = -1;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 1; j++)
				for (int k = 0; k < 2; k++)
					if (frames[i].pages[j].lines[k] == "" && k == 0) {
						index = i;
						return i;
					}

		}
		return index;
	}
public:

	int FindFrameNumber(string jobName) {
		int index = -1;
		for (int i = 0; i < 8; i++) {
			if (jobName == frames[i].jobName) {
				index = i;
				break;
			}
		}
		return index;
	}

public:

	int findLeastUsedFrame() {
		int index = 0;
		int leastTime = frames[0].usedTime;
		for (int i = 0; i < 8; i++) {
			if (frames[i].usedTime < leastTime) {
				index = i;
				leastTime = frames[i].usedTime;
			}
		}
		return index;
	}

public:

	void setMemory(string jobName, string content[], int position,
			int numberOfLines,string storedContent[], int time) {
		int lineCnt = 0;
		int pageCnt = 0;
		int p = position;
		bool memoryFull = false;
		int frameNumber = -1;

		int sCCnt = 0;
		frameNumber = FindFrameNumber(jobName);
		if (frameNumber == -1) {
			frameNumber = findEmptyFrame();
		}
		if (frameNumber == -1) {
			// MEMORY FULL
			frameNumber = findLeastUsedFrame();
		}
		if (numberOfLines == 0){
			frames[frameNumber].pages[pageCnt].lines[lineCnt] = content[p];
			frames[frameNumber].usedTime = time;
			storedContent[0] = content[p];
	}
		else {
			for (int j = 0; j < numberOfLines; j++) {
				frames[frameNumber].pages[pageCnt].lines[lineCnt] = content[p];
				frames[frameNumber].usedTime = time;
				storedContent[sCCnt++] = content[p];
				lineCnt++;
				if (lineCnt == 2) {
					lineCnt = 0;
					pageCnt++;
					if (pageCnt == 1) {
						pageCnt = 0;
						if (!memoryFull)
							frameNumber++;
						else
							frameNumber = findLeastUsedFrame();
						if (frameNumber > 7) {
							cout << "\t&\n\tMain memory full, implementing lru";
							memoryFull = true;

						}

					}
				}
				p++;
			}
		}

	}

	LinePosition searchMemory(string line) {
		LinePosition linePosition;
		linePosition.initialize();
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 1; j++) {
				for (int k = 0; k < 2; k++) {
					if (line == frames[i].pages[j].lines[k]) {
						linePosition.frameNumber = i;
						linePosition.pageNumber = j;
						linePosition.lineNumber = k;
						return linePosition;
					}
				}
			}
		}
		return linePosition;
	}

	void getFrames(LinePosition linePos, string storedContent[], string jobName) {

		int storedContCnt = 0;
		for (int i = linePos.frameNumber; i < linePos.frameNumber + 2; i++) {
			if (i < 8) {
				for (int j = 0; j < 2; j++) {
					storedContent[storedContCnt]
							= frames[i].pages[linePos.pageNumber].lines[j];
					storedContCnt++;
				}
			} else {
				for (int j = 0; j < 2; j++) {
					storedContent[i]
							= frames[0].pages[linePos.pageNumber].lines[j];
					storedContCnt++;
				}
			}
		}
	}
};

struct CacheMemory {
	Page pages[2];
	string name[4];
	void setMemory(string storedContent[], string jobName) {
		int lineCnt = 0;
		int pageCnt = 0;
		int nameCounter = 0;

		for (int j = 0; j < 4; j++) {
			pages[pageCnt].lines[lineCnt] = storedContent[j];
			name[nameCounter] = jobName;
			lineCnt++;
			if (lineCnt == 2) {
				lineCnt = 0;
				pageCnt++;
			}
			nameCounter++;
		}
	}

public:

	LinePosition searchMemory(string line, string jobName) {
		LinePosition linePosition;
		linePosition.initialize();
		int nameCounter = 0;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {

				if (line == pages[i].lines[j] && name[nameCounter] == jobName) {
					linePosition.lineNumber = j;
					linePosition.pageNumber = i;

					return linePosition;
				}
				nameCounter++;
			}
		}
		return linePosition;

	}

public:

	LinePosition indexOf(string line) {
		LinePosition linePosition;
		linePosition.initialize();
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 1; j++)
				if (line == pages[i].lines[j]) {
					linePosition.lineNumber = j;
					linePosition.pageNumber = i;
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

	void Sort(MemoryJobs alljobs[], int length) {
		int flag = 1;
		for (int i = 0; i < length && flag; i++) {
			for (int j = 0; j < length; j++) {
				flag = 0;
				if (alljobs[j + 1].startTime > alljobs[j].startTime) {
					MemoryJobs temp;
					temp = alljobs[j];
					alljobs[j] = alljobs[j + 1];
					alljobs[j + 1] = temp;
					flag = 1;
				}

			}
		}
	}
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
public:

	int findIndex(string content[], string line) {
		int index = -1;
		int i = 0;
		while (content[i] != "") {
			if (line == content[i]) {
				index = i;
				break;
			}
		}
		return index;
	}
public:

	void getLines(string content[], int start, int numberOfLines,
			string result[]) {
		int j = 0;
		for (int i = start; i < numberOfLines; i++) {
			result[j] = content[i];
			j++;
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
			lineNumber--;
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

class Analysis: public General {
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
		while (!isCharacter(fullString[cnt]))
			cnt++;
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
		while (fullString[cnt] == ' ')
			cnt++;
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
			if (fullString[cnt] == 'g' && fullString[cnt + 1] == 'o'
					&& fullString[cnt + 2] == 't' && fullString[cnt + 3] == 'o') {
				cnt += 4;
				break;
			}
		}
		while (fullString[cnt] == ' ')
			cnt++;
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
			if (variables[i] == "")
				break;
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

private:

	void updateAllJobs(MemoryJobs allJobs[], MemoryJobs job, int numberOfJobs) {
		for (int i = 0; i < numberOfJobs; i++) {
			if (allJobs[i].name == job.name) {
				allJobs[i].lastUsedTime = job.lastUsedTime;
				break;
			}
		}
	}

public:

	void FCFS(Jobs *alljobs, string *output, int numberOfJobs, int dumpTime,
			string outfile) {
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

	void displayQueue(queue<MemoryJobs> q, int jobNum) {
		queue<MemoryJobs> myq = q;
		cout << "\n\n";
		if (jobNum == 2)
			cout << "\t\t\t";
		if (jobNum == 3)
			cout << "\t\t\t\t\t\t";
		cout << "Queue: ";
		MemoryJobs job = myq.front();
		cout << "\tjob name: " + job.name + "\n\t\t\t";
		myq.pop();
		while (!myq.empty()) {
			MemoryJobs job = myq.front();
			if (jobNum == 2)
				cout << "\t\t\t";
			if (jobNum == 3)
				cout << "\t\t\t\t\t\t";
			cout << "job name: " + job.name + "\n\t\t\t";
			myq.pop();
		}
		cout << "\n";

	}

	// if memory has been used this function should be called;
public:

	void RR(MemoryJobs *alljobs, string *output, int numberOfJobs, int quantom,
			int dumpTime, string outfile) {
		int jobNum = 1;
		queue<MemoryJobs> q;
		int currentTime = 0;
		bool noMoreJob = false;
		bool pageFault = false;
		string name;
		int index;
		int value;
		int cnt = 0;
		int qcounter = 0;
		string storedContent[16];
		LinePosition linePosition;
		bool eof = false;

		MemoryJobs job;
		job.initializer();
		stringstream out;
		Sort(alljobs, numberOfJobs);
		MainMemory memory;
		CacheMemory cache;
		for (int i = numberOfJobs - 1; i >= 0; i--) {
			q.push(alljobs[i]);
		}
		currentTime = alljobs[numberOfJobs - 1].startTime;
		memory.initialize();
		while (!noMoreJob) {
			displayQueue(q, jobNum);
			out.clear();
			out.str("");
			if (qcounter == 0 && !q.empty()) {
				job = q.front();
				q.pop();
			}
			if (eof == true) {
				cout << "\n---------------------" << job.name
						<< "---------------------------------------------END\n";
				job = q.front();
				eof=false;
				q.pop();
			}
			if (job.startTime <= currentTime) {

				// First time job being processed
				if (job.lastUsedTime == 0) {
					memory.setMemory(job.name, job.content, 0, 4,
							 storedContent,currentTime);
					cache.setMemory(storedContent, job.name);
					linePosition = cache.searchMemory(
							job.content[job.currentLine], job.name);
				} else {
					// check the cache
					linePosition = cache.searchMemory(
							job.content[job.currentLine], job.name);

					//MAIN MEMORY
					if (linePosition.pageNumber == -1) {
						//check the main memory and get the 2 frames for the cache
						linePosition = memory.searchMemory(
								job.content[job.currentLine]);
						pageFault = false;
						if (linePosition.pageNumber != -1) {
							//get the frames lines into storedcontent
							memory.getFrames(linePosition, storedContent,
									job.name);
							//put in cache
							cache.setMemory(storedContent, job.name);
							pageFault = false;
						} else {
							//pagefault, get next pages from disk
							cout
									<< "\n\n\tPagefault happened here, reading from disk\n";
							qcounter = quantom - 1;
							if (job.currentLine < job.numberOfLines - 2)
								memory.setMemory(job.name, job.content,
										job.currentLine, 2,
										 storedContent,currentTime);
							else
								// if current line is end of file/1 line before end of file, take last 2 lines of the file
								memory.setMemory(job.name, job.content,
										job.currentLine, job.numberOfLines
												- job.currentLine,
										 storedContent,currentTime);
							cache.setMemory(storedContent, job.name);
							linePosition = memory.searchMemory(
									job.content[job.currentLine]);
							job.currentLine--;
							pageFault = true;
							//q.push(job);
							//job = q.front();
							//qcounter = 0;
							//q.pop();

						}
						// cache.setMemory()
					}
				}
				job.lastUsedTime = currentTime;
				updateAllJobs(alljobs, job, numberOfJobs);
				if (!pageFault) {

					// put the current line's string into currStr
					string currStr = job.content[job.currentLine];
					//if (qcounter < quantom - 1) {
					if (strncmp(currStr.c_str(), "if", 2) == 0) {
						if (job.name == "job2.txt") {
							cnt = 0;
						}
						name = findVariableName(job.content[job.currentLine]);
						value = findVariableValue(job.content[job.currentLine]);
						index = variableIndex(job.variableName, name);
						if (index == -1) {
							job.variableName[job.numberOfVariables] = name;
							job.variableValue[job.numberOfVariables]++;
							index = job.numberOfVariables;
							job.numberOfVariables++;
						} else {
							job.variableValue[index]++;

						}
						if (job.variableValue[index] < value)
							job.currentLine = findLine(
									job.content[job.currentLine]);
						else {
							job.variableValue[index] = -1;
						}
					}
				}
				// }

				out.clear();
				out.str("");
				out << currentTime;
				string time = out.str();
				out.clear();
				out.str("");
				out << job.currentLine;
				string lineNumber = out.str();
				string currStr = job.content[job.currentLine];

				cout << "\n";
				if (strcmp(job.name.c_str(), "job2.txt") == 0)
					cout << "\t\t\t";
				if (strcmp(job.name.c_str(), "job3.txt") == 0)
					cout << "\t\t\t\t\t\t";
				cout << "--------------------------------------\n";
				if (strcmp(job.name.c_str(), "job2.txt") == 0) {
					cout << "\t\t\t";
					jobNum = 2;
				}
				if (strcmp(job.name.c_str(), "job3.txt") == 0) {
					cout << "\t\t\t\t\t\t";
					jobNum = 3;
				}
				cout << job.name + "\tcurrentLine:" + lineNumber
						+ "\t\tcurrentTime: " + time;
				if (strcmp(job.name.c_str(), "job2.txt") == 0) {
					cout << "\t\t\t";
				}
				if (strcmp(job.name.c_str(), "job1.txt") == 0) {
					cout << "\t\t\t\t\t\t";
				}
				cout << "\t\t\"" + currStr + "\"";
				//if(job.name=="job3.txt")
				//cout << job.name + "\tcurrentLine:" + lineNumber + "\t\tcurrentTime: " + time<<job.content[job.currentLine];
				//if (qcounter == 0){

				//  q.pop();
				//}
				// if end of file
				if (job.currentLine > job.numberOfLines) {

					qcounter = -1;

				} else {
					job.currentLine++;
				}
				// if this is the end of the quantum for this job
				if ((qcounter >= (quantom - 1)) && job.currentLine
						<= job.numberOfLines) {
					qcounter = -1;
					out.clear();
					out.str("");
					out << currentTime;
					job.output = job.output + " " + out.str();
					if (job.currentLine <= job.numberOfLines)
						q.push(job);
					cout << "\n";
					if (strcmp(job.name.c_str(), "job2.txt") == 0)
						cout << "\t\t\t";
					if (strcmp(job.name.c_str(), "job3.txt") == 0)
						cout << "\t\t\t\t\t\t";
					cout << "--------------------------------------\n";
				}
				qcounter++;
				//displayQueue(q, jobNum);
			} else {
				qcounter = 0;

				q.push(job);
			}
			currentTime++;

			if (job.currentLine > job.numberOfLines)
				eof = true;
			if (q.empty())
				noMoreJob = true;
		}

		cout << "\n---------------------" << job.name
				<< "---------------------------------------------END\n";

	}
	//without memory and cache
public:

	void RR(Jobs *alljobs, string *output, int numberOfJobs, int quantom,
			int dumpTime, string outfile) {
		queue<Jobs> q;
		int totalTime;
		int cnt = 0;
		Sort(alljobs, numberOfJobs);
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

void display(char** argv) {
	for (int i = 0; i < 4; i++) {
		cout << argv[i] << "\n";
	}
}

int main(int argc, char** argv) {
	string temp;
	int numberOfJobs = 0;
	int cnt = 0;
	bool memory = false;
	int quantum = 0;
	//display(argv);

	Jobs alljobs[MAXJOBSNUMBER];
	MemoryJobs memoryJobs[MAXJOBSNUMBER];
	FileTransfer filetransfer;
	Analysis analysis;
	string files[MAXLINENUMBER];
	string result[MAXLINENUMBER];
	string output[MAXLINENUMBER];
	string outfile = "";
	int dumpTime = -1;

	// check for -m argument1
	if (strcmp(argv[1], "-m") == 0) {
		memory = true;
	}

	if (memory) {
		// SIMULATED MEMORY
		// get the time to memory dump
		dumpTime = atoi(argv[2]);

		// put each jobfile into files array
		filetransfer.ReadFile(argv[5], files);
		while (files[cnt] != "") {
			temp = files[cnt];
			MemoryJobs memoryJob;
			memoryJob.initializer();
			memoryJob.numberOfLines = filetransfer.ReadFile(temp,
					memoryJob.content);
			memoryJob.name = files[cnt];
			memoryJob.startTime = analysis.StringToDigit(memoryJob.content[0]);
			memoryJobs[cnt] = memoryJob;
			cnt++;
			numberOfJobs++;
		}
		if (strcmp(argv[3], "FCFS") == 0) {
			// get the name of the outfile
			outfile = argv[5];
			analysis.FCFS(alljobs, output, numberOfJobs, dumpTime, outfile);
		} else if (strcmp(argv[3], "RR") == 0) {
			quantum = analysis.StringToDigit(argv[4]);
			// get the name of the outfile
			outfile = argv[6];
			quantum = analysis.StringToDigit(argv[4]);
			analysis.RR(memoryJobs, output, numberOfJobs, quantum, dumpTime,
					outfile);
		}
		filetransfer.WriteFile("write", output, cnt);

	} else {
		// NON-SIMULATED MEMORY
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
		if (strcmp(argv[1], "FCFS") == 0)
			analysis.FCFS(alljobs, output, numberOfJobs, dumpTime, outfile);
		else if (strcmp(argv[1], "RR") == 0)
			analysis.RR(alljobs, output, numberOfJobs, quantum, dumpTime,
					outfile);
		filetransfer.WriteFile("write", output, cnt);
	}
	return 0;
}

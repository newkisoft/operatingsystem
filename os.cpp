//============================================================================
// Name        : os.cpp
// Author      : Kianoush Akhavan Karbasi(20339561), Ashley Ramsey(20543173)
// Version     : 1
// Copyright   : You can copy this as many as you want
// Description : Operating System in C++, UWA 2010
//============================================================================
// it is possible to display cache memory as well just replace memory by cache lik
// cache.displayMemory(output,outputCounter);
// in line 1000 it is displaying cache if you want memory just replace cache by memory
// memory.displayMemory(output,outputCounter);

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <sstream>

using namespace std;
// maximum line number and variables and jobs in the text files
static const int MAXLINENUMBER = 200;
static const int MAXNUMBEROFVARIABLES = 100;
static const int MAXJOBSNUMBER = 400;

// Position of line in a frame or page
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

//page of the memory
struct Page {
	string lines[2];
	int jobLineNumber[2];
};
//frame of the main memory
struct Frame {
	Page pages[2];
	int usedTime;
	string jobName;
};
// jobs specific for memory part of the project which has some extra
// attribues.
// name is the name of the job
// startTime is the time which job should be started it is at the first
// line of the file
// numberOfLines is the number of lines in the text file or same as
// number of processes in the job
// content is all the content of the file
// output is what is going to be print out
// variablName names of variables in the file for loops
// currentLine the line which the cpu is running at the moment
// numberOfVariables number of variables in the file
// lastUsedTime last time this job has been called
// pagefault it will be true if page fault happening in the main memory
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
// main memory of the system which contains 8 frames
struct MainMemory {
	Frame frames[8];
 MainMemory(){
	for (int i = 0; i < 8; i++) {
				frames[i].usedTime = -1;
				for(int j=0;j<2;j++)
					frames[i].pages[0].lines[j]="";
	}
}
 // initializing all the frames by empty string
public:
	void initialize() {
		for (int i = 0; i < 8; i++) {
			frames[i].usedTime = -1;
			for(int j=0;j<2;j++)
				frames[i].pages[0].lines[j]="";
		}
	}
// finding empty frame in the memory to be used and return
// index of that frame
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
// find the frame number of main memory which an specfic job
// has been passed by name
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
// finding the frame which has not been used for a while and
// return the index of that to be used
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
//display the content of the main memory

public:
	void displayMemory(string output[],int &counter){
		stringstream out;
		cout<<"=================================Memory================================================";

		output[counter]="=================================Memory================================================";
		counter++;
		for(int i=0;i<8;i++){
			cout<<"\n Frame :"<<i;
			out.clear();
			out.str("");
			out << i;

			output[counter]="\n Frame :"+out.str();

					counter++;
			for(int j=0;j<2;j++){
				cout<<"\n"<<frames[i].pages[0].lines[j]<< "\t"<< frames[i].jobName<<"\n";
				output[counter]="\n"+frames[i].pages[0].lines[j]+ "\t"+ frames[i].jobName+"\n";
				counter++;
			}
			cout<<"\n........................\n";
			output[counter]="\n........................\n";
			counter++;

		}
		cout<<"=================================End Of Memory================================================";
		output[counter]="=================================End Of Memory================================================";
		counter++;

	}
// for setting the memory by job name and passing the start line and content
// jobName name of the job
// startLineNumber from this line in the content should be set in the memory
// content the whole content of the file
// position start position of the ilne in the memory
// numberOfLines number of lines which should be written in the memory
// storedContent the content which has been written in the memory
// time current time
public:

	void setMemory(string jobName,int startLineNumber, string content[], int position,
			int numberOfLines,string storedContent[], int time) {
		int lineCnt = 0;
		int pageCnt = 0;
		int p = position;
		bool memoryFull = false;
		int frameNumber = -1;

		int sCCnt = 0;
		frameNumber = FindFrameNumber(jobName);
		// if job is not in the memory find the empty frame
		if (frameNumber == -1) {
			frameNumber = findEmptyFrame();
		}
		//if there is not any empty memory find the least used one and replace it
		if (frameNumber == -1) {
			// MEMORY FULL
			frameNumber = findLeastUsedFrame();
		}
		//it means there is only one line
		if (numberOfLines == 0){
			frames[frameNumber].pages[pageCnt].lines[lineCnt] = content[p];
			frames[frameNumber].pages[pageCnt].jobLineNumber[lineCnt] = startLineNumber;
			frames[frameNumber].usedTime = time;
			storedContent[0] = content[p];
	}
		else {
			for (int j = 0; j < numberOfLines; j++) {
				frames[frameNumber].pages[pageCnt].lines[lineCnt] = content[p];
				frames[frameNumber].jobName=jobName;
				frames[frameNumber].pages[pageCnt].jobLineNumber[lineCnt] = startLineNumber;
				startLineNumber++;
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
// search the memory for a specific line and return the position of that line
// line the content of the one line in which is in text file
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
// find the next frame by the jobname and line number
// jobName name of the job in the frame
// lineNumber the number of line which should find the next frame of that
// returning the a frame type which is the next frame that contain this job
 Frame findNextFrame(string jobName,int lineNumber){
	Frame nextFrame;
	for(int j=0;j<2;j++)
		nextFrame.pages[0].lines[j]="";
	for(int i=0;i<8;i++)
	{
		if(frames[i].jobName==jobName && frames[i].pages[0].jobLineNumber[0]>lineNumber){
			nextFrame.pages[0].lines[0]=frames[i].pages[0].lines[0];
			nextFrame.pages[0].lines[1]=frames[i].pages[0].lines[1];
		}
	}
	return nextFrame;
}
 // Get frames releted to a specific job
 // linePos the start line position in the frame that should be fetched
 // storedContent the content of the memory will be saved in this array
 // jobName the name of the job which should get the frames related to that
	void getFrames(LinePosition linePos, string storedContent[], string jobName) {

		int storedContCnt = 0;
		for (int i = linePos.frameNumber; i < linePos.frameNumber + 2; i++) {
			if (i < 8) {
				for (int j = 0; j < 2; j++) {
					storedContent[storedContCnt]
							= frames[i].pages[linePos.pageNumber].lines[j];
					storedContCnt++;
				}
			}
			}
		Frame nextFrame = findNextFrame(jobName,frames[linePos.frameNumber].pages[linePos.pageNumber].jobLineNumber[0]);
		if(nextFrame.pages[0].lines[0]!=""){
			for(int j=0;j<2;j++){
				storedContent[storedContCnt]=nextFrame.pages[0].lines[j];
				storedContCnt++;
			}
		}
	}
};
// cachememory which contains pages and jobs names
struct CacheMemory {
	Page pages[2];
	string name[4];
	//setting the memory by jobname
	// storedContent content which has been saved
	// jobName the name of the job which is going to be save in the memory
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
	// display the content of the memory
	//output what is going to be printed
	// the line of the job in the memory
public:
	void displayMemory(string output[],int &counter){
		int jobNameCounter=0;

		cout<<"=================================Cache Memory================================================";

		output[counter]="=================================Cache Memory================================================";
		counter++;
		for(int i=0;i<2;i++){
			cout<<"\n Page :"<<i;
			for(int j=0;j<2;j++){
				cout<<"\n"<<pages[i].lines[j]<< "\t"<< name[jobNameCounter]<<"\n";
				output[counter]="\n"+pages[i].lines[j]+ "\t"+ name[jobNameCounter]+"\n";
				counter++;
			}
			cout<<"\n........................\n";
			jobNameCounter++;
		}
		cout<<"=================================End Of Cahce Memory================================================";
		output[counter]="=================================End Of Cahce Memory================================================";
		counter++;
	}
	// searching in a memory to find a position of a specfic job
	// jobName name of the job
	// line the line which is giong to be searched in the memory
	// return the position of the line
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
	// search in the cache memroy for a specific line and return the position of that
	// the line which should be looking for that in the cache memory
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
// Jobs specific for the first part of the project which is not using the memory
// name the name of the job
// startTime the time which the job is coming to the computer which can be read at
// the first line of the text file
// duration how long does it take to finish the job
// numberOfLines number of lines in the text file which are the processes for our
// simulator
// output what ever has to be print out
struct Jobs {
	string name;
	int startTime;
	int duration;
	int numberOfLines;
	string output;
};
// general methods which can be used anywhere
class General {
public:
	// sort for sorting all the jobs which are specific for memory part of the project
	// alljobs all the jobs that will be processed
	// length number of jobs
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
	// sort all the jobs specific for the first part of the project without memory
	// alljobs all the jobs that will be processed
	// length number of jobs
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
	// find the index of the specific line in an array of files content
	// content an array of file content
	// line a specifc line in a content
	// return the index of that line in content
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
	// get all the lines in the content from the start positions
	// content the content which lines should be fetched from it
	// start start index which lines should be fetched in the content
	// numberOfLines how many lines should be fetched
	// result lines which has been fetch will be put in this array
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
// For reading and writing to the file
class FileTransfer {
	//Read from a file
	// filename the name of the file
	// content all the content tha thas been read from a file
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
	// write to the file
	// filename the name of the file which has to be written
	// content an array of lines which has to be wrriten in the file
	// length length of the content which is not empty line
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
// Analysis the file for various things like duration and variables

class Analysis: public General {

public:

	Analysis() {

	}
	// converting string to digit
	// number is the number which is in string
	// return the number which has been convertet
public:

	int StringToDigit(string number) {
		int temp;
		temp = atoi(number.c_str());
		return temp;
	}
	// checking whether the char is character or not
	// c the character which has to be checked
	// return true if it is a character otherwise false
public:

	bool isCharacter(char c) {
		if ((c > 'a' && c < 'z') || (c > 'A' && c < 'Z'))
			return true;
		else
			return false;
	}
	// find the variable name in a line
	// fullString one line of the text file
	// return the variable name
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
	// find for how long the loop should go the max value of the variable
	// fullString one line of the text file
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
	// find the start time of the job
	//fullString the first line of the text file
	// return the start of the job
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
	// find the index of the variable in the variable list
	// variables array of variables
	// variable the value of the variable
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
	// initilize all the variables by zero
	// *variable all the variables
public:

	void initialize(int *variable) {
		for (int i = 0; i < MAXNUMBEROFVARIABLES; i++)
			variable[i] = 0;
	}
	// calculate the time is taking to finish for each job
	// fileName the name of the file
	// return the time is taking to finish that job
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
			if (strncmp(content[cnt].c_str(), "if", 2) == 0) {
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
	// update the last time job has been used in the memory
	//alljobs list of all jobs
	// job that specific job which has been called recently
	// numberOfJobs the total number of jobs
private:

	void updateAllJobs(MemoryJobs allJobs[], MemoryJobs job, int numberOfJobs) {
		for (int i = 0; i < numberOfJobs; i++) {
			if (allJobs[i].name == job.name) {
				allJobs[i].lastUsedTime = job.lastUsedTime;
				break;
			}
		}
	}
	// First come first server method for first part of the project which is not
	// using the memory
	// alljbos list of all jobs

public:

	void FCFS(Jobs *alljobs, string *output, int numberOfJobs) {
		queue<Jobs> q;
		General general;
		int cnt;
		int time=0;
		string temp;
		general.Sort(alljobs, numberOfJobs);
		for (int i = 0; i < numberOfJobs; i++) {
			q.push(alljobs[i]);
		}
		stringstream out;
		cnt = 0;
		Jobs job;
		job = q.front();
		time =job.startTime;
		while (!q.empty()) {
			out.clear();
			out.str("");
			Jobs job;
			job = q.front();
			out << job.duration;
			temp=out.str();
			out.clear();
			out.str("");
			out << time;
			output[cnt] = job.name + "," + out.str()+","+temp;
			cout << output[cnt] << "\n";
			q.pop();
			cnt++;
			time +=job.duration;
		}
	}
	// display the content of the queue
	// q queue

public:

	void displayQueue(queue<MemoryJobs> q) {
		queue<MemoryJobs> myq = q;
		cout << "\n----------------------------------QUEUE--------------------------------------------\n";
		MemoryJobs job = myq.front();
		cout << "\njob name: " + job.name + "\n";
		myq.pop();
		while (!myq.empty()) {
			MemoryJobs job = myq.front();
			cout << "job name: " + job.name + "\n";
			myq.pop();
		}
		cout << "\n";
		cout << "\n----------------------------------End Of QUEUE--------------------------------------------\n";

	}
	// if memory has been used this function should be called;
	// alljobs an array of all jobs
	// numberOfJobs the number of jobs which should be processed
	// dumpTime the time which the main memory or cache memory should be shown
public:

	void FCFS(MemoryJobs *alljobs,  int numberOfJobs,
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
		string output[2000];
		int outputCounter=0;
		string storedContent[16];
		LinePosition linePosition;
		bool eof = false;
		FileTransfer filetransfer;
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
		while (!noMoreJob ) {
			if(currentTime%dumpTime ==0)
					memory.displayMemory(output,outputCounter);
			if(!q.empty())
				displayQueue(q);
			out.clear();
			out.str("");
			if (eof == true && !q.empty()) {
							output[outputCounter]="\n---------------------" + job.name+ "---------------------------------------------END\n";
							outputCounter++;
							//job = q.front();
							eof=false;
							//q.pop();
			}
			if (!q.empty() && job.currentLine>job.numberOfLines) {
				job = q.front();
				q.pop();
			}

			if (job.startTime <= currentTime) {

				// First time job being processed
				if (job.lastUsedTime == 0) {
					memory.setMemory(job.name,job.currentLine, job.content, 0, 4,
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
							cout<< "\n\n\tPagefault happened here, reading from disk\n";
							output[outputCounter]="\n\n\tPagefault happened here, reading from disk\n";
							outputCounter++;
							if (job.currentLine < job.numberOfLines - 2)
								memory.setMemory(job.name,job.currentLine, job.content,
										job.currentLine, 2,
										 storedContent,currentTime);
							else
								// if current line is end of file/1 line before end of file, take last 2 lines of the file
								memory.setMemory(job.name,job.currentLine, job.content,job.currentLine ,job.numberOfLines
												 - job.currentLine, storedContent,currentTime);
							cache.setMemory(storedContent, job.name);
							linePosition = memory.searchMemory(
									job.content[job.currentLine]);
							job.currentLine--;
							pageFault = true;

						}
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

				cout << job.name + "\tcurrentLine:" + lineNumber + "\t\tcurrentTime: " + time<<job.content[job.currentLine];
				output[outputCounter]=job.name + "\tcurrentLine:" + lineNumber + "\t\tcurrentTime: " + time+job.content[job.currentLine];
				outputCounter++;
				// if end of file
				if (job.currentLine <= job.numberOfLines) {
					job.currentLine++;
				}
				// if this is the end of the quantum for this job
				if (job.currentLine <= job.numberOfLines) {

					out.clear();
					out.str("");
					out << currentTime;
					job.output = job.output + " " + out.str();
					cout << "\n";
					output[outputCounter]="\n";
					outputCounter++;
				}

			} else {

				q.push(job);
			}
			currentTime++;

			if (job.currentLine > job.numberOfLines)
				eof = true;
			if (q.empty() && job.currentLine>job.numberOfLines)
				noMoreJob = true;
		}

		cout << "\n---------------------" << job.name
				<< "---------------------------------------------END\n";
		output[outputCounter]= "\n---------------------" + job.name
				+ "---------------------------------------------END\n";
							outputCounter++;
		filetransfer.WriteFile(outfile,output,outputCounter);

	}
	// if memory has been used this function should be called;
	// Round Robin method for jobs with memory
	// alljobs all the jobs in the file
	// numberOfJobs the number of jobs which should be processed
	// quantom the time which job should be replaced
	// dumpTime the time which should the content of the memory displayed
	// outfile the file which the result should be print in
public:

	void RR(MemoryJobs *alljobs,  int numberOfJobs, int quantom,
			int dumpTime, string outfile) {
		int jobNum = 1;
		queue<MemoryJobs> q;
		int currentTime = 0;
		FileTransfer filetransfer;
		bool noMoreJob = false;
		bool pageFault = false;
		string name;
		int index;
		int value;
		int outputCounter=0;
		int cnt = 0;
		int qcounter = 0;
		string storedContent[16];
		LinePosition linePosition;
		string output[2000];
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
			displayQueue(q);
			out.clear();
			out.str("");
			if(currentTime%dumpTime ==0)
				cache.displayMemory(output,outputCounter);
			if (qcounter == 0 && !q.empty()) {
				job = q.front();
				q.pop();
			}
			if (eof == true) {
				cout << "\n---------------------" << job.name
						<< "---------------------------------------------END\n";
				output[outputCounter]="\n---------------------" + job.name+ "---------------------------------------------END\n";
				outputCounter++;

				job = q.front();
				eof=false;
				q.pop();
			}
			if (job.startTime <= currentTime) {

				// First time job being processed
				if (job.lastUsedTime == 0) {
					memory.setMemory(job.name,job.currentLine, job.content, 0, 4,
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
							output[outputCounter]="\n\n\tPagefault happened here, reading from disk\n";
											outputCounter++;
							qcounter = quantom - 1;
							if (job.currentLine < job.numberOfLines - 2)
								memory.setMemory(job.name,job.currentLine, job.content,
										job.currentLine, 2,
										 storedContent,currentTime);
							else
								// if current line is end of file/1 line before end of file, take last 2 lines of the file
								memory.setMemory(job.name,job.currentLine, job.content,job.currentLine ,job.numberOfLines
												 - job.currentLine, storedContent,currentTime);
							cache.setMemory(storedContent, job.name);
							linePosition = memory.searchMemory(
									job.content[job.currentLine]);
							job.currentLine--;
							pageFault = true;

						}
					}
				}
				job.lastUsedTime = currentTime;
				updateAllJobs(alljobs, job, numberOfJobs);
				if (!pageFault) {

					// put the current line's string into currStr
					string currStr = job.content[job.currentLine];
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

				out.clear();
				out.str("");
				out << currentTime;
				string time = out.str();
				out.clear();
				out.str("");
				out << job.currentLine;
				string lineNumber = out.str();
				string currStr = job.content[job.currentLine];
				"\n";
				cout << "\n";
				output[outputCounter]="\n";
								outputCounter++;
				cout << job.name + "\tcurrentLine:" + lineNumber + "\t\tcurrentTime: " + time<<job.content[job.currentLine];
				output[outputCounter]=job.name + "\tcurrentLine:" + lineNumber + "\t\tcurrentTime: " + time+job.content[job.currentLine];
				outputCounter++;
				if (job.currentLine > job.numberOfLines) {

					qcounter = -1;

				} else {
					job.currentLine++;
				}
				// if this is the end of the quantum for this job
				if ((qcounter > (quantom - 1)) && job.currentLine
						<= job.numberOfLines) {
					qcounter = -1;
					out.clear();
					out.str("");
					out << currentTime;
					job.output = job.output + " " + out.str();
					if (job.currentLine <= job.numberOfLines)
						q.push(job);
					cout << "\n";
					output[outputCounter]="\n";
									outputCounter++;
				}
				qcounter++;
			} else {
				qcounter = 0;

				q.push(job);
			}
			currentTime++;

			if (job.currentLine > job.numberOfLines)
				eof = true;
			if (q.empty() )
				noMoreJob = true;
		}

		cout << "\n---------------------" << job.name
				<< "---------------------------------------------END\n";
		output[outputCounter]="\n---------------------" + job.name
				+ "---------------------------------------------END\n";
											outputCounter++;
		filetransfer.WriteFile(outfile,output,outputCounter);

	}
	//without memory and cache
	// alljobs all the jobs
	// output whatever should be print out as a result
	// numberOfJobs number of jobs which should be processed
	// quantom the time which job should be replaced
	// outfile the file which the result should be print out
public:

	void RR(Jobs *alljobs, string *output, int numberOfJobs, int quantom,
			 string outfile) {
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
				cout << output[cnt] << "\n";
				cnt++;
			}

		}
	}

};
// displaying all the arguments that has been passed
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
		if(strcmp(argv[3], "FCFS") == 0){
			filetransfer.ReadFile(argv[4], files);
		}else
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
			analysis.FCFS(memoryJobs,  numberOfJobs, dumpTime, outfile);
		} else if (strcmp(argv[3], "RR") == 0) {
			quantum = analysis.StringToDigit(argv[4]);
			// get the name of the outfile
			outfile = argv[6];
			quantum = analysis.StringToDigit(argv[4]);
			analysis.RR(memoryJobs,  numberOfJobs, quantum, dumpTime,
					outfile);
		}
		filetransfer.WriteFile("write", output, cnt);

	} else {
		// NON-SIMULATED MEMORY

		if (strcmp(argv[1],"FCFS")==0) {
			filetransfer.ReadFile(argv[2], files);
		} else if (strcmp(argv[1],"RR")==0) {
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
			alljobs[cnt] = job;
			cnt++;
			numberOfJobs++;
		}
		if (strcmp(argv[1], "FCFS") == 0)
		{
			if(argc>=3){
			   outfile="out.file";
			}else
				outfile=argv[3];
			analysis.FCFS(alljobs, output, numberOfJobs);
		}
		else if (strcmp(argv[1], "RR") == 0)
		{
			if(argc>=4){
			   outfile="out.file";
			}else
				outfile=argv[4];
			analysis.RR(alljobs, output, numberOfJobs, quantum,
					outfile);
		}
		filetransfer.WriteFile(outfile, output, cnt);
	}
	return 0;
}

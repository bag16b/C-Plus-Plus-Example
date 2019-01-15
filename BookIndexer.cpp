#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

char toLower(char a) {
	
	if(a < 91 && a > 64)
		a += 32;
	
	return a;
	
}

string toLower(string a) {
	
	for(int i = 0; i < a.size(); i++)
		a[i] = toLower(a[i]);
	
	return a;
	
}

string removePunctuation(string a) {\

	int size = a.size()-1;
		
	if(a[0] == '.' || a[0] == '?'|| a[0] == '!' || a[0] == ',' || a[0] == ';' || a[0] == ':' || a[0] == '-' || a[0] == '\"' || a[0] == '\'' || a[0] == '_' || a[0] == '(' || a[0] == ')')
		a.erase(0,1);
		
	else if(a[size] == '.' || a[size] == '?'|| a[size] == '!' || a[size] == ',' || a[size] == ';' || a[size] == ':' || a[size] == '-' || a[size] == '\"' || a[size] == '\'' || a[size] == '_' || a[size] == '(' || a[size] == ')')
		a.erase(a.end() -1, a.end()); 
	else 
		return a;
	return removePunctuation(a);
	
}

class wordInstance{
	private:
		string _name;
		string _chapter;
		int _paragraph;
		int _line;
		int _chapterCount;
		int _greatestChapterCount;
		bool _alreadyPushed;
	public:
		wordInstance(string name, string chapter, int paragraph, int line){
			_name = name;
			_chapter = chapter;
			_paragraph = paragraph;
			_line = line;
			_chapterCount = 1;
			_greatestChapterCount = -1;
			_alreadyPushed = false;
		}
		wordInstance(){
			
		}
		string name() {
			return _name;
		}
		void setName(string name) {
			_name = name;
		}
		wordInstance* getAddress() {
			return this;
		}
		string chapter(){
			return _chapter;
		}
		int paragraph(){
			return _paragraph;
		}
		int line(){
			return _line;
		}
		int getChapterCount(){
			return _chapterCount;
		}
		void setChapterCount(int chapterCount){
			_chapterCount = chapterCount;
		}
		void setGreatestChapterCount(int GreatestChapterCount){
			_greatestChapterCount = GreatestChapterCount;
		}
		int getGreatestChapterCount(){
			return _greatestChapterCount;
		}
		bool Pushed() {
			return _alreadyPushed;
		}
		void Pushed(bool a){
			_alreadyPushed = a;
		}
		
};

bool operator< (wordInstance a, wordInstance b) {
	
	if(a.name() == b.name())
	{
		if(a.chapter() == b.chapter()){
			if(a.paragraph() == b.paragraph())
				return a.line() > b.line();
			return a.paragraph() > b.paragraph();
		}
		else if(a.getGreatestChapterCount() != -1 && a.getChapterCount() == a.getGreatestChapterCount())
			return false;
		else if(b.getGreatestChapterCount() != -1 && b.getChapterCount() == b.getGreatestChapterCount())
			return true;
		else if(a.getChapterCount() > b.getChapterCount() && (a.getGreatestChapterCount() == -1))
			return false;
		else if(a.getChapterCount() < b.getChapterCount() && (a.getGreatestChapterCount() == -1))
			return true;
		else
			return a.chapter() > b.chapter();
	}
	return a.name() > b.name();
	
}

int main() {
	
	ifstream fin;
	fin.open("book.txt");
	priority_queue<wordInstance> array;

	if(fin.is_open()) {
		
		string inputLine, word;
		string outputLine = "";
		bool isChapter = false;
		string chapter = "0";
		int paragraph = 1;
		int line = 1;
		bool reachedParagraph = false;
		while(getline(fin, inputLine)) {
			int i = 0;
			stringstream ss(inputLine);
			
			for(int i = 0; ss >> word; i++){
				word = removePunctuation(word);
				word = toLower(word);
				
				if(word == "chapter" && i == 0){
					ss >> word;
					word = toLower(word);
					chapter = removePunctuation(word);
					paragraph = 1;
					isChapter = true;
					reachedParagraph = false;
					break;
				}
					
				outputLine += word + " ";
			
				if(!isChapter && outputLine != "")
					array.push(wordInstance(word, chapter, paragraph, line));
				
			}
			
			
			if(outputLine == "" && !isChapter && reachedParagraph){
//				cout << "new paragraph" << endl;
				paragraph++;
				line = 1;
			}
			else if(outputLine == "" && !isChapter && !reachedParagraph){
				
			}	
			else if(outputLine == "" && isChapter){
				//cout << "chapter " << romanToDecimal(word) << endl;
				line = 1;
			}	
			else{
				line++;
				reachedParagraph = true;
			}
			outputLine = "";
			isChapter = false;
			i++;
			
		}	
		
	}
	fin.close();
	
	ifstream fin1;
	fin1.open("ignore.txt");
	
	queue<wordInstance> queue3;
	
	for(int i = 0; array.size() > 0; i++){
		queue3.push(array.top());
 		array.pop();
	}
	
	if(fin1.is_open()) {
		
		string inputWord;
		int arraysize = queue3.size();
		while(fin1 >> inputWord){
			
			for(int i = 0; i < arraysize; i++){
				wordInstance poppedWord = queue3.front();
				queue3.pop();
				if(poppedWord.name() != inputWord){
					queue3.push(poppedWord);
				}
			}
		}
	}
	
	fin1.close();
	
	ifstream fin2;
	fin2.open("synonyms.txt");

	if(fin2.is_open()) {
		
		string inputWord, inputLine, word, masterWord;
		int arraysize = queue3.size();
			
			while(getline(fin2, inputLine)) {
				int i = 0;
				stringstream ss(inputLine);
				
				ss >> masterWord;
				
				while(ss >> word){
					
					for(int i = 0; i < arraysize; i++) {
						wordInstance poppedWord = queue3.front();
						queue3.pop();
						if(poppedWord.name() == word){
							poppedWord.setName(masterWord);
						}
						queue3.push(poppedWord);
					}
					
				}
	}
}
priority_queue<wordInstance> PQ_Sorted1;

while(!queue3.empty()){
	PQ_Sorted1.push(queue3.front());
	queue3.pop();
}

vector<wordInstance> sortVector(PQ_Sorted1.size());
	
	for(int i = 0; !PQ_Sorted1.empty(); i++){
		sortVector[i] = PQ_Sorted1.top();
		PQ_Sorted1.pop();
	}
	
	queue<wordInstance*> setChapterCountQueue;
	
	for(int i = 0; i < sortVector.size(); i++){
		if(i > 0){
			if(sortVector[i].name() == sortVector[i-1].name()){
				if(sortVector[i].chapter() == sortVector[i-1].chapter()){
						if(sortVector[i].paragraph() > sortVector[i-1].paragraph()){
							int greaterCount = (sortVector[i].getChapterCount() > sortVector[i-1].getChapterCount()) ? sortVector[i].getChapterCount()+1 : sortVector[i-1].getChapterCount()+1;
							sortVector[i-1].setChapterCount(greaterCount);
							sortVector[i].setChapterCount(greaterCount);
							setChapterCountQueue.push(&sortVector[i-1]);
							for(int j = 0; j < greaterCount-1; j++){
								wordInstance* temp = setChapterCountQueue.front();
								temp->setChapterCount(greaterCount);
								setChapterCountQueue.pop();
								setChapterCountQueue.push(temp);
							}
						}
						else if((sortVector[i].paragraph() == sortVector[i-1].paragraph()) && sortVector[i].line() > sortVector[i-1].line()){
							int greaterCount = (sortVector[i].getChapterCount() > sortVector[i-1].getChapterCount()) ? sortVector[i].getChapterCount()+1 : sortVector[i-1].getChapterCount()+1;
							sortVector[i-1].setChapterCount(greaterCount);
							sortVector[i].setChapterCount(greaterCount);
							setChapterCountQueue.push(&sortVector[i-1]);
							for(int j = 0; j < greaterCount-1; j++){
								wordInstance* temp = setChapterCountQueue.front();
								temp->setChapterCount(greaterCount);
								setChapterCountQueue.pop();
								setChapterCountQueue.push(temp);
								
							
							}	
						}
						else if((sortVector[i].paragraph() == sortVector[i-1].paragraph()) && sortVector[i].line() == sortVector[i-1].line() && sortVector[i].getAddress() != sortVector[i-1].getAddress()){
							int greaterCount = (sortVector[i].getChapterCount() > sortVector[i-1].getChapterCount()) ? sortVector[i].getChapterCount()+1 : sortVector[i-1].getChapterCount()+1;
							sortVector[i-1].setChapterCount(greaterCount);
							sortVector[i].setChapterCount(greaterCount);
							setChapterCountQueue.push(&sortVector[i-1]);
							for(int j = 0; j < greaterCount-1; j++){
								wordInstance* temp = setChapterCountQueue.front();
								temp->setChapterCount(greaterCount);
								setChapterCountQueue.pop();
								setChapterCountQueue.push(temp);							
							}
						}
				else{
					
					for(int i = 0; !setChapterCountQueue.empty(); i++){
						wordInstance* temporary;
						temporary = setChapterCountQueue.front();
						
						setChapterCountQueue.pop();
					}
				
						
				}
					
			}
			else{
					for(int i = 0; !setChapterCountQueue.empty(); i++){
						wordInstance* temporary;
						temporary = setChapterCountQueue.front();
						setChapterCountQueue.pop();
					}
					
						
				}
			
		}
		else{
			for(int i = 0; !setChapterCountQueue.empty(); i++){
				wordInstance* temporary;
				temporary = setChapterCountQueue.front();
				setChapterCountQueue.pop();
			}
		}				
	}
}
	
	priority_queue<wordInstance> PQ_sorted;
	queue<wordInstance> Q_sorted;

	for(int i = 0; i < sortVector.size(); i++) {
		
		PQ_sorted.push(sortVector[i]);
		
	}
	while(!PQ_sorted.empty()){
		wordInstance word = PQ_sorted.top();
		PQ_sorted.pop();
		Q_sorted.push(word);
	}
	
	string wordWorking = "";
	int greatestCount = -1;
	
	queue<wordInstance> Q_sorted2;
	
	while(!Q_sorted.empty()) {
		wordInstance* currentWord = &Q_sorted.front();
		Q_sorted.pop();
		if(wordWorking != currentWord->name()){
			wordWorking = currentWord->name();
			greatestCount = currentWord->getChapterCount();
			currentWord->setGreatestChapterCount(greatestCount);
		}
		else{
			currentWord->setGreatestChapterCount(greatestCount);
		}
		Q_sorted2.push(*currentWord);
	}

	while(!Q_sorted2.empty()){
		wordInstance move = Q_sorted2.front();
		Q_sorted2.pop();
		array.push(move);
	}
	
	vector <wordInstance> sortVector2;
	map <wordInstance*,wordInstance> map1;
	int i = 0;
	while(!array.empty()){
		sortVector2.push_back(array.top());
		array.pop();
		i++;
	}
	
	for(int i = 0; i < sortVector2.size(); i++){
			map1[sortVector2[i].getAddress()] = sortVector2[i];	
		}
	
	int sv2size = sortVector2.size();
	for(int i = 0; i < sv2size; i++){
		for(int j = 0; j < sv2size; j++){
			if(sortVector2[j].name() == sortVector2[i].name()){
				if(sortVector2[j].chapter() == sortVector2[i].chapter()){
					if(sortVector2[j].paragraph() == sortVector2[i].paragraph()){
							if(sortVector2[j].getAddress() != sortVector2[i].getAddress()){
								if(j > i){
									if(sortVector2[j].line() >= sortVector2[i].line()){
										auto it = map1.find(sortVector2[j].getAddress());
										if(it != map1.end())
											map1.erase(sortVector2[j].getAddress());
									}
								}
							}	
						}
		
					}
				
				}

			}
			
		}
	
	for(auto it = map1.begin(); it!=map1.end(); it++){
		//cout << sortVector[i].name() << endl;
		array.push(it->second);
	}
	
	ofstream fout;
	fout.open("results.txt");
	
	if(fout.is_open())
	{
		wordInstance previousPop;
		previousPop.name() = "";
		while(array.size() > 0) {
			
			wordInstance word = array.top();
			array.pop();
			
			if(previousPop.name() == "")
				fout << word.name() << " " << word.chapter() << ":P" << word.paragraph() << "L" << word.line();
			else if(previousPop.name() == word.name())
				fout << ", " << word.chapter() << ":P" << word.paragraph() << "L" << word.line();
			else
				fout << endl << word.name() << " " << word.chapter() << ":P" << word.paragraph() << "L" << word.line();
			previousPop = word;
		}	
		fout << endl;
	}
	fout.close();
}

#pragma once
#include <fstream>
#include <iostream>
#include<stdio.h>
//#include <stack>
#include "Stack.h"

class TText;
class TTextLink;
struct TMem
{
	TTextLink* pFirst, * pLast, * pFree;
};

class TTextLink
{
public:
	bool flag;
	TTextLink* pNext, * pDown;
	char str[80];
	static TMem mem;
	TTextLink(char* s = NULL, TTextLink* pn = NULL, TTextLink* ph = NULL);
	void* operator new (std::size_t n);
	void operator delete (void* p);
	static void InitMem(int size = 100);
	static void PrintFree();
	static void Clean(TText& t);
};



class TText
{
	int level;
	TTextLink* pFirst;
	TTextLink* pCurr;
	Stack<TTextLink*> st;
	//static TMem mem;
public:
	void SetCurFlagOne()
	{
		pCurr->flag = true;
	}
	void GoFirstLink()
	{	
		pCurr = pFirst;
		if (!st.Empty())
			st.StClear();
	}
	void GoNextLink()
	{
		if (pCurr->pNext != NULL)
		{
			st.Push(pCurr);
			pCurr = pCurr->pNext;
		}
	}
	void GoDownLink()
	{
		if (pCurr->pDown != NULL)
		{
			st.Push(pCurr);
			pCurr = pCurr->pDown;
		}
	}
	void GoPrevLink()
	{
		if (!st.Empty())
		{
			pCurr = st.Top();
			st.Pop();
		}
	}
	void InsNextLine(char* s);
	void InsNextSection(char* s);
	void InsDownLine(char* s);
	void InsDownSection(char* s);
	void DelNextLine();
	void DelDownLine();
	void Print();
	void PrintRec(TTextLink* t);
	void Save(const char* fn);
	void SaveRec(TTextLink* t, std::ofstream& osf);
	void Read(const char* fn);
	TTextLink* ReadRec(std::ifstream& ifs);
	void Reset();
	void GoNext();
	bool IsEnd();
};



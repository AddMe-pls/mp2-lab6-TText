#pragma once
#include <fstream>
#include <iostream>
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
	TTextLink(char* s = NULL, TTextLink* pn = NULL, TTextLink* ph = NULL)
	{
		if (s == NULL)
			str[0] = '\o';
		else
		{
			int i = 0;
			do
			{
				str[i] = s[i];
				i++;
			} while (s[i - 1] != '\o');
		}
		pNext = pn;
		pDown = ph;
	}
	//static void InitMem(int size = 100);
	void* operator new (std::size_t n)
	{
		TTextLink* pC = mem.pFree;
		if (mem.pFree->pNext != NULL)
		{
			mem.pFree = mem.pFree->pNext;
		}
		return pC;
	}
	void operator delete (void* p)
	{
		TTextLink* pC = (TTextLink*)p;
		pC->pNext = mem.pFree;
		mem.pFree = pC;
	}
	static void InitMem(int size = 100)
	{
		int esize = sizeof(TTextLink) * size;
		mem.pFirst = mem.pFree = (TTextLink*)malloc(esize);
		mem.pLast = mem.pFirst + (size - 1);
		TTextLink* tmp = mem.pFree;
		while (tmp != mem.pLast)
		{
			tmp->pNext = tmp + 1;
			tmp->flag = false;
			tmp->str[0] = '\o';
			tmp = tmp->pNext;
		}
		tmp->pNext = NULL;
		tmp->flag = false;
	}
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
		pCurr->flag = 1;
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
		if (!st.Empty())
		{
			pCurr = st.Top();
			st.Pop();
		}
	}
	void InsNextLine(char* s)
	{
		TTextLink* t = new TTextLink;
		strcpy_s(t->str, s);
		t->pNext = pCurr->pNext;
		t->pDown = NULL;
		pCurr->pNext = t;
	}
	void InsNextSection(char* s)
	{
		//TTextLink* t = new TTextLink(s, NULL, pCurr->pNext);
		TTextLink* t = (TTextLink*)(TTextLink::operator new(1));
		strcpy_s(t->str, s);
		t->pNext = NULL;
		t->pDown = pCurr->pNext;
		pCurr->pNext = t;
	}
	void InsDownLine(char* s) 
	{
		TTextLink* t = (TTextLink*)(TTextLink::operator new(1));
		strcpy_s(t->str, s);
		t->pNext = pCurr->pDown;
		t->pDown = NULL;
		pCurr->pDown = t;
	}
	void InsDownSection(char* s) 
	{
		TTextLink* t = (TTextLink*)(TTextLink::operator new(1));
		strcpy_s(t->str, s);
		t->pNext = pCurr->pDown;
		t->pDown = NULL;
		pCurr->pDown = t;
	}
	void DelNextLine()
	{
		if (pCurr->pNext)
		{
			TTextLink* t = pCurr->pNext;
			pCurr->pNext = t->pNext;
			delete t;
		}
	}
	void DelDownLine()
	{
		if (pCurr->pDown)
		{
			TTextLink *t = pCurr->pDown;
			pCurr->pDown = t->pNext;
			delete t;
		}
	}
	void Print()
	{
		level = 0;
		PrintRec(pFirst);
	}
	void PrintRec(TTextLink* t)
	{
		if (t != NULL)
		{
			for (int i = 0; i < level; i++)
				std::cout << ' ';
			std::cout << t->str << '\n';
			level++;
			PrintRec(t->pDown);
			level--;
			PrintRec(t->pNext);
		}
	}
	void Save(char* fn)
	{
		std::ofstream osf;
		osf.open(fn);
		SaveRec(pFirst, osf);
		osf.close();
	}
	void SaveRec(TTextLink* t, std::ofstream& osf)
	{
		if (t != NULL)
		{
			osf << t->str << std::endl;
			if (t->pDown != NULL)
			{
				osf << '{' << std::endl;
				SaveRec(t->pDown, osf);
				osf << '}' << std::endl;
			}
			if (t->pNext != NULL)
				SaveRec(t->pNext, osf);
		}
	}
	void Read(const char* fn)
	{
		std::ifstream ifs(fn);
		pFirst = ReadRec(ifs);
		ifs.close();
	}
	TTextLink* ReadRec(std::ifstream& ifs)
	{
		TTextLink* pF = NULL, * pC = NULL;
		char Ruff[80];
		while (ifs.eof() == false)
		{
			ifs.getline(Ruff, 80, '\n');
			if (Ruff[0] == '}')
				break;
			else
				if (Ruff[0] == '{')
				{
					pC->pDown = ReadRec(ifs);
				}
				else
				{
					TTextLink* tmp = (TTextLink (Ruff))(TTextLink::operator new(1));
					if (pC == NULL)
					{
						pF = pC = tmp;
					}
					else
					{
						pC->pNext = tmp;
						pC = tmp;
					}
				}
		}
		return pF;
	}
	void Reset()
	{
		if (pFirst != NULL)
		{
			st.StClear();
		}
		pCurr = pFirst;
		st.Push(pFirst);
		if (pFirst->pNext)
		{
			st.Push(pFirst->pNext);
			if (pFirst->pDown)
				st.Push(pFirst->pDown);
		}
	}
	void GoNext()
	{
		pCurr = st.Pop();
		if (pCurr != pFirst)
		{
			if (pCurr->pNext)
				st.Push(pCurr->pNext);
			if (pCurr->pDown)
				st.Push(pCurr->pDown);
		}
	}
	bool IsEnd()
	{
		return st.Empty();
	}
};


void TTextLink::Clean(TText& t)
	{
	TTextLink* tmp = mem.pFree;
	while (tmp != NULL)
	{
		tmp->flag = 1;
		tmp = tmp->pNext;
	}
	for (t.Reset(); !t.IsEnd(); t.GoNext())
	{
		//t.pCurr->flag = 1;
		t.SetCurFlagOne();
		tmp = mem.pFirst;
		while (tmp <= mem.pLast)
		{
			if (tmp->flag == 1)
				tmp->flag = 0;
			else
				delete tmp;
		}
	}
	}
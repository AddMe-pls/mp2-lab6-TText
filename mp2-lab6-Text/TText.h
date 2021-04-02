#pragma once
#include <iostream>
#include <stack>

class TTextLink
{
public:
	bool flag;
	TTextLink* pNext, * pDown;
	char str[80];
	//static TMem mem;
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
};

class TText
{
	int level;
	TTextLink* pFirst;
	TTextLink* pCurr;
	std::stack<TTextLink> st;
public:
	void GoFirstLink()
	{	
		pCurr = pFirst;
		if (!st.empty())
		{
			while (!st.empty())
				st.pop();
		}
	}
	void GoNextLink()
	{
		if (pCurr->pNext != NULL)
		{
			st.push(pCurr);
			pCurr = pCurr->pNext;
		}
	}
	void GoDownLink()
	{
		if (!st.empty())
		{
			pCurr = st.top();
			st.pop();
		}
	}
	void InsNextLine(char* s)
	{
		TTextLink* t = new TTextLink;
		strcpy(t->str, s);
		t->pNext = pCurr->pNext;
		t->pDown = NULL;
		pCurr->pNext = t;
	}
	void InsNextSection(char* s)
	{
		TTextLink* t = new TTextLink(s, NULL, pCurr->pNext);
		pCurr->pNext = t;
	}
	void InsDownLine(char* s) {}
	void InsDownSection(char* s) {}
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
	void Read(char* fn)
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
					TTextLink* tmp = new TTextLink(Ruff);
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
				while (!st.empty())
					st.pop();
		}
		pCurr = pFirst;
		st.push(pFirst);
		if (pFirst->pNext)
		{
			st.push(pFirst->pNext);
			if (pFirst->pDown)
				st.push(pFirst->pDown);
		}
	}
	void GoNext()
	{
		pCurr = st.pop();
		if (pCurr != pFirst)
		{
			if (pCurr->pNext)
				st.push(pCurr->pNext);
			if (pCurr->pDown)
				st.push(pCurr->pDown);
		}
	}
};

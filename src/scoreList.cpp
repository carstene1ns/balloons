/***************************************************************************
 *   BALLOONS - prick'n'puzzle -                                           *
 *                                                                         *
 *   Copyright (C) 2007 by -S-v-N-  R.& M. Lagarden                        *
 *   svnmike@sourceforge.net                                               *
 *   eriban62@sourceforge.net                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <cstdio>
#include <string>

#include "scoreList.h"


// --------------------------------------------------------------------
//	Konstruktor
// --------------------------------------------------------------------

ScoreList::ScoreList()
{
	strcpy(ScoreFile,"HIGH.DAT");
	FILE *hFile;

  if((hFile = fopen(ScoreFile, "rb")) != NULL)
	{
	  fread(List, (size_t)sizeof(HLIST), 10, hFile);
	  fclose(hFile);
	}
	else
		for(int i=0; i<10; i++)
	  {
			strcpy((List+i)->Name, "Simply the best");
			(List+i)->Score= (9-i)*1000;
	  }
 
}

// --------------------------------------------------------------------
//	Destruktor
// --------------------------------------------------------------------

ScoreList::~ScoreList()
{

}


// --------------------------------------------------------------------
//	Neuen Score aufnehmen
// --------------------------------------------------------------------

void ScoreList::Enter(char* Name, unsigned int Score)
{
int in;

	for(int i=0; i<10; i++)
  if((List+i)->Score < Score)
	{
	  for(in=9;in>i;in--)
		{
			strcpy((List+in)->Name,(List+in-1)->Name);
			(List+in)->Score = (List+in-1)->Score;
		}
	  strcpy((List+i)->Name,Name);
	  (List+i)->Score = Score;
	  break;
	} //if

	Save();
}


// --------------------------------------------------------------------
//	Testen ob Score in Liste aufgenommen werden kann
// --------------------------------------------------------------------

bool ScoreList::Check(unsigned int Score)
{
	for(int i=0; i<10; i++)
		if((List+i)->Score < Score)
			return(true);
return(false);
}


// --------------------------------------------------------------------
//	Name von Num auslesen
// --------------------------------------------------------------------

char *ScoreList::GetName(int Num)
{
return((List+Num)->Name);
}


// --------------------------------------------------------------------
//	Score von Num auslesen
// --------------------------------------------------------------------

int ScoreList::GetScore(int Num)
{
return((List+Num)->Score);
}


// --------------------------------------------------------------------
//	Liste speichern
// --------------------------------------------------------------------

void ScoreList::Save (void)
{
FILE *hFile;

	if((hFile = fopen(ScoreFile, "wb")) != NULL)
  {
		fwrite(List,(size_t)sizeof(HLIST), 10, hFile);
		fclose(hFile);
  }

} // Save

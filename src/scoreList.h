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

#ifndef CSCORELIST_H
#define CSCORELIST_H


#define HIGHLEN	20							// max. Anz. Buchst. in Highscoreliste


class ScoreList  
{

public:
	ScoreList();
	virtual ~ScoreList();

	typedef struct
	{
		char Name[21];
		unsigned int Score;
	}HLIST;
	
	HLIST List[10];
	void Enter(char* Name, unsigned int Score);
	bool Check(unsigned int Score);
	char *GetName(int);
	int	  GetScore(int);
private:
	char ScoreFile[13];
	void Save (void);

};

#endif

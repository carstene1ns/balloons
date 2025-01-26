/***************************************************************************
 *   BALLOONS - prick'n'puzzle -                                           *
 *                                                                         *
 *   Copyright (C) 2007 by -S-v-N-  R.& M. Lagarden                        *
 *   See AUTHORS file for full author information.                         *
 *                                                                         *
 *   Balloons is free software under the GNU General Public License        *
 *   (version 2 or later).                                                 *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with balloons; if not, see <http://www.gnu.org/licenses/>.      *
 ***************************************************************************/

#include <cstdio>
#include <cstring>

#include "scoreList.h"
#include "util.h"
#include "cfgpath.h"

// --------------------------------------------------------------------
//	Konstruktor
// --------------------------------------------------------------------

ScoreList::ScoreList()
{
	ScoreFile = new char[MAX_PATH+8];

	get_user_data_folder(ScoreFile, MAX_PATH, "balloons");
	if (ScoreFile[0] == '\0') {
		printf("Unable to find user data directory.\n");
		return;
	}

	strcat(ScoreFile, "high.dat");
	FILE *hFile;

	if ((hFile = fopen(ScoreFile, "rb")) != NULL)
	{
		DEBUG("Loading high score");
		fread(List, (size_t)sizeof(HLIST), 10, hFile);
		fclose(hFile);
	}
	else
		strcpy(List[0].Name, "Michael");
		List[0].Score = 1000;
		strcpy(List[1].Name, "Roland");
		List[1].Score = 900;
		strcpy(List[2].Name, "Carsten");
		List[2].Score = 800;
		for (int i = 3; i < 10; i++)
		{
			strcpy(List[i].Name, "Simply the best");
			List[i].Score = (10 - i) * 100;
		}
}

// --------------------------------------------------------------------
//	Destruktor
// --------------------------------------------------------------------

ScoreList::~ScoreList()
{
	delete[] ScoreFile;
}

// --------------------------------------------------------------------
//	Neuen Score aufnehmen
// --------------------------------------------------------------------

void ScoreList::Enter(char* Name, unsigned int Score)
{
	int in;

	for (int i = 0; i < 10; i++)
		if (List[i].Score < Score)
		{
			for (in = 9; in > i; in--)
			{
				strcpy(List[in].Name, List[in - 1].Name);
				List[in].Score = List[in - 1].Score;
			}
			strcpy(List[i].Name, Name);
			List[i].Score = Score;
			break;
		}

	Save();
}

// --------------------------------------------------------------------
//	Testen ob Score in Liste aufgenommen werden kann
// --------------------------------------------------------------------

bool ScoreList::Check(unsigned int Score)
{
	for (int i = 0; i < 10; i++)
		if (List[i].Score < Score)
			return(true);
	return(false);
}

// --------------------------------------------------------------------
//	Name von Num auslesen
// --------------------------------------------------------------------

const char *ScoreList::GetName(int Num)
{
	return(List[Num].Name);
}

// --------------------------------------------------------------------
//	Score von Num auslesen
// --------------------------------------------------------------------

int ScoreList::GetScore(int Num)
{
	return(List[Num].Score);
}

// --------------------------------------------------------------------
//	Liste speichern
// --------------------------------------------------------------------

void ScoreList::Save (void)
{
	FILE *hFile;

	if (ScoreFile[0] == '\0')
	{
		return;
	}

	if ((hFile = fopen(ScoreFile, "wb")) != NULL)
	{
		DEBUG("Saving high score");

		fwrite(List, (size_t)sizeof(HLIST), 10, hFile);
		fclose(hFile);
	}
}

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

#ifndef B_SCORELIST_H
#define B_SCORELIST_H

#define HIGHLEN 20 // max. Anz. Buchst. in Highscoreliste

class ScoreList
{
public:
	ScoreList();
	~ScoreList();

	typedef struct
	{
		char Name[21];
		unsigned int Score;
	} HLIST;

	HLIST List[10];
	void Enter(char* Name, unsigned int Score);
	bool Check(unsigned int Score);
	const char *GetName(int);
	int GetScore(int);

private:
	char *ScoreFile;
	void Save (void);
};

#endif // B_SCORELIST_H

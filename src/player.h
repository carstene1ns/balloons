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

#ifndef B_PLAYER_H
#define B_PLAYER_H

#define MAX_PLAYER 2

class Player
{
public:
	Player();
	virtual ~Player();

	short int *Lives;
	unsigned short int *Score;
	short int *Level;
	short int ActualPlayer;
	char *Name;

	void Init(int);
	void Next(void);
	bool Check(void);

private:
	short int iLives[MAX_PLAYER];
	unsigned short int iScore[MAX_PLAYER];
	short int iLevel[MAX_PLAYER];
	char sName[MAX_PLAYER][30];
};

#endif // B_PLAYER_H

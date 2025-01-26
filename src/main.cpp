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

#include "main.h"

#include "game_world.h"
#include "game.h"

int main(int argc, char* argv[])
{
	try
	{
		DisplayWindow window("Balloons", 640, 416, false, true);

		World *game_world;
		game_world = new World(window);

		game_world->run(window);

		delete game_world;
	}
	catch (...)
	{
		std::cout << "unrecognized exeception caught...\n";
		return 1;
	}
	return 0;
}

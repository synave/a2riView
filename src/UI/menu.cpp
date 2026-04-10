/*************************************************************************/
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*************************************************************************/

#include "menu.hpp"
using namespace std;

Menu::Menu(QMainWindow *parent):_parent(parent)
{
    file = addMenu("File");
    display = addMenu("Display");
    utils = addMenu("Utils");
    
}

void Menu::addActionToFile(QAction* a)
{
  file->addAction(a);
}

void Menu::addActionToDisplay(QAction* a)
{
  display->addAction(a);
}

void Menu::addActionToUtils(QAction* a)
{
  utils->addAction(a);
}

void Menu::addQMenuToDisplay(QMenu* m)
{
  display->addMenu(m);
}

void Menu::addSeparatorToFile()
{
  file->addSeparator();
}

void Menu::addSeparatorToDisplay()
{
  display->addSeparator();
}

void Menu::addSeparatorToUtils()
{
  utils->addSeparator();
}


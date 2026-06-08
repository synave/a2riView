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

#include <QApplication>
#include <QString>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "mainwindow.hpp"
using namespace std;

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
  }
  if (argc == 2)
  {
    QString filename(argv[1]);
    QApplication app(argc, argv);
    MainWindow window;

    window.openFile(filename);

    window.show();

    return app.exec();
  }
  if (argc == 4 && !strcmp(argv[1], "-convert"))
  {
    vf_model m;
    a2ri_vf_init(&m);
    a2ri_vf_open_file(argv[2], &m);
    a2ri_vf_save_file(argv[3], &m);
    return EXIT_SUCCESS;
  }
  cout << "usage : " << argv[0] << " <modele>" << endl;
  cout << "ou" << endl;
  cout << argv[0] << " -convert <modele> <fichier sortie>" << endl;
  return EXIT_FAILURE;
}
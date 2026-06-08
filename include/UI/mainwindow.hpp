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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QWidget>
#include <Qt>
#include <QSpinBox>
#include "glwidget.hpp"
#include "menu.hpp"

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  MainWindow(QMainWindow *parent=0);
  ~MainWindow();

  int openFile(const QString&);
	       
public slots:
  void openDialBox();
  void saveDialBox();
  void closeModel();
  void FullScreen();
  void normFace();
  void backFace();
  void light();
  void informations();
  void pointSize();
  void lineSize();
  void normSize();
  void selectColorBg();
  void selectColorObject();
  void selectColorNorm();
  void selectColorLine();
  void selectColorPoint();
  void displayLine();
  void displayPoint();
  void displayFill();
  void triangleOrientationCW();
  void triangleOrientationCCW();
  void infosScreenshot();
  void doScreenshot();

signals:
  void executeScreenshot(const QString& text, int resolution);
  
private:
  GLWidget *glWidget;
  Menu *menu;
  QMenu *subTriangleOrientation;
  QAction *lightAction;
  QAction *backFaceAction;
  QAction *infoAction;
  QAction *normFaceAction;
  QAction *dispFill;
  QAction *dispLine;
  QAction *dispPoint;  
  QAction *normSizeAction;
  QAction *colNormAction;
  QAction *colLineAction;
  QAction *colPointAction;
  QAction *colObAction;
  QAction *colBgAction;
  QAction *lineSizeAction;
  QAction *pointSizeAction;
  QAction *FullScreenAction;
  QAction *triOrientCWAction;
  QAction *triOrientCCWAction;
  QAction *screenAction;
  QAction *saveAction;
  QAction *openAction;
  QAction *closeAction;
  QAction *quitAction;
  QString screenshotFileName;
  QSpinBox *coeff_res;
};


#endif // MAINWINDOW_HPP

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

#include "mainwindow.hpp"
using namespace std;

bool fs = false;
bool blight = true;
bool bcface = true;
bool bNormFace = false;
bool bfill = true;
bool bline = false;
bool bpoint = false;

MainWindow::MainWindow(QMainWindow *parent):QMainWindow(parent), glWidget(new GLWidget())
{
  menu = new Menu(this);
  setMenuBar(menu);
  setCentralWidget(glWidget);

  openAction = new QAction("Open",this);
  openAction->setShortcut(tr("Ctrl+O"));
  connect(openAction, SIGNAL(triggered()), this, SLOT(openDialBox()));
  saveAction = new QAction("Save",this);
  saveAction->setShortcut(tr("Ctrl+S"));
  connect(saveAction, SIGNAL(triggered()), this, SLOT(saveDialBox()));
  closeAction = new QAction("Close",this);
  closeAction->setShortcut(tr("Ctrl+C"));
  connect(closeAction, SIGNAL(triggered()), this, SLOT(closeModel()));
  quitAction = new QAction("Quit",this);
  quitAction->setShortcut(tr("Ctrl+Q"));
  connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
  dispFill = new QAction("Fill", this);
  dispFill->setShortcut(tr("F"));
  connect(dispFill, SIGNAL(triggered()), this, SLOT(displayFill()));
  dispLine = new QAction("Line", this);
  dispLine->setShortcut(tr("V"));
  connect(dispLine, SIGNAL(triggered()), this, SLOT(displayLine()));
  dispPoint = new QAction("Point", this);
  dispPoint->setShortcut(tr("P"));
  connect(dispPoint, SIGNAL(triggered()), this, SLOT(displayPoint()));
  screenAction = new QAction("Screenshot", this);
  screenAction->setShortcut(tr("F2"));
  connect(screenAction, SIGNAL(triggered()), this, SLOT(infosScreenshot()));
  connect(this, SIGNAL(executeScreenshot(const QString&, int)), glWidget, SLOT(saveScreenshot(const QString&, int)));
  FullScreenAction = new QAction("Full Screen", this);
  FullScreenAction->setShortcut(tr("F11"));
  connect(FullScreenAction, SIGNAL(triggered()), this, SLOT(FullScreen()));
  lightAction = new QAction("Light", this);
  lightAction->setShortcut(tr("L"));
  connect(lightAction, SIGNAL(triggered()), this, SLOT(light()));
  backFaceAction = new QAction("BackFace culling", this);
  backFaceAction->setShortcut(tr("B"));
  connect(backFaceAction, SIGNAL(triggered()), this, SLOT(backFace()));
  infoAction = new QAction("Informations", this);
  infoAction->setShortcut(tr("I"));
  connect(infoAction, SIGNAL(triggered()), this, SLOT(informations()));
  pointSizeAction = new QAction("Point size", this);
  connect(pointSizeAction, SIGNAL(triggered()), this, SLOT(pointSize()));
  lineSizeAction = new QAction("Line size", this);
  connect(lineSizeAction, SIGNAL(triggered()), this, SLOT(lineSize()));
  colBgAction = new QAction("Background Color", this);
  connect(colBgAction, SIGNAL(triggered()), this, SLOT(selectColorBg()));
  colObAction = new QAction("Object Color", this);
  connect(colObAction, SIGNAL(triggered()), this, SLOT(selectColorObject()));
  colPointAction = new QAction("Point Color", this);
  connect(colPointAction, SIGNAL(triggered()), this, SLOT(selectColorPoint()));
  colLineAction = new QAction("Line Color", this);
  connect(colLineAction, SIGNAL(triggered()), this, SLOT(selectColorLine()));
  colNormAction = new QAction("Normal Color", this);
  connect(colNormAction, SIGNAL(triggered()), this, SLOT(selectColorNorm()));
  triOrientCCWAction = new QAction("CCW", this);
  connect(triOrientCCWAction, SIGNAL(triggered()), this, SLOT(triangleOrientationCCW()));
  triOrientCWAction = new QAction("CW", this);
  connect(triOrientCWAction, SIGNAL(triggered()), this, SLOT(triangleOrientationCW()));
  normSizeAction = new QAction("Normal line size", this);
  connect(normSizeAction, SIGNAL(triggered()), this, SLOT(normSize()));
  normFaceAction = new QAction("Normal face", this);
  normFaceAction->setShortcut(tr("N"));
  connect(normFaceAction, SIGNAL(triggered()), this, SLOT(normFace()));
  quitAction = new QAction("Exit", this);
  quitAction->setShortcut(tr("Ctrl+Q"));
  quitAction->setStatusTip(tr("Exit the application"));
  connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

  menu->addActionToFile(openAction);
  menu->addActionToFile(saveAction);
  menu->addSeparatorToFile();
  menu->addActionToFile(closeAction);
  menu->addSeparatorToFile();
  menu->addActionToFile(quitAction);
  menu->addActionToDisplay(FullScreenAction);
  menu->addSeparatorToDisplay();
  menu->addActionToDisplay(dispFill);
  menu->addActionToDisplay(dispLine);
  menu->addActionToDisplay(dispPoint);
  menu->addSeparatorToDisplay();
  menu->addActionToDisplay(normFaceAction);
  menu->addActionToDisplay(backFaceAction);
  menu->addActionToDisplay(lightAction);
  subTriangleOrientation=new QMenu("Triangle Orientation");
  menu->addQMenuToDisplay(subTriangleOrientation);
  subTriangleOrientation->addAction(triOrientCCWAction);
  subTriangleOrientation->addAction(triOrientCWAction);
  menu->addSeparatorToDisplay();
  menu->addActionToDisplay(infoAction);
  menu->addActionToUtils(screenAction);
  menu->addActionToUtils(pointSizeAction);
  menu->addActionToUtils(lineSizeAction);
  menu->addActionToUtils(normSizeAction);
  menu->addActionToUtils(colBgAction);
  menu->addActionToUtils(colObAction);
  menu->addActionToUtils(colPointAction);
  menu->addActionToUtils(colLineAction);
  menu->addActionToUtils(colNormAction);
  normFaceAction->setCheckable(true);
  lightAction->setCheckable(true);
  lightAction->setChecked(true);
  triOrientCCWAction->setCheckable(true);
  triOrientCCWAction->setChecked(true);
  triOrientCWAction->setCheckable(true);
  triOrientCWAction->setChecked(false);
  backFaceAction->setCheckable(true);
  backFaceAction->setChecked(true);
  dispFill->setCheckable(true);
  dispFill->setChecked(true);
  dispLine->setCheckable(true);
  dispPoint->setCheckable(true);

  glWidget->setDisplayFaceNormal(bNormFace);
}

MainWindow::~MainWindow()
{
  delete glWidget;
  delete subTriangleOrientation;
  delete menu;
  delete lightAction;
  delete backFaceAction;
  delete infoAction;
  delete normFaceAction;
  delete dispFill;
  delete dispLine;
  delete dispPoint;
  delete normSizeAction;
  delete colNormAction;
  delete colLineAction;
  delete colPointAction;
  delete colObAction;
  delete colBgAction;
  delete triOrientCWAction;
  delete triOrientCCWAction;
  delete lineSizeAction;
  delete pointSizeAction;
  delete FullScreenAction;
  delete screenAction;
  delete saveAction;
  delete openAction;
  delete closeAction;
  delete quitAction;
}

/* Ouverture d'un modèle */
int MainWindow::openFile(const QString& filename)
{
  int ret = 0;
  
  if(!filename.isEmpty())
    {
      glWidget->updateGL();
      ret = glWidget->openModel(filename);
      
      if(!ret)
  	cerr << "Open Error" << endl;
      glWidget->updateGL();
    }
  
  return ret;
}

/* Ouverture du dialogue pour un modèle */
void MainWindow::openDialBox()
{
  static QString previous_dir = ".";
  
  QString filename = QFileDialog::getOpenFileName(this, tr("Open Model"), previous_dir, tr("Models (*.off *.obj *.gts *.stl *.pgn *.wrl *.ply *.vef)"));
  
  openFile(filename);
  
}

/* Sauvegarde du modèle */
void MainWindow::saveDialBox()
{
  QString filter;
  QString str = QFileDialog::getSaveFileName(
					     this,
					     "Save",
					     "",
					     tr("Models (*.off *.obj *.gts *.stl *.pgn *.wrl *.ply *.vef)"), &filter);
  if(!str.isEmpty())
    glWidget->saveModel(str);
}

/* Fermeture du modèle */
void MainWindow::closeModel()
{
  glWidget->closeModel();
  glWidget->updateGL();
}

/* Sauvegarde d'une capture d'écran */
/*void MainWindow::saveScreenShot()
{

  QString format = "png";
  QString initialPath = QDir::currentPath() + tr("/untitled.") + format;
  QString filename = QFileDialog::getSaveFileName(this,"Save As",
                                             initialPath,
                                             tr("%1 Files (*.%2);;All Files (*)")
                                             .arg(format.toUpper())
                                             .arg(format));
  if(!filename.isEmpty())
    glWidget->saveScreenshot(filename);
    }*/

void MainWindow::infosScreenshot()
{

  screenshotFileName = QFileDialog::getSaveFileName(this, "Enregistrer le screenshot sous",QString(),"png");

  if(!screenshotFileName.isEmpty()){
    QMdiArea *window = new QMdiArea(this);
    window->setWindowTitle("Screenshot");
    window->move(50,150);
    QLabel *mul_res=new QLabel("Multiplication de la resolution par :",window);
    coeff_res=new QSpinBox(window);
    coeff_res->setMinimum(1);
    coeff_res->setMaximum(9);
    QPushButton *ok = new QPushButton(QString("OK"),window);
    QPushButton *annuler = new QPushButton(QString("Annuler"),window);
    QHBoxLayout *window_layout = new QHBoxLayout;
    window_layout->addWidget(mul_res);
    window_layout->addWidget(coeff_res);
    window_layout->addWidget(ok);
    window_layout->addWidget(annuler);
    window->setLayout(window_layout);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    
    connect(ok, SIGNAL(clicked()), window, SLOT(close()));
    connect(ok, SIGNAL(clicked()), this, SLOT(doScreenshot()));
    connect(annuler, SIGNAL(clicked()), window, SLOT(close()));
    window->resize(300,50);
    window->show();
    coeff_res->setFocus(Qt::OtherFocusReason);
  }
}

void MainWindow::doScreenshot()
{
  emit executeScreenshot(screenshotFileName,coeff_res->value());
}

/* Mode plein écran */
void
MainWindow::FullScreen(){
  if(!fs){
    setWindowState(windowState() | Qt::WindowFullScreen);
    fs=true;
  }
  else{
    setWindowState(windowState() &  ~Qt::WindowFullScreen);
    fs=false;
  }
}

/* Affichage des faces */
void MainWindow::displayFill()
{
    dispFill->setChecked(!bfill);
    bfill = !bfill;
    glWidget->displayFill();
  glWidget->updateGL();
}

/* Affichage des lignes */
void MainWindow::displayLine()
{
    dispLine->setChecked(!bline);
    bline = !bline;
    glWidget->displayLine();
  glWidget->updateGL();
}

/* Affichage des points */
void MainWindow::displayPoint()
{
    dispPoint->setChecked(!bpoint);
    bpoint = !bpoint;
    glWidget->displayPoint();
  glWidget->updateGL();
}

/* Affichage des normales aux faces */
void MainWindow::normFace()
{
  normFaceAction->setChecked(!bNormFace);
  bNormFace = !bNormFace;
  glWidget->setDisplayFaceNormal(bNormFace);
  glWidget->updateGL();
}

/* Affichage des faces arrières */
void MainWindow::backFace()
{
    backFaceAction->setChecked(!bcface);
    bcface = !bcface;
    glWidget->cullFace();
    glWidget->updateGL();
}

/* Affichage de la lumière */
void MainWindow::light()
{

  lightAction->setChecked(!blight);
  blight = !blight;
  glWidget->glLight();
  glWidget->updateGL();
}

/* Réglage de la taille des points */
void MainWindow::pointSize()
{
    bool ok;
    double size = QInputDialog::getDouble(this,"Enter a number","Point size :",0.,1.0,10., 2,&ok);
    if(ok)
        glWidget->changeSizePoint(size);
}

/* Réglage de la taille des lignes */
void MainWindow::lineSize()
{
    bool ok;
    double size = QInputDialog::getDouble(this,"Enter a number","Line size :",0.,1.0,10., 2,&ok);
    if(ok)
        glWidget->changeSizeLine(size);
}

/* Réglage de la taille des normales */
void MainWindow::normSize()
{
  bool ok;
  double size = QInputDialog::getDouble(this,"Enter a number","Normal size :",1.0,0.0,10., 3,&ok);
  if(ok)
    glWidget->changeSizeNormLine(size);
}
  
/* Sélection de la couleur de fond */
void MainWindow::selectColorBg()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if (color.isValid())
        glWidget->changeBgColor(color);
}

/* Sélection de la couleur de l'objet */
void MainWindow::selectColorObject()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if (color.isValid())
        glWidget->changeObColor(color);
}

/* Sélection de la couleur des normales */
void MainWindow::selectColorNorm()
{
  QColor color = QColorDialog::getColor(QColor(255,0,0), this);
  if (color.isValid())
    glWidget->changeNormColor(color);
}

/* Sélection de la couleur des lignes */
void MainWindow::selectColorLine()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if (color.isValid())
        glWidget->changeLineColor(color);
}

/* Sélection de la couleur des points */
void MainWindow::selectColorPoint()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    if (color.isValid())
        glWidget->changePointColor(color);
}

/* Affichage des informations */
void MainWindow::informations()
{
  QString message = glWidget->displayInfos();
  QMessageBox::information(this, "Model Information", message);
  glWidget->updateGL();
}

void MainWindow::triangleOrientationCW()
{
  triOrientCCWAction->setChecked(false);
  triOrientCWAction->setChecked(true);
  glWidget->changeTriangleOrientation(GL_CW);
}

void MainWindow::triangleOrientationCCW()
{
  triOrientCWAction->setChecked(false);
  triOrientCCWAction->setChecked(true);
  glWidget->changeTriangleOrientation(GL_CCW);
}

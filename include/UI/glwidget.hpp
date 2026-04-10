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

#ifndef GLWIDGET_HPP
#define GLWIDGET_HPP



#ifdef __APPLE__
#include <GLEW/glew.h>
#else
#include <GL/glew.h>
#endif


#include <QGLWidget>
#include <QKeyEvent>
#include <iostream>
#include <QMutex>

extern "C"
{
#include <a2ri/io.h>
#include <a2ri/model.h>
#include <a2ri/vector.h>
}

#define COUL_TRIANGLE 0.9,0.9,0.9
#define COUL_ARETE 0.0,0.0,0.0
#define COUL_POINT 0.0,0.0,0.0

#define COUL_MAT_OBJ 0.8f, 0.8f, 0.75f
#define COUL_LUMIERE 0.1f ,0.1f, 0.1f
#define COUL_LUMIERE_AMBIENT  1.0f, 1.0f, 1.0f
#define COUL_LUMIERE_DIFFUSE  1.0f, 1.0f, 1.0f
#define COUL_LUMIERE_SPECULAR  1.0f, 1.0f, 1.0f

#define WIDTH 800
#define HEIGHT 600

class GLWidget : public QGLWidget
{
  Q_OBJECT
  
public:
  GLWidget(QWidget *parent = 0, const QColor& cBg=QColor(0,0,127), const QColor& cOb=QColor(Qt::white), const QColor& cLine=QColor(Qt::black), const QColor& cPoint=QColor(Qt::yellow), const QColor& cNorm=QColor(Qt::red));
  ~GLWidget();

public slots:
  void saveScreenshot(const QString& text, int resolution);

public:
  QSize minimumSizeHint() const;
  QSize sizeHint() const;
  int xRotation() const { return xRot; };
  int yRotation() const { return yRot; };
  int zRotation() const { return zRot; };
  int xTranslation() const { return xTrans; };
  int yTranslation() const { return yTrans; };
  int zTranslation() const { return zTrans; };
  void polyChangeMode(GLenum polyMode);
  int openModel(const QString& filename);
  void closeModel();
  void glLight();
  void cullFace();
  const QString& displayInfos();
  void changeSizePoint(double size);
  void changeSizeLine(double size);
  void changeBgColor(QColor color);
  void changeLineColor(QColor color);
  void changePointColor(QColor color);
  void changeObColor(QColor color);
  void setDisplayFaceNormal(bool a);
  void changeNormColor(QColor color);
  void changeSizeNormLine(double size);
  void changeTriangleOrientation(GLenum mode);
  void saveModel(QString filename);
  void displayFill();
  void displayLine();
  void displayPoint();

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  void keyPressEvent(QKeyEvent *event);
  
  void setScene();
  void setProjection();
  void setCamera();
  void gravity();
  void updateInfosModel();

    
private:
  void normalizeAngle(int *angle);
  void display_triangle();
  void initLight();
  void moveCamera();

  GLuint object;
  double xRot;
  double yRot;
  double zRot;

  double xTrans;
  double yTrans;
  double zTrans;
  double pSize;
  double lSize;
  QPoint lastPos;

  QMutex mutex;
  
  vf_model model;
  QString modelName;
  double xmin, ymin, zmin;
  double xmax, ymax, zmax;
  double diag2d, diag3d;
  double screenratio, modelratio;
  double modelheight, modelwidth;
  double xgravity, ygravity, zgravity;
  
  double *tabVertex;
  int *tabIndex;
  double *tabVertexNormal;
  // int *tabIndexNorm;
  
  //tableau des coordonnées des centres de gravité des faces et de
  //leur normal
  double *tabFaceNormal;
  //tableau de facteurs pour agrandir ou rétrécir la taille de la
  //normale de la face
  double *tabFactorNormal;

  GLuint buf_pos;
  GLuint buf_index;
  GLuint buf_VertexNorm;
  GLuint buf_FaceNorm;
  // GLuint buf_indexNorm;
  
  QString strinfos;
  QColor colorBG;
  QColor colorOb;
  QColor colorL;
  QColor colorP;
  QColor colorNorm;

  char* nameInfos;
  char* nbFaceInfos;
  char* nbVertexInfos;
  
  bool isDisplayFaceNormal;

  //orientation des face Counter ClockWise par défaut
  GLenum triangle_orientation;
};
#endif

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




#include "glwidget.hpp"
#include <QtGui>
//#include <QtOpenGL>
/*#ifdef __APPLE__
#include <gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif*/

#include <locale.h>
#include <math.h>
#include <iostream>

#include <ctime>

using namespace std;

#define WIDTH 800
#define HEIGHT 600

#define WIDTH_MIN 80
#define HEIGHT_MIN 60

static bool open=false;
float scale;

bool fillmode = true;
bool linemode = false;
bool pointmode = false;
bool bcullface=false;
bool bFPS=false;

int w=WIDTH;
int h=HEIGHT;

/********** TEMPORARY FUNCTIONS **********/

/**
   Remplir un tableau de toutes les coordonnées de l'ensemble des
   points du modèle
   @param m modèle
   @param vertexArray tableau de stockages des coordonnées
   @return aucun
*/
void
a2ri_vf_vertexArray(vf_model *m, double *vertexArray)
{
  int indice=0;
  for(int i=0; i<m->nbvertex; i++)
    {
      vertexArray[indice++] = m->ve[i].x;
      vertexArray[indice++] = m->ve[i].y;
      vertexArray[indice++] = m->ve[i].z;
    }
}

/**
   Remplir un tableau de tous les index de l'ensemble des points du modèle
   @param m modèle
   @param indexArray tableau de stockages des index
   @return aucun
*/
void
a2ri_vf_indexArray(vf_model *m, int *indexArray)
{
  int indice = 0;
  for(int i=0; i<m->nbface; i++)
    {
      indexArray[indice++] = m->fa[i].ve1;
      indexArray[indice++] = m->fa[i].ve2;
      indexArray[indice++] = m->fa[i].ve3;
	  
    }
}

/**
   Remplir un tableau de toutes les coordonnées des normales des points
   du modèle
   @param m modèle
   @param normalArray tableau de stockages des coordonnées des normales
   @return aucun
*/
void
a2ri_vf_VertexNormalArray(vf_model *m, double *normalArray)
{
  vector3d *normVertex = (vector3d*)malloc(sizeof(vector3d)*m->nbvertex);
  
  for(int i=0; i<m->nbvertex; i++)
    {
      vector3d_init(&normVertex[i],0.0,0.0,0.0);
    }

  for(int i=0; i<m->nbface; i++)
    {
      int ve1=m->fa[i].ve1;
      int ve2=m->fa[i].ve2;
      int ve3=m->fa[i].ve3;
      /*gestion de la normale*/
      vector3d AB,AC,norm;
      vector3d_init(&AB,
      		    m->ve[ve2].x-m->ve[ve1].x,
      		    m->ve[ve2].y-m->ve[ve1].y,
      		    m->ve[ve2].z-m->ve[ve1].z);
      vector3d_init(&AC,
      		    m->ve[ve3].x-m->ve[ve1].x,
      		    m->ve[ve3].y-m->ve[ve1].y,
      		    m->ve[ve3].z-m->ve[ve1].z);
      //normale de la face
      norm=vector3d_vectorialproduct(&AB,&AC);
            
      normVertex[ve1] = vector3d_add(&norm,&(normVertex[ve1]));
      normVertex[ve2] = vector3d_add(&norm,&(normVertex[ve2]));
      normVertex[ve3] = vector3d_add(&norm,&(normVertex[ve3]));
    }
  
  int indice = 0;
  for(int i=0; i<m->nbvertex; i++)
    {      
      vector3d_normalize(&normVertex[i]);
      normalArray[indice++] = normVertex[i].dx;
      normalArray[indice++] = normVertex[i].dy;
      normalArray[indice++] = normVertex[i].dz;
    }
  free(normVertex);
}

/**
   Remplir un tableau de toutes les coordonnées des normales des faces
   du modèle
   @param m modèle
   @param normalArray tableau de stockages des coordonnées des normales
   @return aucun
*/
void
a2ri_vf_FaceNormalArray(vf_model *m, double *normalArray)
{
  for(int i=0; i<m->nbface; i++)
    {
      vector3d norm;
      norm = a2ri_vf_normal_face(m, i);
      vector3d_normalize(&norm);
      normalArray[i] = norm.dx;
      normalArray[i+1] = norm.dy;
      normalArray[i+2] = norm.dz;
    }
}

vector3d
IF_a2ri_vf_gravity_face(vf_model* m, int numfa)
{
  vector3d AG; //AI;
  
  if(numfa>=m->nbface)
    {
      vector3d_init(&AG, 0,0,0);
      return AG;
    }
  int ve1,ve2, ve3;
  ve1 = m->fa[numfa].ve1;
  ve2 = m->fa[numfa].ve2;
  ve3 = m->fa[numfa].ve3;
  double xgravity = (m->ve[ve1].x + m->ve[ve2].x + m->ve[ve3].x)/3;
  double ygravity = (m->ve[ve1].y + m->ve[ve2].y + m->ve[ve3].y)/3;
  double zgravity = (m->ve[ve1].z + m->ve[ve2].z + m->ve[ve3].z)/3;
  vector3d_init(&AG, xgravity, ygravity, zgravity);
    
  return AG;
}

/**
   Calcul la normale de la face, la normalise, multiplie par un
   facteur, puis la décale au niveau du centre de gravité de celle
   ci. 
   @param m le modèle
   @param numfa le numéro de la face
   @param factor le facteur pour "agrandir/rétrécir" la normale
   @param norm le vector3d de la normale de la face
   @param gravity le vector3d du centre de gravité de la face
   @return aucun
*/
void
a2ri_vf_normal_vector_face(vf_model *m, int numfa, double factor,
				vector3d *norm, vector3d *gravity)
{
  *norm = a2ri_vf_normal_face(m,numfa);
  vector3d_normalize(norm);
  
  norm->dx*=factor;
  norm->dy*=factor;
  norm->dz*=factor;
  
  //calcul de centre de gravité de la face:
  *gravity = IF_a2ri_vf_gravity_face(m,numfa);
  //déplacement de la normale de la face "au dessus du centre de gravité
  *norm = vector3d_add(norm, gravity);
}

/**
   Rempli un tableau pour chaque faces du modèle des coordonnées de la
   normale d'une face puis des
   coordonnées du centre de gravité de la même.
   @param m modèle
   @param normalVectorArray tableau de coordonnées des normales et des
   centres de gravité
   @param factorNormalArray tableau de facteurs associé à chaque face
   pour "agrandir/rétrécir" la normale
   @return aucun
*/
void
a2ri_vf_FaceNormalVectorArray(vf_model *m, double *normalVectorArray, double *factorNormalArray)
{
  int index = 0;
  vector3d norm, gravity;
	    
  if(factorNormalArray == NULL)
    {
      for(int i=0; i<m->nbface; i++)
	{
	  //calcule de la normale et du centre de gravité
	  a2ri_vf_normal_vector_face(m,i,1.0,&norm,&gravity);
	  //stockage des coordonnées de la normale de la face puis des
	  //coordonnées du centre de gravité de la face
	  normalVectorArray[index++] = norm.dx;
	  normalVectorArray[index++] = norm.dy;
	  normalVectorArray[index++] = norm.dz;
	  normalVectorArray[index++] = gravity.dx;
	  normalVectorArray[index++] = gravity.dy;
	  normalVectorArray[index++] = gravity.dz;
	}
    }
  else
    {
      for(int i=0; i<m->nbface; i++)
	{
	  //calcule de la normale et du centre de gravité
	  a2ri_vf_normal_vector_face(m,i,factorNormalArray[i],&norm,&gravity);
	  //stockage des coordonnées de la normale de la face puis des
	  //coordonnées du centre de gravité de la face
	  normalVectorArray[index++] = norm.dx;
	  normalVectorArray[index++] = norm.dy;
	  normalVectorArray[index++] = norm.dz;
	  normalVectorArray[index++] = gravity.dx;
	  normalVectorArray[index++] = gravity.dy;
	  normalVectorArray[index++] = gravity.dz;
	}
    }
}


/********** MAIN FUNCTIONS **********/


GLWidget::GLWidget(QWidget *parent, const QColor& cBg, const QColor& cOb, const QColor& cLine, const QColor& cPoint, const QColor& cNorm)
  : QGLWidget(parent)
{
  /* permet de forcer que la locale C++ soit la même qu'en C*/
  setlocale(LC_NUMERIC, "C");

  xRot = 0;
  yRot = 0;
  zRot = 0;
  xTrans = 0;
  yTrans = 0;
  zTrans = 1;
  xmin = ymin = zmin = 0;
  xmax = ymax = zmax = 0;
  diag2d = diag3d = 0;
  screenratio = (WIDTH*1.0)/(HEIGHT*1.0);
  modelratio = 0;
  modelheight = modelwidth = 0;
  triangle_orientation=GL_CCW;
  xgravity = ygravity = zgravity = 0;

  open = false;
  pSize=1.0;
  lSize=0.5;
  colorBG=cBg;
  colorOb=cOb;
  colorP=cPoint;
  colorL=cLine;
  colorNorm=cNorm;

  isDisplayFaceNormal = true;
  tabFactorNormal = NULL;

  nameInfos = NULL;
  nbFaceInfos = NULL;
  nbVertexInfos = NULL;
  
  //Pour activer les évenements clavier
  setFocusPolicy(Qt::StrongFocus);
}

GLWidget::~GLWidget()
{
  //makeCurrent();
  glDeleteLists(object, 1);
  delete[] tabFactorNormal;
  delete[] nameInfos;
  delete[] nbFaceInfos;
  delete[] nbVertexInfos;
}

QSize
GLWidget::minimumSizeHint() const
{
  return QSize(WIDTH_MIN, HEIGHT_MIN);
}

QSize
GLWidget::sizeHint() const
{
  return QSize(WIDTH, HEIGHT);
}

void
GLWidget::initializeGL()
{
  glClearColor(0.0,0.0,0.0,0.0);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  initLight();
}

void
GLWidget::paintGL()
{
  clock_t t1, t2;
  t1 = clock();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  qglClearColor(colorBG);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if(!bcullface)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);
  
  if(open)
    {
      setScene();
      display_triangle();
      //   makeCurrent();

      t2 = clock();
      static double time_prec = (((double)t2-(double)t1)/CLOCKS_PER_SEC);
      static int nbframe= 1;
      static double  curr = 1./time_prec, min = curr, max = curr;
      if(nbframe != 1)
	{
	  time_prec += (((double)t2-(double)t1)/CLOCKS_PER_SEC);
	  curr =  1./(((double)t2-(double)t1)/CLOCKS_PER_SEC);
	}      

      if(curr<min)
	min = curr;

      if(curr>max)
	max = curr;

      if(bFPS)
	cout << "FPS:\nmin: " << min << "\tmax: " << max << "\ncurr: " << curr << "\tavg: " << nbframe/(time_prec) << endl;
      nbframe++;
    }

}

//Change le mode d'affichage de l'objet
void
GLWidget::polyChangeMode(GLenum polyMode){
  glPolygonMode(GL_FRONT_AND_BACK, polyMode);
}

void
GLWidget::resizeGL(int width, int height)
{
  w=width;
  h=height;
  screenratio = (w*1.0)/(h*1.0);
  
  glViewport(0,0,(GLsizei)width,(GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  setProjection();
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  setCamera();
  
  updateGL();
}

void
GLWidget::mousePressEvent(QMouseEvent *event)
{
  lastPos = event->pos();
}

void
GLWidget::mouseMoveEvent(QMouseEvent *event)
{
  float dx = event->x() - lastPos.x();
  float dy = event->y() - lastPos.y();

  if (event->buttons() & Qt::LeftButton) 
    {
      if(event->modifiers() == Qt::ShiftModifier)
	zRot -= dx * 0.5;
      else
	{
	  xRot += dx * 0.5;
	  yRot += dy * 0.5;
	}
    }
  else if (event->buttons() & Qt::RightButton)
    {
      xTrans += dx*0.002*diag2d/zTrans;
      yTrans -= dy*0.002*diag2d/zTrans;
    }
  lastPos = event->pos();
  updateGL();
}

void
GLWidget::wheelEvent(QWheelEvent *event)
{
  if(event->delta()>0)
    zTrans *= 1.1;
  else
    zTrans /= 1.1;
  updateGL();
}

void
GLWidget::keyPressEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Down)
    yTrans -= 1.1*0.002*diag2d/zTrans;
  else if(event->key() == Qt::Key_Up)
    yTrans += 1.1*0.002*diag2d/zTrans;
  else if(event->key() == Qt::Key_Left)
    xTrans -= 1.1*0.002*diag2d/zTrans;
  else if(event->key() == Qt::Key_Right)
    xTrans += 1.1*0.002*diag2d/zTrans;
  else if((event->key() == Qt::Key_R) ||
	  (event->key() == Qt::Key_Space))
    {
      xTrans=yTrans=xRot=yRot=zRot=0;
      zTrans=1;
    }
  else if((event->key() == Qt::Key_A))
    bFPS = !bFPS;
      
  updateGL();
}

void
GLWidget::setScene()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  setProjection();
  
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  setCamera();
}
 
void
GLWidget::setProjection()
{
  GLdouble  left, right, bottom, top, zNear, zFar;
  left=right=bottom=top=zNear=zFar = 0;
    
  //boite de projection
  if(screenratio>modelratio)
    {
      left = xmin-(((screenratio*modelheight)-modelwidth)/2.0)-(diag2d/20.0);
      right = xmax+(((screenratio*modelheight)-modelwidth)/2.0)+(diag2d/20.0);
      bottom = ymin-(diag2d/20.0);
      top = ymax+(diag2d/20.0);
      zNear = zmin-diag3d;
      zFar = zmax+diag3d;
    }
  else
    {
      left = xmin-(diag2d/20.0);
      right = xmax+(diag2d/20.0);
      bottom = ymin-(((modelwidth/screenratio)-modelheight)/2.0)-(diag2d/20.0);
      top = ymax+(((modelwidth/screenratio)-modelheight)/2.0)+(diag2d/20.0);
      zNear = zmin-diag3d;
      zFar = zmax+diag3d;
    }
  
  if(zNear<0 && zFar<0)
    {
      zNear*=-1;
      zFar*=-1;
    }
  
  if(zNear>=zFar)
    {
      GLdouble tmp = zNear;
      zNear = zFar;
      zFar = tmp;
    }
  
  glOrtho(left, right, bottom, top, zNear, zFar);
}

void
GLWidget::setCamera()
{
  //Zoom 3D
  glTranslatef(xgravity,
	       ygravity,
	       zgravity);
  glScalef(zTrans,zTrans,1.0);
  glTranslatef(-xgravity,
	       -ygravity,
	       -zgravity); 
  
  //Translation Caméra
  glTranslatef(xgravity,
	       ygravity,
	       zgravity);
  glTranslatef(xTrans,yTrans,0.0);
  glTranslatef(-xgravity,
	       -ygravity,
	       -zgravity);
  
  //Rotation Caméra
  glTranslatef(xgravity,
	       ygravity,
	       zgravity);
  glRotatef(xRot, 0.0, 1.0, 0.0);
  glRotatef(yRot, 1.0, 0.0, 0.0);
  glRotatef(zRot, 0.0, 0.0, 1.0);
  glTranslatef(-xgravity,
	       -ygravity,
	       -zgravity);
}
  
void
GLWidget::gravity()
{
  xgravity = 0.0;
  ygravity = 0.0;
  zgravity = 0.0;

  for(int i=0; i<model.nbvertex; i++)
    {
      xgravity += model.ve[i].x;
      ygravity += model.ve[i].y;
      zgravity += model.ve[i].z;
    }
  if(model.nbvertex != 0)
    {
      xgravity /= model.nbvertex;
      ygravity /= model.nbvertex;
      zgravity /= model.nbvertex;
    } 
}

void
GLWidget::updateInfosModel()
{
  if(open)
    {
      if(nameInfos != NULL)
        delete[] nameInfos;
      if(nbFaceInfos != NULL)
        delete[] nbFaceInfos;
      if(nbVertexInfos != NULL)
        delete[] nbVertexInfos;

      QString name = "Model Name: " + modelName;
      nameInfos = new char[name.size()+1];
      strcpy(nameInfos, name.toStdString().data());

      QString face = "Nb Faces: " + QString::number(model.nbface);
      nbFaceInfos = new char[face.size()+1];
      strcpy(nbFaceInfos, face.toStdString().data());

      QString vertex = "Nb Vertex: " + QString::number(model.nbvertex);
      nbVertexInfos = new char[vertex.size()+1];
      strcpy(nbVertexInfos, vertex.toStdString().data());
    }
}

void
GLWidget::normalizeAngle(int *angle)
{
  while (*angle < 0)
    *angle += 360 * 16;
  while (*angle > 360 * 16)
    *angle -= 360 * 16;
}

void
GLWidget::display_triangle()
{
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glPointSize(pSize);
  glLineWidth(lSize);

  bool isLight = glIsEnabled(GL_LIGHTING);
  //TODO mettre en vbo l'affichage des normales
  if(isDisplayFaceNormal)
    {
      if(isLight)
	glDisable(GL_LIGHTING);
      
      qglColor(colorNorm);
      glBegin(GL_LINES);
      for(int i=0; i<model.nbface*6; i+=6)
  	{
  	  int ve1 = i;
  	  int ve2 = i+1;
  	  int ve3 = i+2;
  	  int ve4 = i+3;
  	  int ve5 = i+4;
  	  int ve6 = i+5;
	  
  	  glVertex3d(tabFaceNormal[ve1], tabFaceNormal[ve2], tabFaceNormal[ve3]);
  	  glVertex3d(tabFaceNormal[ve4], tabFaceNormal[ve5], tabFaceNormal[ve6]);
    	}
      glEnd();
      if(isLight)
	glEnable(GL_LIGHTING);
    }
    
  /* on passe en mode VBO */
  glBindBuffer(GL_ARRAY_BUFFER, buf_VertexNorm);
  glNormalPointer(GL_DOUBLE, 0, NULL);
    
  glBindBuffer(GL_ARRAY_BUFFER, buf_pos);
  glVertexPointer(3, GL_DOUBLE, 0, NULL);

  /* activation des tableaux de sommets */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_index);
  /* affichage des faces */
  if(fillmode)
    {
      glEnable(GL_POLYGON_OFFSET_FILL);
      glPolygonOffset(1.0, 1.0);
      qglColor(colorOb);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glDrawElements(GL_TRIANGLES , model.nbface*3, GL_UNSIGNED_INT, NULL);
      glDisable(GL_POLYGON_OFFSET_FILL);
    }
  /* affichage des arêtes */
  if(linemode)
    {
      glDisable(GL_LIGHTING);
      glEnable(GL_POLYGON_OFFSET_LINE);
      glPolygonOffset(0.5, 1.0);
      qglColor(colorL);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawElements(GL_TRIANGLES , model.nbface*3, GL_UNSIGNED_INT, NULL);
      if(isLight)
        glEnable(GL_LIGHTING);
      glDisable(GL_POLYGON_OFFSET_LINE);
    }
  /* affichage des sommets */
  if(pointmode)
    {
      glDisable(GL_LIGHTING);
      qglColor(colorP);
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      glDrawElements(GL_TRIANGLES , model.nbface*3, GL_UNSIGNED_INT, NULL);
      if(isLight)
        glEnable(GL_LIGHTING);
    }
  
  /* desactivation des tableaux de sommet */
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

  glFlush();
}

//Initialise la lumière de la scène
void
GLWidget::initLight()
{
  GLfloat lightDiff[] = {0.2,0.2,0.2,1.0};
  GLfloat lightAmb[] = {0.,0.,0.,1.0};
  GLfloat lightSpec[] = {0.3,0.3,0.3,1.0};

  glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiff);
  glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmb);
  glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpec);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  GLfloat mat_spec[] = {0.9,0.9,0.9,1.0};
  GLfloat mat_emi[] = {0.0,0.0,0.0,1.0};
  GLfloat shininess = 20.;

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emi);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

int
GLWidget::openModel(const QString& filename)
{
  if(open){
    closeModel();
  }
  a2ri_vf_init(&model);
  char * name = new char[filename.size()+1];
  name[0]=0;
  strcpy(name, filename.toStdString().data());
  open = a2ri_vf_open_file(name, &model);
  delete[] name;

  if(!open)
    {
      cerr << "Error open: " << name << endl;
      return open;
    }

  modelName = filename;

  updateInfosModel();
  
  a2ri_vf_center(&model);
  //Remplissage d'un tableau des 3 coordonnées des points
  tabVertex = new double[model.nbvertex*3];
  a2ri_vf_vertexArray(&model, tabVertex);
  //Remplissage d'un tableau des index des 3 points pour les face
  tabIndex = new int[model.nbface*3];
  a2ri_vf_indexArray(&model, tabIndex);
  //Remplissage d'un tableau des 3 coordonnées des normales au point
  tabVertexNormal = new double[model.nbvertex*3];
  a2ri_vf_VertexNormalArray(&model, tabVertexNormal);

  //Initialisation utile pour l'aspect ratio
  xmin = model.xmin; ymin = model.ymin; zmin = model.zmin;
  xmax = model.xmax; ymax = model.ymax; zmax = model.zmax;
  modelheight = ymax - ymin;
  modelwidth = xmax - xmin;
  diag2d = sqrt(pow(xmax-xmin,2)+pow(ymax-ymin,2));
  diag3d = sqrt(pow(xmax-xmin,2)+pow(ymax-ymin,2)+pow(zmax-zmin,2));
  modelratio = (xmax - xmin)/(ymax - ymin);
  //Sert à calculer le centre de graviter du model pour les
  //translation, rotation et zoom du modele
  gravity();

  //Remplissage d'un tableau des 3 coordonnées des normales des faces
  //et des 3 coordonnées des centres de gravité des faces
  tabFaceNormal = new double[model.nbface*6];
  a2ri_vf_FaceNormalVectorArray(&model, tabFaceNormal, tabFactorNormal);


  /* initialisation de glew */
  glewInit();
  /** creation du VBO **/
  /* buffer des vertex */
  glGenBuffers(1, &buf_pos);
  glBindBuffer(GL_ARRAY_BUFFER, buf_pos);
  glBufferData(GL_ARRAY_BUFFER, model.nbvertex*3*sizeof(double), tabVertex, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  /* buffer des normales */
  glGenBuffers(1, &buf_VertexNorm);
  glBindBuffer(GL_ARRAY_BUFFER, buf_VertexNorm);
  glBufferData(GL_ARRAY_BUFFER, model.nbvertex*3*sizeof(double), tabVertexNormal, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  /** construction du IBO **/
  glGenBuffers(1, &buf_index);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_index);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.nbface*3*sizeof(int), tabIndex, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  /** fin **/
  delete[] tabVertex;
  delete[] tabIndex;
  delete[] tabVertexNormal;
  tabVertex=NULL;
  tabIndex=NULL;
  tabVertexNormal=NULL;

  //cout << "Temps d'ouverture: " << ((float)t2 - (float)t1)/CLOCKS_PER_SEC << endl;

  return open;
}

void
GLWidget::closeModel()
{
  if(open)
    {
      open = false;
      displayInfos();
      glDeleteBuffers(1, &buf_index);
      glDeleteBuffers(1, &buf_pos);
      glDeleteBuffers(1, &buf_VertexNorm);
      glDeleteBuffers(1, &buf_FaceNorm);

      a2ri_vf_free(&model);

      /* Réinitilisation de la scène */
      xRot = 0;
      yRot = 0;
      zRot = 0;
      xTrans = 0;
      yTrans = 0;
      zTrans = 1;
      //Tableau des facteurs des normales à 1.0
      changeSizeNormLine(1.0);
    }
}


void
GLWidget::glLight()
{
  if(glIsEnabled(GL_LIGHTING))
    glDisable(GL_LIGHTING);
  else
    glEnable(GL_LIGHTING);
}

void
GLWidget::cullFace()
{
  bcullface = ! bcullface;
}

const QString&
GLWidget::displayInfos()
{
  QString *ret = new QString(nameInfos);
  ret->append("\n");
  ret->append(nbVertexInfos);
  ret->append("\n");
  ret->append(nbFaceInfos);

  return *ret;
}

void
GLWidget::changeSizePoint(double size)
{
  pSize=size;
  updateGL();
}

void
GLWidget::changeSizeLine(double size)
{
  lSize=size;
  updateGL();
}

void
GLWidget::changeBgColor(QColor color)
{
  colorBG=color;
  qglClearColor(colorBG);
  updateGL();
}

void
GLWidget::changeObColor(QColor color)
{
  colorOb=color;
  updateGL();
}

void
GLWidget::changeLineColor(QColor color)
{
  colorL=color;
  updateGL();
}

void
GLWidget::changePointColor(QColor color)
{
  colorP=color;
  updateGL();
}

void
GLWidget::setDisplayFaceNormal(bool a)
{
  if(isDisplayFaceNormal != a)
    {
      isDisplayFaceNormal = a;
      updateGL();
    }
}

void
GLWidget::changeNormColor(QColor color)
{
  colorNorm = color;
  updateGL();
}
void
GLWidget::changeSizeNormLine(double size)
{
  if(tabFactorNormal != NULL)
    {
      delete[] tabFactorNormal;
      tabFactorNormal=NULL;
    }
 
  if(open)
    {
      tabFactorNormal = new double[model.nbface];
      
      for(int i=0; i<model.nbface; i++)
	tabFactorNormal[i] = size;
      
      if(tabFaceNormal != NULL)
	{
	  delete[] tabFaceNormal;
	}

      tabFaceNormal = new double[model.nbface*6];
      
      a2ri_vf_FaceNormalVectorArray(&model, tabFaceNormal, tabFactorNormal);
    }
  updateGL();
}

void 
GLWidget::changeTriangleOrientation(GLenum mode)
{
  //si le mode change
  if(mode!=triangle_orientation)
    {
      //allocation d'un nouveau tableau pour les normales TODO ne pas
      //refaire d'allocation a chaque fois mais une seule a
      //l'ouverture du programme puis une desallocation a la fermeture
      tabVertexNormal = new double[model.nbvertex*3];
      a2ri_vf_VertexNormalArray(&model, tabVertexNormal);

      triangle_orientation=mode;
      glFrontFace(triangle_orientation);

      //inversion des normales
      if(mode==GL_CW)
	for(int i=0;i<model.nbvertex*3;i++)
	  tabVertexNormal[i]*=-1.0;

      //linkage du nouveau tableau des normales
      glBindBuffer(GL_ARRAY_BUFFER, buf_VertexNorm);
      glBufferData(GL_ARRAY_BUFFER, model.nbvertex*3*sizeof(double), tabVertexNormal, GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      delete[] tabVertexNormal;
      tabVertexNormal=NULL;
    }
}

void
GLWidget::saveScreenshot(const QString& text, int resolution)
{
  QImage im[resolution][resolution];
  for(int i=0;i<resolution;i++)
    for(int j=0;j<resolution;j++)
      {
	glViewport(-w*i, -h*j, w*resolution, h*resolution);
	updateGL();
	im[i][j] = grabFrameBuffer();
      }
  
  QImage tosave(w*resolution,h*resolution,im[0][0].format());
  
  for(int i=0;i<w*resolution;i++)
    for(int j=0;j<h*resolution;j++)
      tosave.setPixel(i, j, im[i/w][resolution-j/h-1].pixel(i-((i/w)*w),j-((j/h)*h)));
  
  
  tosave.save(text,"PNG");
  
  glViewport(0, 0, w, h);
  updateGL();
}

void GLWidget::displayFill()
{
  fillmode = !fillmode;
}

void GLWidget::displayLine()
{
  linemode = !linemode;
}

void GLWidget::displayPoint()
{
  pointmode = !pointmode;
}

void
GLWidget::saveModel(QString filename)
{
  char * name = new char[filename.size()+1];
  strcpy(name, filename.toStdString().data());
  a2ri_vf_save_file(name, &model);
  delete[] name;
}

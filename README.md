# a2riView

## Configuration minimale requise

- Bibliothèque a2ri (version 0.2 minimum) installée : voir le site [liba2ri](http://liba2ri.free.fr/?page_id=59)
- Bibliothèque QT (version 6 minimum) installée
- Bibliothèque GSL, GLUT et GLEW.

### Installation sous Debian/Ubuntu

```sh
# Install dependencies
sudo apt-get install build-essential libgsl0-dev freeglut3-dev libglew-dev freeglut3-dev qt6-base-dev && \
# Download a2ri
curl -L http://liba2ri.free.fr/?file=a2ri-0.2.zip -o a2ri-0.2.zip && \
# Unzip a2ri
unzip a2ri-0.2.zip -d /tmp/a2ri && \
# Move a2ri to the current directory
mv /tmp/a2ri/a2ri-0.2 ./a2ri && \
# Install a2ri
cd a2ri && \
chmod +x install.sh && \
sudo ./install.sh && \
# Install a2riView
cd .. && \
chmod +x install && \
sudo ./install
```

### Installation sous Fedora

```sh
# Install dependencies
sudo dnf install @development-tools gsl-devel freeglut-devel glew-devel qt6-qtbase-devel && \
# Download a2ri
curl -L http://liba2ri.free.fr/?file=a2ri-0.2.zip -o a2ri-0.2.zip && \
# Unzip a2ri
unzip a2ri-0.2.zip -d /tmp/a2ri && \
# Move a2ri to the current directory
mv /tmp/a2ri/a2ri-0.2 ./a2ri && \
# Install a2ri
cd a2ri && \
chmod +x install.sh && \
sudo ./install.sh && \
# Install a2riView
cd .. && \
chmod +x install && \
sudo ./install
```

Si vous rencontrez l'erreur suivante lors de l'installation de a2ri:

```sh
a2riView: error while loading shared libraries: liba2ri.so: cannot open shared object file: No such file or directory
```

Vous pouvez résoudre ce problème en ajoutant le chemin du dossier `lib` de a2ri dans la variable d'environnement `LD_LIBRARY_PATH`:

```sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/ && \
a2riView
```

## Fonctionnalités de a2riView

### Lecture Ecriture

- Ouverture d'un fichier: Ctrl+O, File -> Open ou en ligne de commande: `./PFE <nom_modèle>`;
- Les formats de modèles 3D pris en compte à l'heure actuelle sont: off, obj, gts, stl, pgn, wrl, ply, vef.
- Sauvegarde du modèle: Ctrl+S ou File -> Save;
- Fermer le modèle: Ctrl+C ou File -> Close;
- Quitter a2riView: Ctrl+Q ou Alt+F4 ou File -> Quit;
- Conversion d'un modèle en un autre modèle de type différent: `./PFE <modèle_base> <modèle_converti>`

### Affichage

- Plein écran: F11 ou Display -> Full Screen;
- Modèle Face pleine: F ou Display -> Fill;
- Modèle fil de fer: V ou Display -> Line;
- Modèle point: P ou Display -> Point;
- Normale des Faces: N ou Display -> Normal face;
- Back Face: B ou Display -> Back Face;
- Lumière: L ou Display -> Light;
- Informations sur le modèle: I ou Display -> Informations;

### Autre

- Capture d'écran: F2 ou Utils -> ScreenShot;
- Modification de la taille des points, lignes, normales: Utils -> XX Size; //où XX est Point, Line ou Normal line.
- Modification de la couleur des points, lignes, normales, de l'arrière-plan, du modèle: Utils -> XX Size; //où XX est Point, Line, Normal line, Background ou Object.

### Déplacement du modèle

- Clic gauche + déplacement souris: rotation X et Y du modèle
- Clic gauche + Shift + déplacement souris: rotation Z du modèle
- Molette: Zoom du modèle
- Clic droit + déplacement souris: translation X et Y du modèle
- Flèche (Haut, Bas, Droite, Gauche): translation du modèle
- R ou Espace: réinitialisation de la caméra

### Exemples d'utilisation

- Lancer simplement a2riView

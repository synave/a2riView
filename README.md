Configuration minimale requise :
- Bibliothèque a2ri (version 0.2 minimum) installée : voir le site: http://liba2ri.free.fr/?page_id=59
- Bibliothèque QT (version 4.5 minimum) installée
- Bibliothèque GSL, GLUT et GLEW.

Ensuite faire :
./install

Si vous lancez cette commande sans les droits de super utilisateur, le logiciel sera compilé mais pas installé.


-----------------------------------------------------------------


Fonctionnalité de a2riView:

Lecture Ecriture:
Ouverture d'un fichier: Ctrl+O, File -> Open ou en ligne de commande: ./PFE <nom_modèle>;
Les formats de modèles 3D pris en compte à l'heure actuel sont: off, obj, gts, stl, pgn, wrl, ply, vef.
Sauvegarde du modèle: Ctrl+S ou File -> Save;
Fermer le modèle: Ctrl+C ou File -> Close;
Quitter a2riView: Ctrl+Q ou Alt+F4 ou File -> Quit;
Conversion d'un modèle en un autre modèle de type différent: ./PFE <modèle_base> <modèle_converti>

Affichage:
Plein écran: F11 ou Display -> Full Screen;
Modèle Face pleine: F ou Display -> Fill;
Modèle fil de fer: V ou Display -> Line;
Modèle point: P ou Display -> Point;
Normale des Faces: N ou Display -> Normal face;
Back Face: B ou Display -> Back Face;
Lumière: L ou Display -> Light;
Informations sur le modèle: I ou Display -> Informations;

Autre:
Capture d'écran: F2 ou Utils -> ScreenShot;
Modification de la taille des points, lignes, normales: Utils -> XX Size; //où XX est Point, Line ou Normal line.
Modification de la couleur des points, lignes, normales, de l'arrière plan, du modèle: Utils -> XX Size; //où XX est Point, Line, Normal line, Background ou Object.

Déplacement du modèle:
Clic gauche + déplacement souris: rotation X et Y du modèle
Clic gauche + Shift + déplacement souris: rotation Z du modèle
Molette: Zoom du modèle
Clic droit + déplacement souris: translation X et Y du modèle
Flèche (Haut, Bas, Droite, Gauche): translation du modèle
R ou Espace: réinitialisation de la caméra

Exemples d'utilisation:
Lancer simplement a2riView
./a2riView
Ouvrir un modèle en lançant a2riView
./a2riView <modèle>
Convertir un modèle dans un autre format de fichier sans lancer la fenêtre a2riView
./a2riView <modèle_a_convertir> <modèle_de_sortie>

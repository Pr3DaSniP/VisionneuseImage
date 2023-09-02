# Visionneuse d'images

![Language](https://img.shields.io/badge/Language-C++-blue.svg)
![Librairies](https://img.shields.io/badge/Librairies-OpenCV-green.svg)
![IDE](https://img.shields.io/badge/IDE-Visual%20Studio%202022-red.svg)

Ce programme permet de visionner des images en les sélectionnant à l'aide d'un curseur de couleur verte. 

Source :https://youtu.be/aMbPyFoGxdU?si=1_ARnsXUk_bmgxmr&t=2318

## Installation

###### Il faut une webcam pour jouer.

Dans les paramètres du projet :

C/C++ > Général > Autres répertoires include : Ajouter le chemin vers le dossier include de la bibliothèque OpenCV

Éditeur de liens > Général > Répertoires de bibliothèques supplémentaires : Ajouter le chemin vers le dossier lib de la bibliothèque OpenCV

Éditeur de liens > Entrée > Dépendances supplémentaires : Ajouter les librairies suivantes :

opencv_world460d.lib
opencv_world460.lib

## Démonstration

Les images sont dans le dossier images. Priviliégier les images de bonne qualité car on les redimensionne pour les afficher.

![Démonstration](demo.gif)
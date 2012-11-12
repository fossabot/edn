# action a faire (ordonner par révision) :

dans la liste des truc important a refaire
	finir de netoyer la classe widget
	netoyer les classe text ...
	mettre en place le repositionnement automatique quand on change de buffer
	Curseur au bon endroit
	
TODO List :
	- sys : Remove the Open GL ES 1 support of all the system ==> can not be compatible ...
	- normalisation des dossier et des themes
	- recherche automatique des laguages accessibles
	- Recherche automatique des Couleurs
	- Mise en place d'une interface jolie pour les paramètre
	- rendre dynamique le système des menus
	- Demander la création des nouveaux fichier ( create a la place de open dans le le menu d'ouverture des fichiers)
	- Undo par section de mot et non par lettre taper ...
	- ne pas ouvrir les donner en text...
	- Tree view
	- mettre un système de pluggin claire pour cree des nouvelles entréé tel que le lecteur hexa, images ... et aussi pour des pluggin de visualisation ...
	- insert mode correct
	- command line ...
	
	==> les widget sont intégralement a revoir ... ca prendra du temps ...
	==> changer le system de tag ==> mettre dans un fichier tag.txt au lieux de prendre celuis de git ==> trop long dans beaucoup de cas
	==> revoir le system de find ==> il est long et pose donc des problèmes surtout sous windows
	
	- sys : Mise en place des colorisation de base pour le
			* java script
			* SQL
	- BUG : de copier coller sur les éàè Ã¨ ...
	- BUG : les caractère multiples type chapot ...
	- enregistrement des propriaté de config utilisateur
	- gui : parameter gui : et en autre la taille de la police ... et voir pour la récupérer sur le system... et ce serait cool...
	- gui : Color list selection
	- gui : Hightlight gui management
	- sys : Charset UTF-8 et iso 8859-15 correcte
	- sys : Transformation de charset a la volée
	- sys : Charset par defaut
	- Sys : Détection de charset (à mettre dans les todo de charset)
				==> regarder les librairies open sources
	- sys : replace ALL
	- sys : Undo / Redo ajout de caractères autre que " ", "\t" et "\n"
	- sys : right management ...
	- HL  : Normalyse all the system of regular expression with <> chapot ...
	- HL  : sub parsing of the reg exp
	- sys : update all language
	- sys : Select in colones
	- sys : Select all the same search in the current buffer ... ( HL en vert ou jaune ...)
	- sys : Multi-file search ==> display it in a result buffer (clickable only)
	- int : Redo all the doxygen ...
	- gui : open file in binary
	- sys : compare binary files
	- sys : compare normal files
	- gui : Display content of a folder
	- gui : add a treeview display
	- sys : evenement losqu'un fichier est modifier a l'exterieur...
	- sys : image du type de fichier dans le buffer list view
	- PRJ : treeview des dossier a ouvrir
	- PRJ : GUI de management
	- PRJ : Save all in project
	- SYS : Save all modify files
	- sys : version system management (git, cvs, ...)
	- SYS : Macro generation and acquisition
	- GDB : Acces interne pour un debuggeur ==> comme cgdb ... avec l'acces assembleur en plus

# note utiles : 
http://majutsushi.github.com/tagbar/		==> je devrais regarder ca pour savoir ce que je peux faire avec du ctags
CTags : Set the parsing methode : "ctags -R --fields=+n jni ../ewol/Sources/"

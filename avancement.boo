# action a faire (ordonner par révision) :
* 0.2.X :
	- gui : Amelioration du full-screen et du display de base (sans l'entete de la fenetre)
	- gui : display partielle de la fenetre
	- gui : ascenceur quand nécessaire
	- gui : Demander la création de nouveaux fichier quand il n'existe pas (a l'ouverture en ligne de commande)
	- sys : Mise en place des colorisation de base pour le
			* java script
			* SQL
	- sys : replace TAB with space when Tab is pressed
	- sys : Catch F[1-12] ==> for user personal event
	- BUG : Correction du bug des entré bizard tel que les chapot et les guillemets
	- BUG : de sélection quand la ligne est pleine et la première ligne séctionnée. ==> regarder après avoir fait le display ligne par ligne...
	- BUG : de copier coller sur les éàè ...
	- BUG : les caractère multiples type chapot ...
	- BUG : italique non généré
* 0.3.X :
	- SEARCH : get selected text in the search windows
	- SEARCH : Select the search windows when call crtl+F
	- sys : personal property file in the ~/.edn ou ~/.gnome/edn
	- gui : Ordonner les fichier ouvert par nom ...
	- gui : parameter gui : et en autre la taille de la police ... et voir pour la récupérer sur le system... et ce serait cool...
	- gui : Color list selection
	- gui : Hightlight gui management
* 0.4.X :
	- sys : Charset UTF-8 et iso 8859-15 correcte
	- sys : Transformation de charset a la volée
	- sys : Charset par defaut
	- Sys : Détection de charset (à mettre dans les todo de charset)
				==> regarder les librairies open sources
* 0.5.X :
	- prj : list of current files open
	- prj : open ctags file
	- sys : replace ALL
	- sys : Undo / Redo ajout de caractères autre que " ", "\t" et "\n"
* 0.6.X :
	- sys : right management ...
	- HL  : Normalyse all the system of regular expression with <> chapot ...
	- HL  : sub parsing of the reg exp
	- sys : update all language
* 0.7.X :
- sys : Select in colones
	- sys : Select all the same search in the current buffer ... ( HL en vert ou jaune ...)
	- sys : Multi-file search ==> display it in a result buffer (clickable only)
* 0.9.X :
	- int : Redo all the doxygen ...
* 1.0.X :
	- gui : open file in binary
	- sys : compare binary files
	- sys : compare normal files
* 1.1.X :
	- gui : Display content of a folder
	- gui : add a treeview display
	- sys : evenement losqu'un fichier est modifier a l'exterieur...
	- sys : image du type de fichier dans le buffer list view
* 1.2.X :
	- PRJ : treeview des dossier a ouvrir
	- PRJ : GUI de management
	- PRJ : Save all in project
	- SYS : Save all modify files
* 1.3.X :
	- sys : version system management (git, cvs, ...)
* 1.4.X :
	- SYS : Macro generation and acquisition
* 1.5.X :
	- GDB : Acces interne pour un debuggeur ==> comme cgdb ... avec l'acces assembleur en plus

# note utiles : 
http://majutsushi.github.com/tagbar/		==> je devrais regarder ca pour savoir ce que je peux faire avec du ctags
CTags : Set the parsing methode : "ctags -R --fields=+n Sources/ /usr/include/gtk-3.0/"

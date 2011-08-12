# For the first realease 1.0 :
	* Systèmes : 
		- Affichage ligne par ligne
		- Correction du bug de hl(sub patern empty)
		- Verifier si le hldisplay reaserch marche avec l'id et pas l'id-1
		- Gestion correct des charsets
	* Gui :
		- Confirmer avant de fermer
		- Demander de sauver
		- Controle des droits
		- Hl-gui management
		- Event quand un fichier ouvert a changer
		- Affichage des fichiers binaires
		- Ctags phase 1
		- Project manager phase 1

# action a faire (ordonner par révision) :
	* 0.1.X :
		- ctags : Back simple et multiple
		- ctags : Multiple files
		- sys : search complet, replace complet and replace ALL ...
		- gui : demander l'enregistrement avant de fermer (quand c'est nécessaire)
		- Faire les deplacement de EdnXXX dans le nameSpace Edn::XXX
		- Edn::VectorType : Rewrite the erase fuction to support start => stop and Erase len methode ...
	* 0.2.X :
		- gui : Demander la création de nouveaux fichier quand il n'existe pas (a l'ouverture en ligne de commande)
		- sys : Mise en place des colorisation de base pour le 
				* java script
				* SQL
		- gui : ascenceur quand nécessaire
		- Catch Shift+TAB
		- Correction du bug des entré bizard tel que les chapot et les guillemets
		- pb de sélection quand la ligne est pleine et la première ligne séctionnée.
		- PB de copier coller sur les éàè ...
	* 0.3.X :
		- Charset UTF-8 et iso 8859-15 correcte
		- Transformation de charset a la volée
		- Charset par defaut
		- Détection de charset (à mettre dans les todo de charset)
			==> regarder les librairies open sources
	* 0.4.X :
		- SEARCH : get selected text in the search windows
		- SEARCH : Select the search windows when call crtl+F
		- gui : ordonner les fichier ouvert par nom ...
	* 0.5.X :
		- project : list of current files open
		- gui : Parameters : et en autre la taille de la police ... et voir pour la récupérer sur le system... et ce serait cool...

# Text Editor

	-  90% Indent group with Tabulation and shift+tabulation
	- 100% Basic smart indent : Recopy the start of the previous line when return (copy '\t' and ' ')
	-   0% Replace Tab with space when press the key
	-   0% Request save user when quit the software
	-   0% Hide/Display scroll element when not usefull
	-  10% Regular expression search
	-  80% Colorisation syntaxique
	-  80% Copier / coller ==> probleme sur les caractères spéciaux comme ° é à ...
	-  10% Search 
	-  10% Replace
	-  80% Undo / Redo ajout de caractères autre que " ", "\t" et "\n"
	-   0% Selectionner en colone
	-   0% Supression multiple de ligne par la commande ctrl+D
	-   0% Multi-file search ==> display it in a result buffer (clickable only)
	-   0% basic UTF8 string management ==> bad with the curent buffer
	-   0% Replace in all elements of the project file or a part
	-   0% Multiple Text Editor in the same Time
	-   0% Support des Macros ==> In python ??? or other ...

# Ctags :
	- 100% Parse (use the ctags parser)
	- 100% Find
	-  50% Jump
	-  10% History of move (with display)

# Buffer Viewer :
	- 100% Right menu
	- 100% Display buffer in color
	- 100% Display buffer Saved / not Saved
	-   0% Image of the type of buffer (optionnal)

# Configuration :
	-   0% Creer une IHM pour les configurations de base
	-   0% Creer une IHM pour l'edition du hightliner syntaxique (quand il marchera...)
	-   0% Creer une IHM pour les couleurs (voir la lier avec l'IHM pour le hightliner)
	-   0% saugegarde automatique de la configuration ou sur demande...

# Project manager :
	-   0% Faire un editeur des dossiers du projet a ouvrir ==> automatiquement ajouter dans les Ctags
	-   0% Base
	-   0% Save All
	-   0% TreeView

# Tree View :
	-   0% View
	-   0% Open File
	-   0% Jump to the curent File
	-   0% Hide CVS / Git / SVN elements

# Diff :
	-   0% generate a Diff between files
	-   0% same in binary

# Git :
	-   0% Diff with the current wersion commited (local)
	-   0% Git branch local display (as gitk --all) ...
	-   0% view git chawan history ... (git Log)

# CVS :
	-   0% check the version with CVS
	-   0% commit
	-   0% display the Branch
	-   0% Select a new repository ...

# Publication : 
	-   0% faire une publication du logiciel sur le Web... (need a stable version)
	-   0% faire un package

# Repository : 
	-   0% create my own repository (@home)
	-   0% manage right with Git
	-   0% create a website fot it (@home)
	-   0% reserve edn.org

# plugin : (never i think ...)
	-   0% Comprendre comment faire un system avec des plugin (interne et simple)...
	-   0% En c++ compiler seulement, pas d'interface pyton ou autre c'est trop moche.


# note utiles : 
http://majutsushi.github.com/tagbar/		==> je devrais regarder ca pour savoir ce que je peux faire avec du ctags
CTags : Set the parsing methode : "ctags -R --fields=+n Sources/"

/* pour voir les appels system*/

# For the first realease :
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

# action a faire (ordonner) :
	- HL : Parsing caracter/caractère et plus section par section ...
	- sys : real filename ...
	- ctags : Back simple et multiple
	- ctags : Multiple files
	- sys : search complet, replace complet
	- gui : ordonner les fichier ouvert par nom ...
	- sys : Mise en place des colorisation de base pour le 
			* xml
			* makefiles
			* script bash
			* python
			* matlab
			* java script
			* SQL
	- project : list of current files open
	- gui : demander l'enregistrement avant de fermer (quand c'est nécessaire)
	- gui : demande de création de nouveaux fichier ou quiter l'editeur ...
	- gui : ascenceur quand nécessaire
	- gui : Demander la création de nouveaux fichier quand il n'existe pas (a l'ouverture en ligne de commande)
	- Catch Shift+TAB
	- Correction du bug des entré bizard tel que les chapot et les guillemets
	- gui : Parameters : et en autre la taille de la police ... et voir pour la récupérer sur le system... et ce serait cool...
	- PB de copier coller sur les éà ...
	- Charset UTF-8 et iso 8859-15 correcte
	- Transformation de charset a la volé
	- Charset par defaut
	- Déction de charset (à mettre dans les todo de charset)
	- Faire les deplacement de EdnXXX dans le nameSpace Edn::XXX
	- pb de sélection quand la ligne est pleine et la première ligne sélectionné...

# Text Editor

	- [1]  90% Indent group with Tabulation and shift+tabulation
	- [1] 100% Basic smart indent : Recopy the start of the previous line when return (copy '\t' and ' ')
	- [1]   0% Replace Tab with space when press the key
	- [1]   0% Request save user when quit the software
	- [1]   0% Hide/Display scroll element when not usefull
	- [1]  10% Regular expression search
	- [1]  80% Colorisation syntaxique
	- [1]  80% Copier / coller ==> probleme sur les caractères spéciaux comme ° é à ...
	- [1]  10% Search 
	- [1]  10% Replace
	- [1]  80% Undo / Redo ajout de caractères autre que " ", "\t" et "\n"
	- [2]   0% Selectionner en colone
	- [2]   0% Supression multiple de ligne par la commande ctrl+D
	- [3]   0% Multi-file search ==> display it in a result buffer (clickable only)
	- [3]   0% basic UTF8 string management ==> bad with the curent buffer
	- [5]   0% Replace in all elements of the project file or a part
	- [5]   0% Multiple Text Editor in the same Time
	- [5]   0% Support des Macros ==> In python ??? or other ...

# Ctags :
	- [1] 100% Parse (use the ctags parser)
	- [1]  50% Find
	- [1]  50% Jump
	- [1]  10% History of move (with display)

# Buffer Viewer :
	- [1]   0% Right menu
	- [1] 100% Display buffer in color
	- [1] 100% Display buffer Saved / not Saved
	- [2]   0% Image of the type of buffer (optionnal)

# Configuration :
	- [1]   0% Creer une IHM pour les configurations de base
	- [1]   0% Creer une IHM pour l'edition du hightliner syntaxique (quand il marchera...)
	- [1]   0% Creer une IHM pour les couleurs (voir la lier avec l'IHM pour le hightliner)
	- [1]   0% saugegarde automatique de la configuration ou sur demande...

# Project manager :
	- [2]   0% Faire un editeur des dossiers du projet a ouvrir ==> automatiquement ajouter dans les Ctags
	- [2]   0% Base
	- [2]   0% Save All
	- [2]   0% TreeView

# Tree View :
	- [2]   0% View
	- [2]   0% Open File
	- [2]   0% Jump to the curent File
	- [2]   0% Hide CVS / Git / SVN elements

# Diff :
	- [4]   0% generate a Diff between files
	- [4]   0% same in binary

# Git :
	- [4]   0% Diff with the current wersion commited (local)
	- [9]   0% Git branch local display (as gitk --all) ...
	- [4]   0% view git chawan history ... (git Log)

# CVS :
	- [6]   0% check the version with CVS
	- [6]   0% commit
	- [6]   0% display the Branch
	- [6]   0% Select a new repository ...

# Publication : 
	- [1]   0% faire une publication du logiciel sur le Web... (need a stable version)
	- [2]   0% faire un package

# Repository : 
	- [2]   0% create my own repository (@home)
	- [2]   0% manage right with Git
	- [2]   0% create a website fot it (@home)
	- [2]   0% reserve edn.org

# plugin : (never i think ...)
	- [ ]   0% Comprendre comment faire un system avec des plugin (interne et simple)...
	- [ ]   0% En c++ compiler seulement, pas d'interface pyton ou autre c'est trop moche.

# notes : 
	The [x] Represent the version expected of the feature


# Global TODO List:
	- [1]   0% EdnString ==> a revoir pour etre plus rapide et plus complète... et suporter le << et >>
	- [1]   0% Remove wxString from the current code...


# note utiles : 
http://majutsushi.github.com/tagbar/		==> je devrais regarder ca pour savoir ce que je peux faire avec du ctags


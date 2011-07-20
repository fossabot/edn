##################################################################################################################
#                                                                                                                #
#   Fichier     :   Makefile                                                                                     #
#                                                                                                                #
#   Type        :   Makefile d'un programme complet                                                              #
#                                                                                                                #
#   Auteur      :   Heero Yui                                                                                    #
#                                                                                                                #
#   Evolutions  :   Date          Auteur        Raison                                                           #
#                2010-01-29      Heero Yui      Mise en place d'un makefile ultra simple                         #
#                2011-07-14      Heero Yui      Rework the current dorder includion (simplification)             #
#                                                                                                                #
#   Notes       :   This makefile might be edited with an editor compatible with escape char and carrer return   #
#                   char                                                                                         #
#                                                                                                                #
# Concu Pour le projet edn                                                                                       #
#                                                                                                                #
##################################################################################################################
export F_GRAS=[1m
export F_INVERSER=[7m
export F_SOUSLIGNER=[4m
export F_NORMALE=[m
export F_NOIR=[31m
export F_ROUGE=[31m
export F_VERT=[32m
export F_MARRON=[33m
export F_BLUE=[34m
export F_VIOLET=[35m
export F_CYAN=[36m
export F_GRIS=[37m
export CADRE_HAUT_BAS='	$(F_INVERSER)                                                                    $(F_NORMALE)'
export CADRE_COTERS='	$(F_INVERSER)  $(F_NORMALE)								  $(F_INVERSER)  $(F_NORMALE)'


###############################################################################
### Compilateur base system                                                 ###
###############################################################################
CXX=$(BIN_PREFIX)g++
CC=$(BIN_PREFIX)gcc
AR=$(BIN_PREFIX)ar

###############################################################################
### Compilation Define                                                      ###
###############################################################################
DEFINE= -DEDN_DEBUG_LEVEL=3


GTKFLAGS= 
ifeq ($(shell if `pkg-config --exists gtk+-3.0` ; then echo "yes"; else echo "no"; fi), yes)
    GTKFLAGS= `pkg-config --cflags --libs gtk+-3.0` -DUSE_GTK_VERSION_3_0
else
    ifeq ($(shell if `pkg-config --exists gtk+-2.0` ; then echo "yes"; else echo "no"; fi), yes)
        GTKFLAGS= `pkg-config --cflags --libs gtk+-2.0` -DUSE_GTK_VERSION_2_0
    else
        $(error No GTK 3.0 or 2.0 librairies ...)
    endif
endif

###############################################################################
### Basic Cfags                                                             ###
###############################################################################

# basic GTK librairy
CXXFLAGS=  $(GTKFLAGS)
# Linux thread system
CXXFLAGS+= -lpthread
# Enable debug (cgdb edn)
CXXFLAGS+= -g -O0
#CXXFLAGS+= -O2
# display all flags
CXXFLAGS+= -Wall
# ...
CXXFLAGS+= -D_REENTRANT
# internal defines
CXXFLAGS+= $(DEFINE)

CFLAGS=    $(CXXFLAGS) -std=c99

# basic GTK librairy
LDFLAGS=  $(GTKFLAGS)
# Linux thread system
LDFLAGS+= -lpthread
# Dynamic connection of the CALLBACK of the GUI
LDFLAGS+= -Wl,--export-dynamic

###############################################################################
### Project Name                                                            ###
###############################################################################
OUTPUT_NAME=edn

###############################################################################
### Basic Project description Files                                         ###
###############################################################################
FILE_DIRECTORY=Sources
OBJECT_DIRECTORY=Object

###############################################################################
### Generique dependency                                                    ###
###############################################################################
MAKE_DEPENDENCE=Makefile

###############################################################################
### Files Listes                                                            ###
###############################################################################

# tiny XML (extern OPEN Sources) :
CXXFILES =		tools/tinyXML/tinyxml.cpp				\
				tools/tinyXML/tinyxmlparser.cpp			\
				tools/tinyXML/tinyxmlerror.cpp			\
				tools/tinyXML/tinystr.cpp

# ExuberantCtags reading file tools (extern OPEN Sources) :
CXXFILES+=		ctags/readtags.cpp                      \
				ctags/CTagsManager.cpp


# Globals debug tool:
CXXFILES+=		tools/debug/tools_debug.cpp				\
				tools/globals/tools_globals.cpp			\
				tools/memory/toolsMemory.cpp

# Buffers internal:
CXXFILES+=		tools/EdnTemplate/EdnVectorBuf.cpp		\
				tools/EdnBuf/EdnBuf.cpp					\
				tools/EdnBuf/EdnBuf_HighLight.cpp		\
				tools/EdnBuf/EdnBuf_History.cpp			\
				tools/EdnBuf/EdnBuf_Selection.cpp		\
				tools/EdnBuf/EdnBufHistory.cpp
				

# Tools internal:
CXXFILES+=		tools/AL/AL_Mutex.cpp					\
				tools/Display/Display.cpp				\
				tools/ClipBoard/ClipBoard.cpp			\
				tools/MsgBroadcast/MsgBroadcast.cpp		\
				tools/MsgBroadcast/AccelKey.cpp			\
				tools/charset/charsetTable.cpp			\
				tools/charset/charset.cpp				\
				tools/EdnRegExp/EdnRegExp.cpp			\
				tools/NameSpaceEdn/File.cpp				\
				tools/NameSpaceEdn/String.cpp

# Gui:
CXXFILES+=		CustumWidget/BufferView/BufferView.cpp		\
				CustumWidget/CodeView/CodeView.cpp			\
				GuiTools/WindowsManager/WindowsManager.cpp	\
				GuiTools/MainWindows/MainWindows.cpp		\
				GuiTools/MainWindows/StatusBar.cpp			\
				GuiTools/MainWindows/MenuBar.cpp			\
				GuiTools/MainWindows/ToolBar.cpp			\
				GuiTools/Search/Search.cpp					\
				GuiTools/Search/SearchData.cpp

# Basic Interface :
CXXFILES+=		Buffer/Buffer.cpp						\
				Buffer/BufferText.cpp					\
				Buffer/BufferEmpty.cpp					\
				Buffer/BufferManager.cpp				\
				Colorize/Colorize.cpp					\
				Colorize/ColorizeManager.cpp			\
				Highlight/HighlightPattern.cpp			\
				Highlight/Highlight.cpp					\
				Highlight/HighlightManager.cpp

# Main entry file : 
CXXFILES+=		init.cpp


###############################################################################
### Liste of folder where .h can be                                         ###
###############################################################################
LISTE_MODULES = $(dir $(CXXFILES))
INCLUDE_DIRECTORY = $(addprefix -I$(FILE_DIRECTORY)/, $(LISTE_MODULES)) 

###############################################################################
### Build Object Files List                                                 ###
###############################################################################
OBJ =	$(addprefix $(OBJECT_DIRECTORY)/, $(CXXFILES:.cpp=.o))



###############################################################################
### Main Part of Makefile                                                   ###
###############################################################################
all: build

-include $(OBJ:.o=.d) 

build: .encadrer $(OUTPUT_NAME) $(MAKE_DEPENDENCE)


.encadrer:
	@echo $(CADRE_HAUT_BAS)
	@echo $(CADRE_COTERS)
	@echo '           DEBUT DE COMPILATION DU PROGRAMME :'$(CADRE_COTERS)
	@echo '             Repertoire Sources : $(FILE_DIRECTORY)/'$(CADRE_COTERS)
	@echo '             Repertoire object  : $(OBJECT_DIRECTORY)/'$(CADRE_COTERS)
	@echo '             Binaire de sortie  : $(F_VIOLET)$(OUTPUT_NAME) / $(OUTPUT_NAME)-stripped$(F_NORMALE)'$(CADRE_COTERS)
	@echo $(CADRE_COTERS)
	@echo $(CADRE_HAUT_BAS)
	@mkdir -p $(addprefix $(OBJECT_DIRECTORY)/, $(LISTE_MODULES))
	@mkdir -p ~/.bin


FILE_IMAGES=	data/imagesSources/*.png

# Tool used to create a binary version of every element png or other needed by the application
pngToCpp: tools/pngToCpp/pngToCpp.c
	@echo $(F_ROUGE)"          (bin) $@"$(F_NORMALE)
	@$(CXX) $< -o $@
	@strip -s $@

# Generate basic 
$(FILE_DIRECTORY)/GuiTools/myImage.cpp: $(FILE_IMAGES) $(MAKE_DEPENDENCE) pngToCpp
	@echo $(F_BLUE)"          (.cpp)  *.png ==> $@"$(F_NORMALE)
	@#echo ./pngToCpp $@ $(FILE_IMAGES)
	@./pngToCpp $@ $(FILE_IMAGES)


# build C++
$(OBJECT_DIRECTORY)/%.o: $(FILE_DIRECTORY)/%.cpp $(MAKE_DEPENDENCE)
	@echo $(F_VERT)"          (.o)  $<"$(F_NORMALE)
	@#echo $(CXX)  $< -c -o $@  $(INCLUDE_DIRECTORY) $(CXXFLAGS) -MMD
	@$(CXX)  $< -c -o $@  $(INCLUDE_DIRECTORY) $(CXXFLAGS) -MMD

# build binary
$(OUTPUT_NAME): $(OBJ)
	@echo $(F_ROUGE)"          (bin) $@ & $@-stripped"$(F_NORMALE)
	@$(CXX) $(OBJ) $(LDFLAGS) -o $@
	@cp $@ $@-stripped
	@strip -s $@-stripped 
	@#cp $@-stripped ~/.bin/$@


clean:
	@echo $(CADRE_HAUT_BAS)
	@echo '           CLEANING : $(F_VIOLET)$(OUTPUT_NAME)$(F_NORMALE)'$(CADRE_COTERS)
	@echo $(CADRE_HAUT_BAS)
	@echo Remove Folder : $(OBJECT_DIRECTORY)
	@rm -rf $(OBJECT_DIRECTORY) 
	@echo Remove File : $(OUTPUT_NAME)
	@rm -f $(OUTPUT_NAME)
	@echo Remove File : $(OUTPUT_NAME)-stripped 
	@rm -f $(OUTPUT_NAME)-stripped 
	@echo Remove File : pngToCpp
	@rm -f pngToCpp
	@echo Remove File : $(FILE_DIRECTORY)/GuiTools/myImage.*
	@rm -f $(FILE_DIRECTORY)/GuiTools/myImage.*
	@echo Remove doxygen  files : doxygen/*
	@rm -rf doxygen
	@rm -f doxygen.log
	@echo Remove temporary files *.bck
	@rm -f `find . -name "*.bck"`

count:
	wc -l Makefile `find $(FILE_DIRECTORY)/ -name "*.cpp"` `find $(FILE_DIRECTORY)/ -name "*.h"` 


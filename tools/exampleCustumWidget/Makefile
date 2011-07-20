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
#                                                                                                                #
# Concu Pour le projet edn                                                                                     #
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




###############################################################################
### Basic Cfags                                                             ###
###############################################################################

# basic GTK librairy
CXXFLAGS=  `pkg-config --cflags --libs gtk+-2.0`
#CXXFLAGS=  `wx-config --libs --cppflags`
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
LDFLAGS=  `pkg-config --cflags --libs gtk+-2.0`
#LDFLAGS=  `wx-config --libs --cppflags`
# Linux thread system
LDFLAGS+= -lpthread
# Dynamic connection of the CALLBACK of the GUI
LDFLAGS+= -Wl,--export-dynamic

###############################################################################
### Project Name                                                            ###
###############################################################################
OUTPUT_NAME=example

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
### Liste of folder where .h can be                                         ###
###############################################################################
LISTE_MODULES = .
				

INCLUDE_DIRECTORY = $(addprefix -I$(FILE_DIRECTORY)/, $(LISTE_MODULES)) 

###############################################################################
### Files Listes                                                            ###
###############################################################################

CFILES=	\
				main.c				\
				cpu.c			\
###############################################################################
### Build Object Files List                                                 ###
###############################################################################
OBJ =	$(addprefix $(OBJECT_DIRECTORY)/, $(CFILES:.c=.o))



###############################################################################
### Main Part of Makefile                                                   ###
###############################################################################
all: build

-include $(OBJ:.o=.d) 

build: .encadrer $(OUTPUT_NAME) $(MAKE_DEPENDENCE)

.encadrer:
	@echo $(CADRE_HAUT_BAS)
	@echo $(CADRE_COTERS)
	@echo '           DEBUT DE COMPILATION DU PROGRAMME :'$(CADRE_COTERS)
	@echo '             Repertoire Sources : $(FILE_DIRECTORY)/'$(CADRE_COTERS)
	@echo '             Repertoire object  : $(OBJECT_DIRECTORY)/'$(CADRE_COTERS)
	@echo '             Binaire de sortie  : $(F_VIOLET)$(OUTPUT_NAME) / $(OUTPUT_NAME)-stripped$(F_NORMALE)'$(CADRE_COTERS)
	@echo $(CADRE_COTERS)
	@echo $(CADRE_HAUT_BAS)
	@mkdir -p $(OBJECT_DIRECTORY)/


	

# build C
$(OBJECT_DIRECTORY)/%.o: $(FILE_DIRECTORY)/%.c $(MAKE_DEPENDENCE)
	@echo $(F_VERT)"          (.o)  $<"$(F_NORMALE)
	@#echo $(CXX)  $< -c -o $@  $(INCLUDE_DIRECTORY) $(CXXFLAGS) -MMD
	@$(CXX)  $< -c -o $@  $(INCLUDE_DIRECTORY) $(CXXFLAGS) -MMD

# build C++
$(OBJECT_DIRECTORY)/%.o: $(FILE_DIRECTORY)/%.cpp $(MAKE_DEPENDENCE)
	@echo $(F_VERT)"          (.o)  $<"$(F_NORMALE)
	@#echo $(CXX)  $< -c -o $@  $(INCLUDE_DIRECTORY) $(CXXFLAGS) -MMD
	@$(CXX)  $< -c -o $@  $(INCLUDE_DIRECTORY) $(CXXFLAGS) -MMD

# build binary
$(OUTPUT_NAME): $(OBJ)
	@echo $(F_ROUGE)"          (bin) $@ & $@-stripped"$(F_NORMALE)
	@$(CXX) $(OBJ) $(LDFLAGS) -o $@
	@cp $@ $@-stripped
	@strip -s $@-stripped 


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




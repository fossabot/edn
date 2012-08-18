
###############################################################################
### Some Preject properties :                                               ###
###############################################################################

# group name or constructor ... (no dot, no MAJ no Numerical char)
PROJECT_VENDOR=heeroyui
# Binary name ... (no dot, no MAJ no Numerical char)
PROJECT_NAME=edn

# Add package needed
USER_PACKAGES =$(shell pwd)/Sources/
# Add ewol packages :
USER_PACKAGES+=$(shell realpath $(shell pwd)/../ewol/Sources/)

# include basic makefile for EWOL 
include $(shell pwd)/../ewol/Build/Makefile.mk


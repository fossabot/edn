#############################################################################"
# main makefile for a basic application
# this is minimal application main makefile
#############################################################################"

#need to define the project name in hard to prevent some error in the config system :
CONFIG___EWOL_APPL_NAME__="edn"

# Add package needed :
# current user packages
USER_PACKAGES =$(shell pwd)/Sources/
# Add ewol packages
USER_PACKAGES+=$(shell realpath $(shell pwd)/../ewol/Sources/)

# include basic makefile for ewol generic toolchain
include $(shell pwd)/../ewol/Build/Makefile.mk

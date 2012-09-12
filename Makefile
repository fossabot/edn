
# Add package needed
USER_PACKAGES =$(shell pwd)/Sources/
# Add ewol packages :
USER_PACKAGES+=$(shell realpath $(shell pwd)/../ewol/Sources/)

# include basic makefile for EWOL 
include $(shell pwd)/../ewol/Build/Makefile.mk

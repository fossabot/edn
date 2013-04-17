#!/usr/bin/python
import make

make.ImportPath(make.GetCurrentPath(__file__) + "/sources/")
make.ImportPath(make.GetCurrentPath(__file__) + "/../ewol/")


make.Dump()

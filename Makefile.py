#!/usr/bin/python
import module
import buildTools

module.ImportPath(buildTools.GetCurrentPath(__file__) + "/sources/")
module.ImportPath(buildTools.GetCurrentPath(__file__) + "/../ewol/")

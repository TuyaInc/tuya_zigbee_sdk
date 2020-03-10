#!/usr/bin/env python

###################################################################################################
# File: code_generator.py                                                                                #
#                                                                                                 #
# Description: Code Generator API                                                                 #
#                                                                                                 #
# Details: The SDK Code Generator Framework allows you to control firmware code generation based  #
#          on Studio/Appbuilder(/Platform Builder?) GUI and/or project configuration. One of its  #
#          most important goals is to move all code generator logic from Studio into SDK space,   #
#          allowing SDK content owners full control over generated files, their location and      #
#          content.
#          The framework uses the Jinja2 templating engine where applicable.
#                                                                                                 #
# Author: tahornos                                                                                #
###################################################################################################

import os
import sys
import re
import fnmatch
import jinja2


optVerbose = False

# Code generator dictionary holding the results
codeGeneratorResult = \
{
  "success" : True,
  "message" : "",
  "content" : []
}


# Jinja environment
jinja2Environment = None
cgInputDir = ""
cgImportDirs = []
cgOutputDir = ""


#### Registered Jinja Custom Filters ##########################################

# Regex replace
def regexSub(string, pattern, repl, count=0, flags=0):
  return re.sub(pattern, repl, string, count, flags)


# Search for pattern in string, return True if found, False otherwise
def regexSearch(string, pattern, flags=0):
  if re.search(pattern, string, flags):
    return True
  return False


# Check if architecture is compatible with detected architecture
def isArchCompatible(archs, actArch):
  # we only accept string or list type
  assert isinstance(archs, (basestring, list))
  # empty architecture is compatible with everything
  if not archs:
    return True
  if isinstance(archs, basestring):
    archs = [archs]
  for arch in archs:
    fullArchList = []
    fullActArchList = []
    actArchList = re.split("\+", actArch.lower())
    for e in actArchList:
      fullActArchList.extend(re.split("\~", e))
    archList = re.split("\+", arch.lower())
    for e in archList:
      fullArchList.extend(re.split("\~", e))
    # all elements in full architecture list has to be present in actual architecture list
    compatible = True
    for e in fullArchList:
      # e doesn't match any substring of any element of actual (Studio)
      # architecture nor start both with or without '!' character at the same time
      # this covers the two cases when we declare incompatibility immediately:
      #   |compArch|studioArch|COMP/NOT|
      #   |-------------------|--------|
      #   |~module |~module   |COMP    |
      #   |~!module|~module   |NOT COMP|
      #   |~module |~!module  |NOT COMP|
      #   |~!module|~!module  |COMP    |
      #   |----------------------------|
      if not [i for i in fullActArchList if not (e.startswith("!") ^ i.startswith("!")) and e in i]:
        compatible = False
        break
    # compatible, if any of the archs is compatible
    if compatible:
      break
  return compatible


def listAllFiles(path):
  return [os.path.join(root, name) for root, dirs, files in os.walk(path) for name in files]


def pathJoinNormalise(path1, path2):
  return os.path.normpath(os.path.join(path1, path2))


def pathRelpath(path, startPath):
  return os.path.relpath(path, startPath)


def pathDirname(path):
  return os.path.dirname(path)


def pathBasename(path):
  return os.path.basename(path)


def pathIsDir(path):
  return os.path.isdir(path)


def fnMatch(filename, pattern):
  return fnmatch.fnmatch(filename, pattern)


#### Code Generator Public API ################################################

def setVerbose(a):
  global optVerbose
  optVerbose = a


def printLog(text):
  print "[LOG] %s" % text


def reportError(message):
  codeGeneratorResult["success"] = False
  codeGeneratorResult["message"] = message
  codeGeneratorResult["content"] = []


def setupEnvironment(inputDir, outputDir, importDirs=[]):
  global cgInputDir
  global cgImportDirs
  global cgOutputDir
  global jinja2Environment
  # ensure POSIX path separators are used; Jinja2 seems to dislike everything else
  cgInputDir = inputDir.replace("\\", "/")
  cgImportDirs = [d.replace("\\", "/") for d in importDirs]
  cgOutputDir = outputDir.replace("\\", "/")
  # set up Jinja2 environment:
  #   - no new lines
  #   - root dirs; remove duplicate items
  #   - enable loopcontrol extension: adds 'break' and 'continue'
  jinja2Environment = jinja2.Environment(trim_blocks=True,
                                   lstrip_blocks=True,
                                   loader=jinja2.FileSystemLoader(list(set([cgInputDir]+cgImportDirs))),
                                   extensions=["jinja2.ext.loopcontrols",
                                               "jinja2.ext.do"])
  # register custom filter
  jinja2Environment.filters["regexsub"] = regexSub
  jinja2Environment.filters["regexsearch"] = regexSearch
  jinja2Environment.filters["isarchcompatible"] = isArchCompatible
  jinja2Environment.filters["pathjoinnormalise"] = pathJoinNormalise
  jinja2Environment.filters["pathrelpath"] = pathRelpath
  jinja2Environment.filters["pathdirname"] = pathDirname
  jinja2Environment.filters["pathbasename"] = pathBasename
  jinja2Environment.filters["pathisdir"] = pathIsDir
  jinja2Environment.filters["listallfiles"] = listAllFiles
  jinja2Environment.filters["fnmatch"] = fnMatch


def copyFile(sourceFile, destinationFile):
  global codeGeneratorResult
  if not cgInputDir or not cgOutputDir:
    codeGeneratorResult["success"] = False
    codeGeneratorResult["message"] = "Environment hasn't been set up!"
    return False
  # ensure POSIX path separators are used, to be in sync with Jinja2
  sourceFile = sourceFile.replace("\\", "/")
  destinationFile = destinationFile.replace("\\", "/")
  if os.path.isabs(sourceFile):
    srcFile = sourceFile.replace("\\", "/")
  else:
    srcFile = os.path.normpath(os.path.join(cgInputDir, sourceFile)).replace("\\", "/")
  dstFile = os.path.normpath(os.path.join(cgOutputDir, destinationFile)).replace("\\", "/")
  if srcFile == dstFile:
    codeGeneratorResult["success"] = False
    codeGeneratorResult["message"] = "Source and destination path are the same!"
    return False
  codeGeneratorResult["content"].append({"commandType" : "fileCopy",
                                         "fileSourcePath" : srcFile,
                                         "fileDestinationPath" : destinationFile})
  if optVerbose:
    printLog("[C] %s" % (dstFile))
  return True


def linkFile(sourceFile, destinationFile):
  global codeGeneratorResult
  if not cgInputDir or not cgOutputDir:
    codeGeneratorResult["success"] = False
    codeGeneratorResult["message"] = "Environment hasn't been set up!"
    return False
  # ensure POSIX path separators are used, to be in sync with Jinja2
  sourceFile = sourceFile.replace("\\", "/")
  destinationFile = destinationFile.replace("\\", "/")
  if os.path.isabs(sourceFile):
    srcFile = sourceFile.replace("\\", "/")
  else:
    srcFile = os.path.normpath(os.path.join(cgInputDir, sourceFile)).replace("\\", "/")
  dstFile = os.path.normpath(os.path.join(cgOutputDir, destinationFile)).replace("\\", "/")
  if srcFile == dstFile:
    codeGeneratorResult["success"] = False
    codeGeneratorResult["message"] = "Source and destination path are the same!"
    return False
  # add file and its content to dict as key-value pairs
  codeGeneratorResult["content"].append({"commandType" : "linkLibrary",
                                         "fileSourcePath" : srcFile,
                                         "fileDestinationPath" : destinationFile})
  if optVerbose:
    printLog("[L] %s" % (dstFile))
  return True


def renderTemplate(templateFile, context, outputFile):
  global codeGeneratorResult
  if not cgInputDir or not cgOutputDir:
    codeGeneratorResult["success"] = False
    codeGeneratorResult["message"] = "Environment hasn't been set up!"
    return False
  # ensure POSIX path separators are used; Jinja2 seems to dislike everything else
  templateFile = templateFile.replace("\\", "/")
  outputFile = outputFile.replace("\\", "/")
  modContext = {"context":context}
  outFile = os.path.normpath(os.path.join(cgOutputDir, outputFile)).replace("\\", "/")
  if templateFile == outFile:
    codeGeneratorResult["success"] = False
    codeGeneratorResult["message"] = "Template and output path are the same!"
    return False
  output = jinja2Environment.get_template(templateFile).render(modContext)
  # add file and its content to dict as key-value pairs
  codeGeneratorResult["content"].append({"commandType" : "fileCreate",
                                         "filePath" : outputFile,
                                         "fileContent" : output})
  if optVerbose:
    printLog("[R] %s" % (outFile))
  return True


def generateFile(content, outputFile):
  global codeGeneratorResult
  if not cgInputDir or not cgOutputDir:
    codeGeneratorResult["success"] = False
    codeGeneratorResult["message"] = "Environment hasn't been set up!"
    return False
  outputFile = outputFile.replace("\\", "/")
  outFile = os.path.normpath(os.path.join(cgOutputDir, outputFile)).replace("\\", "/")
  # add file and its content to dict as key-value pairs
  codeGeneratorResult["content"].append({"commandType" : "fileCreate",
                                         "filePath" : outputFile,
                                         "fileContent" : content})
  if optVerbose:
    printLog("[G] %s" % (outFile))
  return True


def addIncludeDirectory(destinationDir):
  global codeGeneratorResult
  if not cgInputDir or not cgOutputDir:
    codeGeneratorResult["success"] = False
    codeGeneratorResult["message"] = "Environment hasn't been set up!"
    return False
  # ensure POSIX path separators are used, to be in sync with Jinja2
  destinationDir = destinationDir.replace("\\", "/")
  dstDir = os.path.normpath(os.path.join(cgOutputDir, destinationDir)).replace("\\", "/")
  # add file and its content to dict as key-value pairs
  codeGeneratorResult["content"].append({"commandType" : "includeDirectoryAdd",
                                         "directoryPath" : dstDir})
  if optVerbose:
    printLog("[I] %s" % (dstDir))
  return True


def addMacro(macro):
  global codeGeneratorResult
  macroDissected = macro.split("=")
  macroParts = len(macroDissected)
  if macroParts > 2 or macroParts < 1:
    codeGeneratorResult["success"] = False
    codeGeneratorResult["message"] = "Wrong macro format: %s!" % macro
    return False
  # remove whitespaces
  macroDissected = [e.strip() for e in macroDissected]
  if macroParts == 1:
    macroDissected.append("")
  # add macro name and its value to dict as key-value pairs
  codeGeneratorResult["content"].append({"commandType" : "macroAdd",
                                         "macroName" : macroDissected[0],
                                         "macroValue" : macroDissected[1]})
  if optVerbose:
    printLog("[M] %s" % (macro))
  return True


def incrementalGeneration():
  global codeGeneratorResult
  codeGeneratorResult["content"].append({"commandType" : "incrementalGeneration",
                                         "enabled" : True})
  if optVerbose:
    printLog("[INCREMENTAL GENERATION]")
  return True


def getInputDir():
  return cgInputDir


def getImportDir():
  return cgImportDirs


def getOutputDir():
  return cgOutputDir


def getResult():
  return codeGeneratorResult


def getVersion():
  return "1.0.0"


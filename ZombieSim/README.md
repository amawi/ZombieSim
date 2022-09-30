
An explanation of each file in the code of our ZombieSim project
corresponds to:

BB_PropertyBag: A header file that creates the Propertybag, stores all variables
that’s read from a propertybag

BB_General: A header file containing standard functions used within the rest of the
BB files and sometimes the ZombieSim files.

BB_LinuxDefs: By default programs are normally developed with a Windows based
by Maaike. She created function definitions to match the C++ version of Windows within
Linux.

BB_DirList: Basically get a list of files with extension <xx> from directory defined
by the user. Is used in PropertyBag to get the configuration file.

City: All the class relevant function and variables related to the City Objects.

ClearVector: A template to clear out vectors and destroy (delete) it’s objects.
  
Country: All the class relevant function and variables related to the Country Object.
  
CreateObjects: Basically contains the functions to create the different playfield objects
gets called by the main
  
Helper: Just includes standard macros and also the CGlobal class which contains the
propertybag and the management objects. Defines global object variable CGlobal glb.
  
Humans: All the class relevant function and variables related to the Human Objects.
  
Initialize: Contains the ParseCommandLine function and the InitProgramVariables.
That initialize all the variables used within ZombieSim
  
Management: Basically a class that handles the error messaging.
  
olcPixelGameEngine: The Visual layer lib used within ZombieSim. Made by OneLoneCoder.com
  
PhysicalEngine: Class that contains what the program actually has to do in the visual
layer. Uses olc::PixelGameEngine as a derived (polymorphism) class for the actual
drawing of the playfield
  
RunThreads: All the different thread functions used within ZombieSim

Stdafx: Contains all standard libs used throughout the program. In windows this is
of course a precompiled header.
  
Thread: Contains the different thread classes used within ZombieSim
  
ZombieSim.conf: The configuration file of ZombieSim

ZombieSim.cpp: The main of ZombieSim
  
  
ZS_PropertyBag: Uses CPropertyBag (BB_PropertyBag) as a derived class for storing
variables from the command line or configuration file. But in itself has getters and
setters towards these specific variables. BB_Propertybag stores everything the user adds
into the configuration file. But ZS_PropertyBag makes them accessible throughout the
program.


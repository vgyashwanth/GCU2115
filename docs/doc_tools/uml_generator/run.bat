@echo off
rem Delete previous output files if present
del uml.svg 2>nul
del uml.png 2>nul
del uml.puml 2>nul
@echo on
call files.bat
rem Set graph viz path
set GRAPHVIZ_DOT=%cd%\plantuml\dot.exe
set PLANTUML_LIMIT_SIZE=8192
rem Generate UML diagram from the plant UML i/p file
java.exe -jar "%cd%\plantuml\plantuml-8059.jar" "%cd%\uml.puml" -tsvg -o %cd%
java.exe -jar "%cd%\plantuml\plantuml-8059.jar" "%cd%\uml.puml" -tpng -o %cd%
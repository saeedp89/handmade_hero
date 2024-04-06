@echo off
pushd ..\..\build
cl -Zi ..\HandmadeHero\code\main.cpp user32.lib gdi32.lib
popd

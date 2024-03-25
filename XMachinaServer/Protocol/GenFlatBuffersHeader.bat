@echo off
flatc --cpp Player.fbs
flatc --cpp Enum.fbs
flatc --cpp FBProtocol.fbs
flatc --cpp Transform.fbs
flatc --cpp Struct.fbs

IF ERRORLEVEL 1 PAUSE

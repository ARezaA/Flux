::Please Modify the variables into a suitable
::More Info visit: http:://www.cocos2d-x.org
@echo on

set PROJECT_NAME=FluxProto
set COCOS2D_DIR=C:\Users\Reza A\School\IMD 2900\Tutorial\cocos2dx-win8-master
set PROJECT_DIR=C:\Users\Reza A\School\IMD 2900\Tutorial\FluxProto

xcopy /S  "%COCOS2D_DIR%\tinyxml" "%PROJECT_DIR%\tinyxml\"
xcopy /S  "%COCOS2D_DIR%\Box2D" "%PROJECT_DIR%\Box2D\"
xcopy /S  "%COCOS2D_DIR%\chipmunk" "%PROJECT_DIR%\chipmunk\"
xcopy /S  "%COCOS2D_DIR%\cocos2dx" "%PROJECT_DIR%\cocos2dx\"
xcopy /S  "%COCOS2D_DIR%\CocosDenshion" "%PROJECT_DIR%\CocosDenshion\"
xcopy /S  "%COCOS2D_DIR%\.gitignore" "%PROJECT_DIR%\"
xcopy /S  "%COCOS2D_DIR%\HelloWorld\Classes" "%PROJECT_DIR%\%PROJECT_NAME%\Classes\"
xcopy /S  "%COCOS2D_DIR%\HelloWorld\AppDelegate.cpp" "%PROJECT_DIR%\%PROJECT_NAME%"
xcopy /s  "%COCOS2D_DIR%\HelloWorld\AppDelegate.h" "%PROJECT_DIR%\%PROJECT_NAME%"

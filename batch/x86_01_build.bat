echo off
rem -----------------------------------------------------
rem Set path QTDIR (x64) - QT x64 버전의 디렉토리를 설정해준다.
rem -----------------------------------------------------
rem set path=%path%;"C:\apps\Python34;"

rem -----------------------------------------------------
rem Create Visual Studio Project Directory (x64) - VS 프로젝트 디렉토리를 생성해준다.
rem -----------------------------------------------------
mkdir ..\..\Virgo-Desktop-MSVC2013-32bit-Release & pushd ..\..\Virgo-Desktop-MSVC2013-32bit-Release

rem -----------------------------------------------------
rem Create Visual Studio Project (x86) - 소스로부터 x86용 VS 프로젝트를 생성해준다.
rem -----------------------------------------------------
echo on
cmake -G "Visual Studio 12 2013" "..\Virgo" -DCMAKE_PREFIX_PATH="C:\apps\Qt5.5.1-x86\5.5\msvc2013\lib\cmake"
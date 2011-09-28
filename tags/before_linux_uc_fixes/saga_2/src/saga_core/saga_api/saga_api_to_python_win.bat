@echo off

set SWIG_PATH=D:\tools\develop\swigwin-1.3.29
set PYTHON_PATH=C:\Program Files\Python23

echo SWIG compilation...
"%SWIG_PATH%\swig" -c++ -python -includeall saga_api.h
echo SWIG compilation finished.

echo Python compilation...
"%PYTHON_PATH%\python.exe" saga_api_to_python_win.py install
copy saga_api.py "%PYTHON_PATH%\Lib\site-packages\saga_api.py"
echo Python compilation finished.

pause
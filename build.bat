:: delete old build
del main.exe
:: build new
gcc main.c -o main
:: close old instance
taskkill /IM main.exe /F
:: start new instance with file as argument
start main.exe %1
gcc shell.c helper.c -o shell.o
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi
sudo ./shell.o
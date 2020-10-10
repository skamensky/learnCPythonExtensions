gcc copy_stdin_to_file.c \
    -g \
    -o copy_stdin_to_file  \
    && echo "HELLO THIS IS MORE THAN 8 BYTESDONE" \
    |valgrind ./copy_stdin_to_file output.txt
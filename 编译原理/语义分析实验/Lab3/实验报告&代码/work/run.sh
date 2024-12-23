bison -d -o parser.tab.cpp example.y
flex -o lex.yy.cpp example.l   
g++ -o sysy lex.yy.cpp parser.tab.cpp -std=c++17

./sysy /home/ubuntu/Desktop/Lab3/tests/1/1.c
gcc -o   test  assemble.s 
stdbuf -oL ./test | tee /home/ubuntu/Desktop/Lab3/tests/1/output.txt
mv assemble.s /home/ubuntu/Desktop/Lab3/tests/1/

./sysy /home/ubuntu/Desktop/Lab3/tests/2/2.c
gcc -o   test  assemble.s 
stdbuf -oL ./test | tee /home/ubuntu/Desktop/Lab3/tests/2/output.txt
mv assemble.s /home/ubuntu/Desktop/Lab3/tests/2/

./sysy /home/ubuntu/Desktop/Lab3/tests/3/3.c
gcc -o   test  assemble.s 
stdbuf -oL ./test | tee /home/ubuntu/Desktop/Lab3/tests/3/output.txt
mv assemble.s /home/ubuntu/Desktop/Lab3/tests/3/

./sysy /home/ubuntu/Desktop/Lab3/tests/4/4.c
gcc -o   test  assemble.s 
stdbuf -oL ./test | tee /home/ubuntu/Desktop/Lab3/tests/4/output.txt
mv assemble.s /home/ubuntu/Desktop/Lab3/tests/4/

./sysy /home/ubuntu/Desktop/Lab3/tests/6/6.c ?
gcc -o   test  assemble.s 
stdbuf -oL ./test | tee /home/ubuntu/Desktop/Lab3/tests/6/output.txt
mv assemble.s /home/ubuntu/Desktop/Lab3/tests/6/

./sysy /home/ubuntu/Desktop/Lab3/tests/8/8.c
gcc -o   test  assemble.s 
stdbuf -oL ./test | tee /home/ubuntu/Desktop/Lab3/tests/8/output.txt
mv assemble.s /home/ubuntu/Desktop/Lab3/tests/8/

./sysy /home/ubuntu/Desktop/Lab3/tests/10/10.c
gcc -o   test  assemble.s 
stdbuf -oL ./test | tee /home/ubuntu/Desktop/Lab3/tests/10/output.txt
mv assemble.s /home/ubuntu/Desktop/Lab3/tests/10/


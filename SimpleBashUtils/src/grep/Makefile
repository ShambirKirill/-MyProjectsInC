s21_grep: s21_grep.o open_read_file.o array_processing.o
	gcc s21_grep.o open_read_file.o array_processing.o -o s21_grep
	
s21_grep.o: s21_grep.c
	gcc -c -g -Wall -Wextra -Werror s21_grep.c

open_read_file.o: ../common/open_read_file.c
	gcc -c -g -Wall -Wextra -Werror ../common/open_read_file.c

array_processing.o: ../common/array_processing.c
	gcc -c -g -Wall -Wextra -Werror ../common/array_processing.c

clean:
	rm *.o
	rm s21_grep
rb:
	make clean
	make s21_grep
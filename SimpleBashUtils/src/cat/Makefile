s21_cat: s21_cat.o open_read_file.o array_processing.o
	gcc s21_cat.o open_read_file.o array_processing.o -o s21_cat
	
s21_cat.o: s21_cat.c
	gcc -c -g -Wall -Wextra -Werror s21_cat.c

open_read_file.o: ../common/open_read_file.c
	gcc -c -g -Wall -Wextra -Werror ../common/open_read_file.c

array_processing.o: ../common/array_processing.c
	gcc -c -g -Wall -Wextra -Werror ../common/array_processing.c

clean:
	rm *.o
	rm s21_cat
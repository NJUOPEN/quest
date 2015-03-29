source_file = quest.c function.c menu.c SL.c
quest : $(source_file)
	gcc -Wall -std=c99 $(source_file) -o quest
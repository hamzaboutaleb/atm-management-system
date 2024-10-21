SRCS = src/main.c src/newAccount.c src/auth.c src/database.c src/utils.c src/registration.c src/checkAccount.c src/updateAccount.c src/deleteAccount.c src/transaction.c src/transfer.c

OBJECT = $(SRCS:src/%.c=src/%.o)

NAME = atm

HEADER = src/header.h

FLAGS = -lsqlite3

all : $(NAME)

$(NAME) : $(OBJECT)
	cc -o $(NAME)  $(OBJECT) $(FLAGS)
	clear

src/%.o: src/%.c $(HEADER)
	cc -c $< -o $@

clean :
	rm -rf $(OBJECT)
	clear

fclean : clean
	rm -rf $(NAME)
	clear

re : fclean all

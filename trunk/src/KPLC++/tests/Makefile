GPP		=	g++
NAME		=	test
SRC		=	test1.cpp
OBJ		=	$(SRC:.cpp=.o)
RM		=	rm -f
INCFLAGS	=	-I../includes
LFLAGS		=	-L../ -lkopul `llvm-config --ldflags --libs `
CPPFLAGS	= 	-W -Wall `llvm-config --cxxflags` $(INCFLAGS) $(LFLAGS)
CMD		=	$(GPP) -o $(NAME) $(OBJ) $(LFLAGS)

$(NAME):	$(OBJ)
		$(CMD)

all:	$(NAME)

obj:
		$(GPP) $(CPPFLAGS) -o $@-c $<

clean:
		$(RM) *.o *~

fclean: clean 
	$(RM) $(NAME)

re:	fclean all

go:	
	cd .. && make && cd - && make re && ./test

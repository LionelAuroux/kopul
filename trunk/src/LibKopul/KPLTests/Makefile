GPP		=	g++
NAME		=	test
SRC		=	srcs/main.cpp			\
			srcs/testBitfield.cpp		\
			srcs/testStaticArray.cpp	\
			srcs/testStaticStruct.cpp	\
			srcs/testValue.cpp		\
			srcs/testDynamicArray.cpp	\
			srcs/testSwitch.cpp
OBJ		=	$(SRC:.cpp=.o)
RM		=	rm -f
INCFLAGS	=	-I../KPLC++/includes -I./includes/
LFLAGS		=	-L../KPLC++/ -lkopul `llvm-config --ldflags --libs `
CPPFLAGS	= 	-W -Wall `llvm-config --cxxflags` $(INCFLAGS) $(LFLAGS)
CMD		=	$(GPP) -o $(NAME) $(OBJ) $(LFLAGS)

$(NAME):	$(OBJ)
		$(CMD)

all:	$(NAME)

obj:
		$(GPP) $(CPPFLAGS) -o $@-c $<

clean:
		$(RM) srcs/*.o srcs/*~

fclean: clean 
	$(RM) $(NAME)

re:	fclean all

go:	
	cd ../KPLC++/ && make re && cd - && make re && ./test

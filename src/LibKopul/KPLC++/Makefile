GPP		=	g++
NAME		=	libkopul.a
SRCDIR		=	srcs
SRC		=	srcs/Type.cpp			\
			srcs/Value.cpp			\
			srcs/Variable.cpp		\
			srcs/Bitfield.cpp		\
			srcs/StaticArray.cpp		\
			srcs/DynamicArray.cpp		\
			srcs/Switch.cpp			\
			srcs/StaticStruct.cpp		\
			srcs/StaticType.cpp		\
			srcs/Condition.cpp		\
			srcs/ConditionNode.cpp		\
			srcs/Register.cpp		\
			srcs/FunctionList.cpp		\
			srcs/Container.cpp		\
			srcs/SwitchCondition.cpp	\
			srcs/ConstantValue.cpp		\
			srcs/VariableIterator.cpp

OBJDIR		=	objs
OBJ		=	$(SRC:.cpp=.o)
RM		=	rm -f
INCFLAGS	=	-I./includes/
LFLAGS		=	`llvm-config --ldflags --libs ` -fexceptions
CPPFLAGS	= 	-W -Wall `llvm-config --cxxflags` $(INCFLAGS) $(LFLAGS)
CMD		=	ar rcs $(NAME) $(OBJ)

$(NAME):	$(OBJ)
		$(CMD)

all:	$(NAME)

obj:
		$(GPP) $(CPPFLAGS) -o $@-c $<

clean:
		$(RM) $(SRCDIR)/*.o $(SRCDIR)/*~

fclean: clean 
	$(RM) $(NAME)

re:	fclean all

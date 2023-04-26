# -----( FILES )-----

# NAME
NAME					=	test

# TEST_LIB
TEST_LIB_DIR	=	../test_lib
TEST_LIB_SRC	=	kunit.cpp

# SRCS
SRC_DIR				=	../../src/http
SRC						=	HttpRequest.cpp\
								UriParser.cpp

UTIL_DIR			=	../../src
UTIL_SRC			=	Util.cpp

# TEST_SRCS
TEST_SRC_DIR	=	.
TEST_SRC			=	main.cpp\
								HttpRequestTest.cpp\
								UriParserTest.cpp

# OBJS
OBJ						=	$(addprefix $(SRC_DIR)/, $(SRC:.cpp=.o))\
								$(addprefix $(UTIL_DIR)/, $(UTIL_SRC:.cpp=.o))\
								$(addprefix $(TEST_SRC_DIR)/, $(TEST_SRC:.cpp=.o))\
								$(addprefix $(TEST_LIB_DIR)/, $(TEST_LIB_SRC:.cpp=.o))

# -----( COLOR )-----

BLUE_CR				=	"\033[1;34m"
GRREN_CR			=	"\033[1;32m"
RED_CR				=	"\033[1;31m"
RESET_CR			=	"\033[0;0m"

# -----( CRULE )-----

CXX						=	c++
# CXXFLAGS			=	-Wall -Wextra -Werror -std=c++98
# CPPFLAGS			=	-I$(SRC_DIR)-I$(TEST_LIB_DIR)

# -----( RULES )-----

all: $(NAME)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@./$(NAME)
	@make fclean

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re:
	@make fclean
	@make all

.PHONY: all clean fclean re


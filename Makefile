NAME = ircserv
FILES = main.cpp $(wildcard src/*.cpp) $(wildcard src/commands/*.cpp)
RM = rm -rf
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
OBJ = $(FILES:.cpp=.o)

BLUE = \033[34m
GREEN = \033[32m
YELLOW = \033[33m
RED = \033[31m
RESET = \033[0m
BOLD = \033[1m

all: $(NAME)
	@echo "$(GREEN)$(BOLD)Build complete! Ready for launch!$(RESET)"

$(NAME): $(OBJ)
	@echo "$(YELLOW)$(BOLD)Igniting thrusters...$(RESET)"
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)$(BOLD)ircserv successfully compiled!$(RESET)"

%.o: %.cpp
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "$(YELLOW)Cleaning up launch pad...$(RESET)"
	@$(RM) $(OBJ)
	@echo "$(GREEN)Object files removed!$(RESET)"

fclean: clean
	@echo "$(YELLOW)Performing deep clean...$(RESET)"
	@$(RM) $(NAME)
	@echo "$(GREEN)Executable removed!$(RESET)"

re: fclean all

.PHONY: all clean fclean re
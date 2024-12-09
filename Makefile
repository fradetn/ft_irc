
# Name of the program
NAME	=	ircserv
INC		=	-I./includes

# Flags
CC			=	clang++
CPPFLAGS	=	-Wshadow -Wall -Werror -Wextra -std=c++98

# Directories
VPATH   =   src
OBJ_DIR	=	obj/

# .cpp and .o files

SRC		 =		main.cpp 	\
				utils.cpp	\
				Server.cpp	\
				Client.cpp	\
				Parser.cpp

OBJ		 =	$(SRC:%.cpp=$(OBJ_DIR)%.o)

# The main rule
all				:	$(NAME)

# The name rule
$(NAME)			:	$(OBJ_DIR) $(OBJ)
	@echo "\033[0;33mCompiling the whole project -> ⏳\033[0m"
	@$(CC) $(OBJ) -o $@
	@echo "\033[0;32mProject successfuly compiled -> ✅\033[0m\n"

# Compile .cpp to .o files
$(OBJ_DIR)%.o	:	%.cpp
	@echo "\033[0;33mCompiling project src -> ⏳\033[0m"
	@$(CC) $(CPPFLAGS) -c $< -o $@ $(INC)
	@echo "\033[0;32mSrc's project successfuly compiled -> ✅\033[0m\n"

# Create the obj directory
$(OBJ_DIR)		:
	@mkdir -p $@

# Clean, fclean and re rules
clean			:
	@echo "\033[0;31mCleaning obj files -> 🗑️\033[0m"
	@rm -rf $(OBJ_DIR)
	@echo "\033[0;32mDone -> ✅\033[0m\n"

fclean			:	clean
	@echo "\033[0;31mCleaning program -> 🗑️\033[0m"
	@rm -rf $(NAME)
	@echo "\033[0;32mDone -> ✅\033[0m\n"

re				:	fclean all
#░░░░█░█░█▀█░█▀▄░▀█▀░█▀█░█▀▄░█░░░█▀▀░█▀▀░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
#░░░░▀▄▀░█▀█░█▀▄░░█░░█▀█░█▀▄░█░░░█▀▀░▀▀█░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
#░░░░░▀░░▀░▀░▀░▀░▀▀▀░▀░▀░▀▀░░▀▀▀░▀▀▀░▀▀▀░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░

NAME			:=	ircserv

CXX 			:=	c++
CXX_FLAGS		:=	-g -Wall -Wextra -Werror -std=c++98

BIN_DIR			:=	bin
LOG_DIR			:=	logs

SRC_FILES		:=	main\
					Server\
					Client\
					Channel\
					Commands/ACommand\
					Commands/Authentification/Nick\
					Commands/Authentification/Pass\
					Commands/Authentification/User\
					Commands/Authentification/Ping\
					Commands/Channels/Invite\
					Commands/Channels/Join\
					Commands/Channels/Topic\
					Commands/Channels/Kick\
					utils
OBJ_FILES		:=	$(addprefix $(BIN_DIR)/, $(addsuffix .o, $(SRC_FILES)))
OBJ_TEST_FILES	:=	$(addprefix $(BIN_DIR)/, $(addsuffix .o, $(SRC_TEST_FILES)))
SRC_DIR			:=	src
INC_DIR			:=	-I include/Commands \
					-I include/Commands/Authentification \
					-I include/Commands/Channels \
					-I include

#░░░░█▀▄░█▀▀░█▀▀░▀█▀░█▀█░█▀▀░█▀▀░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
#░░░░█▀▄░█▀▀░█░░░░█░░█▀▀░█▀▀░▀▀█░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
#░░░░▀░▀░▀▀▀░▀▀▀░▀▀▀░▀░░░▀▀▀░▀▀▀░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░

all: title $(NAME)

$(NAME): $(OBJ_FILES)
	@$(CXX) -o $(NAME) $(OBJ_FILES)
# @echo "\n📣 $(GREEN_BLINK)$(NAME) compiled $(GREEN)!!! ‍️💻$(END_COLOR) \n"
	@echo
	@toilet -f term -F border --gay " $(NAME) compiled ! " | sed 's/^/\t/'
	@echo

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp Makefile | $(BIN_DIR)
	@$(CXX) -MMD -c $(CXX_FLAGS) $(INC_DIR) $< -o $@
	@printf "\t$(BLUE)> compiling $(notdir $<)$(END_COLOR)\n"

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)/Commands/Authentification $(BIN_DIR)/Commands/Channels
	@echo "\t$(IPURPLE)Created $(BIN_DIR)/ directory.$(END_COLOR)"

$(LOG_DIR):
	@mkdir $(LOG_DIR)
	@echo "\t$(IPURPLE)Created $(LOG_DIR)/ directory.$(END_COLOR)"

clean: title
	@echo "\t🧹 clean"
	@[ -d $(BIN_DIR) ] && rm -rf $(BIN_DIR) && echo "\t[✔] $(YELLOW).o files cleaned$(END_COLOR) "\
	|| echo "\t[✔] $(DIM).o files were already cleaned$(END_COLOR)"
	@[ -d $(LOG_DIR) ] && rm -rf $(LOG_DIR) && echo "\t[✔] $(YELLOW).log files cleaned$(END_COLOR)"\
	|| echo "\t[✔] $(DIM)no log files to clean$(END_COLOR)"
	@echo

fclean: clean
	@echo "\t💣 fclean"
	@[ -f $(NAME) ] && rm -f $(NAME) && echo "\t[✔] $(YELLOW)$(NAME) executable cleaned$(END_COLOR)"\
	|| echo "\t[✔] $(DIM)$(NAME) executable was already cleaned$(END_COLOR)"
	@echo

re: fclean all

launch: all $(LOG_DIR)
	@clear -x && make && clear -x && valgrind --log-file="$(LOG_DIR)/leaks.log" ./${NAME} 6667 pwd | tee $(LOG_DIR)/serverOutput.log

valgrind: all
	@clear -x && make && clear -x && valgrind ./${NAME} 6667 coucou

-include $(OBJ_FILES:%.o=%.d)

.PHONY: all clean fclean re title launch valgrind

#░░░░█░█░▀█▀░▀█▀░█░░░▀█▀░▀█▀░▀█▀░█▀▀░█▀▀░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
#░░░░█░█░░█░░░█░░█░░░░█░░░█░░░█░░█▀▀░▀▀█░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
#░░░░▀▀▀░░▀░░▀▀▀░▀▀▀░▀▀▀░░▀░░▀▀▀░▀▀▀░▀▀▀░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░

title:
	@clear -x
	@echo
	@echo "                               $(GREEN_BLINK)🔥$(END_COLOR) "
	@echo
	@echo "\t███████╗████████╗      ██╗██████╗  ██████╗ "
	@echo "\t██╔════╝╚══██╔══╝      ██║██╔══██╗██╔════╝ "
	@echo "\t█████╗     ██║         ██║██████╔╝██║      "
	@echo "\t██╔══╝     ██║         ██║██╔══██╗██║      "
	@echo "\t██║        ██║  █████╗ ██║██║  ██║╚██████╗ "
	@echo "\t╚═╝        ╚═╝  ╚════╝ ╚═╝╚═╝  ╚═╝ ╚═════╝ "
	@echo     
	@echo                                                                                                                  

#░░░░█▀▀░█▀█░█░░░█▀█░█▀▄░█▀▀░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
#░░░░█░░░█░█░█░░░█░█░█▀▄░▀▀█░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
#░░░░▀▀▀░▀▀▀░▀▀▀░▀▀▀░▀░▀░▀▀▀░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░

GREEN				:=	\033[0;92m
BGREEN				:=	\033[1;92m
BRED				:=	\033[1;31m
DIM					:=	\033[2m
YELLOW				:=	\033[0;93m
GREEN_BLINK			:=	\033[92;5m
BLUE				:=	\033[0;94m
IPURPLE				:=	\033[3;35m
END_COLOR			:=	\033[0;39m

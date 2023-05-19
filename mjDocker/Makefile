IMAGE_NAME := ft_irc-img
CONTAINER_NAME := ft_irc-cntnr
PORT := 6667
PROJECT_FOLDER := $(abspath $(CURDIR)/..)

all: tmux-config build clear run

build:
	@docker build -t ${IMAGE_NAME} .

run:
	@docker run --rm -it --name ${CONTAINER_NAME} \
	-p 6667:6667 \
	--volume ${PROJECT_FOLDER}:/ft_irc \
	${IMAGE_NAME}
	@rm tmux.conf

clean:
	@docker stop $$(docker ps -qa);\
	docker rm $$(docker ps -qa);\
	docker rmi -f $$(docker images -qa);
#docker volume rm $$(docker volume ls -q);
#docker network rm $$(docker network ls -q);

tmux-config:
	@echo "setw -g mouse on" > tmux.conf

re: clean all

clear:
	@clear -x

.PHONY: all run build clean re clear tmux-config
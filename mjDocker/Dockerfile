# Tip: I created a Makefile for this

# Build from latest ubuntu version
FROM	ubuntu:latest

# Install build-essential (to create C++ projects), vim, curl and zsh
# run apt-get clean to clean the package cache and free up space
# rm -rf any lists that are no longer needed after åinstalling the packages

RUN		apt-get update && \
        apt-get install -y build-essential vim git curl zsh toilet && \
		apt-get clean && \
		rm -rf /var/lib/apt/lists/*

# Install Oh My Zsh, because I want to
RUN		sh -c "$(curl -fsSL https://raw.github.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

# Change the zsh prompt so I can identify the container easily
RUN 	echo 'export PS1="\033[1;93m🌞 🌓 🌙 [ft_irc] \033[0;39m"' > ~/.zshrc

# Create the directory we will be working from
RUN		mkdir /ft_irc

# Set that directory as the start directory
WORKDIR	/ft_irc

# Notify that a volume will be needed when running the container
VOLUME	[ "/ft_irc" ]

# Get into the container with zsh
#CMD		[ "zsh" ]
CMD [ "zsh", "-c", "echo '📢 Ça va être tout noir !\n'; zsh" ]
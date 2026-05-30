FLAGS=''
COMPILER=gcc
FILES='multicp.c helpers.c'
OUTPUT=/usr/bin/multicp

sudo ${COMPILER} ${FLAGS} ${FILES} -o ${OUTPUT}

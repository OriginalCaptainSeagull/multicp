FLAGS='-O2 -Iincludes/'
COMPILER='gcc'
FILES='src/multicp.c src/helpers.c src/cliargs.c'
OUTPUT='/usr/bin/multicp'

sudo ${COMPILER} ${FLAGS} ${FILES} -o ${OUTPUT} && echo Success!

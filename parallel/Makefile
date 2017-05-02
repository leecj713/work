CC=gcc
LIB_PTHREAD=pthread
RESULT_SEQ_DIR=./result_sequential
RESULT_PAR_DIR=./result_parallel


all: sequential	parallel


sequential:	sequential.c
	${CC} -o $@ $^

parallel: parallel.c
	${CC} -o $@ $^ -l${LIB_PTHREAD}

clean:
	rm -f sequential
	rm -f parallel
	cd ${RESULT_SEQ_DIR} && rm -f * && cd ..
	cd ${RESULT_PAR_DIR} && rm -f * && cd ..

remove:
	cd ${RESULT_SEQ_DIR} && rm -f * && cd ..
	cd ${RESULT_PAR_DIR} && rm -f * && cd ..



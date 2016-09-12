SUBDIRS	:=	chap08 core memory timer

all : 
	@for dir in $(SUBDIRS); do\
		make -C $$dir || exit $?; \
		done
		

clean :
	@for dir in $(SUBDIRS); do \
		make -C $$dir clean; \
		done
		

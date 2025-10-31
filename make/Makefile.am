AUTOMAKE_OPTION = foreign

bin_PROGRAMS = helloworld

helloworld_SOURCES = hello_world.c hello_func.c hello_make.h

include_HEADERS = hello_make.h

clean-all: maintainer-clean

	-sudo rm -f $(DESTDIR)$(bindir)/$(bin_PROGRAMS)
	-sudo rm -f $(DESTDIR)$(includedir)/$(include_HEADERS)
	-rm -f aclocal.m4 configure compile depcomp install-sh missing Makefile.in
	-rm -rf autom4te.cache .deps
	-rm -f *~


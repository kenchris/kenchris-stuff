CSC = mcs

CSFILES =					\
	WordWeaver.cs				\

ASSEMBLIES = 					\
	-r:gtk-sharp	 			\
	-r:System.Drawing 			\
	-r:glib-sharp 				\
	-r:pango-sharp				\

bin_SCRIPTS = wordweaver

EXTRA_DIST =					\
	$(CSFILES)				\

CLEANFILES =					\
	wordweaver.exe

wordweaver.exe: $(CSFILES)
	$(CSC) -o $@ $(CSFLAGS) $(CSFILES) $(ASSEMBLIES) $(RESOURCES)

all: wordweaver.exe

clean:
	rm -rf $(CLEANFILES) *~

upload:
	scp * kenneth@master.gnome.org:~kenneth/public_html/WordWeaver

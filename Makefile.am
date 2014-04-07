AUTOMAKE_OPTIONS = dist-bzip2 no-dist-gzip

bin_SCRIPTS = skype
CLEANFILES = $(bin_SCRIPTS) skype.1 skype.txt
EXTRA_DIST = skype.in serializeimages.py skype.txt.in docgen.py rst2man.py
man_MANS = skype.1

skype: skype.in serializeimages.py
	python serializeimages.py $(PACKAGE_VERSION) $(datadir)/applications < skype.in > skype
	chmod +x skype

skype.1: skype.txt.in skype docgen.py
	python docgen.py $(PACKAGE_VERSION) < skype.txt.in > skype.txt
	$(RST2MAN) skype.txt > skype.1

SUBDIRS = data src

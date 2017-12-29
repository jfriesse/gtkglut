default:htmldoc

htmldoc:
	doxygen doxygen.cfg

pdfdoc:
	$(MAKE) -C latex refman.pdf

clean:
	rm -rf html latex doxygen.log

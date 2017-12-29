all:demos.d redbook.d tests.d

tests.d:
	$(MAKE) -C tests

demos.d:
	$(MAKE) -C demos

redbook.d:
	$(MAKE) -C redbook

clean:
	$(MAKE) -C redbook clean
	$(MAKE) -C demos clean
	$(MAKE) -C tests clean



install:
	cat qb64.1 | gzip -9 > /usr/share/man/man1/qb64.1.gz
	cp qb64.desktop /usr/share/applications/
	cp internal/source/qb64icon32.png /usr/share/icons/

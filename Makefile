CFLAGS+=-Wall
LDFLAGS+=
LDLIBS+=-lusb

.PHONY: all clean

all: riech-o-mat-backend riech-o-mat-backend-iowarrior

riech-o-mat-backend-iowarrior: riech-o-mat-backend-iowarrior.o iowkit.o

riech-o-mat-backend: riech-o-mat-backend-iowarrior
	ln -s riech-o-mat-backend-iowarrior riech-o-mat-backend

# Run this target as root.
install-udev-rule:
	cp 99-iowarrior.rules /etc/udev/rules.d/

# Run this target as simple user.
install-desktop-entry:
	cp riech-o-mat.desktop ~/.local/share/applications/

riech-o-mat.ico: riech-o-mat.svg
	inkscape riech-o-mat.svg --export-dpi 240 --export-png riech-o-mat-128.png
	inkscape riech-o-mat.svg --export-dpi 120 --export-png riech-o-mat-64.png
	inkscape riech-o-mat.svg --export-dpi 60 --export-png riech-o-mat-32.png
	inkscape riech-o-mat.svg --export-dpi 30 --export-png riech-o-mat-16.png
	convert riech-o-mat-16.png riech-o-mat-32.png riech-o-mat-64.png riech-o-mat-128.png riech-o-mat.ico

clean:
	rm -f riech-o-mat-backend riech-o-mat-backend-iowarrior riech-o-mat-backend-iowarrior.o iowkit.o


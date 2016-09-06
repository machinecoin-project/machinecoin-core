
Debian
====================
This directory contains files used to package machinecoind/machinecoin-qt
for Debian-based Linux systems. If you compile machinecoind/machinecoin-qt yourself, there are some useful files here.

## machinecoin: URI support ##


machinecoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install machinecoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your machinecoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/machinecoin128.png` to `/usr/share/pixmaps`

machinecoin-qt.protocol (KDE)


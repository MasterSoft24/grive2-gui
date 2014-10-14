Grive2 GUI v0.1 (grive2-gui)

8 October 2014

Grive2 GUI is a set various dialogs for customize sync process settings. 
It is used by grive2m, but can also be caused independently

This program uses Qt5

grive2-gui uses qmake to build:

1. Download source code archive
2. Unpack
3. cd to unpacking directory
4. mkdir build
5. cd build
6. qmake ../
7. make

Resulting binary file you can copy to destination folder e.g. /usr/bin

Notice for Ubuntu users:
	If application tray icon shows on top left corner instead system tray area you need make follow actions:
	
	sudo apt-add-repository ppa:gurqn/systray-trusty
	sudo apt-get update
	sudo apt-get upgrade
	
	and reboot...
	
This is known bugs. See 
http://stoflru.org/out.php?link=https://bugreports.qt-project.org/browse/QTBUG-31762?page=com.atlassian.jira.plugin.system.issuetabpanels:all-tabpanel
for details.
This problem has been reproduced at least on Ubuntu 14.04

USAGE:
	grive2-gui account - show account preferences dialog
	
	grive2-gui filelist - show select files to sync dialog
	

Enjoy!

Version History:

	0.1:
		:GUI preferences dialogs for tune 
		
		 sync parameters and list of files for sync.
		

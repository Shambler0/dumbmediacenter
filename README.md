# dumbmediacenter

This project is work-in-progress and is not yet usable. 

Turns an old PC into a very simplistic mediaplayer. 
This is actually a simplistic fullscreen VLC shell/launcher

Runs on bare X Window under Linux and *BSD.
Written in C++ (mostly C), compiles through gcc.
Requires VLC to run.

WARNING! This program grabs keyboard globally and does not let go! Press Ctrl+F11 to quit it and release keyboard.

----------------------------------------------------------------
## How to build

* Install GCC C++
* Run 'make' in project directory


----------------------------------------------------------------
## How to install

* Install X Window and VLC
* Just copy the binary file 'dumbmediacenter' anywhere you like


----------------------------------------------------------------
## How to use

WARNING! This program grabs keyboard globally and does not let go! Press Ctrl+F11 to quit it and release keyboard.

As of this writing, the program is in early stages and does not do anything useful at all. It is just a bunch of function tests

It might delete your files (probably not) or hang your computer (much more likely)
Please do not attempt to start it unless I asked you to do so

* Start X Window
* Run 'dumbmediacenter' binary (without arguments) from wherever you put it
* Controls are F1-F12 to select mode, numpad 0-9 to select file, other keys to control player
* To exit press Ctrl+F11
* The manual is not done yet


----------------------------------------------------------------
## How to remove

* delete the binary file 'dumbmediacenter' and the source directory
* also delete file '~/.config/dumbmediacenter.txt' where '~' is your home directory path


----------------------------------------------------------------
## Copyright

License: Unlicense.  
See LICENSE file or http://unlicense.org

In short, you are free to use, distribute, modify, copy-paste code and sell this, and do not need to retain any copyright notices.

Original author: Shambler0


----------------------------------------------------------------
## Contact

Please browse my contacts here:
http://shambler0.livejournal.com/profile

Feel free to jabber-chat with me if you are interested in the project

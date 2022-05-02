What is WordWeaver
------------------

Bosk WordWeaver is a program for creating and maintaining dictionaries. 
WordWeaver stores the data as XML, so it is possible to transform the data 
with a XLST Style Sheet to any format you like, being HTML or the format 
needed by the layouter (when you are at the final step at getting you 
dictionary published)

WordWeaver is coded in C# using the known open-source GTK Toolkit. This 
means that WordWeaver should run under Linux, various UNIX versions (in 
the future including MacOS X) as well as Windows!


How to install on Linux
-----------------------

Copy wordweaver.exe and data.dict in the same dictionary.
For instance ~/wordweaver/
                                                                                
Download Mono and GtkSharp from http://www.go-mono.org/download.html
- or from Red Carpet

Run the application:

"mono ~/wordweaver/wordweaver.exe"


How to compile on Linux
-----------------------
                                                                                
Make sure that you have Mono, GtkSharp and all requirements installed.
                                                                                
Copy WordWeaver.cs and Makefile to the same dir
Change to the dir and type 'make'.


How to install on Windows
-------------------------

Copy wordweaver.exe and data.dict in the same dictionary.
For instance C:\WordWeaver\

Download Mono and GtkSharp from http://www.go-mono.org/download.html

It is known to work with:
http://www.go-mono.org/archive/0.30/windows/mono-0.30-win32-2.exe
http://www.go-mono.org/archive/0.30/windows/mono-0.30-gtksharp-0.15-win32-1.exe

Install the above (or newer versions)

Mono seems to require msvcr70.dll I have heard - I don't know how to fix 
this


Run the application from the Run menu (or from DOS)

"C:\PATH_TO_MONO\MONO.EXE C:\PATH_TO_WORDWEAVER\wordweaver.exe"

Replace PATH_TO_MONO with the path where you installed Mono. Also replace 
the PATH_TO_WORDWEAVER with you WordWeaver path.

Ex. "C:\Program Files\Mono-0.30\bin\mono.exe C:\WordWeaver\WordWeaver.exe"

The above is ONE command, ie. mono.exe takes an .exe file as argument!
Think of mono.exe as Java. Mono is a C# Runtime (JIT actually) that can 
run .exe programs coded in C#

If it works create a WordWeaver.bat file with the above line (without the 
"'s)

Enjoy, Kenneth

(C) Copyright Bosk Software 2004

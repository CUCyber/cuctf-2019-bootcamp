# Stego3

A common challenge in the Stego category involves files appended or embedded in other files. One of the best tools to use for this is binwalk. Binwalk iterates over files and checks to see if [magic numbers](https://en.wikipedia.org/wiki/Magic_number_(programming)) for common file types exist. If they do, it can extract them.

`binwalk --dd='.*' stego3.jpg`

The above command will extract a zip file, which, when unzipped, gives us the flag.

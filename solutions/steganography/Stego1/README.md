# Stego1

`IEND` marks the end of a PNG image. Text can be appended to an image and the file will still result in a valid picture. One of the easiest and first steps you can do on a stego problem with a picture is run strings. It doesn't take long at all and can quickly solve the easiest of stego problems.

```Bash
$ strings Stego1.png
...
IEND
BOOTCAMP{welcome_to_stego}
```

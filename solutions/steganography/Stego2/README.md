# Stego2

Steghide is one of the most common steganography tools for beginner CTFs. It embeds data into JPG or WAV files. If you get one of these file types in a steganography challenge, it's worth trying steghide if strings doesn't work.

```Bash
$ steghide extract -sf Stego2.jpg
Enter passphrase: 
wrote extracted data to "flag.txt".
$ cat flag.txt
BOOTCAMP{always_steghide_jpgs}
```

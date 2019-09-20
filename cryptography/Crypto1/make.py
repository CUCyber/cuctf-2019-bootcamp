import base64
FLAG = b"BOOTCAMP{base64_encoding}"
print(base64.b64encode(FLAG).decode())

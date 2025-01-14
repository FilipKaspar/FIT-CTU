some_bytes = b'\xA3\x90\x21\x11\xFB'
with open("config.txt", "wb") as binary_file:
    binary_file.write(some_bytes)    # Write your original bytes
    #binary_file.write(b'\n') 
import os, sys

IMG="floppy.img"
IMG_SIZE=1474560            # 1.44MB
SECTOR=512
def write_at(path, offset_sectors):
    with open(IMG, "r+b") as f, open(path, "rb") as b:
        f.seek(offset_sectors * SECTOR)
        f.write(b.read())

# create empty image
with open(IMG, "wb") as f:
    f.truncate(IMG_SIZE)

# paths (adjust if different)
files = [
    ("bootloader.bin", 0),   # sector 0
    ("stage2.bin",    1),    # sector 1
    ("kernel.bin",    9),    # sector 9 (must match KERNEL_LBA)
]

# verify files exist and write
for p, s in files:
    pp = os.path.join(os.path.dirname(__file__), p)
    if not os.path.isfile(pp):
        print("Missing:", pp); sys.exit(2)
    print("Writing", pp, "to sector", s)
    write_at(pp, s)

print("floppy image created:", os.path.abspath(IMG))
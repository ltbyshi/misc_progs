## Remove disk from LVM

Assume that the logical volume group (VG) is `$vg`,
the logical volume (LV) is `$lv`, and the physical volume (PV) to be removed is `$pv`.
1. Shrink the ext4 file system to minimal size
```bash
e2fsck -f /dev/mapper/${vg}-${lv}
resizefs -d 63 -M /dev/mapper/${vg}-${lv}
```
2. Find the size of the ext4 filesystem
```bash
dump2fs -h /dev/mapper/${vg}-${lv}
```
Find the two lines:
```
Block size:               4096
Block count:              420864
```
Then the size of the file system is: 4096*420864 = 1723858944 bytes.
3. Shrink the logical volume
Calculate the new size that is slightly larger than the filesystem for the logical volume.
For example, set the new size to 4096*430000 = 1761280000 bytes.
Then resize the logical volume:
```bash
lvresize -L 1761280000B /dev/mapper/${vg}-${lv}
```
4. Shrink the logical volume group
Find the maps of the physical volumns:
```bash
pvdisplay -v -m
```
It show something like:
```
--- Physical Segments ---
Physical extent 0 to 155:
  Logical volume	/dev/vgtest/lvtest
  Logical extents	255 to 410
Physical extent 156 to 254:
  FREE
```
If one of the physical volumes is already free, then remove the PV:
```bash
vgreduce /dev/mapper/${vg}-${lv} /dev/${pv}
```
If none of the PVs are free, then move data from the PV first:
```bash
pvmove /dev/${pv}:1000-1999 /dev/other:0-999
```

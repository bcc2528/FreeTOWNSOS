import os
import sys
import shutil
import subprocess

THISFILE=os.path.realpath(__file__)
THISDIR=os.path.dirname(THISFILE)


def Copy(file):
	shutil.copyfile(
		os.path.join(THISDIR,"..","resources",file),
		os.path.join(THISDIR,"..","release",file))

def Run(argv):
	files=[
		"IO.SYS",
		"TGBIOS.SYS",
		"TGBIOS.BIN",
		"FD_IPL.BIN",
		"FDIMG.BIN",
		"FDIMG_USEROM.BIN",
	]
	for file in files:
		Copy(file)
		subprocess.Popen([
			"git","add","-f",os.path.join(os.path.join(THISDIR,"..","release",file))]).wait()


if __name__=="__main__":
	Run(sys.argv[1:])
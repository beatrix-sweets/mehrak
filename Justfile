# Wont work because just executes commands in new a shell
# export:
# 	pushd ~/clonedRepos/esp-idf-v5.3.2/ && source export.sh && popd
# 	echo "esp32 ready to build"
monitor:
	picocom -b 115200 /dev/ttyUSB0
	# minicom -D /dev/ttyUSB0 -b 115200

logs:
	monitor

build:
	idf.py build

flash:
	idf.py flash
	

images:
	bmp_scripts/convert-to-header.sh bmp_scripts/images/bmp/* 

build-full:
	images
	idf.py build
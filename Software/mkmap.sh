
TOOLS="C:\arduino-0023\hardware\tools\avr\bin"
BUILD="C:/Users/besemerj/AppData/Local/Temp/build6267863249409647460.tmp"

# PATH="$TOOLS:$PATH"

date >> sizes.txt
$TOOLS/avr-size $BUILD/ControllerApp.cpp.elf >> sizes.txt
tail -12 sizes.txt

$TOOLS/avr-nm -n -C $BUILD/ControllerApp.cpp.elf > map.txt

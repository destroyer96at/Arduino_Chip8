import sys
import os

def EOF(f):
    current_pos = f.tell()
    file_size = os.fstat(f.fileno()).st_size
    return current_pos >= file_size
def main():
	count = 0;
	adress = 0x200;
	fileName = sys.argv[1]
	inFile = open(fileName, "rb")
	outFile = open("rom.h","w")
	outFile.write("#ifndef _ROM_H_\n#define _ROM_H_\n")
	outFile.write("uint8_t memory [] = {")
	while not EOF(inFile):
		count = count + 1
		if count > 1:
			outFile.write(",")
		byteCell = inFile.read(1)
		outFile.write(hex(ord(byteCell)))
		if (count % 2 == 0):
                    outFile.write("//"+ hex(adress)+ " " + hex(adress-0x200) +"\n")
                    adress = adress+1
                    
	print(str(count) + " Bytes written")
	outFile.write("};\nuint16_t length = " + str(count) + ";" )
	outFile.write("\n#endif")
	
	
if __name__ == "__main__":
	main()
	

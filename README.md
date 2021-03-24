# Freetype4UEFI
A example for UEFI to use free type 2
 
![screen shot](https://github.com/ay123net/uefift2/blob/master/20210323163153.jpg)

* Freetype version: 2.10.4
* Freetype download: https://download.savannah.gnu.org/releases/freetype/

## Build
* Put the project file to the folder AppPkg/Applications
* Add Freetype.inf to AppPkg/AppPkg.dsc
* Build: build -p AppPkg/AppPkg.dsc
* Support build tools: GCC and VS

## Run
* Put the font file to the efi file folder
* Run in uefi shell 

## Author
* Email: cbaoyun@gmail.com
* Blog: https://ay123.net

## Reference
* [FreeType2使用总结](https://blog.csdn.net/finewind/article/details/38009731)
* [FreeType example1](https://www.freetype.org/freetype2/docs/tutorial/example1.c)

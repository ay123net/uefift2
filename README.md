# Freetype4UEFI
A example for UEFI to use freetype 2
 
![screen shot](https://github.com/ay123net/uefift2/blob/master/20210323163153.jpg)

* Freetype2 version: 2.10.4
* Freetype2 download: https://download.savannah.gnu.org/releases/freetype/

## Build
* Put the project file to the folder AppPkg/Applications
* Add Freetype.inf to AppPkg/AppPkg.dsc
* Build: build -p AppPkg/AppPkg.dsc
* Support build tools: GCC and VS

## Run
* Put the ttc font file to the efi file folder
* Run in uefi shell 

## Author
* Email: cbaoyun@gmail.com
* Blog: https://ay123.net

## Reference
* [FreeType2使用总结](https://blog.csdn.net/finewind/article/details/38009731)
* [FreeType example1](https://www.freetype.org/freetype2/docs/tutorial/example1.c)
* [UEFI_FreeType_MSVC](https://github.com/tenpoku1000/UEFI_FreeType_MSVC/)

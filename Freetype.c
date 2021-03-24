/*
 *
 * This is an example for UEFI to use freetype2
 *
 * GitHub   : https://github.com/ay123net/uefift2
 * Gitee    : https://gitee.com/ay123net/uefift2
 *
 * Author   : Baoyun Chen
 * Page     : Https://ay123.net
 * 
 */

#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HiiLib.h>
#include <Library/UefiHiiServicesLib.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <ft2build.h>
#include FT_FREETYPE_H

EFI_GUID    mPackageListGuid = {0xd8ebc548, 0xd7ea, 0x46e6, {0x98, 0x0d, 0x0e, 0xa0, 0x04, 0xfd, 0xc7, 0x00}};

EFI_STATUS 
AllocateBltBuffer (
    IN   EFI_GRAPHICS_OUTPUT_PROTOCOL     *  GraphicsOutput,
    IN   UINTN                               SourceX,
    IN   UINTN                               SourceY,
    IN   UINTN                               Width,
    IN   UINTN                               Height,
    IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL **  BltBuffer
)
{
    EFI_STATUS Status;

    *BltBuffer = AllocateZeroPool(Width * Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));

    Status =  GraphicsOutput->Blt(
                GraphicsOutput,
                *BltBuffer,
                EfiBltVideoToBltBuffer,
                SourceX, SourceY,
                0, 0,
                Width,
                Height,
                0
    );

    return Status;
}

VOID 
BitmapToUefiBltBuffer (
    IN UINT8 * Bitmap,
    IN UINTN   Width,
    IN UINTN   Height,
    IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL * BltBuffer
)
{
    UINTN Rows;
    UINTN Columns;
    UINTN Color;

    for (Rows = 0; Rows < Height; Rows ++) {
        for (Columns = 0; Columns < Width; Columns ++) {
            if (Bitmap[Rows * Width + Columns]) {
                
                /* Fill with white */
                Color = 0xFF;
                BltBuffer[Rows * Width + Columns].Blue  = Color;
                BltBuffer[Rows * Width + Columns].Green = Color;
                BltBuffer[Rows * Width + Columns].Red   = Color; 
            }
        }
    }
}

EFI_STATUS
DrawChar (
    IN   EFI_GRAPHICS_OUTPUT_PROTOCOL  * GraphicsOutput,
    IN   UINTN                           DestinationX,
    IN   UINTN                           DestinationY,
    IN   UINTN                           Width,
    IN   UINTN                           Height,
    IN   EFI_GRAPHICS_OUTPUT_BLT_PIXEL * BltBuffer
)
{
    EFI_STATUS Status;

    Status =  GraphicsOutput->Blt(
                GraphicsOutput,
                BltBuffer,
                EfiBltBufferToVideo,
                0, 0,
                DestinationX, DestinationY,
                Width,
                Height,
                0
    );

    return Status;
}


int main(int argc, char **argv)
{
	FT_Library	                     library;
	FT_Face		                     face;
	FT_Error	                     error;
	FT_UInt		                     charIdx;

    EFI_STATUS                       Status;
    EFI_GRAPHICS_OUTPUT_PROTOCOL  *  GraphicsOutput;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *  BltBuffer;
    UINTN                            Width, Height;
    UINTN                            X, Y;
    UINTN                            Index;

    EFI_HANDLE     HiiHandle;
    EFI_STRING     String;
    //CHAR8        * Language = "en-US";
    CHAR8        * Language = "zh-Hans";

    HiiHandle = HiiAddPackages (&mPackageListGuid, gImageHandle, FreetypeStrings, NULL);  
    String    = HiiGetString (HiiHandle, STRING_TOKEN (STR_STRING), Language);

    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **) &GraphicsOutput);
    if (EFI_ERROR(Status)) {
        return 0;
    }

	/* Init FT2 */ 
	error = FT_Init_FreeType(&library);
 
	/* Load font */
	error = FT_New_Face(library, "./SourceHanSans-Normal.ttc", 0, &face);

    X = 200;
    Y = 200;

    for (Index = 0; String[Index] != 0; Index ++) {
        /* Set font size */
        error = FT_Set_Char_Size(face, (40 + Index * 15) * 64, (40 + Index * 15) * 64, 80, 80);

        /* Get graphy font char index */
        charIdx = FT_Get_Char_Index(face, String[Index]);
    
        /* Load char graphy */
        FT_Load_Glyph(face, charIdx, FT_LOAD_COLOR);

        /* Get bitmap */
        if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
        {
            FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        }

        Width  = face->glyph->bitmap.width;
        Height = face->glyph->bitmap.rows;

        AllocateBltBuffer(GraphicsOutput, X, Y, Width, Height, &BltBuffer);
        BitmapToUefiBltBuffer(face->glyph->bitmap.buffer, Width, Height, BltBuffer);
        DrawChar(GraphicsOutput, X, Y, Width, Height, BltBuffer);

        X += (Width + 10);
    }

    return 0;
}
/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   Skeleton_png;
    const int            Skeleton_pngSize = 3296938;

    extern const char*   V_C1_png;
    const int            V_C1_pngSize = 20409;

    extern const char*   V_C2_png;
    const int            V_C2_pngSize = 20351;

    extern const char*   V_C3_png;
    const int            V_C3_pngSize = 20427;

    extern const char*   M_C_png;
    const int            M_C_pngSize = 358590;

    extern const char*   ADSR_C_png;
    const int            ADSR_C_pngSize = 22729;

    extern const char*   B1_Norm_png;
    const int            B1_Norm_pngSize = 1202;

    extern const char*   B2_Norm_png;
    const int            B2_Norm_pngSize = 1330;

    extern const char*   B3_Norm_png;
    const int            B3_Norm_pngSize = 1493;

    extern const char*   B1_Over_png;
    const int            B1_Over_pngSize = 1158;

    extern const char*   B2_Over_png;
    const int            B2_Over_pngSize = 1353;

    extern const char*   B3_Over_png;
    const int            B3_Over_pngSize = 1481;

    extern const char*   B1_Off_png;
    const int            B1_Off_pngSize = 1129;

    extern const char*   B2_Off_png;
    const int            B2_Off_pngSize = 1261;

    extern const char*   B3_Off_png;
    const int            B3_Off_pngSize = 1456;

    extern const char*   AvenirBook_ttf;
    const int            AvenirBook_ttfSize = 27432;

    extern const char*   AvenirRoman_ttf;
    const int            AvenirRoman_ttfSize = 27304;

    extern const char*   AvenirBlack_ttf;
    const int            AvenirBlack_ttfSize = 26980;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 18;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}

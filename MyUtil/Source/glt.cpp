#include <stdafx.h>
#include <glt.h>
#include <MySafeCast.h>

namespace glt {

// Define targa header. This is only used locally.
#pragma pack(1)
typedef struct
{
    GLbyte    identsize;              // Size of ID field that follows header (0)
    GLbyte    colorMapType;           // 0 = None, 1 = paletted
    GLbyte    imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
    unsigned short    colorMapStart;          // First colour map entry
    unsigned short    colorMapLength;         // Number of colors
    unsigned char     colorMapBits;   // bits per palette entry
    unsigned short    xstart;                 // image x origin
    unsigned short    ystart;                 // image y origin
    unsigned short    width;                  // width in pixels
    unsigned short    height;                 // height in pixels
    GLbyte    bits;                   // bits per pixel (8 16, 24, 32)
    GLbyte    descriptor;             // image descriptor
} TGAHEADER;
#pragma pack(8)

} // namespace glt

// TGADecoder
namespace glt {

static ankh::images::ImageFormatId const handledFormat("tga");

template <typename T>
static inline bool successful_whole_read (GenericReader& r, T& obj) {
    bool const readprecice(r.read(&obj, sizeof(T)));
    DASSERT(readprecice);
    return readprecice;
}

static inline bool successful_whole_read (GenericReader& r, void* const buf, size_t const bufbytesize) {
    bool const readprecice(r.read(buf, bufbytesize));
    DASSERT(readprecice);
    return readprecice;
}

ankh::images::Image* TGADecoder::Decode (
        GenericReader&                                reader,
        ankh::images::ImageCharacteristics const&    imgchar) {
    DASSERT(imgchar.fmt == handledFormat);

    TGAHEADER                    tgaHeader;        // TGA file header
    short                        depth;            // Pixel depth;
    GLuint                        width        (0);
    GLuint                        height        (0);
    ankh::images::PixelFormat    pixelFormat;

    // Read in header (binary)
    successful_whole_read(reader, tgaHeader);

    // Do byte swap for big vs little endian
#ifdef __APPLE__
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapStart);
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapLength);
    LITTLE_ENDIAN_WORD(&tgaHeader.xstart);
    LITTLE_ENDIAN_WORD(&tgaHeader.ystart);
    LITTLE_ENDIAN_WORD(&tgaHeader.width);
    LITTLE_ENDIAN_WORD(&tgaHeader.height);
#endif


    // Get width, height, and depth of texture
    width = tgaHeader.width;
    height = tgaHeader.height;
    depth = tgaHeader.bits / 8;
    DASSERT(tgaHeader.bits % 8 == 0);

    // Put some validity checks here. Very simply, I only understand
    // or care about 8, 24, or 32 bit targa's.
    if(depth != 1 && depth != 3 && depth != 4)
        return NULL;

    // Set OpenGL format expected
    switch(depth) {
#ifndef OPENGL_ES
        case 3:     // Most likely case
            pixelFormat = ankh::images::PixelFormats::BGR;
            break;
#endif
        case 4:
            pixelFormat = ankh::images::PixelFormats::BGRA;
            break;
        case 1:
            pixelFormat = ankh::images::PixelFormats::LUMINANCE;
            break;
        default:
            DASSERT(!"Invalid program state");
    }
    DASSERT(ankh::images::Bytesize(pixelFormat) == psafecast<size_t>(depth));

    using ankh::images::Image;
    // Allocate an image
    Image* image(DNEWCLASS(Image, (width, height, 1, imgchar, pixelFormat)));
    DASSERT(DPTR(image)->GetBytesize() == width * height * depth);

    // Read in the bits
    successful_whole_read(reader, DPTR(image)->GetBytes(), DPTR(image)->GetBytesize());

    return DPTR(image);
}

ankh::images::ImageFormatId const& TGADecoder::GetFormatId (void) const {
    return handledFormat;
}

bool TGADecoder::CanHandleFormat (ankh::images::ImageFormatId const& fmt) const {
    return handledFormat == fmt;
}

TGADecoder::TGADecoder (void):
    ankh::images::GenericReaderImageDecoder("[ImageDecoder/Superbible TGA/tga/Generic Reader]")
{}

TGADecoder::~TGADecoder (void) {}

} // namespace glt

namespace glt {

////////////////////////////////////////////////////////////////////
// Allocate memory and load targa bits. Returns pointer to new buffer,
// height, and width of texture, and the OpenGL format of data.
// Call free() on buffer when finished!
// This only works on pretty vanilla targas... 8, 24, or 32 bit color
// only, no palettes, no RLE encoding.
// This function also takes an optional final parameter to preallocated
// storage for loading in the image data.
GLbyte *ReadTGABits(
        const char*    szFileName,
        void*        (*memalloc) (size_t),
        void        (*memfree) (void*),
        GLenum*        pFormat,
        GLint*        pWidth,
        GLint*        pHeight,
        GLshort*    opt_pDepth,
        GLuint*        opt_pImageSize,
        GLint*        opt_pComponents,
        GLbyte*        opt_pData)
    {
    FILE *pFile;            // File pointer
    TGAHEADER tgaHeader;        // TGA file header
    unsigned long lImageSize;        // Size in bytes of image
    short sDepth;            // Pixel depth;
    GLbyte    *pBits = NULL;          // Pointer to bits

    // Default/Failed values
    GLuint iWidth = 0;
    GLuint iHeight = 0;
    GLuint iComponents = GL_RGB;
    GLenum eFormat = GL_RGB;

    // Attempt to open the file
    errno_t fopen_errno(fopen_s(&pFile, szFileName, "rb"));
    if(fopen_errno != 0)
        return NULL;

    // Read in header (binary)
    fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);

    // Do byte swap for big vs little endian
#ifdef __APPLE__
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapStart);
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapLength);
    LITTLE_ENDIAN_WORD(&tgaHeader.xstart);
    LITTLE_ENDIAN_WORD(&tgaHeader.ystart);
    LITTLE_ENDIAN_WORD(&tgaHeader.width);
    LITTLE_ENDIAN_WORD(&tgaHeader.height);
#endif


    // Get width, height, and depth of texture
    iWidth = tgaHeader.width;
    iHeight = tgaHeader.height;
    sDepth = tgaHeader.bits / 8;

    // Put some validity checks here. Very simply, I only understand
    // or care about 8, 24, or 32 bit targa's.
    if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
        return NULL;

    // Calculate size of image buffer
    lImageSize = psafecast<unsigned long>(tgaHeader.width * tgaHeader.height * sDepth);

    // Allocate memory and check for success
    if(opt_pData == NULL)
        pBits = (GLbyte*)(*memalloc)(lImageSize * sizeof(GLbyte));
    else
        pBits = opt_pData;

    // Read in the bits
    // Check for read error. This should catch RLE or other
    // weird formats that I don't want to recognize
    if(fread(pBits, lImageSize, 1, pFile) != 1)
        {
        if(pBits != NULL)
            (*memfree)(pBits);
        return NULL;
        }

    // Set OpenGL format expected
    switch(sDepth)
        {
#ifndef OPENGL_ES
        case 3:     // Most likely case
            eFormat = GL_BGR;
            iComponents = GL_RGB;
            break;
#endif
        case 4:
            eFormat = GL_BGRA;
            iComponents = GL_RGBA;
            break;
        case 1:
            eFormat = GL_LUMINANCE;
            iComponents = GL_LUMINANCE;
            break;
        default:        // RGB
            // If on the iPhone, TGA's are BGR, and the iPhone does not
            // support BGR without alpha, but it does support RGB,
            // so a simple swizzle of the red and blue bytes will suffice.
            // For faster iPhone loads however, save your TGA's with an Alpha!
#ifdef OPENGL_ES
    for(int i = 0; i < lImageSize; i+=3)
        {
        GLbyte temp = pBits[i];
        pBits[i] = pBits[i+2];
        pBits[i+2] = temp;
        }
#endif
        break;
        }

    // Done with File
    fclose(pFile);

    // Store information to outputs
    psafecast(*pWidth, iWidth);
    psafecast(*pHeight, iHeight);
    *pFormat = eFormat;
    if (opt_pDepth)
        *opt_pDepth = sDepth;
    if (opt_pImageSize)
        *opt_pImageSize = lImageSize;
    if (opt_pComponents)
        psafecast(*opt_pComponents, iComponents);

    // Return pointer to image data
    return pBits;
    }




} // namespace glt

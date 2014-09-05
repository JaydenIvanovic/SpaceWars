/* Texture loader for Linux using gdk-pixbuf-2.0 (a component of GTK).
 * Can load PNG, JPEG, GIF, BMP, TIFF, ..., etc.  
 *
 * Make sure the image has dimensions that are powers of two! (e.g., 32x64 is
 * ok, but 30x70 is not).
 */

#include "texture.h"

void freeImage(I3DImage *image){
    free(image->data);
    free(image);
}

bool isPowerOf2(const unsigned int val){
    return val && !(val & (val-1));
}

/* Flip the rows of the image data in-place */
void flipData(unsigned char *data, const unsigned int rowstride, const unsigned int height){
    unsigned char *row1 = data, *row2 = data + (height - 1) * rowstride, tmp;
    unsigned int x, y;

    for (y = 0; y < (height >> 1); y++){
        for (x = 0; x < rowstride; x++){
            tmp = row1[x];
            row1[x] = row2[x];
            row2[x] = tmp;
        }

        row1 += rowstride;
        row2 -= rowstride;
    }
}

GLuint loadTexture(I3DImage *image){
    GLuint id;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, image->internalFormat, image->width, image->height, 
                    0, image->format, image->type, image->data);

    //Use GL_NEAREST filtering and see the difference in results
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glBindTexture(GL_TEXTURE_2D, 0);

    return id;
}

I3DImage* loadImage(char *filename){
    I3DImage *image;
#ifdef __APPLE__
    I3DImage *image;
    FSSpec fss;
    GraphicsImportComponent gi;
    Rect rect;
    GWorldPtr world;
    
    image = (I3DImage *)malloc(sizeof(I3DImage));

    NativePathNameToFSSpec(filename, &fss);
    GetGraphicsImporterForFile(&fss, &gi);
    GraphicsImportGetNaturalBounds(gi, &rect);

    image->width = rect.right;
    image->height = rect.bottom;
    int pitch = width * 4;
    image->data = (unsigned char *)malloc(pitch * height * sizeof(unsigned char));
    QTNewGWorldFromPtr(&world, k32ARGBPixelFormat, &rect, NULL, NULL, 0,
                       image->data, pitch);

    GraphicsImportSetGWorld(gi, world, NULL);
    GraphicsImportDraw(gi);
    DisposeGWorld(world);
    CloseComponent(gi);

    if (pitch < 0){
        pitch = -pitch;
        flipData(image->data, pitch, image->height);
    }

    image->internalFormat = GL_RGBA;
    image->format = GL_BGRA;
    image->type = ARGB_TYPE;
#else
    GdkPixbuf *pixbuf;

    int channels,rowstride, bps;
    
    g_type_init();
    
    if (!(pixbuf = gdk_pixbuf_new_from_file(filename, NULL))){
        fprintf(stderr, "Could not load file %s\n", filename);
        return NULL;
    }
    
    image = (I3DImage *)malloc(sizeof(I3DImage));

    image->width = gdk_pixbuf_get_width(pixbuf);
    image->height = gdk_pixbuf_get_height(pixbuf);
    image->type = GL_UNSIGNED_BYTE;

    if (isPowerOf2(image->width) && isPowerOf2(image->height)){
        rowstride = gdk_pixbuf_get_rowstride(pixbuf);
        channels = gdk_pixbuf_get_n_channels(pixbuf);
	bps = gdk_pixbuf_get_bits_per_sample(pixbuf);  

        unsigned char *data;
        unsigned int length = rowstride * image->height + 
			image->width * ((channels * bps + 7) / 8);
        data = gdk_pixbuf_get_pixels(pixbuf);

        image->data = (unsigned char *)malloc(length * sizeof(unsigned char));
        memcpy(image->data, data, length);

        switch (channels) {
            case 3:
                image->internalFormat = image->format = GL_RGB;
                break;
            case 4:
                image->internalFormat = image->format = GL_RGBA;
                break;
            default:
                fprintf(stderr, "Unsuported number of image channels");
        }

        /* If rowstride is negative, flip order of rows from top-to-bottom to
           bottom-to-top. */
        if (rowstride < 0){
            rowstride = -rowstride;
            flipData(image->data, rowstride, image->height);
        }
    }else{
        fprintf(stderr, "Image %s does not have dimensions of power 2\n", 
                filename);
        free(image);
        image = NULL;
    }

    g_object_unref(pixbuf);
#endif
    return image;
}

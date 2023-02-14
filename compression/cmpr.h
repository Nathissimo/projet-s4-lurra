#ifndef _CMPR_H_
#define _CMPR_H_

typedef struct comprime
{
    unsigned long int donnee;
    unsigned int lon;
    unsigned int lag;
}comprime;

typedef struct
{
    GLuint  width;           /* largeur */
    GLuint  height;          /* hauteur */
    GLenum  format;          /* RVB, RVBA, Luminance, Luminance Alpha */
    GLint   internalFormat;  /* composantes d'un texel */
    GLubyte *texels;         /* données de l'image */
} gl_texture_t;

comprime rle(int *image, int lon, int lag);
void *derle(comprime rslt, int *image);

#endif



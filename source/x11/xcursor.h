#ifndef __X_CURSOR_H__
#define __X_CURSOR_H__

struct XcursorImages {
	U32	nimage; /* number of images */
	U32 imagesAddress; /* array of XcursorImage pointers */
	U32 nameAddress; /* name used to load images */

	void read(KMemory* memory, U32 address);
	static void write(KMemory* memory, U32 address, U32 nimage, U32 imageAddress);
};

struct XcursorImage {
	U32	    version;	/* version of the image data */
	U32	    size;	/* nominal size for matching */
	U32	    width;	/* actual width */
	U32	    height;	/* actual height */
	U32	    xhot;	/* hot spot x (must be inside image) */
	U32	    yhot;	/* hot spot y (must be inside image) */
	U32	    delay;	/* animation delay to next frame (ms) */
	U32     pixelsAddress;	/* pointer to pixels */

	void read(KMemory* memory, U32 address);
	static void write(KMemory* memory, U32 address, U32 width, U32 height, U32 pixels);
};

class XCursor {
public:
	XCursor(U32 pixmap, U32 mask, const XColor& fg, const XColor& bg, U32 x, U32 y);
	XCursor(U32 shape);

	const U32 id;
	U32 pixmap = 0;
	U32 mask = 0;
	XColor fg = { 0 };
	XColor bg = { 0 };
	U32 x = 0;
	U32 y = 0;
	U32 shape = 0;
};

typedef std::shared_ptr<XCursor> XCursorPtr;

#endif
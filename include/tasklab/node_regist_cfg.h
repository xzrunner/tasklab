#ifndef EXE_FILEPATH
#error "You must define EXE_FILEPATH macro before include this file"
#endif

// io

#define PARM_NODE_CLASS ReadImage
#define PARM_NODE_NAME read_image
#include EXE_FILEPATH

#define PARM_NODE_CLASS WriteImage
#define PARM_NODE_NAME write_image
#include EXE_FILEPATH

// image

#define PARM_NODE_CLASS Mipmap
#define PARM_NODE_NAME mipmap
#include EXE_FILEPATH

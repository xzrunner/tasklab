#ifndef EXE_FILEPATH
#error "You must define EXE_FILEPATH macro before include this file"
#endif

// io

#define PARM_NODE_CLASS ReadImage
#define PARM_NODE_NAME read_img
#include EXE_FILEPATH

#define PARM_NODE_CLASS WriteImage
#define PARM_NODE_NAME write_img
#include EXE_FILEPATH

// image

#define PARM_NODE_CLASS CropImage
#define PARM_NODE_NAME crop_img
#include EXE_FILEPATH

#define PARM_NODE_CLASS Mipmap
#define PARM_NODE_NAME mipmap
#include EXE_FILEPATH

#define PARM_NODE_CLASS SplitImage
#define PARM_NODE_NAME split_img
#include EXE_FILEPATH

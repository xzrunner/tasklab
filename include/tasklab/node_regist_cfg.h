#ifndef EXE_FILEPATH
#error "You must define EXE_FILEPATH macro before include this file"
#endif

// file

#define PARM_NODE_CLASS FileCompress
#define PARM_NODE_NAME file_compress
#include EXE_FILEPATH

#define PARM_NODE_CLASS FilePattern
#define PARM_NODE_NAME file_pattern
#include EXE_FILEPATH

#define PARM_NODE_CLASS FileRemove
#define PARM_NODE_NAME file_remove
#include EXE_FILEPATH

#define PARM_NODE_CLASS FileUncompress
#define PARM_NODE_NAME file_uncompress
#include EXE_FILEPATH

#define PARM_NODE_CLASS MakeDirectory
#define PARM_NODE_NAME make_dir
#include EXE_FILEPATH

// io

#define PARM_NODE_CLASS BuildVTex
#define PARM_NODE_NAME build_vtex
#include EXE_FILEPATH

#ifndef SKIP_FILE_NODE
#define PARM_NODE_CLASS ReadImage
#define PARM_NODE_NAME read_img
#include EXE_FILEPATH
#endif // SKIP_FILE_NODE

#ifndef SKIP_FILE_NODE
#define PARM_NODE_CLASS WriteImage
#define PARM_NODE_NAME write_img
#include EXE_FILEPATH
#endif // SKIP_FILE_NODE

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

// tools

#define PARM_NODE_CLASS Group
#define PARM_NODE_NAME group
#include EXE_FILEPATH

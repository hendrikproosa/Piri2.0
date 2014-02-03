#ifndef PIRILIB_H
#define PIRILIB_H

#include "pirilib_global.h"

#define DAG_MODE_PAN        0
#define DAG_MODE_SELECT     1
#define DAG_MODE_INSERTEDGE 2
#define DAG_MODE_INSERTNODE 3
#define DAG_MODE_BREAKEDGE  4

#define NODE_TYPE_VIEWER    0
#define NODE_TYPE_DEFAULT   1
#define NODE_TYPE_READ      2
#define NODE_TYPE_WRITE     3
#define NODE_TYPE_GEOMETRY  4
#define NODE_TYPE_DOT       99

#define NODE_DRAW_WIDTH     40
#define NODE_DRAW_HEIGHT    20
#define NODE_DRAW_RADIUS    2
#define NODE_DRAW_RADIUS2   5
#define NODE_DRAW_RADIUS3   15
#define NODE_DRAW_RADIUS_DOT 5
#define NODE_DRAW_TEXTSIZE  10

#define EDGE_TYPE_VIEWER    0
#define EDGE_TYPE_DEFAULT   1
#define EDGE_TYPE_BASE      2
#define EDGE_TYPE_MASK      4

#define EDGE_NOT_MAINEDGE   0
#define EDGE_IS_MAINEDGE    1

#define EDGE_ARROWSIZE      10
#define EDGE_BBOX_PENWIDTH  8

class PIRILIBSHARED_EXPORT PiriLib
{
    
public:
    PiriLib();
};

#endif // PIRILIB_H

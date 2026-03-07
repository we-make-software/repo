#pragma once
#include ".setup"

#define Get(obj,field)              ((obj)->field)

#define Set(obj,field,value)        ((obj)->field=(value))

#define Or(obj,field,mask)          ((obj)->field|=(mask))
#define And(obj,field,mask)         ((obj)->field&=(mask))
#define Xor(obj,field,mask)         ((obj)->field^=(mask))

#define Clear(obj,field,mask)       ((obj)->field&=~(mask))

#define Has(obj,field,mask)         (((obj)->field&(mask))!=0)
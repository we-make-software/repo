#pragma once
#include ".h"

#define StructListNode(...) struct list_head __VA_ARGS__

#define InitListManyPtr(...) \
    ({ \
        struct list_head *lists[] = { __VA_ARGS__, NULL }; \
        int i = 0; \
        while (lists[i]) { \
            INIT_LIST_HEAD(lists[i]); \
            i++; \
        } \
        0; \
    })

#define InitListMany1(a)                 InitListManyPtr(&(a))
#define InitListMany2(a,b)               InitListManyPtr(&(a),&(b))
#define InitListMany3(a,b,c)             InitListManyPtr(&(a),&(b),&(c))
#define InitListMany4(a,b,c,d)           InitListManyPtr(&(a),&(b),&(c),&(d))
#define InitListMany5(a,b,c,d,e)         InitListManyPtr(&(a),&(b),&(c),&(d),&(e))
#define InitListMany6(a,b,c,d,e,f)       InitListManyPtr(&(a),&(b),&(c),&(d),&(e),&(f))

#define _PickInitListMany(_1,_2,_3,_4,_5,_6,NAME,...) NAME
#define InitListMany(...) \
    _PickInitListMany(__VA_ARGS__, \
        InitListMany6,InitListMany5,InitListMany4,InitListMany3,InitListMany2,InitListMany1 \
    )(__VA_ARGS__)


/* basic node and head helpers */
#define InitList(node)                      INIT_LIST_HEAD(&(node))
#define ListEmpty(head)                     list_empty(&(head))
#define ListIsLast(node,head)               list_is_last(&(node), &(head))
#define ListIsSingular(head)                list_is_singular(&(head))

#define AddTop(node,head)                   list_add(&(node), &(head))
#define AddBottom(node,head)                list_add_tail(&(node), &(head))

#define DelList(node)                       list_del(&(node))
#define DelInitList(node)                   list_del_init(&(node))

/* safe in if/else, no do while */
#define MoveToTop(node,head)                ({ list_del(&(node)); list_add(&(node), &(head)); })
#define MoveToBottom(node,head)             ({ list_del(&(node)); list_add_tail(&(node), &(head)); })

/* name you already use */
#define UpdateList(node,head)               MoveToTop(node,head)

/* splice and replace */
#define SpliceList(lst,head)                list_splice(&(lst), &(head))
#define SpliceTailList(lst,head)            list_splice_tail(&(lst), &(head))
#define ReplaceList(old_node,new_node)      list_replace(&(old_node), &(new_node))

/* entry helpers */
#define ListEntry(ptr,type,member)          list_entry((ptr), type, member)
#define FirstEntry(head,type,member)        list_first_entry(&(head), type, member)
#define LastEntry(head,type,member)         list_last_entry(&(head), type, member)
#define NextEntry(pos,member)               list_next_entry((pos), member)
#define PrevEntry(pos,member)               list_prev_entry((pos), member)

/* iteration */
#define ForEach(pos,head)                           list_for_each((pos), &(head))
#define ForEachSafe(pos,n,head)                     list_for_each_safe((pos), (n), &(head))
#define ForEachEntry(pos,head,member)               list_for_each_entry((pos), &(head), member)
#define ForEachEntrySafe(pos,n,head,member)         list_for_each_entry_safe((pos), (n), &(head), member)

/* declare heads */
#define DefineListHead(name)                 LIST_HEAD(name)
#define DefineListHeadInit(name)             ((struct list_head)LIST_HEAD_INIT(name))
#define StaticDefineListHead(name)           static LIST_HEAD(name)
/*
How to use these list macros

1) Node inside a struct

DefineStruct(item){
    int id;
    StructListNode(node);
};

2) Create a list head

DefineListHead(items);

or static global

StaticDefineListHead(items);

or value init

struct list_head items2 = DefineListHeadInit(items2);

3) Init nodes

struct item a,b,c;

InitList(a.node);
InitList(b.node);
InitList(c.node);

or init many at once (no need to type &)

InitListMany(a.node,b.node,c.node);

4) Add to list

AddTop(a.node,items);
AddBottom(b.node,items);

5) Move existing node

MoveToTop(b.node,items);
MoveToBottom(a.node,items);

6) Remove from list

DelList(a.node);
DelInitList(b.node);

7) Iterate

struct item *it;

ForEachEntry(it,items,node){
    use it->id;
}

Safe iterate when you delete while iterating

struct item *tmp;

ForEachEntrySafe(it,tmp,items,node){
    if(condition){
        DelList(it->node);
    }
}

8) Check list state

if(ListEmpty(items)){
    empty list
}

if(ListIsSingular(items)){
    only 1 element
}
*/
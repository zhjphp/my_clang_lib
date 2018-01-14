#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LL_NODE_SIZE (sizeof(struct ll_node) + 50) //节点占用内存大小

/*
    节点结构
    pre: 前一个节点
    next: 后一个节点
    data： 节点所存储数据的指针
*/
struct ll_node
{
    struct ll_node *pre;
    struct ll_node *next;
    void *data;
};

/*
    链表结构
    top: 头结点
    end: 尾节点
*/
struct ll
{
    struct ll_node *top;
    struct ll_node *end;
};

/*
    创建节点
    data: 存入数据指针
    d_size: 存入数据大小
    返回: 节点指针
*/
struct ll_node *ll_create_node(void *data, size_t d_size)
{
    struct ll_node *node; //节点指针
    void *n_data;         //节点数据指针

    //分配节点基础内存
    node = malloc(LL_NODE_SIZE);
    if (data == NULL)
        return NULL;

    //分配节点数据存储内存
    n_data = malloc(d_size);
    if (n_data == NULL)
        return NULL;

    //复制数据到节点存储
    n_data = memcpy(n_data, data, d_size);
    node->data = n_data;

    //返回节点指针
    return node;
}

/*
    追加链表末尾
    node: 节点指针
    list: 链表指针
*/
void ll_append(struct ll_node *node, struct ll *list)
{
    //把原尾节点设为当前追加节点的前一个节点
    node->pre = list->end;
    //由于追加节点为尾节点，把下一个节点设为空
    node->next = NULL;

    //改变原尾节点的next值
    if (list->end != NULL)
        list->end->next = node;

    //改变链表的尾结点指针为新追加节点
    list->end = node;
    //如果是空链表进入的第一个节点，则把头节点也设为这个节点
    if (list->top == NULL)
        list->top = node;
}

/*
    压入链表顶部
    node: 节点指针
    list: 链表指针
*/
void ll_push(struct ll_node *node, struct ll *list)
{
    //把原头节点设尾当前压入节点的后一个节点
    node->next = list->top;
    //由于压入节点为头节点，把上一个节点设为空
    node->pre = NULL;

    //改变链表的头结点指针为新压入节点
    list->top = node;
    //如果是空链表进入的第一个节点，则把尾节点也设为这个节点
    if (list->end == NULL)
        list->end == node;
}

/*
    从链表尾部弹出节点
    list: 链表指针
    pop_node: 弹出节点
    返回: 弹出节点指针
*/
struct ll_node *ll_pop(struct ll *list, struct ll_node *pop_node)
{
    //赋值弹出节点
    pop_node = list->end;
    //赋值新尾节点尾原尾节点的前一个节点
    list->end = list->end->pre;
    //把新尾节点的下一个节点设为空
    list->end->next = NULL;

    return pop_node;
}

/*
    从链表顶部弹出节点
    list: 链表指针
    pop_node: 弹出节点
    返回: 弹出节点指针
*/
struct ll_node *ll_shift(struct ll *list, struct ll_node *pop_node)
{
    //赋值弹出节点
    pop_node = list->top;
    //赋值新头节点尾原头节点的下一个节点
    list->top = list->top->next;
    //把新头节点的前一个节点设为空
    list->top->pre = NULL;

    return pop_node;
}

/*
    具体节点按类型比较的实现方法
    src: 原数据指针
    dst: 需要比较的数据指针
    返回: 比较结果，0: 不相同， 1: 相同
*/
int ll_data_compare_int(const void *src, const void *dst)
{
    if (*(int *)src == *(int *)dst)
        return 1;
    return 0;
}

/*
    节点查找
    list: 链表指针
    ll_data_compare: 比较数据是否一致的函数指针
    data: 需要查找数据的指针
    find_node: 查找的节点指针
    返回: 查找的节点指针
*/
struct ll_node *ll_find(struct ll *list, int (*ll_data_compare)(const void *, const void *), void *data, struct ll_node *find_node)
{
    struct ll_node *node_temp; //临时节点
    //从头结点开始遍历
    node_temp = list->top;
    int res = 0; //比较结果，0: 不相同， 1: 相同

    //遍历链表
    for (; node_temp != NULL; node_temp = node_temp->next)
    {
        //比较需要查找的值是否等于
        res = ll_data_compare(data, node_temp->data);
        if (res)
            return node_temp;
    }

    return NULL;
}

/*
    插入节点
    list: 链表指针
    insert_node: 需要插入的节点
    pre_node: 插入节点的前一个节点
*/
void ll_insert(struct ll *list, struct ll_node *pre_node, struct ll_node *insert_node)
{
    //设置插入节点的前后节点
    insert_node->pre = pre_node;
    insert_node->next = pre_node->next;
    //改变插入节点前一个节点的next值
    pre_node->next = insert_node;
    ///改变插入节点后一个节点的pre值
    insert_node->next->pre = insert_node;
}

/*
    更新节点数据
    update_node: 需要更新的节点指针
    data: 更要更新的节点数据指针
*/
void ll_update(struct ll_node *update_node, void *data)
{
    update_node->data = data;
}

/*
    删除节点，【并不释放内存】
    delete_node: 需要删除的节点指针
*/
void ll_delete(struct ll_node *delete_node)
{
    //设置删除节点的前一个节点的next值尾删除节点的next值
    delete_node->pre->next = delete_node->next;
}

void ll_free(struct ll_node *delete_node)
{
    free(delete_node->data);
    delete_node->pre = NULL;
    delete_node->next = NULL;
    delete_node->data = NULL;
}

/*
    测试用，遍历输出链表节点
    list: 链表指针
*/
void test_ll_echo(struct ll *list)
{
    struct ll_node *node_temp; //临时节点
    node_temp = list->top;
    int i = 0;
    //遍历链表
    for (; node_temp != NULL; node_temp = node_temp->next, i++)
    {
        printf("%d = %d \n", i, *(int *)(node_temp->data));
    }
}

int main(int argc, char *argv[])
{
    struct ll *list;
    list = malloc(sizeof(struct ll));

    int a = 1011;
    int b = 1000;
    int c = 10;
    int d = 100;
    struct ll_node *node;

    node = ll_create_node(&a, sizeof(a));
    ll_append(node, list);

    node = ll_create_node(&b, sizeof(b));
    ll_push(node, list);

    node = ll_create_node(&c, sizeof(c));
    ll_push(node, list);

    node = ll_create_node(&d, sizeof(d));
    ll_append(node, list);

    //printf("%d\n", *((int *)node->data));
    printf("top = %d\n", *((int *)list->top->data));
    printf("end = %d\n", *((int *)list->end->data));

    struct ll_node *find_node;
    find_node = ll_find(list, ll_data_compare_int, &c, find_node);
    printf("find = %d\n", *((int *)find_node->data));

    test_ll_echo(list);
    printf("\n");

    node = ll_pop(list, node);
    printf("pop = %d\n", *((int *)node->data));

    test_ll_echo(list);
    printf("\n");

    node = ll_shift(list, node);
    printf("shift = %d\n", *((int *)node->data));

    test_ll_echo(list);

    node = ll_create_node(&c, sizeof(c));
    ll_push(node, list);

    node = ll_create_node(&d, sizeof(d));
    ll_append(node, list);

    test_ll_echo(list);
    printf("\n");

    int e = 9999;
    struct ll_node *insert_node;
    insert_node = ll_create_node(&e, sizeof(e));
    find_node = ll_find(list, ll_data_compare_int, &b, find_node);
    ll_insert(list, find_node, insert_node);

    test_ll_echo(list);
    printf("\n");

    /*
    删除节点测试
    int f = 8888;
    int *ff;
    ff = malloc(sizeof(int));
    ff = &f;
    find_node = ll_find(list, ll_data_compare_int, &a, find_node);
    insert_node = ll_create_node(ff, sizeof(f));
    ll_insert(list, find_node, insert_node);

    test_ll_echo(list);
    printf("\n");

    ll_delete(insert_node);
    printf("delete = %d\n", *((int *)insert_node->data));

    test_ll_echo(list);
    printf("\n");

    ll_free(insert_node);
    free(insert_node);
    insert_node = NULL;
    printf("free = %d\n", *((int *)insert_node->data));
    printf("free->next = %p\n", insert_node->next);
    */

    int g = 777;
    struct ll_node *update_node;
    update_node = ll_find(list, ll_data_compare_int, &c, find_node);
    ll_update(update_node, &g);
    printf("update = %d\n", *((int *)update_node->data));
    test_ll_echo(list);
    printf("\n");

    return 0;
}
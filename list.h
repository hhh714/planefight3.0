//记录飞机、子弹相关数据
#include<stdlib.h>
#include<assert.h>
//链表结点的结构体
typedef struct NODE {
	int x;
	int y;
	int speed;
	//指针，用来指向下一个节点
	struct NODE* next;
	

}Node;
//整个链表结构的节点
typedef struct LinkList
{
	struct NODE* head;
	struct NODE* end;
}LL;
//创建链表节点函数
NODE* Node_init(int x,int y,int speed)
{
	NODE* temp = (NODE*)malloc(sizeof(Node));
	assert(temp);
	temp->x = x;
	temp->y = y;
	temp->speed = speed;
	temp->next = NULL;
	return temp;
}
//单链表节点插入函数
void LinkList_insert(LL* list, int x, int y, int speed)
{
	if (list == NULL)
	{
		return;
	}
	//创建一个新的节点
	if (list->head == NULL)
	{
		list->head = list->end = Node_init(x, y, speed);
	}
	else
	{
		list->end->next = Node_init(x, y, speed);
		//更新尾指针
		list->end = list->end->next;
	}
}
//链表结点的删除
void Node_delete(LL*list,Node *p)
{
if(list==NULL||list->head==NULL)
{
	return;
}
//删除节点的第一种情况：删除第一个节点
if (p == list->head)
{
	list->head = p->next;
	free(p);
	p = NULL;
	return;
}

//删除节点的第二种情况：中间结点及最后一个节点
//定义一个temp指针，找到p节点的前一个结点
Node* temp = list->head;
//循环的目的使temp指向P前一个结点
for (; temp != NULL; temp = temp->next)
{
	if (temp->next == p)
	{
		if (p == list->end)
		{
			list->end = temp;
		}
		temp->next = p->next;
		free(p);
		p = NULL;
		return;
	}

}
}
//整个单链表节点的释放
void LinkList_all(LL*list)
{
	if (list == NULL || list->head == NULL)
	{
		return;
	}
	//删除单链表节点
	//需要有两个指针变量,temp1遍历，temp2删除 
	for (Node* temp1 = list->head; temp1 != NULL;)
	{
		Node* temp2 = temp1;
		temp1 = temp1->next;
		free(temp2);
	}

	list->head = list->end = NULL;


}
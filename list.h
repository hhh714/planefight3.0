//��¼�ɻ����ӵ��������
#include<stdlib.h>
#include<assert.h>
//������Ľṹ��
typedef struct NODE {
	int x;
	int y;
	int speed;
	//ָ�룬����ָ����һ���ڵ�
	struct NODE* next;
	

}Node;
//��������ṹ�Ľڵ�
typedef struct LinkList
{
	struct NODE* head;
	struct NODE* end;
}LL;
//��������ڵ㺯��
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
//������ڵ���뺯��
void LinkList_insert(LL* list, int x, int y, int speed)
{
	if (list == NULL)
	{
		return;
	}
	//����һ���µĽڵ�
	if (list->head == NULL)
	{
		list->head = list->end = Node_init(x, y, speed);
	}
	else
	{
		list->end->next = Node_init(x, y, speed);
		//����βָ��
		list->end = list->end->next;
	}
}
//�������ɾ��
void Node_delete(LL*list,Node *p)
{
if(list==NULL||list->head==NULL)
{
	return;
}
//ɾ���ڵ�ĵ�һ�������ɾ����һ���ڵ�
if (p == list->head)
{
	list->head = p->next;
	free(p);
	p = NULL;
	return;
}

//ɾ���ڵ�ĵڶ���������м��㼰���һ���ڵ�
//����һ��tempָ�룬�ҵ�p�ڵ��ǰһ�����
Node* temp = list->head;
//ѭ����Ŀ��ʹtempָ��Pǰһ�����
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
//����������ڵ���ͷ�
void LinkList_all(LL*list)
{
	if (list == NULL || list->head == NULL)
	{
		return;
	}
	//ɾ��������ڵ�
	//��Ҫ������ָ�����,temp1������temp2ɾ�� 
	for (Node* temp1 = list->head; temp1 != NULL;)
	{
		Node* temp2 = temp1;
		temp1 = temp1->next;
		free(temp2);
	}

	list->head = list->end = NULL;


}
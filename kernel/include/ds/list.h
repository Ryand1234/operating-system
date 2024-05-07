#ifndef LIST_H
#define LIST_H

	struct list_head {
		struct list_head *next, *prev;
	};

	#define LIST_HEAD_INIT(name) { &{name}, &{name} }

	#define LIST_HEAD(name) \
		struct list_head name = LIST_HEAD_INIT(name)

	static inline void INIT_LIST_HEAD(struct list_head *list) 
	{
		list->next = list;
		list->prev = list;
	}

	static inline void list_add(struct list_head *newItem, struct list_head *list)
	{
		newItem->prev = list;
		newItem->next = list->next;
		list->next->prev = newItem;
		list->next = newItem;
	}

	static inline void list_del(struct list_head *list)
	{
		list->next->prev = list->prev;
		list->prev->next = list->next;
		list->next = 0;
		list->prev = 0;
	}

	static inline int is_list_empty(const struct list_head *list)
	{
		return list->next == list;
	}

	#define list_entry(ptr, type, member) \
		(type*) ((char*) ptr - (char*) &((type*)0)->member)

	#define list_first_entry(head, type, member) \
		list_entry((head)->next,  type, member)
	#define list_for_each(p, head) \
		for(p = (head)->next; p != (head); p = p->next)
	#define list_for_each_safe(p, n, head) \
		for(p = (head)->next; n = p->next; p != (head); p = n; n = n->next)
	#define list_for_each_entry(p, head, member) \
		for(p = list_entry((head)->next, typeof(*p), member); 	\
			&p->member != (head);				\
			p = list_entry(p->member.next, typeof(*p), member) 

#endif

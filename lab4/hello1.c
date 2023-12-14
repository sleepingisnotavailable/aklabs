// SPDX-License-Identifier: GPL-2-Clause
#include <hello1.h>

MODULE_AUTHOR("Володін Валерій Володимирович ІО-11\n");
MODULE_DESCRIPTION("Лабораторна робота 4\n");
MODULE_LICENSE("GPL");

struct time_data {
	struct list_head list_node;
	ktime_t time_start;
	ktime_t time_end;
};

static struct list_head head_node = LIST_HEAD_INIT(head_node);

int show_greeting(uint count)
{
	struct time_data *item, *temp_item;
	uint i;

	if (count <= 0) {
	    pr_err("Помилка: Значення count не є позитивним\n");
	} else if (count > 0 && count < 5) {
	    pr_info("Примітка: Значення count менше 5\n");
	} else if (count >= 5 && count <= 10) {
	    pr_warn("Увага: Значення count знаходиться в діапазоні від 5 до 10\n");
	} else { 
	    pr_err("Помилка: Значення count перевищує 10\n");
	    return -EINVAL;
	}

	for (i = 0; i < count; i++) {
		item = kmalloc(sizeof(struct time_data), GFP_KERNEL);
		if (ZERO_OR_NULL_PTR(item))
			goto clean_up;
		item->time_start = ktime_get();
		pr_info("Привітання Linux: \n");
		item->time_end = ktime_get();
		list_add_tail(&item->list_node, &head_node);
	}
	return 0;

clean_up:
	list_for_each_entry_safe(item, temp_item, &head_node, list_node) {
		list_del(&item->list_node);
		kfree(item);
	}
	pr_err("Не вистачає пам'яті\n");
	return -1;
}
EXPORT_SYMBOL(show_greeting);

static int __init hello_start(void)
{
	pr_info("hello1 ініційовано\n");
	return 0;
}

static void __exit hello_end(void)
{
	struct time_data *item, *temp_item;

	list_for_each_entry_safe(item, temp_item, &head_node, list_node) {
		pr_info("Час: %lld", item->time_end - item->time_start);
		list_del(&item->list_node);
		kfree(item);
	}
	BUG_ON(!list_empty(&head_node));
	pr_info("Успіх! hello1 вивантажено\n");
}

module_init(hello_start);
module_exit(hello_end);


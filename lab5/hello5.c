// SPDX-License-Identifier: GPL-2.0+
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

struct time_marker {
    struct list_head list_node;
    ktime_t time_begin;
    ktime_t time_end;
};

MODULE_AUTHOR("Володін Валерій Володимирович ІО-11");
MODULE_DESCRIPTION("Лабораторна 5");
MODULE_LICENSE("GPL");

static LIST_HEAD(time_list);

static uint count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "Параметр лічильника");

static int __init start_hello(void)
{
    uint i = 0;
    struct time_marker *marker;

    pr_info("Лічильник: %d\n", count);

    BUG_ON(count == 2);

    for (i = 0; i < count; i++) {
        marker = kmalloc(sizeof(struct time_marker), GFP_KERNEL);

        if (i == 5)
            marker = 0;

        if (ZERO_OR_NULL_PTR(marker))
            goto handle_error;

        marker->time_begin = ktime_get();
        pr_info("Привітання Linux\n");
        marker->time_end = ktime_get();

        list_add_tail(&(marker->list_node), &time_list);
    }

    return 0;

handle_error:
    {
        struct time_marker *item, *temp;

        pr_err("Закінчилася пам'ять...\n");

        list_for_each_entry_safe(item, temp, &time_list, list_node) {
            list_del(&item->list_node);
            kfree(item);
        }

        BUG();
        return -ENOMEM;
    }
}


static void __exit stop_hello(void)
{
    struct time_marker *item, *temp;

    list_for_each_entry_safe(item, temp, &time_list, list_node) {
        pr_info("Час: %lld", item->time_end - item->time_begin);

        list_del(&item->list_node);
        kfree(item);
    }

    BUG_ON(!list_empty(&time_list));
}

module_init(start_hello);
module_exit(stop_hello);


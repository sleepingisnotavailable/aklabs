
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Володін Валерій Володимирович ІО-11");
MODULE_DESCRIPTION("Лабораторна робота 3");
MODULE_LICENSE("GPL");

static uint count = 1;

module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "Кількість привітань для друку");

struct time_record {
    struct list_head node_list;
    ktime_t time_stamp;
};

static LIST_HEAD(time_record_list);

static int __init module_start(void)
{
    int i;    
    
    if (count == 0) {
        printk(KERN_WARNING "Параметр count дорівнює 0\n");
    } else if (count > 10) {
        printk(KERN_WARNING "Параметр count перевищує 10\n");
        return -EINVAL;
    }

    for (i = 0; i < count; i++) {
        struct time_record *record = kmalloc(sizeof(struct time_record), GFP_KERNEL);
        record->time_stamp = ktime_get();
        list_add_tail(&record->node_list, &time_record_list);
        printk(KERN_INFO "Привітання Linux: %d\n", i);
    }

    return 0;
}

static void __exit module_end(void)
{
    struct time_record *item, *temp_item;
    int counter = 0;

    list_for_each_entry_safe(item, temp_item, &time_record_list, node_list) {
        pr_info("Індекс: %d; Часова мітка = %lld\n", counter++, ktime_to_ns(item->time_stamp));
        list_del(&item->node_list);
        kfree(item);
    }
    printk(KERN_INFO "Модуль успішно вивантажено\n");
}

module_init(module_start);
module_exit(module_end);

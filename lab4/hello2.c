// SPDX-License-Identifier: GPL-2-Clause
#include <hello1.h>

MODULE_AUTHOR("Володін Валерій Володимирович ІО-11\n");
MODULE_DESCRIPTION("Лабораторна 4\n");
MODULE_LICENSE("GPL");

static uint count = 1;

module_param(count, uint, 0);
MODULE_PARM_DESC(count, "Лічильник для моєї лабораторної 4\n");

static int __init initialize_hello(void)
{
	pr_info("hello2 ініційовано\n");
	show_greeting(count);
	return 0;
}

static void __exit cleanup_hello(void)
{
	pr_info("hello2 вивантажено\n");
}

module_init(initialize_hello);
module_exit(cleanup_hello);


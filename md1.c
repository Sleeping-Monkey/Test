#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/timer.h>
#include <linux/ktime.h>
#include <linux/jiffies.h>
 
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Appa"); 

struct timer_list my_timer;

void	my_func(struct timer_list *data)
{
	struct file		*file = NULL;
	ssize_t			n = 0;
	loff_t			offset = 0;
	mm_segment_t	fs;

	printk("[!] my_funk start\n");
	file = filp_open("/root/Test2/test2.txt", O_WRONLY|O_APPEND, 0644);
	if (IS_ERR(file))
	{
		printk("[!] file open failed\n");
		return ;
	}
	fs = get_fs();
	set_fs(get_ds());
	if ((n = vfs_write(file, "Dub\n", 4, &offset)) != 4)
	{
		printk("[!] failed write: %ld\n", n);
		filp_close(file, NULL);
		return ;
	}
	printk("[!] writing is ok\n");
	set_fs(fs);
	filp_close(file, NULL);
	mod_timer(&my_timer, jiffies + 60 * HZ);
}

static int __init md_init(void)
{
	struct file		*file = NULL;
	mm_segment_t	fs;
	ssize_t			n = 0;
	loff_t			offset = 0;

	printk("[!] module md1 start!\n");
	file = filp_open("test2.txt", O_WRONLY|O_CREAT|O_APPEND, 0644);
	if (IS_ERR(file))
	{
		printk("[!] file open failed\n");
		return (-1);
	}
	fs = get_fs();
	set_fs(get_ds());
	if ((n = vfs_write(file, "Hello! =)\n", 10, &offset)) != 10)
	{
		printk("[!] failed to write: %ld\n", n);
		filp_close(file, NULL);
		return (-1);
	}
	printk("[!] write %ld bytes\n", n);
	filp_close(file, NULL);
	set_fs(fs);
//	init_timers();
	my_timer.expires = jiffies + 10 * HZ;
//	my_timer.data = 0;
	my_timer.function = my_func;
	add_timer(&my_timer);
	return 0;
}

static void __exit md_exit(void) {
	del_timer(&my_timer);
	printk("[!] module md1 unloaded!\n"); 
}

module_init(md_init); 
module_exit(md_exit);

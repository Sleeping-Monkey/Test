#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/timer.h>
#include <linux/ktime.h>
#include <linux/jiffies.h>
#include <linux/time.h>
 
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Appa"); 

struct timer_list	my_timer;
char				time[7]={'\0'};

void	convert_jiffies_to_time(void)
{
	struct timeval	timespec;
	long			x;

	do_gettimeofday(&timespec);
	x = (timespec.tv_sec) % 86400;
	time[0] = '0' + (x / 3600) / 10;
	time[1] = '0' + (x / 3600) % 10;
	time[2] = ':';
	time[3] = '0' + ((x % 3600) / 60) / 10;
	time[4] = '0' + ((x % 3600) / 60) % 10;
	time[5] = '\n';
	time[6] = '\0';
}

void	my_func(struct timer_list *data)
{
	struct file		*file = NULL;
	ssize_t			n = 0;
	unsigned long	start_jiffies;
	loff_t			offset = 0;
	mm_segment_t	fs;

	start_jiffies = jiffies;
	printk("[!] my_funk start\n");
	file = filp_open("/tmp/current_time", O_WRONLY|O_APPEND, 0644);
	if (IS_ERR(file))
	{
		printk("[!] file open failed\n");
		return ;
	}
	fs = get_fs();
	set_fs(get_ds());
	convert_jiffies_to_time();
	if ((n = vfs_write(file, time, 6, &offset)) != 6)
	{
		printk("[!] failed write: %ld\n", n);
		filp_close(file, NULL);
		return ;
	}
	printk("[!] writing is ok\n");
	set_fs(fs);
	filp_close(file, NULL);
	mod_timer(&my_timer, start_jiffies + 60 * HZ);
}

static int __init md_init(void)
{
	struct file		*file = NULL;

	printk("[!] module md1 start!\n");
	file = filp_open("/tmp/current_time", O_WRONLY|O_CREAT|O_APPEND, 0644);
	if (IS_ERR(file))
	{
		printk("[!] file open failed\n");
		return (-1);
	}
	filp_close(file, NULL);
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

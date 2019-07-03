#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
 
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Appa"); 

static int __init md_init(void)
{
	struct file		*file = NULL;
	mm_segment_t	fs;
	ssize_t			n = 0;
	loff_t			offset = 0;

	printk("[!] module md1 start!\n");
	file = filp_open("test.txt", O_WRONLY|O_CREAT, 0644);
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
	return 0;
}

static void __exit md_exit(void) {
	printk("[!] module md1 unloaded!\n"); 
}

module_init(md_init); 
module_exit(md_exit);

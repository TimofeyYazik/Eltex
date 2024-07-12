#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/rwlock.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/kobject.h>

#define SIZE 20

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tim");
static int major = 0;
static rwlock_t lock;
static char string[SIZE] = "HELLO\n\0";

ssize_t read_f(struct file *fd, char __user *buff, size_t size, loff_t *off){
   size_t rc = 0;
   read_lock(&lock);
   rc = simple_read_from_buffer(buff, size, off, string, SIZE);
   read_unlock(&lock);
   return rc;
}
ssize_t write_f(struct file *fd, const char __user *buff, size_t size, loff_t *off){
  size_t rc = 0;
  if(size > SIZE){
    return -EINVAL;
  }
  write_lock(&lock);
  rc = simple_write_to_buffer(string, SIZE - 1, off, buff, size);
  write_unlock(&lock);
  return rc;
}

static struct file_operations fops = {
  .owner = THIS_MODULE,
  .read = read_f,
  .write = write_f
};

int init_module(void){
  pr_info("module loaded\n");
  rwlock_init(&lock);
  major = register_chrdev(major, "FF", &fops);
  if(major < 0){
    return major;
  }
  pr_info("Major number is %d\n", major);
  return 0;
}
 
void cleanup_module(void){
  unregister_chrdev(major, "FF");
  pr_info("module unloaded\n");
}


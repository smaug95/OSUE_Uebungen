/* Secvault module */
/* Author: Johannes Vass <johannes.vass@student.tuwien.ac.at> */


#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <linux/moduleparam.h>
#include "svctl_ioctl.h"
#include "secvault.h"

static int debug;
module_param(debug, bool, S_IRUGO);

const struct file_operations secvault_fops = {
	.owner =	THIS_MODULE,
	.llseek =	secvault_llseek,
	.read =		secvault_read,
	.write =	secvault_write,
	.open =		secvault_open,
	.release =	secvault_release
};

const struct file_operations secvault_ctl_fops = {
	.owner =			THIS_MODULE,
	.unlocked_ioctl =	secvault_ctl_ioctl,
	.open =				secvault_ctl_open,
	.release =			secvault_ctl_release
};

static struct sv_ctl_dev control_dev;
static struct sv_dev secvault_devs[SECVAULT_NR_DEVS];

/************** Local Prototypes **********************/

static int  secvault_init(void);
static void secvault_cleanup(void);
static void crypt(char *buf, unsigned long pos, char *data,
					char *key, unsigned long count);
static int create_secvault(struct ioctl_data *call_data);
static void DEBUG(char *msg);

/************** Implementations ******************/

static void crypt(char *buf, unsigned long pos, char *data,
					char *key, unsigned long count)
{
	unsigned long i;

	for (i = 0; i < count; i++)
		buf[i] = data[pos + i] ^ key[(pos + i) % SECVAULT_KEY_LENGTH];
}

int secvault_open(struct inode *inode, struct file *filp)
{
	int major, minor;
	struct sv_dev *dev; /* device information */

	DEBUG("opening secvault device");

	major = imajor(inode);
	minor = iminor(inode);

	if (major != SECVAULT_MAJOR || minor < 0 || minor >= SECVAULT_NR_DEVS) {
		DEBUG("Invalid device number");
		return -ENODEV;
	}

	dev = &secvault_devs[minor];
	filp->private_data = dev; /* for other methods */

	/* now trim to 0 the length of the device if open was write-only */
	if ((filp->f_flags & O_ACCMODE) == O_WRONLY) {
		if (down_interruptible(&dev->sem))
			return -ERESTARTSYS;
		dev->cur_size = 0;
		up(&dev->sem);
	}

	return 0;          /* success */
}


int	secvault_release(struct inode *inode, struct file *filp)
{
	DEBUG("releasing secvault device");
	return 0;
}

ssize_t secvault_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	ssize_t cur_count, rest, cur_rest, total;
	loff_t pos;
	char crypt_buf[SECVAULT_KEY_LENGTH];
	struct sv_dev *dev = filp->private_data;
	ssize_t ret = 0;

	DEBUG("Read call");

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	if (filp->f_pos >= dev->cur_size) {
		ret = 0;
		goto fail;
	}
	pos = filp->f_pos;
	if (dev->cur_size - pos < count)
		total = dev->cur_size - pos;
	else
		total = count;
	rest = total;

	while (rest > 0) {
		cur_count = rest < SECVAULT_KEY_LENGTH ? rest : SECVAULT_KEY_LENGTH;
		crypt(&crypt_buf[0], pos, dev->data, dev->key, cur_count);

		cur_rest = copy_to_user(buf + total - rest, crypt_buf, cur_count);
		if ( cur_rest != 0) {
			DEBUG("Copy to user failed");
			ret = -EFAULT;
			goto fail;
		}
		rest -= cur_count;
		pos += cur_count;
		ret += cur_count;
	}

	*f_pos += total;
	ret = total;

	up(&dev->sem);
	return ret;

	fail:
		up(&dev->sem);
		return ret;
}


ssize_t secvault_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	unsigned long rest;
	struct sv_dev *dev = filp->private_data;
	ssize_t ret = 0;

	DEBUG("Write call");

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	if (dev->size - filp->f_pos < count) {
		DEBUG("Write not possible, not enough space.");
		ret = -ENOSPC;
		goto fail;
	}

	if ( (rest = copy_from_user(&dev->data[filp->f_pos], buf, count)) == 0) {
		crypt(&dev->data[filp->f_pos], filp->f_pos, dev->data, dev->key, count);
	} else {
		DEBUG("Copy from user failed");
	ret = -EFAULT;
		goto fail;
	}

	*f_pos += count;
	dev->cur_size = *f_pos;
	ret = count;

	up(&dev->sem);
	return ret;

	fail:
		up(&dev->sem);
	return ret;
}

loff_t secvault_llseek(struct file *filp, loff_t off, int whence)
{
	struct sv_dev *dev;
	loff_t newpos;

	DEBUG("llseek call");
	dev = filp->private_data;

	switch (whence) {
		case 0:		/* SEEK_SET */
			newpos = off;
			break;
		case 1:		/* SEEK_CUR */
			newpos = filp->f_pos + off;
			break;
		case 2:		/* SEEK_END */
			newpos = dev->size + off;
			break;
		default:	/* can't happen */
			return -EINVAL;
	}

	if (newpos < 0)
		return -EINVAL;
	filp->f_pos = newpos;

	return newpos;
}

int	secvault_ctl_open(struct inode *inode, struct file *filp)
{
    struct sv_ctl_dev *dev; /* device information */
    dev = container_of(inode->i_cdev, struct sv_ctl_dev, cdev);
    filp->private_data = dev; /* for other methods */

	return 0;
}

int	secvault_ctl_release(struct inode *inode, struct file *filp)
{
	return 0;
}

long secvault_ctl_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret = 0;
	struct sv_dev *dev;
	struct ioctl_data call_data;

    /*
     * extract the type and number bitfields, and don't decode
     * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
     */
    if (_IOC_TYPE(cmd) != SV_IOC_MAGIC) return -ENOTTY;
    if (_IOC_NR(cmd) > SV_IOC_MAXNR) return -ENOTTY;

	ret = copy_from_user(&call_data, (void *)arg, sizeof call_data);
	if (ret < 0)
		return ret;

	if (call_data.dev_nr < 0 || call_data.dev_nr >= SECVAULT_NR_DEVS)
		return -ENODEV;

	dev = &secvault_devs[call_data.dev_nr];

	switch (cmd) {

		case SV_CREATE_SECVAULT:
			DEBUG("IOCTL call SV_CREATE_SECVAULT");
			ret = create_secvault(&call_data);
			break;

		case SV_GET_SIZE:
			DEBUG("IOCTL call SV_GET_SIZE");
			call_data.size = dev->size;
			ret = copy_to_user((void *)arg, &call_data, sizeof call_data);
			break;

		case SV_CHANGE_KEY:
			DEBUG("IOCTL call SV_CHANGE_KEY");
			if (down_interruptible(&dev->sem))
				return -ERESTARTSYS;
			crypt(dev->data, 0, dev->data, dev->key, dev->size);
			memcpy(dev->key,  call_data.key, sizeof dev->key);
			crypt(dev->data, 0, dev->data, dev->key, dev->size);
			up(&dev->sem);
			break;

		case SV_WIPE_SECVAULT:
			DEBUG("IOCTL call SV_WIPE_SECVAULT");
			if (down_interruptible(&dev->sem))
				return -ERESTARTSYS;
			memset(dev->data, 0, dev->size * sizeof (char));
			crypt(dev->data, 0, dev->data, dev->key, dev->size);
			up(&dev->sem);
			break;

		case SV_DELETE_SECVAULT:
			DEBUG("IOCTL call SV_DELETE_SECVAULT");
			if (dev->size == 0) {
				DEBUG("Secvault to delete was not loaded");
				return -EINVAL;
			}
			if (down_interruptible(&dev->sem))
				return -ERESTARTSYS;
			cdev_del(&dev->cdev);
			kfree(dev->data);
			dev->size = 0;
			up(&dev->sem);
			break;

        default:  /* redundant, as cmd was checked against MAXNR */
          return -ENOTTY;
	}

	return ret;
}

static int create_secvault(struct ioctl_data *call_data)
{
	int result = 0;
	struct sv_dev *dev;

	if (call_data->dev_nr < 0 || call_data->dev_nr >= SECVAULT_NR_DEVS) {
		DEBUG("Invalid device-number range");
		return -ENODEV;
	}

	dev = &secvault_devs[call_data->dev_nr];
	if (dev->size != 0) {
		DEBUG("Device already in use");
		return -1;
	}

	/* create the device */
	sema_init(&dev->sem, 0);

	cdev_init(&dev->cdev, &secvault_fops);
	memcpy(dev->key, call_data->key, sizeof dev->key);
	dev->size = call_data->size;
	dev->cdev.owner = THIS_MODULE;
	printk("key of new device is %s", call_data->key);

	/* allocate size bytes of memory */
	dev->data = kmalloc(dev->size * sizeof (char), GFP_KERNEL);
	if (dev->data == NULL) {
		DEBUG("Could not allocate memory.");
		result = -ENOMEM;
		goto fail2;
	}
	memset(dev->data, 0, dev->size * sizeof (char));
	crypt(&dev->data[0], 0, dev->data, dev->key, dev->size);

	result = cdev_add(&dev->cdev, MKDEV(SECVAULT_MAJOR, call_data->dev_nr), 1);
	if (result < 0) {
		printk(KERN_WARNING "secvault: could not add data device %d\n", call_data->dev_nr);
		result = -1;
		goto fail1;
	}

	up(&dev->sem);
	return result;

	fail1:
		kfree(dev->data);
	fail2:
		dev->size = 0;

	return result;
}

static void secvault_cleanup(void)
{
	int i;
	DEBUG("Starting Cleanup");

	cdev_del(&control_dev.cdev);

	for (i = 0; i < SECVAULT_NR_DEVS; i++) {
		if (secvault_devs[i].size != 0)
			cdev_del(&secvault_devs[i].cdev);
			kfree(secvault_devs[i].data);
			secvault_devs[i].size = 0;
	}

	unregister_chrdev_region(MKDEV(SECVAULT_MAJOR, 0),  SECVAULT_NR_DEVS + 1);
}

static int __init secvault_init(void)
{
	dev_t first_major;
	int result;

	DEBUG("Starting the secvault-module.");

	/* Register character-device-region for all 5 devices */
	first_major = MKDEV(SECVAULT_MAJOR, 0);
    result = register_chrdev_region(first_major, SECVAULT_NR_DEVS + 1, "secvault");

    if (result < 0) {
        printk(KERN_WARNING "secvault: can't get major %d\n",SECVAULT_MAJOR);
        return result;
    }

	/* Create sv_ctl device now, the secvaults themselves get created via IOCTL later */
	sema_init(&control_dev.sem, 1);
	cdev_init(&control_dev.cdev, &secvault_ctl_fops);
	control_dev.cdev.owner = THIS_MODULE;

	result = cdev_add(&control_dev.cdev, MKDEV(SECVAULT_MAJOR, SECVAULT_NR_DEVS), 1);
	if (result < 0) {
        printk(KERN_WARNING "secvault: can't add control device\n");
		unregister_chrdev_region(first_major,  SECVAULT_NR_DEVS + 1);
        return result;
	}

	DEBUG("Successfully started the secvault-module.");
	return 0;
}

static void __exit secvault_exit(void)
{
	secvault_cleanup();
	DEBUG("Secvault successfully unloaded.");
}

static void DEBUG(char *msg)
{
	if (debug) {
		printk("Secvault DBG: %s\n", msg);
	}
}

module_init(secvault_init);
module_exit(secvault_exit);

MODULE_LICENSE("GPL");

/*
Filesystem Lab disigned and implemented by Liang Junkai,RUC
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <fuse.h>
#include <errno.h>
#include "disk.h"
#include <stdlib.h>
#include <libgen.h>

#define DIRMODE S_IFDIR|0755
#define REGMODE S_IFREG|0644

#define ROOT_INODE 0         // 根目录的 i 节点编号
#define INODE_BLOCKS 256 	 // 一共需要多少block来存放inode
#define INODE_NUM 128        // 一个block可以存放的inode数
#define INODE_SIZE 32        //一个inode占用的byte
#define NAME_MAX 24  		 // 文件名长度上限
#define MAX_INODE 32768		 //最大inode
#define DIR_ENTRY_SIZE 32           //1个目录有32字节
#define DIR_ENTRY_PER_BLOCK 128     //一个block有128个目录
#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))

//1 super block
//2 data bitmap
//1 inode bitmap
//256 inode block     256 * (4KB)/32B = 32768个inode
//1 根节点
//包括根节点在内一共65536 - 256-4 = 65276个datablock，block260是第一个datablock

int safe_read(int block_id, void* buffer) {
    if (disk_read(block_id, buffer) != 0) {
        fprintf(stderr, "Error reading block %d\n", block_id);
        return -1;
    }
    return 0;
}

int safe_write(int block_id, void* buffer) {
    if (disk_write(block_id, buffer) != 0) {
        fprintf(stderr, "Error writing block %d\n", block_id);
        return -1;
    }
    return 0;
}

//删除时，文件应该顺序前挪，为了保证完整
//但是文件夹无所谓，可以直接与最后一个交换再删

struct inode {
    uint8_t is_file;           	// 文件类型（文件或目录）           // 1 byte
    uint16_t indirect_pointer;   // 一级间接块编号                  // 2 bytes
    uint32_t size;               // 文件大小（字节）                // 4 bytes
	time_t st_Atime;             // 被访问的时间                     // 8 bytes 
    time_t st_Mtime;             // 被修改的时间                     // 8 bytes
    time_t st_Ctime;             // 状态改变时间                     // 8 bytes
};//共32 byte

struct superblock {
    unsigned long  f_bsize;  // 块大小
    uint32_t       f_blocks; // 块数量
    uint32_t       f_bfree;  // 空闲块数量
    uint32_t       f_bavail; // 可用块数量
    uint32_t       f_files;  // 文件节点数
    uint32_t       f_ffree;  // 空闲节点数
    uint32_t       f_favail; // 可用节点数
    unsigned long  f_namemax; // 文件名长度上限
    uint32_t       root_inode; // 根目录的 i 节点编号
};


struct dir_entry {
    char name[NAME_MAX];
    uint8_t padding[2];     // 填充字节使其凑齐 32 字节
    uint8_t is_used;    
    uint32_t inode_index;
};

//以下存放函数声明：

//super
void init_superblock();
void data_plus();
void data_minus();
void inode_plus();
void inode_minus();

//bitmap
void set_bit(uint8_t *bitmap, int index);
void clear_bit(uint8_t *bitmap, int index);
int test_bit(uint8_t *bitmap, int index);
int allocate_block_in_bitmap();
void free_block_in_bitmap(int block_id);
int get_new_inode_bit_in_bitmap();
void free_inode_in_bitmap(int inode_id);

//inode
void create_inode(int inode_id, uint8_t is_file);
int write_inode(int inode_index, struct inode *a);

//datablock
int clear_block(int block_id);

//声明结束

//super
//初始化sb并写入sb区
void init_superblock() {
	struct superblock sb;
    sb.f_bsize = BLOCK_SIZE;
    sb.f_blocks = BLOCK_NUM;
    sb.f_bfree = BLOCK_NUM - (1 + 2 + 1 + INODE_BLOCKS); // 初始化时空闲块数
    sb.f_bavail = sb.f_bfree;
    sb.f_files = MAX_INODE; // 总文件节点数
    sb.f_ffree = sb.f_files; // 初始化时空闲节点数
    sb.f_favail = sb.f_ffree;
    sb.f_namemax = NAME_MAX;
    sb.root_inode = ROOT_INODE; // 根目录的 i 节点编号	
	safe_write(0,&sb);
}
//sb的空闲data块++并写入sb区。其他三个同
//！！！这部分的东西等写完基础的再加上吧~~~
void data_plus(){
	struct superblock sb;
    safe_read(0, &sb);
    sb.f_bfree++;
    sb.f_bavail++;
    safe_write(0, &sb);
}

void data_minus(){
	struct superblock sb;
    safe_read(0, &sb);
    sb.f_bfree--;
    sb.f_bavail--;
    safe_write(0, &sb);
}

void inode_plus(){
	struct superblock sb;
    safe_read(0, &sb);
    sb.f_ffree++;
    sb.f_favail++;
    safe_write(0, &sb);
}

void inode_minus(){
	struct superblock sb;
    safe_read(0, &sb);
    sb.f_ffree--;
    sb.f_favail--;
    safe_write(0, &sb);
}

//bitmap: data 1 data 2 inode 3
//以下三个都是对栈上的bitmap操作
void set_bit(uint8_t *bitmap, int index) {
    bitmap[index / 8] |= (1 << (index % 8));
}

void clear_bit(uint8_t *bitmap, int index) {
    bitmap[index / 8] &= ~(1 << (index % 8));
}

int test_bit(uint8_t *bitmap, int index) {
    return bitmap[index / 8] & (1 << (index % 8));
}

// BITMAP上分配1个新的数据块
int allocate_block_in_bitmap(){
    uint8_t data_block_bitmap[BLOCK_SIZE * 2]; 
    safe_read(1, data_block_bitmap); 
    safe_read(2, data_block_bitmap + BLOCK_SIZE);
    for (int i = 0; i < BLOCK_NUM; ++i) {
        if (!test_bit(data_block_bitmap, i)) {
            set_bit(data_block_bitmap, i);
			safe_write(1,data_block_bitmap);
			safe_write(2, data_block_bitmap + BLOCK_SIZE);
            return i; // 返回分配的数据块号
        }
    }
    return -1;
}
// BITMAP上释放1个数据块
void free_block_in_bitmap(int block_id) {
    uint8_t data_block_bitmap[BLOCK_SIZE * 2]; 
    safe_read(1, data_block_bitmap); 
    safe_read(2, data_block_bitmap + BLOCK_SIZE);
    clear_bit(data_block_bitmap, block_id);
	safe_write(1,data_block_bitmap);
	safe_write(2, data_block_bitmap + BLOCK_SIZE);
}
// BITMAP上分配新的1个inode
int get_new_inode_bit_in_bitmap(){
	uint8_t inode_bitmap[BLOCK_SIZE]; 
	safe_read(3, inode_bitmap); 
    for (int i = 0; i < MAX_INODE; ++i) {
        if (!test_bit(inode_bitmap, i)) {
            set_bit(inode_bitmap, i);
			safe_write(3,inode_bitmap);
            return i; // 返回分配的 inode 号
        }
    }
    return -1; // 没有空闲 inode
}
// BITMAP上释放1个inode
void free_inode_in_bitmap(int inode_id) {
	uint8_t inode_bitmap[BLOCK_SIZE]; 
	safe_read(3, inode_bitmap); 
    clear_bit(inode_bitmap, inode_id);
	safe_write(3,inode_bitmap);
}
//bitmap上数有多少1
int count_free_bits(uint8_t *bitmap, int size) {
    int count = 0;
    for (int i = 0; i < size * 8; ++i) {
        if (!test_bit(bitmap, i)) {
            count++;
        }
    }
    return count;
}

//inode
//创建一个新的inode并写入inode区(已知inode的id了)
void create_inode(int inode_id, uint8_t is_file) {
	struct inode a;
    a.is_file = is_file;
    a.size = 0;
    a.indirect_pointer = allocate_block_in_bitmap(); 
	clear_block(a.indirect_pointer);
    a.st_Atime = time(NULL);
    a.st_Mtime = a.st_Atime;
    a.st_Ctime = a.st_Atime;
	write_inode(inode_id, &a);
}

//把栈上的inode写入inode_index指定的inode区
int write_inode(int inode_index, struct inode *a) {
    uint8_t inode_block[BLOCK_SIZE];
    int block_num = 4 + (inode_index / INODE_NUM); 
    int offset = (inode_index % INODE_NUM) * INODE_SIZE; 

    if (safe_read(block_num, inode_block) != 0) {
        return -1;
    }

    memcpy(inode_block + offset, a, sizeof(struct inode));


    if (safe_write(block_num, inode_block) != 0) {
        return -1;
    }

    return 0;
}

//把inode_index指定的inode写入栈上的inode
int read_inode(int inode_index, struct inode *inode) {
    uint8_t buffer[BLOCK_SIZE];
    int block_num = 4 + (inode_index / INODE_NUM); 
    int offset = (inode_index % INODE_NUM) * INODE_SIZE; 

    if (safe_read(block_num, buffer) != 0) {
        return -1;
    }

    memcpy(inode, buffer + offset, sizeof(struct inode));
    return 0;
}


//通过path找inode,成功则返回inode的index
int get_inode_by_path(const char *path, struct inode *inode) {
    if (strncmp(path, "/", 1) == 0 && strlen(path) == 1) {
        return read_inode(ROOT_INODE, inode);  
    }

    int inode_index = ROOT_INODE;
    if (read_inode(inode_index, inode) != 0) {
        return -ENOENT;
    }

    char *path_copy = strdup(path);
    if (!path_copy) {
        return -ENOMEM;
    }

    char *token = strtok(path_copy, "/");
    while (token != NULL) {
        if(inode->is_file)
            return -ENOENT;
        uint16_t pointers[BLOCK_SIZE / sizeof(uint16_t)];
        if (safe_read(inode->indirect_pointer, pointers) != 0) {
            free(path_copy);
            return -ENOENT;
        }

        int found = 0;
        for (int i = 0; i < BLOCK_SIZE / sizeof(uint16_t); ++i) {
            if (pointers[i] == 0) {
                continue;;  
            }

            uint8_t buffer[BLOCK_SIZE];
            if (safe_read(pointers[i], buffer) != 0) {
                free(path_copy);
                return -ENOENT;
            }

            struct dir_entry *entries = (struct dir_entry *)buffer;
            for (int j = 0; j < DIR_ENTRY_PER_BLOCK; ++j) {
                if(!entries[j].is_used)
                    continue;
                if (strcmp(entries[j].name, token) == 0) {
                    inode_index = entries[j].inode_index;
                    if (read_inode(inode_index, inode) != 0) {
                        free(path_copy);
                        return -ENOENT;
                    }
                    found = 1;
                    break;
                }
            }

            if (found) {
                break;
            }
        }

        if (!found) {
            free(path_copy);
            return -ENOENT;
        }

        token = strtok(NULL, "/");
    }

    free(path_copy);
    return inode_index;
}

//创建完文件/文件夹后，在父文件夹中添加相关信息并更新时间
int update_parent_directory(struct inode *parent_inode, int parent_inode_index, const char *name, int inode_index) {
    uint8_t pointers[BLOCK_SIZE];
    if (safe_read(parent_inode->indirect_pointer, pointers) != 0) {
        return -EIO;
    }

    uint16_t *dir_block_pointers = (uint16_t *)pointers;
    for (int i = 0; i < BLOCK_SIZE / sizeof(uint16_t); ++i) {
        if (dir_block_pointers[i] == 0) {
            dir_block_pointers[i] = allocate_block_in_bitmap();
            clear_block(dir_block_pointers[i]);
            safe_write(parent_inode->indirect_pointer,pointers);
            if (dir_block_pointers[i] < 0) {
                return -ENOSPC;
            }
        }

        uint8_t block[BLOCK_SIZE];
        if (safe_read(dir_block_pointers[i], block) != 0) {
            return -EIO;
        }

        struct dir_entry *entries = (struct dir_entry *)block;
        for (int j = 0; j < DIR_ENTRY_PER_BLOCK; ++j) {
            if (!entries[j].is_used) {
                strncpy(entries[j].name, name, NAME_MAX);
                entries[j].inode_index = inode_index;
                entries[j].is_used = 1;
                if (safe_write(dir_block_pointers[i], block) != 0) {
                    return -EIO;
                }
                parent_inode->st_Mtime = parent_inode->st_Ctime = time(NULL);
                if (write_inode(parent_inode_index, parent_inode) != 0) {
                    return -EIO;
                }
                return 0;
            }
        }
    }

    return -ENOSPC; // 没有空闲目录项
}

//删除完文件/文件夹后，在父文件夹中删除相关信息并更新时间
int delete_directory_entry(struct inode *parent_inode, int parent_inode_index, const char *name) {
    uint8_t pointers[BLOCK_SIZE];
    if (safe_read(parent_inode->indirect_pointer, pointers) != 0) {
        return -EIO;
    }

    uint16_t *dir_block_pointers = (uint16_t *)pointers;
    for (int i = 0; i < BLOCK_SIZE / sizeof(uint16_t); ++i) {
        if (dir_block_pointers[i] == 0) {
            continue;
        }

        uint8_t block[BLOCK_SIZE];
        if (safe_read(dir_block_pointers[i], block) != 0) {
            return -EIO;
        }

        struct dir_entry *entries = (struct dir_entry *)block;
        for (int j = 0; j < DIR_ENTRY_PER_BLOCK; ++j) {
            if (entries[j].is_used && strcmp(entries[j].name, name) == 0) {
                int inode_index = entries[j].inode_index;
                entries[j].is_used = 0;
                entries[j].inode_index = 0;
                if (safe_write(dir_block_pointers[i], block) != 0) {
                    return -EIO;
                }

                // 更新父目录的 mtime 和 ctime
                parent_inode->st_Mtime = parent_inode->st_Ctime = time(NULL);
                if (write_inode(parent_inode_index, parent_inode) != 0) {
                    return -EIO;
                }

                return inode_index;
            }
        }
    }

    return -ENOENT; // 目录项未找到
}

//往parent_inode这个文件夹中写入一个文件name
int add_directory_entry(struct inode *parent_inode, int parent_inode_index, const char *name, int inode_index) {
    uint8_t pointers[BLOCK_SIZE];
    if (safe_read(parent_inode->indirect_pointer, pointers) != 0) {
        return -EIO;
    }

    uint16_t *dir_block_pointers = (uint16_t *)pointers;
    for (int i = 0; i < BLOCK_SIZE / sizeof(uint16_t); ++i) {
        if (dir_block_pointers[i] == 0) {
            dir_block_pointers[i] = allocate_block_in_bitmap();
            if (dir_block_pointers[i] < 0) {
                return -ENOSPC;
            }
            clear_block(dir_block_pointers[i]);
            safe_write(parent_inode->indirect_pointer,pointers);
        }

        uint8_t block[BLOCK_SIZE];
        if (safe_read(dir_block_pointers[i], block) != 0) {
            return -EIO;
        }

        struct dir_entry *entries = (struct dir_entry *)block;
        for (int j = 0; j < DIR_ENTRY_PER_BLOCK; ++j) {
            if (!entries[j].is_used) {
                strncpy(entries[j].name, name, NAME_MAX);
                entries[j].inode_index = inode_index;
                entries[j].is_used = 1;
                if (safe_write(dir_block_pointers[i], block) != 0) {
                    return -EIO;
                }
                parent_inode->st_Mtime = parent_inode->st_Ctime = time(NULL);
                if (write_inode(parent_inode_index, parent_inode) != 0) {
                    return -EIO;
                }
                return 0;
            }
        }
    }

    return -ENOSPC; // 没有空闲目录项
}

//修改inode的大小为new_size
int resize_file(struct inode *inode, size_t new_size) {
    size_t current_blocks = (inode->size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    size_t new_blocks = (new_size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    uint16_t pointers[BLOCK_SIZE / sizeof(uint16_t)];
    if (safe_read(inode->indirect_pointer, pointers) != 0) {
        return -EIO;
    }

    if (new_size > inode->size) { // 扩展文件
        for (size_t i = current_blocks; i < new_blocks; ++i) {
            int new_block = allocate_block_in_bitmap();
            if (new_block < 0) {
                return -ENOSPC;
            }
            clear_block(new_block);
            pointers[i] = new_block;
        }
    } else if (new_size < inode->size) { // 缩小文件
        for (size_t i = new_blocks; i < current_blocks; ++i) {
            clear_block(pointers[i]);
            free_block_in_bitmap(pointers[i]);
            pointers[i] = 0;
        }
    }

    if (safe_write(inode->indirect_pointer, pointers) != 0) {
        return -EIO;
    }

    inode->size = new_size;
    return 0;
}
//datablock

int clear_block(int block_id) {
    uint8_t buffer[BLOCK_SIZE] = {0};
    return safe_write(block_id, buffer); 
}

void free_inode_and_blocks(struct inode *inode, int inode_index) {
    // 读取 inode->indirect_pointer 指向的块
    uint16_t pointers[BLOCK_SIZE / sizeof(uint16_t)];
    if (safe_read(inode->indirect_pointer, pointers) != 0) {
        return;
    }

    // 清空并释放所有指向的数据块
    for (int i = 0; i < BLOCK_SIZE / sizeof(uint16_t); ++i) {
        if (pointers[i] != 0) {
            clear_block(pointers[i]);
            free_block_in_bitmap(pointers[i]);
        }
    }

    // 清空并释放 indirect_pointer 块
    clear_block(inode->indirect_pointer);
    free_block_in_bitmap(inode->indirect_pointer);

    // 释放 inode 本身
    free_inode_in_bitmap(inode_index);
}




//Format the virtual block device in the following function
int mkfs()
{
	//初始化super blcok
    struct superblock sb;
    init_superblock(&sb);

    // 初始化data Bitmap
    uint8_t data_block_bitmap[BLOCK_SIZE * 2] = {0}; 
    for (int i = 0; i <= 259; i++) {
        set_bit(data_block_bitmap, i);
    }
    if (safe_write(1, data_block_bitmap) != 0 || safe_write(2, data_block_bitmap + BLOCK_SIZE) != 0) {
        return -1;
    }

    // 初始化inode Bitmap
    uint8_t inode_bitmap[BLOCK_SIZE] = {0}; 
    set_bit(inode_bitmap, ROOT_INODE); 
    if (safe_write(3, inode_bitmap) != 0) {
        return -1;
    }

    // 初始化inode
    uint8_t inode_blocks[BLOCK_SIZE * INODE_BLOCKS] = {0};
    for (int i = 0; i < INODE_BLOCKS; i++) {
        if (safe_write(4 + i, inode_blocks + i * BLOCK_SIZE) != 0) {
            return -1;
        }
    }

	//写入根目录inode
    create_inode(0,0);
    return 0;
}

//Filesystem operations that you need to implement
int fs_getattr (const char *path, struct stat *attr)
{
    printf("fs_getattr is called:%s\n",path);
    struct inode inode;
    if (get_inode_by_path(path, &inode) < 0) {
        return -ENOENT;
    }

    attr->st_mode = inode.is_file ? REGMODE : DIRMODE;
    attr->st_nlink = 1;  // 这个简单文件系统不支持硬链接
    attr->st_uid = getuid();
    attr->st_gid = getgid();
    attr->st_size = inode.size;
    attr->st_atime = inode.st_Atime;
    attr->st_mtime = inode.st_Mtime;
    attr->st_ctime = inode.st_Ctime;

	return 0;
}

int fs_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    printf("fs_readdir is called:%s\n",path);
    struct inode dir_inode;
    int inode_index = get_inode_by_path(path, &dir_inode);
    if (inode_index < 0) {
        return inode_index;
    }

    // 检查 inode 类型是否为目录
    if (dir_inode.is_file) {
        return -ENOTDIR;
    }

    // 读取包含目录项块的指针块
    uint16_t pointers[BLOCK_SIZE / sizeof(uint16_t)];
    if (safe_read(dir_inode.indirect_pointer, pointers) != 0) {
        return -EIO;
    }

    for (int i = 0; i < BLOCK_SIZE / sizeof(uint16_t); ++i) {
        if (pointers[i] == 0) {
            continue;  // 跳过未使用的指针
        }

        uint8_t block[BLOCK_SIZE];
        if (safe_read(pointers[i], block) != 0) {
            return -EIO;
        }

        struct dir_entry *entries = (struct dir_entry *)block;
        for (int j = 0; j < DIR_ENTRY_PER_BLOCK; ++j) {
            if(!entries[j].is_used)
                continue;
            if (filler(buffer, entries[j].name, NULL, 0) != 0) {
                return -ENOMEM;
            }
        }
    }

    // 更新访问时间
    dir_inode.st_Atime = time(NULL);
    if (write_inode(inode_index, &dir_inode) != 0) {
        return -EIO;
    }

    return 0;
}

int fs_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi) {
    printf("fs_read is called:%s\n",path);
    struct inode file_inode;
    int inode_index = get_inode_by_path(path, &file_inode);
    if (inode_index < 0) {
        return inode_index;
    }

    if (!file_inode.is_file) {
        return -EISDIR;
    }

    if (offset >= file_inode.size) {
        return 0; 
    }

    size_t bytes_to_read = min(size, file_inode.size - offset);
    uint8_t indirect_block[BLOCK_SIZE];
    if (safe_read(file_inode.indirect_pointer, indirect_block) != 0) {
        return -EIO;
    }

    uint16_t *data_block_pointers = (uint16_t *)indirect_block;
    size_t bytes_read = 0;
    size_t block_offset = offset / BLOCK_SIZE;
    size_t block_start_offset = offset % BLOCK_SIZE;

    while (bytes_read < bytes_to_read) {
        if (data_block_pointers[block_offset] == 0) {
            return -EIO;
        }

        uint8_t data_block[BLOCK_SIZE];
        if (safe_read(data_block_pointers[block_offset], data_block) != 0) {
            return -EIO;
        }

        size_t bytes_from_block = min(BLOCK_SIZE - block_start_offset, bytes_to_read - bytes_read);
        memcpy(buffer + bytes_read, data_block + block_start_offset, bytes_from_block);

        bytes_read += bytes_from_block;
        block_offset++;
        block_start_offset = 0;  // 之后的块从块起始位置读取
    }

    // 更新访问时间
    file_inode.st_Atime = time(NULL);
    if (write_inode(inode_index, &file_inode) != 0) {
        return -EIO;
    }

    return bytes_read;
}

int fs_mknod(const char *path, mode_t mode, dev_t dev) {
    printf("fs_mknod is called:%s\n",path);
    // 提取父目录路径和新文件名
    char *path_copy1 = strdup(path);
    char *path_copy2 = strdup(path);
    if (!path_copy1 || !path_copy2) {
        free(path_copy1);
        free(path_copy2);
        return -ENOMEM;
    }

    char *dir_path = dirname(path_copy1);
    char *base_name = basename(path_copy2);

    struct inode dir_inode;
    int parent_inode_index = get_inode_by_path(dir_path, &dir_inode);
    if (parent_inode_index < 0) {
        free(path_copy1);
        free(path_copy2);
        return parent_inode_index;
    }

    // 检查 inode 类型是否为目录
    if (dir_inode.is_file) {
        free(path_copy1);
        free(path_copy2);
        return -ENOTDIR;
    }

    // 分配新的 inode 和数据块
    int new_inode_index = get_new_inode_bit_in_bitmap();
    if (new_inode_index < 0) {
        free(path_copy1);
        free(path_copy2);
        return -ENOSPC;
    }

    // 使用辅助函数创建新的 inode
    create_inode(new_inode_index, 1);

    // 更新父目录的目录项
    int ret = update_parent_directory(&dir_inode, parent_inode_index, base_name, new_inode_index);
    free(path_copy1);
    free(path_copy2);
    return ret;
}

int fs_mkdir(const char *path, mode_t mode) {
    printf("fs_mkdir is called:%s\n",path);
    // 提取父目录路径和新目录名
    char *path_copy1 = strdup(path);
    char *path_copy2 = strdup(path);
    if (!path_copy1 || !path_copy2) {
        free(path_copy1);
        free(path_copy2);
        return -ENOMEM;
    }

    char *dir_path = dirname(path_copy1);
    char *base_name = basename(path_copy2);

    struct inode dir_inode;
    int parent_inode_index = get_inode_by_path(dir_path, &dir_inode);
    if (parent_inode_index < 0) {
        free(path_copy1);
        free(path_copy2);
        return parent_inode_index;
    }

    // 检查 inode 类型是否为目录
    if (dir_inode.is_file) {
        free(path_copy1);
        free(path_copy2);
        return -ENOTDIR;
    }

    // 分配新的 inode
    int new_inode_index = get_new_inode_bit_in_bitmap();
    if (new_inode_index < 0) {
        free(path_copy1);
        free(path_copy2);
        return -ENOSPC;
    }

    // 使用辅助函数创建新的 inode
    create_inode(new_inode_index, 0);

    // 更新父目录的目录项
    int ret = update_parent_directory(&dir_inode, parent_inode_index, base_name, new_inode_index);
    free(path_copy1);
    free(path_copy2);
	return ret;
}

int fs_rmdir(const char *path) {
    printf("fs_rmdir is called:%s\n",path);
    // 提取父目录路径和要删除的目录名
    char *path_copy1 = strdup(path);
    char *path_copy2 = strdup(path);
    if (!path_copy1 || !path_copy2) {
        free(path_copy1);
        free(path_copy2);
        return -ENOMEM;
    }

    char *dir_path = dirname(path_copy1);
    char *base_name = basename(path_copy2);

    struct inode dir_inode;
    int parent_inode_index = get_inode_by_path(dir_path, &dir_inode);
    if (parent_inode_index < 0) {
        free(path_copy1);
        free(path_copy2);
        return parent_inode_index;
    }

    // 检查 inode 类型是否为目录
    if (dir_inode.is_file) {
        free(path_copy1);
        free(path_copy2);
        return -ENOTDIR;
    }

    struct inode target_inode;
    int target_inode_index = get_inode_by_path(path, &target_inode);
    if (target_inode_index < 0) {
        free(path_copy1);
        free(path_copy2);
        return target_inode_index;
    }

    // 检查要删除的 inode 是否为目录
    if (target_inode.is_file) {
        free(path_copy1);
        free(path_copy2);
        return -ENOTDIR;
    }

    // 删除目标目录的目录项
    int ret = delete_directory_entry(&dir_inode, parent_inode_index, base_name);
    if (ret < 0) {
        free(path_copy1);
        free(path_copy2);
        return ret;
    }

    // 释放目标目录的 inode 和数据块
    free_inode_and_blocks(&target_inode, target_inode_index);

    free(path_copy1);
    free(path_copy2);
	return 0;
}

int fs_unlink(const char *path) {
    printf("fs_unlink is called:%s\n",path);
    // 提取父目录路径和要删除的文件名
    char *path_copy1 = strdup(path);
    char *path_copy2 = strdup(path);
    if (!path_copy1 || !path_copy2) {
        free(path_copy1);
        free(path_copy2);
        return -ENOMEM;
    }

    char *dir_path = dirname(path_copy1);
    char *base_name = basename(path_copy2);

    struct inode dir_inode;
    int parent_inode_index = get_inode_by_path(dir_path, &dir_inode);
    if (parent_inode_index < 0) {
        free(path_copy1);
        free(path_copy2);
        return parent_inode_index;
    }

    // 检查 inode 类型是否为目录
    if (dir_inode.is_file) {
        free(path_copy1);
        free(path_copy2);
        return -ENOTDIR;
    }

    struct inode target_inode;
    int target_inode_index = get_inode_by_path(path, &target_inode);
    if (target_inode_index < 0) {
        free(path_copy1);
        free(path_copy2);
        return target_inode_index;
    }

    // 检查要删除的 inode 是否为常规文件
    if (!target_inode.is_file) {
        free(path_copy1);
        free(path_copy2);
        return -EISDIR;
    }

    // 删除目标文件的目录项
    int ret = delete_directory_entry(&dir_inode, parent_inode_index, base_name);
    if (ret < 0) {
        free(path_copy1);
        free(path_copy2);
        return ret;
    }

    // 释放目标文件的 inode 和数据块
    free_inode_and_blocks(&target_inode, target_inode_index);

    free(path_copy1);
    free(path_copy2);
	return 0;
}

int fs_rename(const char *oldpath, const char *newpath) {
    printf("fs_rename is called:%s\n",oldpath);
    // 提取旧路径的父目录路径和文件名
    char *oldpath_copy1 = strdup(oldpath);
    char *oldpath_copy2 = strdup(oldpath);
    if (!oldpath_copy1 || !oldpath_copy2) {
        free(oldpath_copy1);
        free(oldpath_copy2);
        return -ENOMEM;
    }

    char *old_dir_path = dirname(oldpath_copy1);
    char *old_base_name = basename(oldpath_copy2);

    struct inode old_dir_inode;
    int old_parent_inode_index = get_inode_by_path(old_dir_path, &old_dir_inode);
    if (old_parent_inode_index < 0) {
        free(oldpath_copy1);
        free(oldpath_copy2);
        return old_parent_inode_index;
    }

    struct inode target_inode;
    int target_inode_index = get_inode_by_path(oldpath, &target_inode);
    if (target_inode_index < 0) {
        free(oldpath_copy1);
        free(oldpath_copy2);
        return target_inode_index;
    }

    // 提取新路径的父目录路径和文件名
    char *newpath_copy1 = strdup(newpath);
    char *newpath_copy2 = strdup(newpath);
    if (!newpath_copy1 || !newpath_copy2) {
        free(oldpath_copy1);
        free(oldpath_copy2);
        free(newpath_copy1);
        free(newpath_copy2);
        return -ENOMEM;
    }

    char *new_dir_path = dirname(newpath_copy1);
    char *new_base_name = basename(newpath_copy2);

    struct inode new_dir_inode;
    int new_parent_inode_index = get_inode_by_path(new_dir_path, &new_dir_inode);
    if (new_parent_inode_index < 0) {
        free(oldpath_copy1);
        free(oldpath_copy2);
        free(newpath_copy1);
        free(newpath_copy2);
        return new_parent_inode_index;
    }

    // 检查新目录项是否存在
    struct inode check_inode;
    int check_result = get_inode_by_path(newpath, &check_inode);
    if (check_result > 0) {
        free(oldpath_copy1);
        free(oldpath_copy2);
        free(newpath_copy1);
        free(newpath_copy2);
        return -EEXIST;
    }

    // 添加新目录项
    int ret = add_directory_entry(&new_dir_inode, new_parent_inode_index, new_base_name, target_inode_index);
    if (ret < 0) {
        free(oldpath_copy1);
        free(oldpath_copy2);
        free(newpath_copy1);
        free(newpath_copy2);
        return ret;
    }

    // 删除旧目录项
    ret = delete_directory_entry(&old_dir_inode, old_parent_inode_index, old_base_name);
    if (ret < 0) {
        free(oldpath_copy1);
        free(oldpath_copy2);
        free(newpath_copy1);
        free(newpath_copy2);
        return ret;
    }

    free(oldpath_copy1);
    free(oldpath_copy2);
    free(newpath_copy1);
    free(newpath_copy2);
    return 0;
}

int fs_write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *fi) {    
    printf("fs_write is called:%s\n",path);
    struct inode file_inode;
    int inode_index = get_inode_by_path(path, &file_inode);
    if (inode_index < 0) {
        return 0;
    }

    // 检查 inode 类型是否为常规文件
    if (!file_inode.is_file) {
        return 0;
    }

    // 处理 O_APPEND 标志
    if (fi->flags & O_APPEND) {
        offset = file_inode.size;
    }

    // 调整文件大小
    size_t end_offset = offset + size;
    if(end_offset>file_inode.size){
        if (resize_file(&file_inode, end_offset) < 0) {
            return 0;
        }
    }
    // 写入数据
    size_t bytes_written = 0;
    uint16_t pointers[BLOCK_SIZE / sizeof(uint16_t)];
    if (safe_read(file_inode.indirect_pointer, pointers) != 0) {
        return 0;
    }
    while (bytes_written < size) {
        size_t block_index = (offset + bytes_written) / BLOCK_SIZE;
        size_t block_offset = (offset + bytes_written) % BLOCK_SIZE;
        size_t bytes_to_write = min(BLOCK_SIZE - block_offset, size - bytes_written);

        uint8_t block[BLOCK_SIZE];
        if (safe_read(pointers[block_index], block) != 0) {
            return 0;
        }

        memcpy(block + block_offset, buffer + bytes_written, bytes_to_write);
        if (safe_write(pointers[block_index], block) != 0) {
            return 0;
        }

        bytes_written += bytes_to_write;
    }

    // 更新文件的时间戳
    file_inode.st_Mtime = file_inode.st_Ctime = time(NULL);
    if (write_inode(inode_index, &file_inode) != 0) {
        return 0;
    }

	
    return bytes_written;
}

int fs_truncate(const char *path, off_t size) {
    printf("fs_truncate is called:%s\n",path);
    struct inode file_inode;
    int inode_index = get_inode_by_path(path, &file_inode);
    if (inode_index < 0) {
        return inode_index;
    }

    // 检查 inode 类型是否为常规文件
    if (!file_inode.is_file) {
        return -EISDIR;
    }

    // 调整文件大小
    int ret = resize_file(&file_inode, size);
    if (ret < 0) {
        return ret;
    }

    // 更新文件的 ctime
    file_inode.st_Ctime = time(NULL);
    if (write_inode(inode_index, &file_inode) != 0) {
        return -EIO;
    }

    return 0;
}

int fs_utime(const char *path, struct utimbuf *buffer) {
    printf("fs_utime is called:%s\n",path);
    struct inode file_inode;
    int inode_index = get_inode_by_path(path, &file_inode);
    if (inode_index < 0) {
        return inode_index;
    }

    // 更新 atime 和 mtime
    file_inode.st_Atime = buffer->actime;
    file_inode.st_Mtime = buffer->modtime;

    // 更新 ctime
    file_inode.st_Ctime = time(NULL);

    // 将更新后的 inode 写回磁盘
    if (write_inode(inode_index, &file_inode) != 0) {
        return -EIO;
    }

    return 0;
}

int fs_statfs(const char *path, struct statvfs *stat) {
    printf("fs_statfs is called:%s\n",path);
    struct superblock sb;
    if (safe_read(0, &sb) != 0) {
        return -EIO;
    }

    // 初始化statvfs结构
    memset(stat, 0, sizeof(struct statvfs));
    stat->f_bsize = sb.f_bsize;
    stat->f_blocks = sb.f_blocks;
    stat->f_files = sb.f_files;
    stat->f_namemax = sb.f_namemax;

    // 读取块位图
    uint8_t block_bitmap[BLOCK_SIZE * 2];
    if (safe_read(1, block_bitmap) != 0 || safe_read(2, block_bitmap + BLOCK_SIZE) != 0) {
        return -EIO;
    }

    // 读取 inode 位图
    uint8_t inode_bitmap[BLOCK_SIZE];
    if (safe_read(3, inode_bitmap) != 0) {
        return -EIO;
    }

    // 计算空闲块和空闲节点数量
    int free_blocks = count_free_bits(block_bitmap, BLOCK_SIZE * 2);
    int free_inodes = count_free_bits(inode_bitmap, BLOCK_SIZE);

    // 填写statvfs结构
    sb.f_bfree=sb.f_bavail=stat->f_bfree = stat->f_bavail = free_blocks;
    sb.f_ffree=sb.f_favail=stat->f_ffree = stat->f_favail = free_inodes;
    if (safe_write(0, &sb) != 0) {
        return -EIO;
    }

    return 0;
}

int fs_open(const char *path, struct fuse_file_info *fi) {
    printf("fs_open is called:%s\n",path);
    struct inode file_inode;
    int inode_index = get_inode_by_path(path, &file_inode);
    if (inode_index < 0) {
        return inode_index;
    }

    // 检查 inode 类型是否为常规文件
    if (!file_inode.is_file) {
        return -EISDIR;
    }

    // 处理 O_APPEND 标志
    if (fi->flags & O_APPEND) {
        fi->fh = file_inode.size;
    } else {
        fi->fh = 0;
    }

    return 0;
}

//Functions you don't actually need to modify
int fs_release (const char *path, struct fuse_file_info *fi)
{
	printf("Release is called:%s\n",path);
	return 0;
}

int fs_opendir (const char *path, struct fuse_file_info *fi)
{
	printf("Opendir is called:%s\n",path);
	return 0;
}

int fs_releasedir (const char * path, struct fuse_file_info *fi)
{
	printf("Releasedir is called:%s\n",path);
	return 0;
}

static struct fuse_operations fs_operations = {
	.getattr    = fs_getattr,
	.readdir    = fs_readdir,
	.read       = fs_read,
	.mkdir      = fs_mkdir,
	.rmdir      = fs_rmdir,
	.unlink     = fs_unlink,
	.rename     = fs_rename,
	.truncate   = fs_truncate,
	.utime      = fs_utime,
	.mknod      = fs_mknod,
	.write      = fs_write,
	.statfs     = fs_statfs,
	.open       = fs_open,
	.release    = fs_release,
	.opendir    = fs_opendir,
	.releasedir = fs_releasedir
};

int main(int argc, char *argv[])
{
	if(disk_init())
		{
		printf("Can't open virtual disk!\n");
		return -1;
		}
	if(mkfs())
		{
		printf("Mkfs failed!\n");
		return -2;
		}
    return fuse_main(argc, argv, &fs_operations, NULL);
}

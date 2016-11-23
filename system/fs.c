#include <xinu.h>
#include <kernel.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#if FS
#include <fs.h>

static struct fsystem fsd;
int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

char block_cache[512];

#define SB_BLK 0
#define BM_BLK 1
#define RT_BLK 2

#define NUM_FD 16
#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2
struct filetable oft[NUM_FD];

// Global variable for inode number.
int inode_count = 0;

int next_open_fd = -1;
int inode_id=1;

static int next_free_inode = FIRST_INODE_BLOCK;
static int next_free_data_block = -1;

struct filetable *fileTablePtr;

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock);

/* YOUR CODE GOES HERE */

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock) {
  int diskblock;

  if (fileblock >= INODEBLOCKS - 2) {
    printf("No indirect block support\n");
    return SYSERR;
  }

  diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address

  return diskblock;
}

/* read in an inode and fill in the pointer */
int fs_get_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;
  int inode_off;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_get_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  inode_off = inn * sizeof(struct inode);

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  printf("inn*sizeof(struct inode): %d\n", inode_off);
  */

  bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
  memcpy(in, &block_cache[inode_off], sizeof(struct inode));

  return OK;

}

int fs_put_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_put_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  */

  bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
  memcpy(&block_cache[(inn*sizeof(struct inode))], in, sizeof(struct inode));
  bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);

  return OK;
}
     
int fs_mkfs(int dev, int num_inodes) {
  int i;
  
  if (dev == 0) {
    fsd.nblocks = dev0_numblocks;
    fsd.blocksz = dev0_blocksize;
  }
  else {
    printf("Unsupported device\n");
    return SYSERR;
  }

  if (num_inodes < 1) {
    fsd.ninodes = DEFAULT_NUM_INODES;
  }
  else {
    fsd.ninodes = num_inodes;
  }

  i = fsd.nblocks;
  while ( (i % 8) != 0) {i++;}
  fsd.freemaskbytes = i / 8; 
  
  if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR) {
    printf("fs_mkfs memget failed.\n");
    return SYSERR;
  }
  
  /* zero the free mask */
  for(i=0;i<fsd.freemaskbytes;i++) {
    fsd.freemask[i] = '\0';
  }
  
  fsd.inodes_used = 0;
  
  /* write the fsystem block to SB_BLK, mark block used */
  fs_setmaskbit(SB_BLK);
  bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
  
  /* write the free block bitmask in BM_BLK, mark block used */
  fs_setmaskbit(BM_BLK);
  bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

  return 1;
}

void fs_print_fsd(void) {

  printf("fsd.ninodes: %d\n", fsd.ninodes);
  printf("sizeof(struct inode): %d\n", sizeof(struct inode));
  printf("INODES_PER_BLOCK: %d\n", INODES_PER_BLOCK);
  printf("NUM_INODE_BLOCKS: %d\n", NUM_INODE_BLOCKS);
}

/* specify the block number to be set in the mask */
int fs_setmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  fsd.freemask[mbyte] |= (0x80 >> mbit);
  return OK;
}

/* specify the block number to be read in the mask */
int fs_getmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
  return OK;

}

/* specify the block number to be unset in the mask */
int fs_clearmaskbit(int b) {
  int mbyte, mbit, invb;
  mbyte = b / 8;
  mbit = b % 8;

  invb = ~(0x80 >> mbit);
  invb &= 0xFF;

  fsd.freemask[mbyte] &= invb;
  return OK;
}

/* This is maybe a little overcomplicated since the lowest-numbered
   block is indicated in the high-order bit.  Shift the byte by j
   positions to make the match in bit7 (the 8th bit) and then shift
   that value 7 times to the low-order bit to print.  Yes, it could be
   the other way...  */
void fs_printfreemask(void) {
  int i,j;

  for (i=0; i < fsd.freemaskbytes; i++) {
    for (j=0; j < 8; j++) {
      printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
    }
    if ( (i % 8) == 7) {
      printf("\n");
    }
  }
  printf("\n");
}

int fs_mount(int dev) {
  fsd.root_dir.numentries = 0;
  return OK;
}

int fs_get_inode_id_by_filename(int dev, char *filename, int *inode_id){
  //printf("%s\n",filename);
  if(filename == NULL) {
    printf("ERROR: No filename provided.\n");
    return SYSERR;
  }
  for(int x = 0; x < fsd.ninodes; x++) {
    //printf("File : %s, Inode : %d\n",fsd.root_dir.entry[x].name, fsd.root_dir.entry[x].inode_num);
    if(!strcmp(filename, fsd.root_dir.entry[x].name)) {
      *inode_id = fsd.root_dir.entry[x].inode_num;
      break;
    }
  }
  return OK;
}

int fs_get_dirent_from_inodeid(int dev, int inode_id, struct dirent *dir_ent_ret){

  //dir_ent_ret = NULL;

  struct dirent dir_ent;

  struct directory root_dir = fsd.root_dir;

  for(int x = 0; x < fsd.ninodes; x++) {
    dir_ent = root_dir.entry[x];

    if(inode_id == dir_ent.inode_num) {
      *dir_ent_ret = dir_ent;
      break;
    }
  }

  return OK;

}

void print_oft(){
  int i = 0;
  while(i<=next_open_fd){
    printf("File name: %s\n", oft[i].de->name);
    printf("Inode Number: %d\n", oft[i].de->inode_num);
    printf("File State: %d\n",  oft[i].state);
    printf("File Pointer: %d\n",  oft[i].fileptr);
    printf("File Inode Id: %d\n", oft[i].in.id);
    printf("File Inode Size: %d\n", oft[i].in.size);
    i++;
  }
}

/* System function to open the file. */
int fs_open(char *filename, int flags){
  int i, file_inode_id=-1;
  next_open_fd++;
  // Check if max open file reached.
  if(next_open_fd == NUM_FD) {
    printf("ERROR: Maximum limit of open file has been reached.\n");
    return SYSERR;
  }

  for(i=0;i<next_open_fd;i++){
    if(!strcmp(oft[i].de->name, filename)){
      if(oft[i].state == FSTATE_OPEN){
	printf("ERROR: File already open.\n");
	return SYSERR;
      }
      else{
	oft[i].state = FSTATE_OPEN;
	oft[i].fileptr = 0;
	oft[i].in.flags = flags;
	fs_put_inode_by_num(dev0, oft[i].in.id, &oft[i].in);
	printf("File %s is again open.\n", filename);
	return i;
      }
    }
  }

  if(fs_get_inode_id_by_filename(dev0, filename, &file_inode_id) != OK) {
     printf("ERROR: Unable to fetch inode id from filename.\n");
    return SYSERR;
  }
  

  //printf("Inode ID got : %d\n",file_inode_id);
  //Create a new Inode for the new file
  if(file_inode_id == -1){
    //Search for existing Inodes
    for(i=FIRST_INODE_BLOCK; i<fsd.ninodes; i++) {
      fs_get_inode_by_num(0, next_free_inode, &oft[next_open_fd].in);
      //printf("ID %d\n", next_free_inode);
      if(oft[next_open_fd].in.id == 0 && oft[next_open_fd].in.nlink == 0) {
	break;
      }
      next_free_inode++;
      if(next_free_inode >= fsd.ninodes)
	next_free_inode = FIRST_INODE_BLOCK; 
    }
    fsd.root_dir.entry[fsd.root_dir.numentries].inode_num = next_free_inode;
    oft[next_open_fd].de = &fsd.root_dir.entry[fsd.root_dir.numentries];
    oft[next_open_fd].in.size = 0;
    oft[next_open_fd].in.nlink = 1;
    oft[next_open_fd].in.device = 0;
    oft[next_open_fd].in.id = next_free_inode;
    fsd.inodes_used++;
    fsd.root_dir.numentries++;
  }
  else{
    //Get Existing Inode attributes
    struct dirent *file_ent = NULL;
    struct inode file_inode;
    if(fs_get_dirent_from_inodeid(dev0, file_inode_id, file_ent) != OK) {
      printf("ERROR: Error while fetching directory structure.\n");
      return SYSERR;
    }

    if(fs_get_inode_by_num(dev0, file_inode_id, &file_inode) != OK) {
      printf("ERROR: Error while fetching Inode structure.\n");
      return SYSERR;
    }
    oft[next_open_fd].de = file_ent;
    oft[next_open_fd].in = file_inode;
    oft[next_open_fd].in.id = file_inode_id;
  }
  oft[next_open_fd].state = FSTATE_OPEN;	
  oft[next_open_fd].fileptr = 0;
  oft[next_open_fd].in.flags = flags;
  fs_put_inode_by_num(dev0, oft[next_open_fd].in.id, &oft[next_open_fd].in);
  printf("File %s is now open.\n", filename);
  return next_open_fd;
}


int fs_close(int fd) {
  if(oft[fd].state != FSTATE_OPEN) {
    printf("ERROR: State in the file table is not valid. File with ID %d might be closed.\n", fd);
    return SYSERR;	
  }
  
  oft[fd].state = FSTATE_CLOSED;
  printf("File is now closed.\n");
  return OK;
}

int fs_create(char *filename, int mode){
  int i;			
  
  // Check if file present in dir
  for (i = 0; i < DIRECTORY_SIZE; i++) {
    if (!strcmp(fsd.root_dir.entry[i].name, filename)){
      printf("ERROR: File %s exists in Directory.\n", filename);
      return SYSERR;
    }
  }
  
  // Add new file to directory
  strcpy(fsd.root_dir.entry[fsd.root_dir.numentries].name, filename);
  fsd.root_dir.entry[fsd.root_dir.numentries].inode_num=-1;

  
  printf("File %s created.\n", filename);  
  return fs_open(filename, O_RDWR);
}

int fs_seek(int fd, int offset){
  if(oft[fd].state != FSTATE_OPEN) {
    printf("ERROR: File not open\n");
    return SYSERR;	
  }
  
  if( (offset + oft[fd].fileptr) >= oft[fd].in.size ) {
    printf("ERROR: Seek position is greater than File Size\n");
    return SYSERR;
  }
  
  oft[fd].fileptr += offset;
  return OK;
}

int fs_read(int fd, void *buf, int nbytes){
  int blk, oset, len;
  int bytes_moved = 0;
  
  if(oft[fd].state != FSTATE_OPEN) {
    printf("ERROR: File not open\n");
    return SYSERR;	
  }
  
  if((nbytes + oft[fd].fileptr) > oft[fd].in.size ) {
    printf("ERROR: Bytes to read exceed the file size %d\n",  oft[fd].in.size);
    return SYSERR;
  }
  
  for(;nbytes > 0; nbytes -= len, oft[fd].fileptr += len, bytes_moved += len) {
    int blk_idx = oft[fd].fileptr/MDEV_BLOCK_SIZE;
    blk = oft[fd].in.blocks[blk_idx];
    oset = oft[fd].fileptr % MDEV_BLOCK_SIZE;
    if(nbytes < (MDEV_BLOCK_SIZE-oset))
      len = nbytes;
    else
      len = MDEV_BLOCK_SIZE-oset;
    int res = bs_bread(dev0, blk, oset, buf+bytes_moved, len);
    if(res!=OK) {
      printf("ERROR: Error occured while reading the block %d\n", blk);
      return SYSERR;
    }
  }
  //print_oft();
  return bytes_moved;
}

int fs_write(int fd, void *buf, int nbytes){
  int blk, oset, len;	
  int bytes_moved = 0;
	
  if(oft[fd].state != FSTATE_OPEN) {
    printf("ERROR: File not open.\n");
    return SYSERR;	
  }
  if(next_free_data_block == -1)
    next_free_data_block = NUM_INODE_BLOCKS+1;
  if(next_free_data_block > MDEV_NUM_BLOCKS){
    printf("ERROR: Disk Full.\n");
    return SYSERR;
  } 
  
  printf("Writing Data to File:\n%s\n", (char *) buf);
  
  for(;nbytes > 0;nbytes -= len, oft[fd].fileptr += len, bytes_moved += len) {
    int blk_idx = oft[fd].fileptr/MDEV_BLOCK_SIZE;
    oft[fd].in.blocks[blk_idx] = next_free_data_block;
    next_free_data_block++;
    
    blk = oft[fd].in.blocks[blk_idx];
    oset = oft[fd].fileptr % MDEV_BLOCK_SIZE;
    if(nbytes < (MDEV_BLOCK_SIZE-oset))
      len = nbytes;
    else
      len = MDEV_BLOCK_SIZE-oset;
    int res = bs_bwrite(dev0, blk, oset, buf+bytes_moved, len);
    if (res!=OK) {
      printf("ERROR: Block write failed for block %d\n",blk);
      return SYSERR;
    }
  }
  if (oft[fd].in.size < (nbytes + oft[fd].fileptr)) {
    oft[fd].in.size = oft[fd].fileptr;
  }
  //print_oft();
  fs_put_inode_by_num(dev0, oft[fd].in.id, &oft[fd].in);
  return bytes_moved;
}

#endif /* FS */

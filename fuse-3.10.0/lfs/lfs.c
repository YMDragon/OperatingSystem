#include "lfs.h"
#include "lfs_utils.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define ALLOCATESIZE 100 * 1024 * 1024

struct Segment *segBegin, *segEnd;
struct SegBuffer buffer;
int curBlk;
int cur_disk_pos;
int file_num;

int _fd;

struct MemoryData
{
    struct Segment *segBegin, *segEnd;
    struct SegBuffer buffer;
    int curBlk;
    int cur_disk_pos;
    int file_num;
};

struct MemoryData memoryData;

void lfs_init()
{
    _fd = open("data", O_RDWR);
    if (_fd < 0)
    {
        _fd = open("./data", O_RDWR | O_CREAT);
        char *file = malloc(ALLOCATESIZE * sizeof(char));
        write(_fd, file, ALLOCATESIZE);
        segBegin = segEnd = NULL;
        buffer.imap.numEntries = 0;
        curBlk = 0;
        cur_disk_pos = sizeof(struct MemoryData);
        file_num = 0;
        lfs_save();
    }
    else
    {
        _read_disk((char *)&memoryData, 0, sizeof(struct MemoryData));
        segBegin = memoryData.segBegin;
        segEnd = memoryData.segEnd;
        buffer = memoryData.buffer;
        curBlk = memoryData.curBlk;
        cur_disk_pos = memoryData.cur_disk_pos;
        file_num = memoryData.file_num;
        logger(DEBUG, "file numbers %d\n", file_num);
        logger(DEBUG, "%d\n", buffer.imap.numEntries);
    }
}

int _lfs_read(const int fileId, char *buf, int blkNum, int offset, int size)
{
    if (offset < 0 || offset + size > BLOCKSIZE || size <= 0)
    {
        logger(ERROR, "_lfs_read error\n");
        return -1;
    }

    int inodePos, dataPos, fileSize;
    char tbuff[BLOCKSIZE];

    fileSize = _get_file_size(fileId);
    if (blkNum * BLOCKSIZE + offset >= fileSize)
        return 0;
    if (blkNum * BLOCKSIZE + offset + size > fileSize)
        size = MIN(size, fileSize % BLOCKSIZE);

    if ((inodePos = _fit_in_imap(buffer.imap, fileId)) != -1)
    {
        memcpy(tbuff, buffer.blockdata[inodePos].data, BLOCKSIZE);
        struct Inode inode = _get_inode_from_string(tbuff);
        if ((dataPos = _fit_in_inode(NULL, inode, blkNum)) != -1)
        {
            memcpy(tbuff, buffer.blockdata[dataPos].data, BLOCKSIZE);
            memcpy(buf, tbuff + offset, size);
            return size;
        }
    }
    struct Segment *segPos = segBegin;
    for (segPos = segBegin; segPos != NULL; segPos = segPos->nextSegment)
    {
        if ((inodePos = _fit_in_imap(segPos->imap, fileId)) != -1)
        {
            _read_disk(tbuff, segPos->filePos + inodePos * BLOCKSIZE, BLOCKSIZE);
            struct Inode inode = _get_inode_from_string(tbuff);
            if ((dataPos = _fit_in_inode(segPos, inode, blkNum)) != -1)
            {
                _read_disk(tbuff, dataPos * BLOCKSIZE, BLOCKSIZE);
                memcpy(buf, tbuff + offset, size);
                return size;
            }
        }
    }
}

int lfs_read(const int fileId, char *buf, int offset, int size)
{
    if (offset < 0)
    {
        logger(ERROR, "read at negtive position\n");
        return -1;
    }
    int endAddr = offset + size;
    int pos, end, amount = 0;
    for (pos = offset; pos < endAddr; pos = end)
    {
        end = MIN(pos / BLOCKSIZE * BLOCKSIZE + BLOCKSIZE, endAddr);
        if (end <= pos)
            break;
        int tamount = _lfs_read(fileId, buf + pos - offset, pos / BLOCKSIZE, pos % BLOCKSIZE, end - pos);
        if (tamount == -1)
            return -1;
        amount += tamount;
    }
    return amount;
}

int _lfs_write(const int fileId, char *buf, int blkNum, int offset, int size)
{
    //logger(DEBUG, "_lfs_write %d %s %d %d %d\n", fileId, buf, blkNum, offset, size);
    if (offset < 0 || offset + size > BLOCKSIZE || size <= 0)
    {
        logger(ERROR, "_lfs_read error\n");
        return -1;
    }
    if (curBlk > BLOCKS_PER_SEGMENT - 5)
        lfs_fflush();

    int inodePos, fileSize;
    char tbuff[BLOCKSIZE];
    struct Inode inode;

    if (_fit_in_imap(buffer.imap, fileId) == -1)
    {
        //logger(DEBUG, "not found in buffer\n");
        inode = _fetch_inode(fileId);
        _clear_inode(&inode);
        _get_string_from_inode(inode, tbuff);
        memcpy(buffer.blockdata[curBlk].data, tbuff, BLOCKSIZE);
        struct ImapEntry imapentry;
        imapentry.fileId = fileId;
        imapentry.blkNum = curBlk;
        buffer.imap.entry[buffer.imap.numEntries++] = imapentry;
        curBlk++;
    }

    inodePos = _fit_in_imap(buffer.imap, fileId);

    //logger(DEBUG, "inodePos, %d\n", inodePos);

    if (offset != 0 || offset + size != BLOCKSIZE)
    {
        _lfs_read(fileId, tbuff, blkNum, 0, BLOCKSIZE);
    }
    memcpy(tbuff + offset, buf, size);
    memcpy(buffer.blockdata[curBlk].data, tbuff, BLOCKSIZE);

    memcpy(tbuff, buffer.blockdata[inodePos].data, BLOCKSIZE);
    inode = _get_inode_from_string(tbuff);
    //logger(DEBUG, "%d %d %d\n", inode.size, inode.st.st_size, sizeof(inode.st));
    curBlk++;
    _insert_inode(&inode, blkNum, curBlk - 1);

    inode.size += size;
    //logger(DEBUG, "%d %d %d\n", inode.size, inode.st.st_size, sizeof(inode.st));

    _get_string_from_inode(inode, tbuff);
    //logger(DEBUG, "%d %s\n", inodePos, tbuff);
    memcpy(buffer.blockdata[inodePos].data, tbuff, BLOCKSIZE);
    //memcpy(tbuff, buffer.blockdata[inodePos].data, BLOCKSIZE);
    //struct Inode inode2 = _get_inode_from_string(tbuff);
    //logger(DEBUG, "%d %d\n", inode2.size, inode2.st.st_size);
    return size;
}

int lfs_write(const int fileId, char *buf, int offset, int size)
{
    int fileSize = _get_file_size(fileId);
    //logger(DEBUG, "lfs_write %d %s %d %d\n", fileId, buf, offset, size);
    int endAddr = offset + size;
    int pos, end, amount = 0;

    if (offset > fileSize || offset < 0)
    {
        logger(ERROR, "Append at nonexistent position\n");
        return -1;
    }
    for (pos = offset; pos < endAddr; pos = end)
    {
        end = MIN(pos / BLOCKSIZE * BLOCKSIZE + BLOCKSIZE, endAddr);
        if (end <= pos)
            break;
        int tamount = _lfs_write(fileId, buf + pos - offset, pos / BLOCKSIZE, pos % BLOCKSIZE, end - pos);
        if (tamount == -1)
            return -1;
        amount += tamount;
    }
    return amount;
}

int lfs_create(struct stat st)
{
    char tbuff[BLOCKSIZE];
    if (curBlk > BLOCKS_PER_SEGMENT - 5)
        lfs_fflush();
    file_num++;
    int fileId = file_num;
    struct Inode inode;
    inode.st = st;
    //atime ctime mtime
    _clear_inode(&inode);
    inode.size = 0;
    inode.level = 0;
    _get_string_from_inode(inode, tbuff);
    memcpy(buffer.blockdata[curBlk].data, tbuff, BLOCKSIZE);
    struct ImapEntry imapentry;
    imapentry.fileId = fileId;
    imapentry.blkNum = curBlk;
    buffer.imap.entry[buffer.imap.numEntries++] = imapentry;
    curBlk++;
    return fileId;
}

int lfs_truncate(const int fileId, int size)
{
    if (size < 0)
    {
        logger(ERROR, "size cannot be negtive\n");
        return -1;
    }

    if (curBlk > BLOCKS_PER_SEGMENT - 5)
        lfs_fflush();

    int inodePos;
    char tbuff[BLOCKSIZE];
    struct Inode inode;

    if (_fit_in_imap(buffer.imap, fileId) == -1)
    {
        inode = _fetch_inode(fileId);
        _clear_inode(&inode);
        _get_string_from_inode(inode, tbuff);
        memcpy(buffer.blockdata[curBlk].data, tbuff, BLOCKSIZE);
        struct ImapEntry imapentry;
        imapentry.fileId = fileId;
        imapentry.blkNum = curBlk;
        buffer.imap.entry[buffer.imap.numEntries++] = imapentry;
        curBlk++;
    }

    if ((inodePos = _fit_in_imap(buffer.imap, fileId)) != -1)
    {
        memcpy(tbuff, buffer.blockdata[inodePos].data, BLOCKSIZE);
        struct Inode inode = _get_inode_from_string(tbuff);
        if (inode.size < size)
        {
            logger(WARN, "remove to larger size\n");
            return 1;
        }
        else
        {
            inode.size = size;
            _get_string_from_inode(inode, tbuff);
            memcpy(buffer.blockdata[inodePos].data, tbuff, BLOCKSIZE);
            return 0;
        }
    }
    else
    {
        logger(ERROR, "Unexcepted error happened\n");
        return -1;
    }
}

void lfs_fflush()
{
    _write_disk((char *)&buffer.blockdata, cur_disk_pos, sizeof(buffer.blockdata));
    struct Segment segment;
    segment.imap = buffer.imap;
    segment.filePos = cur_disk_pos;
    segment.nextSegment = segBegin;
    segBegin = &segment;
    memset(buffer.blockdata, 0, sizeof(buffer.blockdata));
    buffer.imap.numEntries = 0;
    curBlk = 0;
    cur_disk_pos += sizeof(buffer.blockdata);
}

int lfs_metadata(const int fileId, struct stat *st)
{
    int inodePos;
    char tbuff[BLOCKSIZE];
    //logger(DEBUG, "get metadata %d\n", fileId);
    if ((inodePos = _fit_in_imap(buffer.imap, fileId)) != -1)
    {
        //logger(DEBUG, "find in buffer %d %d\n", fileId, inodePos);
        memcpy(tbuff, buffer.blockdata[inodePos].data, BLOCKSIZE);
        struct Inode inode = _get_inode_from_string(tbuff);
        //logger(DEBUG, "%d %d\n", inode.size, inode.st.st_size);
        (*st) = inode.st;
        st->st_size = inode.size;
        return 0;
    }
    struct Segment *segPos = segBegin;
    for (segPos = segBegin; segPos != NULL; segPos = segPos->nextSegment)
    {
        if ((inodePos = _fit_in_imap(segPos->imap, fileId)) != -1)
        {
            _read_disk(tbuff, segPos->filePos + inodePos * BLOCKSIZE, BLOCKSIZE);
            struct Inode inode = _get_inode_from_string(tbuff);
            (*st) = inode.st;
            return 0;
        }
    }
    logger(ERROR, "cannot find such file\n");
    return -1;
}
int lfs_change(const int fileId, struct stat st)
{
    if (curBlk > BLOCKS_PER_SEGMENT - 5)
        lfs_fflush();

    int inodePos;
    char tbuff[BLOCKSIZE];
    struct Inode inode;

    if (_fit_in_imap(buffer.imap, fileId) == -1)
    {
        inode = _fetch_inode(fileId);
        _clear_inode(&inode);
        _get_string_from_inode(inode, tbuff);
        memcpy(buffer.blockdata[curBlk].data, tbuff, BLOCKSIZE);
        struct ImapEntry imapentry;
        imapentry.fileId = fileId;
        imapentry.blkNum = curBlk;
        buffer.imap.entry[buffer.imap.numEntries++] = imapentry;
        curBlk++;
    }

    if ((inodePos = _fit_in_imap(buffer.imap, fileId)) != -1)
    {

        memcpy(tbuff, buffer.blockdata[inodePos].data, BLOCKSIZE);
        struct Inode inode = _get_inode_from_string(tbuff);
        inode.st = st;
        _get_string_from_inode(inode, tbuff);
        memcpy(buffer.blockdata[inodePos].data, tbuff, BLOCKSIZE);
        return 0;
    }
    else
    {
        logger(ERROR, "Unexcepted error happened\n");
    }
}

int file_number()
{
    return file_num;
}

void lfs_save()
{
    memoryData.segBegin = segBegin;
    memoryData.segEnd = segEnd;
    memoryData.buffer = buffer;
    memoryData.curBlk = curBlk;
    memoryData.cur_disk_pos = cur_disk_pos;
    memoryData.file_num = file_num;
    _write_disk((char *)&memoryData, 0, sizeof(struct MemoryData));
}

void _read_disk(char *buff, int offset, int size)
{
    pread(_fd, buff, size, offset);
}

void _write_disk(char *buff, int offset, int size)
{
    pwrite(_fd, buff, size, offset);
}

void _insert_inode_level(struct Inode *inode, int blockId, int blkNum, int level)
{
    int i, index;
    struct Inode nextInode;
    char tbuff[BLOCKSIZE];
    index = blockId;
    for (i = 0; i < inode->level; i++)
        index /= MAX_BLOCKS_INODE;
    index %= MAX_BLOCKS_INODE;
    if (level == inode->level)
    {
        if (level == 0)
        {
            inode->entry[index].blockId = blockId;
            inode->entry[index].blkNum = blkNum;
        }
        else
        {
            if (inode->entry[index].blockId != -1)
            {
                memcpy(tbuff, buffer.blockdata[inode->entry[index].blkNum].data, BLOCKSIZE);
                nextInode = _get_inode_from_string(tbuff);
            }
            else
            {
                inode->entry[index].blockId = blockId;
                inode->entry[index].blkNum = curBlk;
                _clear_inode(&nextInode);
                _get_string_from_inode(nextInode, tbuff);
                memcpy(buffer.blockdata[curBlk].data, tbuff, BLOCKSIZE);
                curBlk++;
            }
            _insert_inode_level(&nextInode, blockId, blkNum, level - 1);

            _get_string_from_inode(nextInode, tbuff); //write back to buffer
            memcpy(buffer.blockdata[inode->entry[index].blkNum].data, tbuff, BLOCKSIZE);
        }
    }
    else
    {
        nextInode = (*inode);
        _clear_inode(inode);
        inode->level = level;
        inode->entry[index].blockId = blockId;
        inode->entry[index].blkNum = curBlk;
        _get_string_from_inode(nextInode, tbuff);
        memcpy(buffer.blockdata[curBlk].data, tbuff, BLOCKSIZE);
        curBlk++;
        _insert_inode_level(&nextInode, blockId, blkNum, level - 1);

        _get_string_from_inode(nextInode, tbuff); // write back to buffer
        memcpy(buffer.blockdata[inode->entry[index].blkNum].data, tbuff, BLOCKSIZE);
    }
}

void _insert_inode(struct Inode *inode, int blockId, int blkNum)
{
    int t = blockId, level = 0;
    while (t >= MAX_BLOCKS_INODE)
    {
        level++;
        t /= MAX_BLOCKS_INODE;
    }
    //logger(DEBUG, "insert inode %d %d\n", level, inode->level);
    if (level < inode->level)
        level = inode->level;
    _insert_inode_level(inode, blockId, blkNum, level);
}

void _clear_inode(struct Inode *inode)
{
    int i;
    for (i = 0; i < MAX_BLOCKS_INODE; i++)
        inode->entry[i].blockId = -1;
}

int _fit_in_inode(struct Segment *segPos, struct Inode inode, int blkNum)
{
    int i, index;
    index = blkNum;
    for (i = 0; i < inode.level; i++)
        index /= MAX_BLOCKS_INODE;
    index %= MAX_BLOCKS_INODE;
    if (inode.level == 0)
    {
        if (inode.entry[index].blockId != -1)
            return inode.entry[index].blkNum;
        else
            return -1;
    }
    else
    {
        if (inode.entry[index].blockId != -1)
        {
            struct Inode nextInode;
            char tbuff[BLOCKSIZE];
            if (segPos == NULL)
                memcpy(tbuff, buffer.blockdata[inode.entry[index].blkNum].data, BLOCKSIZE);
            else
                _read_disk(tbuff, segPos->filePos + inode.entry[index].blkNum * BLOCKSIZE, BLOCKSIZE);
            nextInode = _get_inode_from_string(tbuff);
            return _fit_in_inode(segPos, nextInode, blkNum);
        }
        else
            return -1;
    }
}

int _fit_in_imap(struct Imap imap, const int fileId)
{
    int i;
    for (i = 0; i < imap.numEntries; i++)
        if (imap.entry[i].fileId == fileId)
            return imap.entry[i].blkNum;
    return -1;
}

struct Inode _get_inode_from_string(char *buff)
{
    struct Inode inode;
    memcpy((char *)&inode, buff, sizeof(struct Inode));
    return inode;
}

void _get_string_from_inode(struct Inode inode, char *buff)
{
    memcpy(buff, (char *)&inode, sizeof(struct Inode));
}

struct Inode _fetch_inode(int fileId)
{
    int inodePos;
    struct Segment *segPos = segBegin;
    char tbuff[BLOCKSIZE];
    for (segPos = segBegin; segPos != NULL; segPos = segPos->nextSegment)
    {
        if ((inodePos = _fit_in_imap(segPos->imap, fileId)) != -1)
        {
            _read_disk(tbuff, segPos->filePos + inodePos * BLOCKSIZE, BLOCKSIZE);
            struct Inode inode = _get_inode_from_string(tbuff);
            return inode;
        }
    }
    logger(ERROR, "cannot find inode for file %d", fileId);
}

int _get_file_size(int fileId)
{
    int inodePos;
    char tbuff[BLOCKSIZE];
    if ((inodePos = _fit_in_imap(buffer.imap, fileId)) != -1)
    {
        memcpy(tbuff, buffer.blockdata[inodePos].data, BLOCKSIZE);
        struct Inode inode = _get_inode_from_string(tbuff);
        return inode.size;
    }
    struct Segment *segPos = segBegin;
    for (segPos = segBegin; segPos != NULL; segPos = segPos->nextSegment)
    {
        if ((inodePos = _fit_in_imap(segPos->imap, fileId)) != -1)
        {
            _read_disk(tbuff, segPos->filePos + inodePos * BLOCKSIZE, BLOCKSIZE);
            struct Inode inode = _get_inode_from_string(tbuff);
            return inode.size;
        }
    }
    return 0;
}

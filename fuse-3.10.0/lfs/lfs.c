#include "lfs.h"
#include "cache.h"
#include "lfs_utils.h"
#include <assert.h>
#include <pthread.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define ALLOCATESIZE (110 * 1024 * 1024)
#define NUM_OF_COLLECT (50) ////////////////////////////////////////////////////////////////////

struct Segment *segBegin, *segEnd;
struct SegBuffer buffer;
int curBlk;
//int cur_disk_pos;
int file_num;

int _fd;

struct MemoryData
{
    int curBlk;
    //int cur_disk_pos;
    int file_num;
    int seg_num;
    int free_seg_num;
    struct Segment segData[MAX_SEGMENTS];
    struct SegBuffer buffer;
    int segOrder[MAX_SEGMENTS];
    int segFree[MAX_SEGMENTS];
};

struct MemoryData memoryData;

pthread_rwlock_t bufferLock;

void lfs_init()
{
    pthread_rwlock_init(&bufferLock, NULL);
    _fd = open("data", O_RDWR);
    if (_fd < 0)
    {
        int cur_disk_pos, i;
        _fd = open("data", O_RDWR | O_CREAT);
        char *file = malloc(ALLOCATESIZE * sizeof(char));
        write(_fd, file, ALLOCATESIZE);
        segBegin = segEnd = NULL;
        buffer.imap.numEntries = 0;
        curBlk = 0;
        cur_disk_pos = (sizeof(struct MemoryData) / BLOCKSIZE + 1) * BLOCKSIZE;
        file_num = 0;
        memoryData.seg_num = 0;
        memoryData.free_seg_num = MAX_SEGMENTS;
        for (i = 0; i < memoryData.free_seg_num; i++)
        {
            memoryData.segFree[i] = memoryData.free_seg_num - i - 1;
            memoryData.segData[i].filePos = cur_disk_pos;
            cur_disk_pos += sizeof(buffer.blockdata);
        }
        lfs_save();
        free(file);
    }
    else
    {
        _read_disk((char *)&memoryData, 0, sizeof(struct MemoryData));
        segBegin = segEnd = NULL;
        buffer = memoryData.buffer;
        curBlk = memoryData.curBlk;
        //cur_disk_pos = memoryData.cur_disk_pos;
        file_num = memoryData.file_num;
        int i;
        for (i = 0; i < memoryData.seg_num; i++)
        {
            int id = memoryData.segOrder[i];
            if (i == 0)
            {
                segBegin = &memoryData.segData[id];
            }
            else
            {
                memoryData.segData[id].nextSegment = segBegin;
                segBegin = &memoryData.segData[id];
            }
        }
        /*segBegin = &memoryData.segData[0];
        for (i = 1; i < memoryData.seg_num; i++)
        {
            memoryData.segData[i].nextSegment = &memoryData.segData[i - 1];
            segBegin = &memoryData.segData[i];
        }*/
        logger(DEBUG, "file numbers %d\n", file_num);
        //logger(DEBUG, "%d\n", buffer.imap.numEntries);
    }
}

int _lfs_read(const int fileId, char *buf, int blkNum, int offset, int size)
{
    //logger(DEBUG, "_lfs_read %d %d %d %d\n", fileId, blkNum, offset, size);
    if (offset < 0 || offset + size > BLOCKSIZE || size <= 0)
    {
        logger(ERROR, "_lfs_read error\n");
        return -1;
    }

    int inodePos, dataPos, fileSize;
    char tbuff[BLOCKSIZE];

    fileSize = _get_file_size(fileId);
    //logger(DEBUG, "file size %d\n", fileSize);
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
        //logger(DEBUG, "%d\n", segPos->filePos);
        if ((inodePos = _fit_in_imap(segPos->imap, fileId)) != -1)
        {
            _read_disk(tbuff, segPos->filePos + inodePos * BLOCKSIZE, BLOCKSIZE);
            struct Inode inode = _get_inode_from_string(tbuff);
            if ((dataPos = _fit_in_inode(segPos, inode, blkNum)) != -1)
            {
                _read_disk(tbuff, segPos->filePos + dataPos * BLOCKSIZE, BLOCKSIZE);
                memcpy(buf, tbuff + offset, size);
                return size;
            }
        }
    }
    logger(ERROR, "not found in anywhere!!!!!!!!!! %d %d\n", fileId, blkNum);
}

int lfs_read(const int fileId, char *buf, int offset, int size)
{
    //logger(DEBUG, "lfs_read %d %d %d %d\n", fileId, offset, size);
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

int _lfs_write(const int fileId, const char *buf, int blkNum, int offset, int size)
{
    logger(DEBUG, "_lfs_write %d %d %d %d\n", fileId, blkNum, offset, size);
    if (offset < 0 || offset + size > BLOCKSIZE || size <= 0)
    {
        logger(ERROR, "_lfs_read error\n");
        return -1;
    }
    if (curBlk > BLOCKS_PER_SEGMENT - 10) //
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

int lfs_write(const int fileId, const char *buf, int offset, int size)
{
    int fileSize = _get_file_size(fileId);
    //logger(DEBUG, "lfs_write %d %d %d\n", fileId, buf, offset, size);
    int endAddr = offset + size;
    int pos, end, amount = 0;

    if (offset < 0)
    {
        logger(ERROR, "Append at nonexistent position\n");
        return -1;
    }

    if (offset > fileSize)
    {
        char tbuff[offset - fileSize];
        memset(tbuff, 0, sizeof(tbuff));
        lfs_write(fileId, tbuff, fileSize, offset - fileSize);
    }
    if (_get_file_size(fileId) < offset)
    {
        logger(DEBUG, "file has no such long, unexception!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
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
    logger(DEBUG, "create, curBlk %d\n", curBlk);
    char tbuff[BLOCKSIZE];
    if (curBlk > BLOCKS_PER_SEGMENT - 10)
        lfs_fflush();
    file_num++;
    int fileId = file_num;
    struct Inode inode;
    inode.st = st;
    inode.st.st_ino = fileId;

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    inode.st.st_ctim = ts;
    inode.st.st_atim = ts;
    inode.st.st_mtim = ts;

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
    logger(DEBUG, "create, curBlk %d\n", curBlk);
    logger(DEBUG, "fileId %d\n", fileId);
    return fileId;
}

int lfs_truncate(const int fileId, int size)
{
    if (size < 0)
    {
        logger(ERROR, "size cannot be negtive\n");
        return -1;
    }

    if (curBlk > BLOCKS_PER_SEGMENT - 10)
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
            logger(WARN, "truncate to larger size\n");
            return -1;
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

int lfs_remove(const int fileId)
{
    int i, ok = -1;
    struct Segment *segPos;

    for (i = 0; i < buffer.imap.numEntries; i++)
    {
        if (buffer.imap.entry[i].fileId == fileId)
        {
            buffer.imap.entry[i].fileId = -1;
            ok = 0;
        }
    }
    for (segPos = segBegin; segPos != segEnd; segPos = segPos->nextSegment)
    {
        for (i = 0; i < segPos->imap.numEntries; i++)
            if (segPos->imap.entry[i].fileId == fileId)
            {
                segPos->imap.entry[i].fileId = -1;
                ok = 0;
            }
    }
    return ok;
}

void lfs_fflush()
{
    if (curBlk == 0)
        return;

    memoryData.free_seg_num--;
    int id = memoryData.segFree[memoryData.free_seg_num];

    logger(DEBUG, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!flush %d %d %d\n", memoryData.seg_num, id, memoryData.segData[id].filePos);

    _write_disk((char *)&buffer.blockdata, memoryData.segData[id].filePos, sizeof(buffer.blockdata));

    //struct Segment segment;
    memoryData.segData[id].imap = buffer.imap;
    //segment.filePos = cur_disk_pos;
    memoryData.segData[id].nextSegment = segBegin;
    segBegin = &memoryData.segData[id];

    memoryData.segOrder[memoryData.seg_num] = id;
    memoryData.seg_num++;

    //logger(DEBUG, "%d\n", segBegin->filePos);

    buffer.imap.numEntries = 0;
    memset(buffer.blockdata, 0, sizeof(buffer.blockdata));
    curBlk = 0;
    //cur_disk_pos += sizeof(buffer.blockdata);

    if (memoryData.seg_num > NUM_OF_COLLECT)
        garbage_collection();

    if (curBlk > BLOCKS_PER_SEGMENT - 10)
    {
        logger(DEBUG, "%d\n", curBlk);
        memoryData.free_seg_num--;
        int id = memoryData.segFree[memoryData.free_seg_num];

        logger(DEBUG, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!reflush %d %d %d\n", memoryData.seg_num, id, memoryData.segData[id].filePos);

        _write_disk((char *)&buffer.blockdata, memoryData.segData[id].filePos, sizeof(buffer.blockdata));

        //struct Segment segment;
        memoryData.segData[id].imap = buffer.imap;
        //segment.filePos = cur_disk_pos;
        memoryData.segData[id].nextSegment = segBegin;
        segBegin = &memoryData.segData[id];

        memoryData.segOrder[memoryData.seg_num] = id;
        memoryData.seg_num++;

        //logger(DEBUG, "%d\n", segBegin->filePos);

        buffer.imap.numEntries = 0;
        memset(buffer.blockdata, 0, sizeof(buffer.blockdata));
        curBlk = 0;
    }
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
        st->st_blksize = BLOCKSIZE;
        st->st_blocks = (inode.size + BLOCKSIZE - 1) / BLOCKSIZE;
        return 0;
    }
    struct Segment *segPos = segBegin;
    for (segPos = segBegin; segPos != NULL; segPos = segPos->nextSegment)
    {
        if ((inodePos = _fit_in_imap(segPos->imap, fileId)) != -1)
        {
            //logger(DEBUG, "find in disk %p %d %d\n", segPos, inodePos, segPos->filePos);
            _read_disk(tbuff, segPos->filePos + inodePos * BLOCKSIZE, BLOCKSIZE);
            struct Inode inode = _get_inode_from_string(tbuff);
            //logger(DEBUG, "%d %d\n", inode.size, inode.st.st_size);
            (*st) = inode.st;
            st->st_size = inode.size;
            st->st_blksize = BLOCKSIZE;
            st->st_blocks = (inode.size + BLOCKSIZE - 1) / BLOCKSIZE;
            return 0;
        }
    }
    logger(ERROR, "cannot find such file\n");
    return -1;
}

int lfs_change(const int fileId, struct stat st)
{
    if (curBlk > BLOCKS_PER_SEGMENT - 10)
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
    lfs_writeback();
    memoryData.buffer = buffer;
    memoryData.curBlk = curBlk;
    //memoryData.cur_disk_pos = cur_disk_pos;
    memoryData.file_num = file_num;
    _write_disk((char *)&memoryData, 0, sizeof(struct MemoryData));
}

void garbage_collection()
{
    int id = memoryData.segOrder[0]; //TODO
    int i;
    struct Segment *seg = &memoryData.segData[id];

    printf("!!!!!!!!!!!!!!!!!!!!!!!!garbage %d\n", id);

    for (i = 0; i < seg->imap.numEntries; i++)
        if (seg->imap.entry[i].fileId != -1)
        {
            //acquire seg->imap.entry[i].fileId
            _rewrite(seg, seg->imap.entry[i].fileId, seg->imap.entry[i].blkNum);
            //releaseseg->imap.entry[i].fileId
        }

    memoryData.segData[memoryData.segOrder[1]].nextSegment = NULL; //
    for (i = 0; i < memoryData.seg_num - 1; i++)
        memoryData.segOrder[i] = memoryData.segOrder[i + 1];
    memoryData.seg_num--;
    memoryData.segFree[memoryData.free_seg_num] = id;
    memoryData.free_seg_num++;
}

void _rewrite(struct Segment *seg, int fileId, int blkNum)
{
    struct Inode inode;
    char tbuff[BLOCKSIZE];
    int i;
    _read_disk(tbuff, seg->filePos + blkNum * BLOCKSIZE, BLOCKSIZE);
    inode = _get_inode_from_string(tbuff);
    if (inode.level == 0)
    {
        for (i = 0; i < MAX_BLOCKS_INODE; i++)
            if (inode.entry[i].blockId != -1)
            {
                if (_is_new_available(seg, fileId, inode.entry[i].blockId) > 0)
                {
                    printf("start!!!!!!!!!!!!!!!!!!!!!!!! %d %d %d\n", fileId, inode.entry[i].blockId, blkNum);
                    lfs_read(fileId, tbuff, inode.entry[i].blockId * BLOCKSIZE, BLOCKSIZE);
                    //printf("%s\n", tbuff);
                    lfs_write(fileId, tbuff, inode.entry[i].blockId * BLOCKSIZE, BLOCKSIZE);
                    char tbuff2[BLOCKSIZE];
                    lfs_read(fileId, tbuff2, inode.entry[i].blockId * BLOCKSIZE, BLOCKSIZE);
                    int k = 0;
                    for (k = 0; k < BLOCKSIZE; k++)
                        if (tbuff[k] != tbuff2[k])
                        {
                            printf("%c %c\n", tbuff[k], tbuff2[k]);
                            assert(tbuff[k] == tbuff2[k]);
                        }
                    //printf("%s\n", tbuff);
                }
            }
    }
    else
    {
        for (i = 0; i < MAX_BLOCKS_INODE; i++)
            if (inode.entry[i].blockId != -1)
            {
                _rewrite(seg, fileId, inode.entry[i].blkNum);
            }
    }
}

int _is_new_available(struct Segment *seg, int fileId, int blockId)
{
    int inodePos;
    struct Inode inode;
    char tbuff[BLOCKSIZE];
    int hasCheckedSize = 0;
    struct Segment *segPos;
    inodePos = _fit_in_imap(buffer.imap, fileId);
    if (inodePos != -1)
    {
        hasCheckedSize = 1;
        memcpy(tbuff, buffer.blockdata[inodePos].data, BLOCKSIZE);
        inode = _get_inode_from_string(tbuff);
        if (inode.size < blockId * BLOCKSIZE)
            return 0;
        if (_fit_in_inode(NULL, inode, blockId) != -1)
            return 0;
    }
    for (segPos = segBegin; segPos != seg; segPos = segPos->nextSegment)
    {
        inodePos = _fit_in_imap(segPos->imap, fileId);
        if (inodePos != -1)
        {
            _read_disk(tbuff, segPos->filePos + inodePos * BLOCKSIZE, BLOCKSIZE);
            inode = _get_inode_from_string(tbuff);
            if (hasCheckedSize == 0)
            {
                hasCheckedSize = 1;
                if (inode.size < blockId * BLOCKSIZE)
                    return 0;
            }
            if (_fit_in_inode(segPos, inode, blockId) != -1)
                return 0;
        }
    }
    if (hasCheckedSize == 0)
    {
        inodePos = _fit_in_imap(seg->imap, fileId);
        _read_disk(tbuff, segPos->filePos + inodePos * BLOCKSIZE, BLOCKSIZE);
        inode = _get_inode_from_string(tbuff);
        if (inode.size < blockId * BLOCKSIZE)
            return 0;
    }
    return 1;
}
void lfs_writeback()
{
    cache_writeback(_fd);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _read_disk(char *buff, int offset, int size)
{
    pread(_fd, buff, size, offset);
    /*int i;
    for (i = 0; i < 100; i++)
        logger(DEBUG, "%c", buff[i]);
    logger(DEBUG, "\n");*/
}

void _write_disk(char *buff, int offset, int size)
{
    pwrite(_fd, buff, size, offset);
    /*int i;
    for (i = 0; i < 100; i++)
        logger(DEBUG, "%c", buff[i]);
    logger(DEBUG, "\n");*/
}

void _insert_inode_level(struct Inode *inode, int blockId, int blkNum, int level)
{
    int i, index;
    struct Inode nextInode;
    char tbuff[BLOCKSIZE];
    index = blockId;
    for (i = 0; i < level; i++)
        index /= MAX_BLOCKS_INODE;
    index %= MAX_BLOCKS_INODE;
    if (level > inode->level)
    {
        nextInode = (*inode);
        _get_string_from_inode(nextInode, tbuff);
        memcpy(buffer.blockdata[curBlk].data, tbuff, BLOCKSIZE);
        curBlk++;
        for (i = inode->level + 1; i < level; i++)
        {
            struct Inode tInode;
            tInode.level = i;
            _clear_inode(&tInode);
            tInode.entry[0].blockId = blockId;
            tInode.entry[0].blkNum = curBlk - 1;
            _get_string_from_inode(tInode, tbuff);
            memcpy(buffer.blockdata[curBlk].data, tbuff, BLOCKSIZE);
            curBlk++;
        }
        _clear_inode(inode);
        inode->level = level;
        inode->entry[0].blockId = blockId;
        inode->entry[0].blkNum = curBlk - 1;
    }

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
            nextInode.level = level - 1;
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
    //logger(DEBUG, "_fit_in_inode %d %d, index %d\n", inode.level, blkNum, index);
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

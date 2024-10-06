#include "file.h"
#include "log.h"

NS_HIVE_BEGIN

File::File(const std::string& name) : m_fileName(name), m_fileLength(0), m_seekCursor(-1), m_fileHandle(-1), m_expandSize(0){

}
File::File(void) : m_fileName(""), m_fileLength(0), m_seekCursor(-1), m_fileHandle(-1), m_expandSize(0) {

}
File::~File(void){
    closeFile();
}
bool File::isOpen(){
    if(-1 == m_fileHandle){
//        LOG_ERROR("File::isOpen -1 == m_fileHandle");
        return false;
    }
    return true;
}
bool File::openFile(bool createNew){
    if(createNew){
        if(!touchFile(NULL, 0)){
            return false;
        }
    }
    if(!openReadWrite("rb+")){
        return false;
    }
    m_fileLength = writeTell();
    return true;
}
void File::closeFile(void){
    if(m_fileHandle >= 0){
        close(m_fileHandle);
        m_fileHandle = -1;
        m_seekCursor = -1;
    }
}
bool File::touchFile(const char* checkHead, int checkLength){
    if(!openReadWrite("ab+")){
        LOG_ERROR("touchFile open or create error file=%s", m_fileName.c_str());
        return false;
    }
    m_fileLength = writeTell();
    if(0 == m_fileLength){
        closeFile();
//        LOG_DEBUG("touchFile current file is empty file=%s", m_fileName.c_str());
        return true;
    }
    if(NULL != checkHead){
        char head[checkLength];
        fileSeek(0, SEEK_SET);
        fileRead(head, 1, checkLength);
        if(strncmp(head, checkHead, checkLength) != 0){
            // 发生错误了，这里需要关闭文件
            closeFile();
            LOG_ERROR("touchFile check error file=%s", m_fileName.c_str());
            return false;
        }
    }
    closeFile();
    return true;
}
bool File::writeData(int64 offset, const char* pData, int64 length){
    if(!isOpen()){
        return false;
    }
    correctOffset(&offset);
    int64 saveLength = (int64)length;
    // 这个数据的保存不超出当前文件的长度
    int64 endOffset = offset + (int64)saveLength;
    if(endOffset <= m_fileLength){
        if((int64)length == seekWrite(pData, 1, length, offset, SEEK_SET)){
            return true;
        }
        return false;
    }
    // 这个数据写入的偏移在文件的内部，特殊处理文件增长的数值
    if(offset <= m_fileLength){
        fileSeek(offset, SEEK_SET);
        // Linux 下需要拼接数据块，之后再一次性写入
        int64 alignLength = 0;
        if(0 != m_expandSize){
            alignLength = (endOffset) % m_expandSize;
            if(alignLength != 0){
                alignLength = m_expandSize - alignLength;
            }
        }
        if(alignLength == 0){
            if(length != fileWrite(pData, 1, length)){
                return false;
            }
            m_fileLength = offset + length;
            return true;
        }
        int64 saveBufferSize = saveLength + alignLength;
        char* saveBuffer = new char[saveBufferSize];
        memset(saveBuffer, 0, saveBufferSize);
        memcpy(saveBuffer, pData, length);
        if(saveBufferSize != fileWrite(saveBuffer, 1, saveBufferSize)){
            delete []saveBuffer;
            return false;
        }
        m_fileLength = offset + saveBufferSize;
        delete []saveBuffer;
        return true;
    } // end offset < m_fileLength
    // 数据写入的偏移在文件在文件的长度以外，需要填充部分数据
    fileSeek(0, SEEK_END);
    // (1) 检查总长度，在可接受的范围内，直接生成并拼接写入
    int64 alignLength = 0;
    if(0 != m_expandSize){
        alignLength = (endOffset) % m_expandSize;
        if(alignLength != 0){
            alignLength = m_expandSize - alignLength;
        }
    }
    int64 blankSize = offset - m_fileLength;
    int64 totalWriteSize = blankSize + saveLength + alignLength;
    if(totalWriteSize <= MAX_EXPAND_BLOCK_SIZE){
        char* saveBuffer = new char[totalWriteSize];
        memset(saveBuffer, 0, totalWriteSize);
        memcpy(saveBuffer + blankSize, pData, length);
        if(totalWriteSize != fileWrite(saveBuffer, 1, totalWriteSize)){
            delete []saveBuffer;
            return false;
        }
        m_fileLength += totalWriteSize;
        delete []saveBuffer;
        return true;
    }
    // (2) 区分前半部分（空白填充）和后半部分（真实数据）写入
    char* saveBuffer = new char[MAX_EXPAND_BLOCK_SIZE];
    memset(saveBuffer, 0, MAX_EXPAND_BLOCK_SIZE);
    // 写入空白数据
    while (blankSize > MAX_EXPAND_BLOCK_SIZE) {
        if(MAX_EXPAND_BLOCK_SIZE != fileWrite(saveBuffer, 1, MAX_EXPAND_BLOCK_SIZE)){
            delete []saveBuffer;
            return false;
        }
        blankSize -= MAX_EXPAND_BLOCK_SIZE;
        m_fileLength += MAX_EXPAND_BLOCK_SIZE;
    }
    if(blankSize != fileWrite(saveBuffer, 1, blankSize)){
        delete []saveBuffer;
        return false;
    }
    m_fileLength += blankSize;
    // 写入后面的数据
    int64 saveBufferSize = saveLength + alignLength;
    memcpy(saveBuffer, pData, length);
    if(saveBufferSize != fileWrite(saveBuffer, 1, saveBufferSize)){
        delete []saveBuffer;
        return false;
    }
    m_fileLength += saveBufferSize;
    delete []saveBuffer;
    return true;
}
bool File::readData(int64 offset, char* pData, int64 length){
    if(!isOpen()){
        return false;
    }
    correctOffset(&offset);
    if((int64)length != seekRead(pData, 1, length, offset, SEEK_SET)){
        return false;
    }
    return true;
}
/*
    mmap
参数：
    addr通常都指定为NULL，让操作系统来决定在内存空间中开辟映射的空间；
    length：为所映射的长度；
    prot：表示protect，该参数描述了映射的所需内存保护；并且不能与文件的开放模式发生冲突)。
    flags： MAP_SHARED才会将内存体现在硬盘, MAP_PRIVATE不会将数据保存到硬盘
    fd：文件描述符
    offset：从文件的哪部分开始映射
*/
bool File::fastReadData(int64 offset, char* pData, int64 length){
    if(!isOpen()){
        return false;
    }
    int64 mapLength = length / BLOCK_SIZE;
    if(length % BLOCK_SIZE != 0){
        mapLength += 1;
    }
    mapLength = BLOCK_SIZE * mapLength;
    int64 mapOffset;
    if(offset + (int64)mapLength > m_fileLength){
        mapOffset = m_fileLength - mapLength;
    }else{
        mapOffset = offset;
    }
    char* pAddr = (char*)mmap(NULL, mapLength, PROT_READ, MAP_PRIVATE, m_fileHandle, mapOffset);
    if(pAddr == MAP_FAILED){
        LOG_ERROR("mmap file failed file=%s", m_fileName.c_str());
        return false;
    }
    memcpy(pData, pAddr + (offset - mapOffset), length);
    munmap(pAddr, mapLength);
    return true;
}
bool File::fastWriteData(int64 offset, const char* pData, int64 length, bool clearAll){
    if(!isOpen()){
        return false;
    }
    int64 mapLength;
    if(clearAll){
        mapLength = m_fileLength;
        char* pAddr = (char*)mmap(NULL, mapLength, PROT_WRITE, MAP_SHARED, m_fileHandle, 0);
        if(pAddr == MAP_FAILED){
            LOG_ERROR("mmap file failed file=%s", m_fileName.c_str());
            return false;
        }
        memset(pAddr, 0, mapLength);
        memcpy(pAddr + offset, pData, length);
        munmap(pAddr, mapLength);
    }else{
        mapLength = length / BLOCK_SIZE;
        if(length % BLOCK_SIZE != 0){
            mapLength += 1;
        }
        mapLength = BLOCK_SIZE * mapLength;
        if(offset + (int64)length > m_fileLength){
            return false;
        }
        char* pAddr = (char*)mmap(NULL, mapLength, PROT_WRITE, MAP_SHARED, m_fileHandle, offset);
        if(pAddr == MAP_FAILED){
            LOG_ERROR("mmap file failed file=%s", m_fileName.c_str());
            return false;
        }
        memcpy(pAddr, pData, length);
        munmap(pAddr, mapLength);
    }
//    LOG_DEBUG("fastWriteData OK offset=%lld length=%lld mapLength=%lld", offset, (int64)length, (int64)mapLength);
    return true;
}
char* File::fastOpenRead(int64 offset, int64 length, int64& mapLength, int64& mapOffset){
    if(!isOpen()){
        return NULL;
    }
    mapLength = length / BLOCK_SIZE;
    if(length % BLOCK_SIZE != 0){
        mapLength += 1;
    }
    mapLength = BLOCK_SIZE * mapLength;
    if(offset + (int64)mapLength > m_fileLength){
        mapOffset = m_fileLength - mapLength;
    }else{
        mapOffset = offset;
    }
    char* pAddr = (char*)mmap(NULL, mapLength, PROT_READ, MAP_PRIVATE, m_fileHandle, mapOffset);
    if(pAddr == MAP_FAILED){
        LOG_ERROR("mmap file failed file=%s", m_fileName.c_str());
        return NULL;
    }
    return pAddr;
}
void File::fastCloseRead(char* pAddr, int64 mapLength){
    if(NULL != pAddr){
        munmap(pAddr, mapLength);
    }
}
bool File::openReadWrite(const char* mode){
    closeFile();
    if(*mode == 'a'){
//		O_RDONLY:以只读方式打开文件
//        O_WRONLY:以只写方式打开文件
//        O_RDWR:以读写方式打开文
//          0666 = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
			m_fileHandle = open(m_fileName.c_str(), O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IROTH|S_IWOTH);   // O_APPEND
    }else{
        m_fileHandle = open(m_fileName.c_str(), O_RDWR);
    }
    if(-1 == m_fileHandle){
        m_fileHandle = -1;
        LOG_ERROR("openReadWrite open block failed file=%s mode=%s", m_fileName.c_str(), mode);
        return false;
    }
    return true;
}
void File::setExpandSize(int exp){
    if(exp > 0){
        int number = exp / BLOCK_SIZE;
        if(exp % BLOCK_SIZE != 0){
            number += 1;
        }
        m_expandSize = BLOCK_SIZE * number;
    }else if(exp == 0){
        m_expandSize = 0;
    }else{
        m_expandSize = BLOCK_SIZE;
    }
}
int64 File::fileSeek(int64 offset, int seek){
    int64 pointPos = offset;
    if(seek == SEEK_END){
        pointPos += m_fileLength;
    }
    if(pointPos == m_seekCursor){
//        LOG_DEBUG("fileSeek use m_seekCursor=%lld", m_seekCursor);
        return pointPos;
    }
    m_seekCursor = pointPos;
//    LOG_DEBUG("fileSeek seek offset=%lld", offset);
    return lseek(m_fileHandle, offset, seek);
}
int64 File::fileRead(void * ptr, int64 size, int64 n){
    int64 readLength = read(m_fileHandle, ptr, size*n);
//    LOG_DEBUG("fileRead readLength=%lld", readLength);
    if(readLength >= 0){
        m_seekCursor += readLength;
    }
    return readLength;
}
int64 File::fileWrite(const void * ptr, int64 size, int64 n){
    int64 writeLength = write(m_fileHandle, ptr, size*n);
//    LOG_DEBUG("fileWrite writeLength=%lld", writeLength);
    if(writeLength >= 0){
        m_seekCursor += writeLength;
    }
    return writeLength;
}
// ===========================================================================
MapFile::MapFile(const std::string& name) : File(name), m_pAddr(NULL) {

}
MapFile::MapFile(void) : File(""), m_pAddr(NULL) {

}
MapFile::~MapFile(void){
    closeMapFile();
}

bool MapFile::openFile(bool createNew, int initSize){
    if( !File::openFile(createNew) ){
        return false;
    }
    if(m_fileLength == 0){
        // 初始化数据
        char* pData = new char[initSize];
        memset(pData, 0, initSize);
        if( !File::writeData(0, pData, initSize) ){
            LOG_ERROR("openFile write empty data failed file=%s", m_fileName.c_str());
            delete [] pData;
            return false;
        }
        delete [] pData;
    }
    if(!openMapFile()){
        return false;
    }
    return true;
}
void MapFile::closeFile(void){
    closeMapFile();
    File::closeFile();
}
bool MapFile::writeData(int64 offset, const char* pData, int64 length){
    correctOffset(&offset);
    int64 saveLength = (int64)length;
    // 这个数据的保存不超出当前文件的长度
    int64 endOffset = offset + (int64)saveLength;
    if(endOffset > m_fileLength){
        closeMapFile();
        if( !File::writeData(offset, pData, length) ){
            LOG_ERROR("writeData after close map failed file=%s", m_fileName.c_str());
            return false;
        }
        if(!openMapFile()){
            LOG_ERROR("writeData close and open mmap again failed file=%s", m_fileName.c_str());
            return false;
        }
        return true;
    }
    memcpy(m_pAddr + offset, pData, length);
    markDirtyBlock(offset, length, true);
    return true;
}
bool MapFile::readData(int64 offset, char* pData, int64 length){
    if(NULL == m_pAddr){
        if(!openMapFile()){
            LOG_ERROR("readData open mmap again failed file=%s", m_fileName.c_str());
            return false;
        }
    }
    correctOffset(&offset);
    int64 saveLength = (int64)length;
    // 这个数据的保存不超出当前文件的长度
    int64 endOffset = offset + (int64)saveLength;
    if(endOffset > m_fileLength){
        LOG_ERROR("readData out of file length file=%s", m_fileName.c_str());
        return false;
    }
    memcpy(pData, m_pAddr + offset, length);
    return true;
}
char* MapFile::getDataPtr(int64 offset){
    if(NULL == m_pAddr){
        return NULL;
    }
    correctOffset(&offset);
    return m_pAddr + offset;
}
bool MapFile::syncFile(bool saveAll, bool asyncSave){
    if(NULL == m_pAddr){
        return false;
    }
    if(saveAll){
        int flags = MS_SYNC;
        if(asyncSave){
            flags = MS_ASYNC;
        }
        if( 0 != msync(m_pAddr, m_fileLength, flags) ){
            LOG_ERROR("msync failed errno=%d file=%s", errno, m_fileName.c_str());
            return false;
        }
        int maxSize = m_fileLength / BLOCK_SIZE;
        if(m_fileLength % BLOCK_SIZE != 0){
            maxSize++;
        }
        m_dirtyMark.clear();
        m_dirtyMark.resize(maxSize, false);
        m_seekCursor = -1;  // todo 待定：落地map数据时重置一下文件读写指针
    }else{
        return checkSync(asyncSave);
    }
    return true;
}
bool MapFile::syncBlock(int64 offset, int64 length, bool asyncSave){
    if(NULL == m_pAddr){
        return false;
    }
    correctOffset(&offset);
    int64 saveLength = (int64)length;
    // 这个数据的保存不超出当前文件的长度
    int64 endOffset = offset + (int64)saveLength;
    if(endOffset > m_fileLength){
        LOG_ERROR("syncBlock out of file length file=%s", m_fileName.c_str());
        return false;
    }
    m_seekCursor = -1;  // todo 待定：落地map数据时重置一下文件读写指针
    return syncBlockDirectory(offset, length, asyncSave);
}
bool MapFile::syncBlockDirectory(int64 offset, int64 length, bool asyncSave){
    int flags = MS_SYNC;
    if(asyncSave){
        flags = MS_ASYNC;
    }
    if( 0 != msync(m_pAddr + offset, length, flags) ){
        LOG_ERROR("msync failed errno=%d", errno);
        return false;
    }
    markDirtyBlock(offset, length, false);
    return true;
}
bool MapFile::openMapFile(void){
    if(m_fileHandle < 0){
        return false;
    }
    m_pAddr = (char*)mmap(NULL, m_fileLength, PROT_READ | PROT_WRITE, MAP_SHARED, m_fileHandle, 0);
    if(m_pAddr == MAP_FAILED){
        LOG_ERROR("mmap file failed file=%s", m_fileName.c_str());
        return false;
    }
    int maxSize = m_fileLength / BLOCK_SIZE;
    if(m_fileLength % BLOCK_SIZE != 0){
        maxSize++;
    }
    m_dirtyMark.clear();
    m_dirtyMark.resize(maxSize, false);
    return true;
}
void MapFile::closeMapFile(void){
    if(NULL != m_pAddr){
//        msync(m_pAddr, m_fileLength, MS_SYNC);
        munmap(m_pAddr, m_fileLength);
        m_pAddr = NULL;
    }
}
bool MapFile::checkSync(bool asyncSave){
    int64 offset = -1;
    int64 length = 0;
    m_seekCursor = -1;  // todo 待定：落地map数据时重置一下文件读写指针
    for(int i=0; i<(int)m_dirtyMark.size(); ++i){
        if(m_dirtyMark[i]){
            if(offset < 0){
                offset = i * BLOCK_SIZE;
            }
            length += BLOCK_SIZE;
        }else{
            if(length > 0){
                if( !syncBlockDirectory(offset, length, asyncSave) ){
                    LOG_ERROR("checkSync failed offset=%lld length=%lld", offset, (int64)length);
                    return false;
                }
                offset = -1;
                length = 0;
            }
        }
    }
    return true;
}
void MapFile::moveTo(int64 to, int64 offset, int64 length){
    if(to == offset){
        return;
    }
    memmove(m_pAddr + to, m_pAddr + offset, length);
    markDirtyBlock(to, to + length, true);
}
void MapFile::markDirtyBlock(int64 offset, int64 length, bool mark){
    int beginIndex = offset / BLOCK_SIZE;
    int endIndex = (offset + length) / BLOCK_SIZE;
    int maxSize = (int)m_dirtyMark.size();
    if(endIndex >= maxSize){
        endIndex = maxSize - 1;
    }
    for(int k=beginIndex; k<=endIndex; ++k){
        m_dirtyMark[k] = mark;
    }
}

NS_HIVE_END
